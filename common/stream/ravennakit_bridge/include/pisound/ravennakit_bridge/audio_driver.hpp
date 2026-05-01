#ifndef PISOUND_RAVENNAKIT_BRIDGE_AUDIO_DRIVER_HPP
#define PISOUND_RAVENNAKIT_BRIDGE_AUDIO_DRIVER_HPP

#include <cstddef>
#include <memory>

#include "bridge_config.hpp"

namespace pisound::ravennakit_bridge {

class IAudioBridgeCallback {
 public:
  virtual ~IAudioBridgeCallback() = default;

  virtual void push_capture_interleaved(const float* samples, std::size_t frames,
                                        std::size_t channels) = 0;
  virtual void pull_playback_interleaved(float* samples, std::size_t frames,
                                         std::size_t channels) = 0;
};

class IDuplexAudioDriver {
 public:
  virtual ~IDuplexAudioDriver() = default;

  virtual void start(IAudioBridgeCallback& callback) = 0;
  virtual void stop() = 0;
};

std::unique_ptr<IDuplexAudioDriver> create_audio_driver(const BridgeConfig& config);
std::string list_audio_devices();

}  // namespace pisound::ravennakit_bridge

#endif
