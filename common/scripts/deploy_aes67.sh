#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: sudo ./deploy_aes67.sh [--skip-apt] [--no-enable] [--enable-direct-bridge]

Deploys the shared AES67-oriented stream infrastructure from the pisound repo:
  - installs linuxptp (unless --skip-apt)
  - builds common/stream/aes67_bridge
  - installs ALSA config and systemd units
  - installs Pi-side AES67 session restore defaults for Pisound_In/Pisound_Out
  - installs the bridge binary to /usr/local/bin
  - optionally enables and starts the canonical RAVENNA services
  - keeps the direct RTP bridge disabled unless --enable-direct-bridge is used
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
ENABLE_DIRECT_BRIDGE=0

for arg in "$@"; do
  case "$arg" in
    --skip-apt)
      SKIP_APT=1
      ;;
    --no-enable)
      ENABLE_SERVICES=0
      ;;
    --enable-direct-bridge)
      ENABLE_DIRECT_BRIDGE=1
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

if [ ! -f "${STREAM_DIR}/aes67_bridge.c" ]; then
  echo "Missing ${STREAM_DIR}/aes67_bridge.c" >&2
  exit 1
fi

echo "[aes67] Using stream directory: ${STREAM_DIR}"

if [ "${SKIP_APT}" -eq 0 ]; then
  echo "[aes67] Installing linuxptp..."
  apt-get update
  DEBIAN_FRONTEND=noninteractive apt-get install -y linuxptp
else
  echo "[aes67] Skipping apt install as requested."
fi

echo "[aes67] Building bridge..."
"${STREAM_DIR}/build_bridge.sh"

echo "[aes67] Installing ALSA and systemd configuration..."
"${STREAM_DIR}/setup_pi.sh"

if [ -f "${STREAM_DIR}/pisound-stream-app@.service" ]; then
  echo "[aes67] Installing stream app service template..."
  install -m 0644 "${STREAM_DIR}/pisound-stream-app@.service" /etc/systemd/system/pisound-stream-app@.service
fi

if [ -f "${STREAM_DIR}/pisound-stream-ravenna@.service" ]; then
  echo "[aes67] Installing direct RAVENNA stream app service template..."
  install -m 0644 "${STREAM_DIR}/pisound-stream-ravenna@.service" /etc/systemd/system/pisound-stream-ravenna@.service
fi

if [ -f "${STREAM_DIR}/pisound-aes67-runtime.target" ]; then
  echo "[aes67] Installing canonical AES67 runtime target..."
  install -m 0644 "${STREAM_DIR}/pisound-aes67-runtime.target" /etc/systemd/system/pisound-aes67-runtime.target
fi

echo "[aes67] Installing bridge binary..."
install -m 0755 "${STREAM_DIR}/aes67_bridge" /usr/local/bin/pisound_aes67_bridge

if [ "${ENABLE_SERVICES}" -eq 1 ]; then
  echo "[aes67] Enabling and starting services..."
  systemctl enable --now pisound-aes67-runtime.target
  if [ "${ENABLE_DIRECT_BRIDGE}" -eq 1 ]; then
    systemctl enable --now pisound-aes67-bridge
  else
    systemctl disable --now pisound-aes67-bridge || true
  fi
  systemctl restart avahi-daemon || true
else
  echo "[aes67] Leaving services disabled/stopped as requested."
fi

echo
echo "[aes67] Verification"
aplay -L | grep -E '^aes67$|aes67' || true
arecord -L | grep -E '^aes67$|aes67' || true
echo "---"
systemctl --no-pager --full status pisound-aes67-runtime.target ptp4l-aes67 phc2sys-aes67 pisound-ravenna-http pisound-ravenna-rtsp pisound-aes67-sessions pisound-aes67-bridge \
  | sed -n '1,120p' || true

echo
echo "[aes67] Done."
