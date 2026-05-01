#ifndef PISOUND_RAVENNAKIT_BRIDGE_CONFIG_HPP
#define PISOUND_RAVENNAKIT_BRIDGE_CONFIG_HPP

#include <cstdint>
#include <string>

namespace pisound::ravennakit_bridge {

enum class AoipBackend {
  stub,
  loopback,
  ravennakit,
};

enum class AudioBackend {
  portaudio,
  null_backend,
};

struct BridgeConfig {
  std::string input_device;
  std::string output_device;
  std::string network_interface;
  std::string tx_destination;
  std::string rx_bind_address = "0.0.0.0";
  std::string session_name = "pisound-bridge";
  std::string tx_session_name;
  std::string rx_session_name;
  std::uint32_t sample_rate_hz = 48000;
  std::uint32_t block_size = 128;
  std::uint32_t rx_require_delay_frames = 0;
  std::uint32_t tx_timestamp_offset_frames = 0;
  std::uint8_t tx_packet_time_num = 1;
  std::uint8_t tx_packet_time_den = 1;
  std::uint16_t channels = 2;
  std::uint16_t input_start_channel = 1;
  std::uint16_t output_start_channel = 3;
  std::uint16_t tx_port = 5004;
  std::uint16_t rx_port = 5004;
  std::uint8_t ttl = 1;
  std::uint8_t payload_type = 96;
  AoipBackend aoip_backend = AoipBackend::stub;
  AudioBackend audio_backend = AudioBackend::portaudio;
  bool verbose = false;
};

struct ParseResult {
  bool ok = false;
  bool show_help = false;
  bool list_devices = false;
  std::string error;
  BridgeConfig config;
};

ParseResult parse_args(int argc, char** argv);
std::string describe_config(const BridgeConfig& config);
const char* to_string(AoipBackend backend);
const char* to_string(AudioBackend backend);

}  // namespace pisound::ravennakit_bridge

#endif
