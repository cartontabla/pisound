#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: sudo ./deploy_aes67.sh [--skip-apt] [--no-enable]

Deploys the shared AES67-oriented stream infrastructure from the pisound repo:
  - installs the Pi-side framework requirements (unless --skip-apt)
  - installs systemd units, defaults, PTP scripts, and Avahi service files
  - installs Pi-side AES67 session restore defaults for Pisound_In/Pisound_Out
  - optionally enables and starts the canonical RAVENNA services
  - publishes a minimal RAVENNA-like Bonjour presence via Avahi

Run this on the target Pi after syncing the repo.
EOF
}

if [ "${1:-}" = "--help" ] || [ "${1:-}" = "-h" ]; then
  usage
  exit 0
fi

SKIP_APT=0
ENABLE_SERVICES=1

for arg in "$@"; do
  case "$arg" in
    --skip-apt)
      SKIP_APT=1
      ;;
    --no-enable)
      ENABLE_SERVICES=0
      ;;
    --enable-direct-bridge)
      echo "[aes67] --enable-direct-bridge is obsolete; canonical runtime uses RAVENNA ALSA." >&2
      ;;
    *)
      echo "Unknown option: $arg" >&2
      usage >&2
      exit 1
      ;;
  esac
done

if [ "$(id -u)" -ne 0 ]; then
  echo "Run with sudo." >&2
  exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
COMMON_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
STREAM_DIR="${COMMON_DIR}/stream"

if [ ! -d "${STREAM_DIR}" ]; then
  echo "Cannot find ${STREAM_DIR}" >&2
  exit 1
fi

echo "[aes67] Using stream directory: ${STREAM_DIR}"

if [ "${SKIP_APT}" -eq 0 ]; then
  echo "[aes67] Installing framework packages..."
  apt-get update
  DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    libasound2-dev \
    libjack-jackd2-dev \
    jackd2 \
    alsa-utils \
    psmisc \
    linuxptp \
    python3 \
    python3-dbus \
    python3-gi \
    avahi-daemon \
    curl \
    kmod \
    rfkill \
    bluez
else
  echo "[aes67] Skipping apt install as requested."
fi

echo "[aes67] Installing ALSA and systemd configuration..."
"${STREAM_DIR}/setup_pi.sh"

if [ "${ENABLE_SERVICES}" -eq 1 ]; then
  echo "[aes67] Enabling and starting services..."
  systemctl enable --now pisound-aes67-runtime.target
  systemctl restart avahi-daemon || true
else
  echo "[aes67] Leaving services disabled/stopped as requested."
fi

echo
echo "[aes67] Verification"
if ! systemctl list-unit-files aes67-daemon.service >/dev/null 2>&1; then
  echo "[aes67] WARNING: aes67-daemon.service is not installed; install/start the RAVENNA ALSA stack before running stream projects." >&2
fi
aplay -L | grep -E 'RAVENNA|aes67' || true
arecord -L | grep -E 'RAVENNA|aes67' || true
echo "---"
systemctl --no-pager --full status pisound-aes67-runtime.target ptp4l-aes67 phc2sys-aes67 pisound-ravenna-http pisound-ravenna-rtsp pisound-aes67-sessions \
  | sed -n '1,120p' || true

echo
echo "[aes67] Done."
