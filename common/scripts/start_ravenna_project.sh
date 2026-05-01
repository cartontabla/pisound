#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: sudo ./common/scripts/start_ravenna_project.sh <project>

Starts a stream_in/stream_out project on the canonical AES67/RAVENNA path:
  - enables the shared AES67 runtime target
  - enables and starts pisound-stream-ravenna@<project>.service

Example:
  sudo ./common/scripts/start_ravenna_project.sh bypass2
EOF
}

if [ "${1:-}" = "--help" ] || [ "${1:-}" = "-h" ] || [ $# -ne 1 ]; then
  usage
  exit $([ $# -eq 1 ] && echo 0 || echo 1)
fi

if [ "$(id -u)" -ne 0 ]; then
  echo "Run with sudo." >&2
  exit 1
fi

PROJECT="$1"
SERVICE_CANONICAL="pisound-stream-ravenna@${PROJECT}.service"

echo "[ravenna] Starting canonical AES67 runtime..."
systemctl enable --now pisound-aes67-runtime.target

echo "[ravenna] Starting ${SERVICE_CANONICAL}..."
systemctl enable --now "${SERVICE_CANONICAL}"

echo
echo "[ravenna] Status"
systemctl is-active pisound-aes67-runtime.target
systemctl is-active "${SERVICE_CANONICAL}"

echo
echo "[ravenna] Ready: ${PROJECT}_app is now running through stream_in/out over RAVENNA."
