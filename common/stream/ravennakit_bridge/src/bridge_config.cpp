#include "pisound/ravennakit_bridge/bridge_config.hpp"

#include <algorithm>
#include <sstream>
#include <string_view>

namespace pisound::ravennakit_bridge {

namespace {

bool is_flag(std::string_view arg, std::string_view short_name,
             std::string_view long_name) {
  return arg == short_name || arg == long_name;
}

bool require_value(int argc, char** argv, int& index, std::string& out,
                   ParseResult& result) {
  if (index + 1 >= argc) {
    result.error = "missing value after " + std::string(argv[index]);
    return false;
  }
  out = argv[++index];
  return true;
}

bool require_u16(int argc, char** argv, int& index, std::uint16_t& out,
                 ParseResult& result) {
  std::string value;
  if (!require_value(argc, argv, index, value, result)) {
    return false;
  }
  try {
    const auto parsed = static_cast<unsigned long>(std::stoul(value));
    if (parsed > 65535UL) {
      result.error = "value out of range for " + value;
      return false;
    }
    out = static_cast<std::uint16_t>(parsed);
    return true;
  } catch (...) {
    result.error = "invalid numeric value: " + value;
    return false;
  }
}

bool require_u8(int argc, char** argv, int& index, std::uint8_t& out,
                ParseResult& result) {
  std::uint16_t value = 0;
  if (!require_u16(argc, argv, index, value, result)) {
    return false;
  }
  if (value > 255) {
    result.error = "value out of range";
    return false;
  }
  out = static_cast<std::uint8_t>(value);
  return true;
}

bool require_u32(int argc, char** argv, int& index, std::uint32_t& out,
                 ParseResult& result) {
  std::string value;
  if (!require_value(argc, argv, index, value, result)) {
    return false;
  }
  try {
    out = static_cast<std::uint32_t>(std::stoul(value));
    return true;
  } catch (...) {
    result.error = "invalid numeric value: " + value;
    return false;
  }
}

bool parse_aoip_backend(std::string_view value, AoipBackend& backend) {
  if (value == "stub") {
    backend = AoipBackend::stub;
    return true;
  }
  if (value == "loopback") {
    backend = AoipBackend::loopback;
    return true;
  }
  if (value == "ravennakit") {
    backend = AoipBackend::ravennakit;
    return true;
  }
  return false;
}

bool parse_audio_backend(std::string_view value, AudioBackend& backend) {
  if (value == "portaudio") {
    backend = AudioBackend::portaudio;
    return true;
  }
  if (value == "null") {
    backend = AudioBackend::null_backend;
    return true;
  }
  return false;
}

}  // namespace

ParseResult parse_args(int argc, char** argv) {
  ParseResult result;
  result.ok = false;

  for (int i = 1; i < argc; ++i) {
    const std::string_view arg = argv[i];

    if (is_flag(arg, "-h", "--help")) {
      result.show_help = true;
      result.ok = true;
      return result;
    }
    if (is_flag(arg, "", "--list-devices")) {
      result.list_devices = true;
      result.ok = true;
      return result;
    }
    if (is_flag(arg, "", "--verbose")) {
      result.config.verbose = true;
      continue;
    }
    if (is_flag(arg, "", "--input-device")) {
      if (!require_value(argc, argv, i, result.config.input_device, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--output-device")) {
      if (!require_value(argc, argv, i, result.config.output_device, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--network-interface")) {
      if (!require_value(argc, argv, i, result.config.network_interface, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--tx-destination")) {
      if (!require_value(argc, argv, i, result.config.tx_destination, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--rx-bind-address")) {
      if (!require_value(argc, argv, i, result.config.rx_bind_address, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--session-name")) {
      if (!require_value(argc, argv, i, result.config.session_name, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--tx-session-name")) {
      if (!require_value(argc, argv, i, result.config.tx_session_name, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--rx-session-name")) {
      if (!require_value(argc, argv, i, result.config.rx_session_name, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--aoip-backend")) {
      std::string value;
      if (!require_value(argc, argv, i, value, result)) {
        return result;
      }
      if (!parse_aoip_backend(value, result.config.aoip_backend)) {
        result.error = "invalid --aoip-backend: " + value;
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--audio-backend")) {
      std::string value;
      if (!require_value(argc, argv, i, value, result)) {
        return result;
      }
      if (!parse_audio_backend(value, result.config.audio_backend)) {
        result.error = "invalid --audio-backend: " + value;
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--sample-rate")) {
      if (!require_u32(argc, argv, i, result.config.sample_rate_hz, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--block-size")) {
      if (!require_u32(argc, argv, i, result.config.block_size, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--rx-require-delay-frames")) {
      if (!require_u32(argc, argv, i, result.config.rx_require_delay_frames, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--tx-timestamp-offset-frames")) {
      if (!require_u32(argc, argv, i, result.config.tx_timestamp_offset_frames, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--tx-packet-time-num")) {
      if (!require_u8(argc, argv, i, result.config.tx_packet_time_num, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--tx-packet-time-den")) {
      if (!require_u8(argc, argv, i, result.config.tx_packet_time_den, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--channels")) {
      if (!require_u16(argc, argv, i, result.config.channels, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--input-start-channel")) {
      if (!require_u16(argc, argv, i, result.config.input_start_channel, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--output-start-channel")) {
      if (!require_u16(argc, argv, i, result.config.output_start_channel, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--tx-port")) {
      if (!require_u16(argc, argv, i, result.config.tx_port, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--rx-port")) {
      if (!require_u16(argc, argv, i, result.config.rx_port, result)) {
        return result;
      }
      continue;
    }
    if (is_flag(arg, "", "--ttl")) {
      std::uint16_t ttl = 0;
      if (!require_u16(argc, argv, i, ttl, result)) {
        return result;
      }
      if (ttl > 255) {
        result.error = "ttl must be <= 255";
        return result;
      }
      result.config.ttl = static_cast<std::uint8_t>(ttl);
      continue;
    }
    if (is_flag(arg, "", "--payload-type")) {
      std::uint16_t payload_type = 0;
      if (!require_u16(argc, argv, i, payload_type, result)) {
        return result;
      }
      if (payload_type > 127) {
        result.error = "payload type must be <= 127";
        return result;
      }
      result.config.payload_type = static_cast<std::uint8_t>(payload_type);
      continue;
    }

    result.error = "unknown argument: " + std::string(arg);
    return result;
  }

  if (result.config.audio_backend != AudioBackend::null_backend &&
      result.config.input_device.empty()) {
    result.error = "missing --input-device";
    return result;
  }
  if (result.config.network_interface.empty()) {
    result.error = "missing --network-interface";
    return result;
  }
  if (result.config.aoip_backend != AoipBackend::loopback &&
      result.config.tx_destination.empty()) {
    result.error = "missing --tx-destination";
    return result;
  }
  if (result.config.channels == 0 || result.config.block_size == 0 ||
      result.config.sample_rate_hz == 0) {
    result.error = "sample rate, block size and channel count must be > 0";
    return result;
  }
  if (result.config.tx_packet_time_num == 0 ||
      result.config.tx_packet_time_den == 0) {
    result.error = "tx packet time numerator and denominator must be > 0";
    return result;
  }

  if (result.config.tx_session_name.empty()) {
    result.config.tx_session_name = result.config.session_name + "-tx";
  }
  if (result.config.rx_session_name.empty()) {
    result.config.rx_session_name = result.config.session_name + "-rx";
  }
  if (result.config.rx_require_delay_frames == 0) {
    result.config.rx_require_delay_frames =
        std::max<std::uint32_t>(result.config.block_size * 2U,
                                result.config.sample_rate_hz / 200U);
  }

  result.ok = true;
  return result;
}

std::string describe_config(const BridgeConfig& config) {
  std::ostringstream oss;

  oss << "input=" << config.input_device << ":" << config.input_start_channel
      << "-" << (config.input_start_channel + config.channels - 1)
      << " output=";
  if (config.output_device.empty()) {
    oss << "disabled";
  } else {
    oss << config.output_device << ":" << config.output_start_channel
        << "-" << (config.output_start_channel + config.channels - 1);
  }
  oss
      << " audio=" << to_string(config.audio_backend)
      << " backend=" << to_string(config.aoip_backend)
      << " if=" << config.network_interface
      << " tx=" << config.tx_destination << ":" << config.tx_port
      << " rx=" << config.rx_bind_address << ":" << config.rx_port
      << " Fs=" << config.sample_rate_hz
      << " N=" << config.block_size
      << " ch=" << config.channels
      << " session=" << config.session_name
      << " tx_session=" << config.tx_session_name
      << " rx_session=" << config.rx_session_name
      << " tx_ptime=" << static_cast<unsigned>(config.tx_packet_time_num)
      << "/" << static_cast<unsigned>(config.tx_packet_time_den)
      << " tx_ts_offset=" << config.tx_timestamp_offset_frames
      << " rx_delay=" << config.rx_require_delay_frames
      << " pt=" << static_cast<unsigned>(config.payload_type)
      << " ttl=" << static_cast<unsigned>(config.ttl);
  return oss.str();
}

const char* to_string(AoipBackend backend) {
  switch (backend) {
    case AoipBackend::stub:
      return "stub";
    case AoipBackend::loopback:
      return "loopback";
    case AoipBackend::ravennakit:
      return "ravennakit";
  }
  return "unknown";
}

const char* to_string(AudioBackend backend) {
  switch (backend) {
    case AudioBackend::portaudio:
      return "portaudio";
    case AudioBackend::null_backend:
      return "null";
  }
  return "unknown";
}

}  // namespace pisound::ravennakit_bridge
