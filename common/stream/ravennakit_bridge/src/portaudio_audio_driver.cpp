#include "pisound/ravennakit_bridge/audio_driver.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#if defined(PISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO)
#include <portaudio.h>
#if defined(__APPLE__)
#include <pa_mac_core.h>
#endif
#endif

namespace pisound::ravennakit_bridge {

namespace {

float compute_peak_abs(const float* samples, std::size_t count) {
  if (samples == nullptr || count == 0) {
    return 0.0f;
  }

  float peak = 0.0f;
  for (std::size_t i = 0; i < count; ++i) {
    peak = std::max(peak, std::abs(samples[i]));
  }
  return peak;
}

class NullAudioDriver final : public IDuplexAudioDriver {
 public:
  explicit NullAudioDriver(BridgeConfig config) : config_(std::move(config)) {}

  ~NullAudioDriver() override { stop(); }

  void start(IAudioBridgeCallback& callback) override {
    if (running_.exchange(true)) {
      return;
    }

    callback_ = &callback;
    capture_buffer_.assign(config_.block_size * config_.channels, 0.0f);
    playback_buffer_.assign(config_.block_size * config_.channels, 0.0f);

    worker_ = std::thread([this]() {
      const auto period = std::chrono::duration<double>(
          static_cast<double>(config_.block_size) /
          static_cast<double>(config_.sample_rate_hz));
      while (running_.load()) {
        if (callback_ != nullptr) {
          callback_->push_capture_interleaved(capture_buffer_.data(),
                                              config_.block_size,
                                              config_.channels);
          callback_->pull_playback_interleaved(playback_buffer_.data(),
                                               config_.block_size,
                                               config_.channels);
        }
        std::this_thread::sleep_for(period);
      }
    });
  }

  void stop() override {
    running_.store(false);
    if (worker_.joinable()) {
      worker_.join();
    }
    callback_ = nullptr;
  }

 private:
  BridgeConfig config_;
  IAudioBridgeCallback* callback_ = nullptr;
  std::atomic<bool> running_ = false;
  std::thread worker_;
  std::vector<float> capture_buffer_;
  std::vector<float> playback_buffer_;
};

#if defined(PISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO)

class PortAudioRuntime {
 public:
  static void ensure_initialized() {
    static PortAudioRuntime runtime;
    (void)runtime;
  }

 private:
  PortAudioRuntime() {
    const auto err = Pa_Initialize();
    if (err != paNoError) {
      throw std::runtime_error(
          std::string("PortAudio initialization failed: ") +
          Pa_GetErrorText(err));
    }
  }

  ~PortAudioRuntime() {
    const auto err = Pa_Terminate();
    (void)err;
  }
};

PaDeviceIndex find_device_index(const std::string& spec, bool input) {
  const auto device_count = Pa_GetDeviceCount();
  if (device_count < 0) {
    throw std::runtime_error(
        std::string("PortAudio failed to get device count: ") +
        Pa_GetErrorText(device_count));
  }

  for (PaDeviceIndex i = 0; i < device_count; ++i) {
    const auto* info = Pa_GetDeviceInfo(i);
    if (info == nullptr) {
      continue;
    }
    if (info->name == nullptr) {
      continue;
    }
    const bool supports_direction =
        input ? (info->maxInputChannels > 0) : (info->maxOutputChannels > 0);
    if (!supports_direction) {
      continue;
    }
    if (spec == info->name || std::string(info->name).find(spec) != std::string::npos) {
      return i;
    }
  }

  throw std::runtime_error("PortAudio device not found: " + spec);
}

class PortAudioDuplexDriver final : public IDuplexAudioDriver {
 public:
  explicit PortAudioDuplexDriver(BridgeConfig config) : config_(std::move(config)) {}

  ~PortAudioDuplexDriver() override { stop(); }

