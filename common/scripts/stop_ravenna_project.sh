#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: sudo ./common/scripts/stop_ravenna_project.sh <project>

Stops a stream_in/stream_out project running on the canonical AES67/RAVENNA path:
  - stops pisound-stream-ravenna@<project>.service

Example:
  sudo ./common/scripts/stop_ravenna_project.sh bypass2
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

echo "[ravenna] Stopping ${SERVICE_CANONICAL}..."
systemctl stop "${SERVICE_CANONICAL}"

echo
echo "[ravenna] Status"
systemctl is-active "${SERVICE_CANONICAL}" || true

echo
echo "[ravenna] Stopped: ${PROJECT}_app is no longer running over RAVENNA."
