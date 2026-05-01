#include "pisound/ravennakit_bridge/audio_driver.hpp"
#include "pisound/ravennakit_bridge/bridge_config.hpp"
#include "pisound/ravennakit_bridge/ravennakit_engine.hpp"

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstring>
#include <iostream>
#include <thread>

namespace pisound::ravennakit_bridge {

namespace {

std::atomic<bool> g_running{true};

class BridgeApp final : public IAudioBridgeCallback {
 public:
  explicit BridgeApp(BridgeConfig config)
      : config_(std::move(config)),
        audio_(create_audio_driver(config_)),
        aoip_(create_aoip_engine(config_)) {}

  void run() {
    aoip_->start();
    audio_->start(*this);

    while (g_running.load()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    audio_->stop();
    aoip_->stop();
  }

  void push_capture_interleaved(const float* samples, std::size_t frames,
                                std::size_t channels) override {
    aoip_->push_to_network(samples, frames, channels);
  }

  void pull_playback_interleaved(float* samples, std::size_t frames,
                                 std::size_t channels) override {
    aoip_->pull_from_network(samples, frames, channels);
  }

 private:
  BridgeConfig config_;
  std::unique_ptr<IDuplexAudioDriver> audio_;
  std::unique_ptr<IAoipEngine> aoip_;
};

void handle_signal(int) { g_running.store(false); }

void print_help(const char* argv0) {
  std::cout
      << "Usage: " << argv0 << " \\\n"
      << "  --audio-backend <portaudio|null> \\\n"
      << "  --input-device <name> \\\n"
      << "  --output-device <name> \\\n"
      << "  --network-interface <ifname> \\\n"
      << "  --aoip-backend <stub|loopback|ravennakit> \\\n"
      << "  --tx-destination <ip> [options]\n\n"
      << "Options:\n"
      << "  --audio-backend <name>      default portaudio\n"
      << "  --input-start-channel <n>   default 1\n"
      << "  --output-start-channel <n>  default 3\n"
      << "  --sample-rate <hz>          default 48000\n"
      << "  --block-size <frames>       default 128\n"
      << "  --channels <n>              default 2\n"
      << "  --tx-packet-time-num <n>    default 1\n"
      << "  --tx-packet-time-den <n>    default 1\n"
      << "  --tx-timestamp-offset-frames <n> default 0\n"
      << "  --tx-session-name <name>    default <session>-tx\n"
      << "  --rx-session-name <name>    default <session>-rx\n"
      << "  --rx-require-delay-frames <n> auto >= 2 blocks\n"
      << "  --tx-port <port>            default 5004\n"
      << "  --rx-port <port>            default 5004\n"
      << "  --payload-type <pt>         default 96\n"
      << "  --ttl <n>                   default 1\n"
      << "  --rx-bind-address <ip>      default 0.0.0.0\n"
      << "  --session-name <name>       default pisound-bridge\n"
      << "  --list-devices\n"
      << "  --verbose\n";
}

}  // namespace

}  // namespace pisound::ravennakit_bridge

int main(int argc, char** argv) {
  using namespace pisound::ravennakit_bridge;

  const auto parsed = parse_args(argc, argv);
  if (parsed.show_help) {
    print_help(argv[0]);
    return 0;
  }
  if (parsed.list_devices) {
    std::cout << list_audio_devices();
    return 0;
  }

  if (!parsed.ok) {
    if (!parsed.error.empty()) {
      std::cerr << "pisound_ravennakit_bridge: " << parsed.error << "\n\n";
    }
    print_help(argv[0]);
    return 1;
  }

  std::signal(SIGINT, handle_signal);
  std::signal(SIGTERM, handle_signal);

  std::cout << "pisound_ravennakit_bridge: " << describe_config(parsed.config)
            << "\n";

  try {
    BridgeApp app(parsed.config);
    app.run();
    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "pisound_ravennakit_bridge: fatal: " << ex.what() << "\n";
    return 2;
  }
}
