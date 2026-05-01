#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [ "$(id -u)" -ne 0 ]; then
  echo "Run with sudo."
  exit 1
fi

install -d /etc/pisound
install -d /etc/pisound/ravenna
install -m 0644 "${SCRIPT_DIR}/aes67-bridge.conf.example" /etc/pisound/aes67-bridge.conf
install -m 0644 "${SCRIPT_DIR}/pisound-aes67.env.example" /etc/default/pisound-aes67
install -m 0644 "${SCRIPT_DIR}/pisound-aes67-sessions.env.example" /etc/default/pisound-aes67-sessions
install -m 0644 "${SCRIPT_DIR}/asound-aes67.conf" /etc/asound.conf
install -m 0644 "${SCRIPT_DIR}/ravenna-index.html" /etc/pisound/ravenna/index.html
install -m 0644 "${SCRIPT_DIR}/aes67-bridge.service" /etc/systemd/system/pisound-aes67-bridge.service
install -m 0644 "${SCRIPT_DIR}/pisound-aes67-runtime.target" /etc/systemd/system/pisound-aes67-runtime.target
install -m 0644 "${SCRIPT_DIR}/pisound-aes67-sessions.service" /etc/systemd/system/pisound-aes67-sessions.service
install -m 0644 "${SCRIPT_DIR}/ptp4l-aes67.conf" /etc/pisound/ptp4l-aes67.conf
install -m 0644 "${SCRIPT_DIR}/ptp4l-aes67.service" /etc/systemd/system/ptp4l-aes67.service
install -m 0644 "${SCRIPT_DIR}/phc2sys-aes67.service" /etc/systemd/system/phc2sys-aes67.service
install -m 0755 "${SCRIPT_DIR}/ptp4l-aes67.sh" /usr/local/bin/pisound_ptp4l_aes67
install -m 0755 "${SCRIPT_DIR}/phc2sys-aes67.sh" /usr/local/bin/pisound_phc2sys_aes67
install -m 0644 "${SCRIPT_DIR}/ravenna-http.service" /etc/systemd/system/pisound-ravenna-http.service
install -m 0644 "${SCRIPT_DIR}/pisound-ravenna-rtsp.service" /etc/systemd/system/pisound-ravenna-rtsp.service
install -m 0755 "${SCRIPT_DIR}/pisound_ravenna_rtsp_server.py" /home/patch/pisound/common/stream/pisound_ravenna_rtsp_server.py
install -d /etc/avahi/services
install -m 0644 "${SCRIPT_DIR}/pisound-ravenna.service.xml" /etc/avahi/services/pisound-ravenna.service
install -m 0644 "${SCRIPT_DIR}/pisound-ravenna-rtsp.service.xml" /etc/avahi/services/pisound-ravenna-rtsp.service

modprobe snd-aloop
if [ ! -f /etc/modules-load.d/snd-aloop.conf ]; then
  printf '%s\n' "snd-aloop" > /etc/modules-load.d/snd-aloop.conf
fi

systemctl daemon-reload

echo "Installed:"
echo "  /etc/asound.conf"
echo "  /etc/pisound/aes67-bridge.conf"
echo "  /etc/default/pisound-aes67"
echo "  /etc/default/pisound-aes67-sessions"
echo "  /etc/pisound/ptp4l-aes67.conf"
echo "  /etc/pisound/ravenna/index.html"
echo "  /etc/systemd/system/pisound-aes67-bridge.service"
echo "  /etc/systemd/system/pisound-aes67-runtime.target"
echo "  /etc/systemd/system/pisound-aes67-sessions.service"
echo "  /etc/systemd/system/ptp4l-aes67.service"
echo "  /etc/systemd/system/phc2sys-aes67.service"
echo "  /usr/local/bin/pisound_ptp4l_aes67"
echo "  /usr/local/bin/pisound_phc2sys_aes67"
echo "  /etc/systemd/system/pisound-ravenna-http.service"
echo "  /etc/systemd/system/pisound-ravenna-rtsp.service"
echo "  /etc/avahi/services/pisound-ravenna.service"
echo "  /etc/avahi/services/pisound-ravenna-rtsp.service"
echo
echo "Next:"
echo "  1. Build the bridge binary with build_bridge.sh"
echo "  2. Copy it to /usr/local/bin/pisound_aes67_bridge"
echo "  3. Start PTP: systemctl enable --now ptp4l-aes67 phc2sys-aes67"
echo "  4. Start discovery HTTP: systemctl enable --now pisound-ravenna-http"
echo "  4b. Start RTSP session advert: systemctl enable --now pisound-ravenna-rtsp"
echo "  5. Start session restore: systemctl enable --now pisound-aes67-sessions"
echo "  6. Restart avahi-daemon if needed"
echo "  7. Canonical runtime: systemctl enable --now pisound-aes67-runtime.target"
echo "  8. Optional only: start the direct RTP bridge: systemctl enable --now pisound-aes67-bridge"
