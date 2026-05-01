#include "pisound/ravennakit_bridge/ravennakit_engine.hpp"

#include <algorithm>
#include <cmath>
#include <deque>
#include <cstring>
#include <iostream>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#if defined(PISOUND_RAVENNAKIT_BRIDGE_HAVE_RAVENNAKIT)
#include "ravennakit/aes67/aes67_packet_time.hpp"
#include "ravennakit/core/audio/audio_buffer_view.hpp"
#include "ravennakit/core/net/interfaces/network_interface_config.hpp"
#include "ravennakit/core/audio/audio_format.hpp"
#include "ravennakit/core/log.hpp"
#include "ravennakit/core/system.hpp"
#include "ravennakit/ptp/ptp_instance.hpp"
#include "ravennakit/ravenna/ravenna_node.hpp"
#include "ravennakit/ravenna/ravenna_receiver.hpp"
#include "ravennakit/ravenna/ravenna_sender.hpp"
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

class BlockQueue {
 public:
  BlockQueue(std::size_t block_frames, std::size_t channels,
             std::size_t max_blocks)
      : max_samples_(block_frames * channels * max_blocks) {}

  void write(const float* samples, std::size_t frames, std::size_t channels) {
    const auto total = frames * channels;
    if (total == 0) {
      return;
    }

    std::lock_guard<std::mutex> guard(mutex_);
    if (samples_.size() + total > max_samples_) {
      const auto drop = std::min(samples_.size() + total - max_samples_,
                                 samples_.size());
      samples_.erase(samples_.begin(), samples_.begin() + drop);
    }
    samples_.insert(samples_.end(), samples, samples + total);
  }

  std::size_t read(float* samples, std::size_t frames, std::size_t channels) {
    const auto requested = frames * channels;
    if (requested == 0) {
      return 0;
    }

    std::lock_guard<std::mutex> guard(mutex_);
    const auto available = std::min(requested, samples_.size());
    std::copy_n(samples_.begin(), available, samples);
    samples_.erase(samples_.begin(), samples_.begin() + available);
    return available / channels;
  }

 private:
  const std::size_t max_samples_;
  std::mutex mutex_;
  std::deque<float> samples_;
};

class LoopbackAoipEngine final : public IAoipEngine {
 public:
  explicit LoopbackAoipEngine(BridgeConfig config)
      : config_(std::move(config)),
        rx_queue_(config_.block_size, config_.channels, 32) {}

  void start() override { running_ = true; }

  void stop() override { running_ = false; }

  void push_to_network(const float* samples, std::size_t frames,
                       std::size_t channels) override {
    if (!running_) {
      return;
    }
    rx_queue_.write(samples, frames, channels);
  }

  void pull_from_network(float* samples, std::size_t frames,
                         std::size_t channels) override {
    const auto total = frames * channels;
    std::memset(samples, 0, total * sizeof(float));
    if (!running_) {
      return;
    }
    const auto frames_read = rx_queue_.read(samples, frames, channels);
    const auto samples_read = frames_read * channels;
    if (samples_read < total) {
      std::memset(samples + samples_read, 0,
                  (total - samples_read) * sizeof(float));
    }
  }

 private:
  BridgeConfig config_;
  BlockQueue rx_queue_;
  bool running_ = false;
};

#if defined(PISOUND_RAVENNAKIT_BRIDGE_HAVE_RAVENNAKIT)

