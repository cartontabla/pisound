#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: sudo ./common/scripts/run_ravenna_project_foreground.sh <project>

Runs a stream_in/stream_out project in the foreground on the canonical
AES67/RAVENNA path:
  - ensures the shared AES67 runtime target is running
  - executes <project>_app directly with the RAVENNA ALSA environment
  - stays attached to the terminal until the app exits with q or Ctrl-C

Example:
  sudo ./common/scripts/run_ravenna_project_foreground.sh bypass2
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
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/../.." && pwd)"
PROJECT_DIR="${ROOT_DIR}/${PROJECT}"
APP_PATH="${PROJECT_DIR}/${PROJECT}_app"
SERVICE_CANONICAL="pisound-stream-ravenna@${PROJECT}.service"

if [ ! -x "${APP_PATH}" ]; then
  echo "Missing app: ${APP_PATH}" >&2
  echo "Compile it first with: common/scripts/compile.sh ${PROJECT}" >&2
  exit 1
fi

if systemctl is-active --quiet "${SERVICE_CANONICAL}"; then
  echo "[ravenna] Stopping background service ${SERVICE_CANONICAL} first..."
  systemctl stop "${SERVICE_CANONICAL}"
fi

echo "[ravenna] Running ${PROJECT}_app in foreground over RAVENNA..."
cd "${PROJECT_DIR}"
exec "${APP_PATH}"