  void start(IAudioBridgeCallback& callback) override {
    if (input_stream_ != nullptr || output_stream_ != nullptr) {
      return;
    }

    PortAudioRuntime::ensure_initialized();
    callback_ = &callback;

    input_device_index_ = find_device_index(config_.input_device, true);
    const auto* input_info = Pa_GetDeviceInfo(input_device_index_);
    if (input_info == nullptr) {
      throw std::runtime_error("PortAudio returned null device info");
    }
    const auto input_channel_count = static_cast<int>(input_info->maxInputChannels);
    const auto max_input_channel_index =
        static_cast<int>(config_.input_start_channel + config_.channels - 1);
    if (input_info->maxInputChannels < max_input_channel_index) {
      throw std::runtime_error("input device does not expose enough channels");
    }

    PaStreamParameters in_params {};
    in_params.device = input_device_index_;
    in_params.channelCount = input_channel_count;
    in_params.sampleFormat = paFloat32;
    in_params.suggestedLatency = input_info->defaultLowInputLatency;
    in_params.hostApiSpecificStreamInfo = nullptr;

    capture_subset_.resize(config_.block_size * config_.channels, 0.0f);
    playback_subset_.resize(config_.block_size * config_.channels, 0.0f);
    input_channel_count_ = static_cast<std::size_t>(input_channel_count);
    input_channel_offset_ =
        static_cast<std::size_t>(config_.input_start_channel - 1);
    output_channel_count_ = 0;
    output_channel_offset_ = 0;

    PaStreamParameters out_params {};
    const PaStreamParameters* out_params_ptr = nullptr;
    if (!config_.output_device.empty()) {
      output_device_index_ = find_device_index(config_.output_device, false);
      const auto* output_info = Pa_GetDeviceInfo(output_device_index_);
      if (output_info == nullptr) {
        throw std::runtime_error("PortAudio returned null output device info");
      }
      const auto max_output_channel_index =
          static_cast<int>(config_.output_start_channel + config_.channels - 1);
      const auto output_channel_count = max_output_channel_index;
      if (output_info->maxOutputChannels < max_output_channel_index) {
        throw std::runtime_error("output device does not expose enough channels");
      }
      out_params.device = output_device_index_;
      out_params.channelCount = output_channel_count;
      out_params.sampleFormat = paFloat32;
      out_params.suggestedLatency = output_info->defaultLowOutputLatency;
      out_params.hostApiSpecificStreamInfo = nullptr;
      out_params_ptr = &out_params;
      output_channel_count_ = static_cast<std::size_t>(output_channel_count);
      output_channel_offset_ =
          static_cast<std::size_t>(config_.output_start_channel - 1);
    }

    const auto input_err = Pa_OpenStream(&input_stream_,
                                         &in_params,
                                         nullptr,
                                         static_cast<double>(config_.sample_rate_hz),
                                         static_cast<unsigned long>(config_.block_size),
                                         paClipOff,
                                         &PortAudioDuplexDriver::pa_input_callback,
                                         this);
    if (input_err != paNoError) {
      input_stream_ = nullptr;
      throw std::runtime_error(std::string("PortAudio open failed: ") +
                               Pa_GetErrorText(input_err));
    }

    const auto input_start_err = Pa_StartStream(input_stream_);
    if (input_start_err != paNoError) {
      Pa_CloseStream(input_stream_);
      input_stream_ = nullptr;
      throw std::runtime_error(std::string("PortAudio start failed: ") +
                               Pa_GetErrorText(input_start_err));
    }

    if (out_params_ptr != nullptr) {
      const auto output_err = Pa_OpenStream(&output_stream_,
                                            nullptr,
                                            out_params_ptr,
                                            static_cast<double>(config_.sample_rate_hz),
                                            static_cast<unsigned long>(config_.block_size),
                                            paClipOff,
                                            &PortAudioDuplexDriver::pa_output_callback,
                                            this);
      if (output_err != paNoError) {
        Pa_StopStream(input_stream_);
        Pa_CloseStream(input_stream_);
        input_stream_ = nullptr;
        output_stream_ = nullptr;
        throw std::runtime_error(std::string("PortAudio start failed: ") +
                                 Pa_GetErrorText(output_err));
      }

      const auto output_start_err = Pa_StartStream(output_stream_);
      if (output_start_err != paNoError) {
        Pa_StopStream(input_stream_);
        Pa_CloseStream(output_stream_);
        Pa_CloseStream(input_stream_);
        input_stream_ = nullptr;
        output_stream_ = nullptr;
        throw std::runtime_error(std::string("PortAudio start failed: ") +
                                 Pa_GetErrorText(output_start_err));
      }
    }
  }

  void stop() override {
    callback_ = nullptr;

    if (input_stream_ != nullptr) {
      Pa_StopStream(input_stream_);
      Pa_CloseStream(input_stream_);
      input_stream_ = nullptr;
    }
    if (output_stream_ != nullptr) {
      Pa_StopStream(output_stream_);
      Pa_CloseStream(output_stream_);
      output_stream_ = nullptr;
    }
  }

 private:
  static int pa_input_callback(const void* input, void* output,
                               unsigned long frames_per_buffer,
                               const PaStreamCallbackTimeInfo* time_info,
                               PaStreamCallbackFlags status_flags,
                               void* user_data) {
    (void)time_info;
    (void)status_flags;
    (void)output;
    return static_cast<PortAudioDuplexDriver*>(user_data)
        ->handle_input_callback(input,
                                static_cast<std::size_t>(frames_per_buffer));
  }

  static int pa_output_callback(const void* input, void* output,
                                unsigned long frames_per_buffer,
                                const PaStreamCallbackTimeInfo* time_info,
                                PaStreamCallbackFlags status_flags,
                                void* user_data) {
    (void)time_info;
    (void)status_flags;
    (void)input;
    return static_cast<PortAudioDuplexDriver*>(user_data)
        ->handle_output_callback(output,
                                 static_cast<std::size_t>(frames_per_buffer));
  }

