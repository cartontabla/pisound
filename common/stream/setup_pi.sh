#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [ "$(id -u)" -ne 0 ]; then
  echo "Run with sudo."
  exit 1
fi

install_if_missing() {
  local src="$1"
  local dst="$2"
  local mode="$3"

  if [ ! -f "${src}" ]; then
    echo "Missing required framework file: ${src}" >&2
    exit 1
  fi
  if [ -e "${dst}" ]; then
    echo "Keeping existing config: ${dst}"
  else
    install -m "${mode}" "${src}" "${dst}"
  fi
}

install_required() {
  local src="$1"
  local dst="$2"
  local mode="$3"

  if [ ! -f "${src}" ]; then
    echo "Missing required framework file: ${src}" >&2
    exit 1
  fi
  install -m "${mode}" "${src}" "${dst}"
}

install -d /etc/pisound
install -d /etc/pisound/ravenna
install -d /etc/default
install -d /etc/systemd/system
install -d /usr/local/bin

install_if_missing "${SCRIPT_DIR}/pisound-aes67.env.example" /etc/default/pisound-aes67 0644
install_if_missing "${SCRIPT_DIR}/pisound-aes67-sessions.env.example" /etc/default/pisound-aes67-sessions 0644
install_required "${SCRIPT_DIR}/ravenna-index.html" /etc/pisound/ravenna/index.html 0644
install_required "${SCRIPT_DIR}/pisound-aes67-runtime.target" /etc/systemd/system/pisound-aes67-runtime.target 0644
install_required "${SCRIPT_DIR}/pisound-aes67-sessions.service" /etc/systemd/system/pisound-aes67-sessions.service 0644
install_required "${SCRIPT_DIR}/ptp4l-aes67.conf" /etc/pisound/ptp4l-aes67.conf 0644
install_required "${SCRIPT_DIR}/ptp4l-aes67.service" /etc/systemd/system/ptp4l-aes67.service 0644
install_required "${SCRIPT_DIR}/phc2sys-aes67.service" /etc/systemd/system/phc2sys-aes67.service 0644
install_required "${SCRIPT_DIR}/ptp4l-aes67.sh" /usr/local/bin/pisound_ptp4l_aes67 0755
install_required "${SCRIPT_DIR}/phc2sys-aes67.sh" /usr/local/bin/pisound_phc2sys_aes67 0755
install_required "${SCRIPT_DIR}/ravenna-http.service" /etc/systemd/system/pisound-ravenna-http.service 0644
install_required "${SCRIPT_DIR}/pisound-ravenna-rtsp.service" /etc/systemd/system/pisound-ravenna-rtsp.service 0644
install_required "${SCRIPT_DIR}/pisound-stream-ravenna@.service" /etc/systemd/system/pisound-stream-ravenna@.service 0644
install -d /etc/avahi/services
install_required "${SCRIPT_DIR}/pisound-ravenna.service.xml" /etc/avahi/services/pisound-ravenna.service 0644
install_required "${SCRIPT_DIR}/pisound-ravenna-rtsp.service.xml" /etc/avahi/services/pisound-ravenna-rtsp.service 0644

if [ -f "${SCRIPT_DIR}/99-pisound-ptp-permissions.rules.example" ]; then
  install -d /etc/udev/rules.d
  install_required "${SCRIPT_DIR}/99-pisound-ptp-permissions.rules.example" /etc/udev/rules.d/99-pisound-ptp-permissions.rules 0644
fi

systemctl daemon-reload

echo "Installed:"
echo "  /etc/default/pisound-aes67"
echo "  /etc/default/pisound-aes67-sessions"
echo "  /etc/pisound/ptp4l-aes67.conf"
echo "  /etc/pisound/ravenna/index.html"
echo "  /etc/systemd/system/pisound-aes67-runtime.target"
echo "  /etc/systemd/system/pisound-aes67-sessions.service"
echo "  /etc/systemd/system/ptp4l-aes67.service"
echo "  /etc/systemd/system/phc2sys-aes67.service"
echo "  /usr/local/bin/pisound_ptp4l_aes67"
echo "  /usr/local/bin/pisound_phc2sys_aes67"
echo "  /etc/systemd/system/pisound-ravenna-http.service"
echo "  /etc/systemd/system/pisound-ravenna-rtsp.service"
echo "  /etc/systemd/system/pisound-stream-ravenna@.service"
echo "  /etc/avahi/services/pisound-ravenna.service"
echo "  /etc/avahi/services/pisound-ravenna-rtsp.service"
echo
echo "Next:"
echo "  1. Verify aes67-daemon.service is installed by the RAVENNA/AES67 stack."
echo "  2. Start canonical runtime: systemctl enable --now pisound-aes67-runtime.target"
echo "  3. Compile a generated project: common/scripts/compile.sh <project>"
echo "  4. Run it: systemctl enable --now pisound-stream-ravenna@<project>.service"