class RavennaKitAoipEngine final : public IAoipEngine,
                                   public rav::RavennaReceiver::Subscriber,
                                   public rav::ptp::Instance::Subscriber {
 public:
  explicit RavennaKitAoipEngine(BridgeConfig config)
      : config_(std::move(config)),
        tx_planar_storage_(config_.channels * config_.block_size, 0.0f),
        rx_planar_storage_(config_.channels * config_.block_size, 0.0f),
        tx_channel_ptrs_(config_.channels, nullptr),
        rx_channel_ptrs_(config_.channels, nullptr) {}

  void start() override {
    rav::set_log_level_from_env();
    rav::do_system_checks();

    const auto interface_config =
        rav::parse_network_interface_config_from_string(config_.network_interface);
    if (!interface_config) {
      throw std::runtime_error("RAVENNAKIT network interface not found: " +
                               config_.network_interface);
    }
    node_.set_network_interface_config(*interface_config).wait();

    rav::RavennaNode::Configuration node_config;
    node_config.enable_dnssd_node_discovery = true;
    node_config.enable_dnssd_session_advertisement = true;
    node_config.enable_dnssd_session_discovery = true;
    node_.set_configuration(node_config).wait();

    rav::RavennaSender::Configuration sender_config;
    sender_config.enabled = true;
    sender_config.session_name = config_.tx_session_name;
    sender_config.payload_type = config_.payload_type;
    sender_config.ttl = config_.ttl;
    // 1 ms / 48-frame packets match the Pi's AES67 tick much better than the
    // old 128-frame packet time and reduce the residual drop/jitter artifacts
    // on the Mac -> Pi path.
    sender_config.packet_time = rav::aes67::PacketTime {
        config_.tx_packet_time_num, config_.tx_packet_time_den};
    sender_config.audio_format.sample_rate = config_.sample_rate_hz;
    sender_config.audio_format.num_channels = config_.channels;
    sender_config.audio_format.encoding = rav::AudioEncoding::pcm_s24;
    sender_config.audio_format.byte_order = rav::AudioFormat::ByteOrder::be;
    sender_config.audio_format.ordering =
        rav::AudioFormat::ChannelOrdering::interleaved;

    rav::RavennaSender::Destination destination;
    destination.interface_by_rank = rav::rank::primary;
    destination.endpoint = {
        boost::asio::ip::make_address_v4(config_.tx_destination), config_.tx_port};
    destination.enabled = true;
    sender_config.destinations.push_back(destination);

    auto sender_id = node_.create_sender(sender_config).get();
    if (!sender_id) {
      throw std::runtime_error("Failed to create RAVENNA sender: " +
                               sender_id.error());
    }
    sender_id_ = *sender_id;

    rav::RavennaReceiver::Configuration receiver_config;
    receiver_config = rav::RavennaReceiver::Configuration::default_config();
    receiver_config.enabled = true;
    receiver_config.session_name = config_.rx_session_name;
    receiver_config.delay_frames = config_.rx_require_delay_frames;
    auto receiver_id = node_.create_receiver(receiver_config).get();
    if (!receiver_id) {
      throw std::runtime_error("Failed to create RAVENNA receiver: " +
                               receiver_id.error());
    }
    receiver_id_ = *receiver_id;

    node_.subscribe_to_receiver(receiver_id_, this).wait();
    node_.subscribe_to_ptp_instance(this).wait();

    tx_started_ = false;
    tx_rtp_timestamp_ = 0;
    rx_rtp_timestamp_.reset();
    rx_ptp_calibrated_ = false;
    receiver_ready_ = false;
    running_ = true;
  }

  void stop() override {
    running_ = false;
    node_.unsubscribe_from_ptp_instance(this).wait();
    if (receiver_id_.is_valid()) {
      node_.unsubscribe_from_receiver(receiver_id_, this).wait();
    }
    if (receiver_id_.is_valid()) {
      node_.remove_receiver(receiver_id_).wait();
      receiver_id_ = {};
    }
    if (sender_id_.is_valid()) {
      node_.remove_sender(sender_id_).wait();
      sender_id_ = {};
    }
  }

  void ravenna_receiver_parameters_updated(
      const rav::rtp::AudioReceiver::ReaderParameters& parameters) override {
    std::lock_guard<std::mutex> guard(parameters_mutex_);
    receiver_parameters_ = parameters;
    receiver_ready_ = parameters.is_valid() &&
                      parameters.audio_format.sample_rate ==
                          config_.sample_rate_hz &&
                      parameters.audio_format.num_channels == config_.channels;
    if (config_.verbose) {
      std::cerr << "ravennakit rx: parameters_updated"
                << " valid=" << parameters.is_valid()
                << " streams=" << parameters.streams.size()
                << " sample_rate=" << parameters.audio_format.sample_rate
                << " channels=" << parameters.audio_format.num_channels
                << " ready=" << receiver_ready_ << "\n";
    }
  }

  void push_to_network(const float* samples, std::size_t frames,
                       std::size_t channels) override {
    if (!running_ || !sender_id_.is_valid()) {
      return;
    }

    if (frames > config_.block_size || channels != config_.channels) {
      return;
    }

    auto& local_clock = get_local_clock();
    if (!tx_started_ && local_clock.is_calibrated()) {
      // Seed the RTP timeline from PTP once at stream start. After that,
      // keep the sender sample-locked by advancing one block per callback;
      // resnapping the RTP timestamp mid-stream introduces audible phase jitter.
      tx_rtp_timestamp_ =
          local_clock.now().to_rtp_timestamp32(config_.sample_rate_hz) +
          config_.tx_timestamp_offset_frames;
      tx_started_ = true;
    } else if (!tx_started_) {
      // Don't start the RTP timeline before PTP is calibrated; otherwise the
      // stream keeps running from an arbitrary zero-based timestamp and any
      // later offset-from-PTP correction never takes effect.
      return;
    }

    const auto view = make_planar_input_view(samples, frames, channels,
                                             tx_planar_storage_,
                                             tx_channel_ptrs_);
    if (config_.verbose) {
      const auto input_peak = compute_peak_abs(samples, frames * channels);
      const auto planar_peak =
          compute_peak_abs(tx_planar_storage_.data(), frames * channels);
      const auto callback_index = ++tx_debug_counter_;
      if ((callback_index % 128U) == 0U) {
        std::cerr << "ravennakit tx: callback=" << callback_index
                  << " input_peak=" << input_peak
                  << " frames=" << frames
                  << " planar_peak=" << planar_peak
                  << " ts=" << tx_rtp_timestamp_ << "\n";
      }
    }
    if (!node_.send_audio_data_realtime(sender_id_, make_const_view(view),
                                        tx_rtp_timestamp_)) {
      return;
    }
    tx_rtp_timestamp_ += static_cast<uint32_t>(frames);
  }

  void pull_from_network(float* samples, std::size_t frames,
                         std::size_t channels) override {
    const auto total = frames * channels;
    std::memset(samples, 0, total * sizeof(float));
    if (!running_ || !receiver_id_.is_valid()) {
      return;
    }
    if (frames > config_.block_size || channels != config_.channels) {
      return;
    }
    if (!receiver_ready_) {
      if (config_.verbose) {
        const auto callback_index = ++rx_debug_counter_;
        if ((callback_index % 128U) == 0U) {
          std::cerr << "ravennakit rx: callback=" << callback_index
                    << " ready=0"
                    << " frames=" << frames << "\n";
        }
      }
      return;
    }

    auto view = make_planar_output_view(frames, channels,
                                        rx_planar_storage_,
                                        rx_channel_ptrs_);

    auto& local_clock = get_local_clock();
    const auto ptp_calibrated = local_clock.is_calibrated();
    if (ptp_calibrated != rx_ptp_calibrated_) {
      rx_ptp_calibrated_ = ptp_calibrated;
      rx_rtp_timestamp_.reset();
      if (config_.verbose) {
        std::cerr << "ravennakit rx: ptp_calibrated="
                  << (ptp_calibrated ? 1 : 0)
                  << " reset_cursor=1\n";
      }
    }
    if (!ptp_calibrated) {
      if (config_.verbose) {
        const auto callback_index = ++rx_debug_counter_;
        if ((callback_index % 128U) == 0U) {
          std::cerr << "ravennakit rx: callback=" << callback_index
                    << " ready=" << receiver_ready_
                    << " frames=" << frames
                    << " ptp=0 hold=1\n";
        }
      }
      return;
    }

    const auto target_rtp_ts =
        local_clock.now().to_rtp_timestamp32(config_.sample_rate_hz) -
        config_.rx_require_delay_frames;

    auto rtp_ts = node_.read_audio_data_realtime(receiver_id_, view,
                                                 std::nullopt, std::nullopt);
    bool realigned = false;
    if (!rtp_ts) {
      rtp_ts = node_.read_audio_data_realtime(receiver_id_, view,
                                             target_rtp_ts, std::nullopt);
      realigned = rtp_ts.has_value();
    } else if (absolute_timestamp_diff(*rtp_ts, target_rtp_ts) >
                   static_cast<uint32_t>(frames * 2U)) {
      auto realigned_ts = node_.read_audio_data_realtime(
          receiver_id_, view, target_rtp_ts, std::nullopt);
      if (realigned_ts) {
        rtp_ts = realigned_ts;
        realigned = true;
      }
    }
    if (!rtp_ts) {
      if (config_.verbose) {
        const auto callback_index = ++rx_debug_counter_;
        if ((callback_index % 128U) == 0U) {
          std::cerr << "ravennakit rx: callback=" << callback_index
                    << " miss=1 ready=" << receiver_ready_
                    << " frames=" << frames
                    << " delay=" << config_.rx_require_delay_frames
                    << " ptp=" << (ptp_calibrated ? 1 : 0) << "\n";
        }
      }
      return;
    }

    const auto output_peak = compute_peak_abs(rx_planar_storage_.data(), total);
    rx_rtp_timestamp_ = *rtp_ts + static_cast<uint32_t>(frames);

    interleave_from_planar(rx_planar_storage_, frames, channels, samples);
    rx_recent_peak_ = (output_peak >= 0.02f) ? output_peak : 0.0f;
    if (config_.verbose) {
      const auto callback_index = ++rx_debug_counter_;
      if ((callback_index % 128U) == 0U) {
        std::cerr << "ravennakit rx: callback=" << callback_index
                  << " ts=" << *rtp_ts
                  << " output_peak=" << output_peak
                  << " frames=" << frames
                  << " delay=" << config_.rx_require_delay_frames
                  << " ptp=" << (ptp_calibrated ? 1 : 0)
                  << " realigned=" << (realigned ? 1 : 0) << "\n";
      }
    }
  }

 private:
  static uint32_t absolute_timestamp_diff(uint32_t lhs, uint32_t rhs) {
    return lhs > rhs ? (lhs - rhs) : (rhs - lhs);
  }

  static rav::AudioBufferView<float> make_planar_output_view(
      std::size_t frames, std::size_t channels,
      std::vector<float>& planar_storage, std::vector<float*>& channel_ptrs) {
    for (std::size_t ch = 0; ch < channels; ++ch) {
      channel_ptrs[ch] = planar_storage.data() + (ch * frames);
    }
    return {channel_ptrs.data(), channels, frames};
  }

  static rav::AudioBufferView<float> make_planar_input_view(
      const float* interleaved, std::size_t frames, std::size_t channels,
      std::vector<float>& planar_storage, std::vector<float*>& channel_ptrs) {
    for (std::size_t ch = 0; ch < channels; ++ch) {
      channel_ptrs[ch] = planar_storage.data() + (ch * frames);
      for (std::size_t frame = 0; frame < frames; ++frame) {
        channel_ptrs[ch][frame] = interleaved[frame * channels + ch];
      }
    }
    return {channel_ptrs.data(), channels, frames};
  }

  static rav::AudioBufferView<const float> make_const_view(
      const rav::AudioBufferView<float>& view) {
    return {view.data(), view.num_channels(), view.num_frames()};
  }

  static void interleave_from_planar(const std::vector<float>& planar_storage,
                                     std::size_t frames, std::size_t channels,
                                     float* interleaved) {
    for (std::size_t frame = 0; frame < frames; ++frame) {
      for (std::size_t ch = 0; ch < channels; ++ch) {
        interleaved[frame * channels + ch] =
            planar_storage[ch * frames + frame];
      }
    }
  }

  BridgeConfig config_;
  rav::RavennaNode node_;
  rav::Id sender_id_;
  rav::Id receiver_id_;
  std::mutex parameters_mutex_;
  rav::rtp::AudioReceiver::ReaderParameters receiver_parameters_ {};
  uint32_t tx_rtp_timestamp_ = 0;
  std::optional<uint32_t> rx_rtp_timestamp_;
  std::vector<float> tx_planar_storage_;
  std::vector<float> rx_planar_storage_;
  std::vector<float*> tx_channel_ptrs_;
  std::vector<float*> rx_channel_ptrs_;
  bool tx_started_ = false;
  bool rx_ptp_calibrated_ = false;
  bool receiver_ready_ = false;
  bool running_ = false;
  std::uint64_t tx_debug_counter_ = 0;
  std::uint64_t rx_debug_counter_ = 0;
  float rx_recent_peak_ = 0.0f;
};

