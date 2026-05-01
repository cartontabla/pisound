#ifndef PISOUND_RAVENNAKIT_BRIDGE_RAVENNAKIT_ENGINE_HPP
#define PISOUND_RAVENNAKIT_BRIDGE_RAVENNAKIT_ENGINE_HPP

#include <cstddef>
#include <memory>

#include "bridge_config.hpp"

namespace pisound::ravennakit_bridge {

class IAoipEngine {
 public:
  virtual ~IAoipEngine() = default;

  virtual void start() = 0;
  virtual void stop() = 0;

  virtual void push_to_network(const float* samples, std::size_t frames,
                               std::size_t channels) = 0;
  virtual void pull_from_network(float* samples, std::size_t frames,
                                 std::size_t channels) = 0;
};

/*
 * Factory contract:
 * - with RAVENNAKIT disabled, returns a stub engine that fails loudly on start()
 * - with RAVENNAKIT enabled, returns an engine backed by rav::RavennaNode
 *   and the sender/receiver objects needed for a duplex bridge
 */
std::unique_ptr<IAoipEngine> create_aoip_engine(const BridgeConfig& config);

}  // namespace pisound::ravennakit_bridge

#endif