  int handle_input_callback(const void* input, std::size_t frames) {
    const auto channels = static_cast<std::size_t>(config_.channels);
    const auto sample_count = frames * channels;

    if (callback_ == nullptr || input == nullptr) {
      return paContinue;
    }

    const auto* input_samples = static_cast<const float*>(input);

    for (std::size_t frame = 0; frame < frames; ++frame) {
      for (std::size_t ch = 0; ch < channels; ++ch) {
        capture_subset_[frame * channels + ch] =
            input_samples[frame * input_channel_count_ +
                          input_channel_offset_ + ch];
      }
    }

    {
      std::lock_guard<std::mutex> guard(callback_mutex_);
      callback_->push_capture_interleaved(capture_subset_.data(), frames, channels);
    }

    if (config_.verbose) {
      const auto callback_index = ++input_callback_counter_;
      if ((callback_index % 128U) == 0U) {
        const auto input_peak = compute_peak_abs(capture_subset_.data(), sample_count);
        std::cerr << "portaudio: input callback=" << callback_index
                  << " input_peak=" << input_peak
                  << " frames=" << frames << "\n";
      }
    }

    return paContinue;
  }

  int handle_output_callback(void* output, std::size_t frames) {
    if (callback_ == nullptr) {
      if (output != nullptr) {
        auto* output_samples = static_cast<float*>(output);
        std::memset(output_samples, 0,
                    frames * output_channel_count_ * sizeof(float));
      }
      return paContinue;
    }

    const auto channels = static_cast<std::size_t>(config_.channels);
    const auto sample_count = frames * channels;
    {
      std::lock_guard<std::mutex> guard(callback_mutex_);
      callback_->pull_playback_interleaved(playback_subset_.data(), frames, channels);
    }

    if (output != nullptr) {
      auto* output_samples = static_cast<float*>(output);
      std::memset(output_samples, 0,
                  frames * output_channel_count_ * sizeof(float));
      for (std::size_t ch = 0; ch < channels; ++ch) {
        for (std::size_t frame = 0; frame < frames; ++frame) {
          output_samples[frame * output_channel_count_ +
                         output_channel_offset_ + ch] =
              playback_subset_[frame * channels + ch];
        }
      }
    }

    if (config_.verbose) {
      const auto callback_index = ++output_callback_counter_;
      if ((callback_index % 128U) == 0U) {
        const auto output_peak = compute_peak_abs(
            output != nullptr ? playback_subset_.data() : nullptr, sample_count);
        std::cerr << "portaudio: output callback=" << callback_index
                  << " output_peak=" << output_peak
                  << " frames=" << frames << "\n";
      }
    }

    return paContinue;
  }

  BridgeConfig config_;
  IAudioBridgeCallback* callback_ = nullptr;
  PaDeviceIndex input_device_index_ = paNoDevice;
  PaDeviceIndex output_device_index_ = paNoDevice;
  PaStream* input_stream_ = nullptr;
  PaStream* output_stream_ = nullptr;
  std::vector<float> capture_subset_;
  std::vector<float> playback_subset_;
  std::atomic<std::uint64_t> input_callback_counter_ = 0;
  std::atomic<std::uint64_t> output_callback_counter_ = 0;
  std::mutex callback_mutex_;
  std::size_t input_channel_count_ = 0;
  std::size_t output_channel_count_ = 0;
  std::size_t input_channel_offset_ = 0;
  std::size_t output_channel_offset_ = 0;
};

#else

class StubAudioDriver final : public IDuplexAudioDriver {
 public:
  explicit StubAudioDriver(BridgeConfig config) : config_(std::move(config)) {}

  void start(IAudioBridgeCallback& callback) override {
    float silence[2 * 128] = {0.0f};
    callback.pull_playback_interleaved(
        silence,
        std::min<std::size_t>(config_.block_size, 128U),
        std::min<std::size_t>(config_.channels, 2U));

    throw std::runtime_error(
        "PortAudio-backed audio driver is not connected yet. "
        "Reconfigure CMake with PISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO=ON "
        "and make PortAudio available.");
  }

  void stop() override {}

 private:
  BridgeConfig config_;
};

#endif

}  // namespace

std::unique_ptr<IDuplexAudioDriver> create_audio_driver(const BridgeConfig& config) {
  if (config.audio_backend == AudioBackend::null_backend) {
    return std::make_unique<NullAudioDriver>(config);
  }

#if defined(PISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO)
  return std::make_unique<PortAudioDuplexDriver>(config);
#else
  return std::make_unique<StubAudioDriver>(config);
#endif
}

std::string list_audio_devices() {
#if defined(PISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO)
  PortAudioRuntime::ensure_initialized();

  std::ostringstream oss;
  const auto device_count = Pa_GetDeviceCount();
  if (device_count < 0) {
    throw std::runtime_error(
        std::string("PortAudio failed to get device count: ") +
        Pa_GetErrorText(device_count));
  }
  if (device_count == 0) {
    return "No PortAudio devices found.\n";
  }

  for (PaDeviceIndex i = 0; i < device_count; ++i) {
    const auto* info = Pa_GetDeviceInfo(i);
    if (info == nullptr || info->name == nullptr) {
      continue;
    }
    oss << "[" << i << "] " << info->name << " in=" << info->maxInputChannels
        << " out=" << info->maxOutputChannels << "\n";
  }
  return oss.str();
#else
  return "PortAudio support is disabled in this build.\n";
#endif
}

}  // namespace pisound::ravennakit_bridge