class StubAoipEngine final : public IAoipEngine {
 public:
  explicit StubAoipEngine(BridgeConfig config) : config_(std::move(config)) {}

  void start() override {
    throw std::runtime_error(
        "RAVENNAKIT engine is not connected yet. "
        "Next step is to back IAoipEngine with rav::RavennaNode, "
        "RavennaSender and RavennaReceiver.");
  }

  void stop() override {}

  void push_to_network(const float*, std::size_t, std::size_t) override {}

  void pull_from_network(float* samples, std::size_t frames,
                         std::size_t channels) override {
    const auto total = frames * channels;
    std::memset(samples, 0, total * sizeof(float));
  }

 private:
  BridgeConfig config_;
};

#endif

}  // namespace

std::unique_ptr<IAoipEngine> create_aoip_engine(const BridgeConfig& config) {
  if (config.aoip_backend == AoipBackend::loopback) {
    return std::make_unique<LoopbackAoipEngine>(config);
  }

#if defined(PISOUND_RAVENNAKIT_BRIDGE_HAVE_RAVENNAKIT)
  if (config.aoip_backend == AoipBackend::ravennakit) {
    return std::make_unique<RavennaKitAoipEngine>(config);
  }
  return std::make_unique<StubAoipEngine>(config);
#else
  if (config.aoip_backend == AoipBackend::ravennakit) {
    throw std::runtime_error(
        "Requested --aoip-backend ravennakit, but the bridge was built "
        "without RAVENNAKIT support.");
  }
  return std::make_unique<StubAoipEngine>(config);
#endif
}

}  // namespace pisound::ravennakit_bridge
