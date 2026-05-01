#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RUN_SCRIPT="${SCRIPT_DIR}/run_ravennakit_bridge_mac.sh"
RUNTIME_DIR="${RAVENNAKIT_BRIDGE_RUNTIME_DIR:-${SCRIPT_DIR}/.runtime/ravennakit_bridge_mac}"
PID_FILE="${RUNTIME_DIR}/bridge.pid"
LOG_FILE="${RUNTIME_DIR}/bridge.log"
BRIDGE_BASENAME="pisound_ravennakit_bridge"
BRIDGE_PATTERN="[p]isound_ravennakit_bridge"

mkdir -p "${RUNTIME_DIR}"

EXISTING_PIDS="$(pgrep -x "${BRIDGE_BASENAME}" 2>/dev/null || pgrep -f "${BRIDGE_PATTERN}" 2>/dev/null || true)"
if [ -n "${EXISTING_PIDS}" ]; then
  echo "[mac-bridge] Refusing to start: existing bridge instance(s) detected:"
  echo "${EXISTING_PIDS}" | tr ' ' '\n'
  echo "[mac-bridge] Stop them first with common/stream/stop_ravennakit_bridge_mac.sh"
  exit 1
fi

if [ -f "${PID_FILE}" ]; then
  PID="$(cat "${PID_FILE}")"
  if [ -n "${PID}" ] && kill -0 "${PID}" 2>/dev/null; then
    echo "[mac-bridge] Already running (pid ${PID})."
    echo "[mac-bridge] Log: ${LOG_FILE}"
    exit 0
  fi
  rm -f "${PID_FILE}"
fi

echo "[mac-bridge] Starting..."
if [ "$(uname -s)" != "Darwin" ] && command -v setsid >/dev/null 2>&1; then
  setsid "${RUN_SCRIPT}" </dev/null >>"${LOG_FILE}" 2>&1 &
else
  nohup "${RUN_SCRIPT}" </dev/null >>"${LOG_FILE}" 2>&1 &
fi
PID=$!
echo "${PID}" > "${PID_FILE}"
sleep 1

if kill -0 "${PID}" 2>/dev/null; then
  echo "[mac-bridge] Running (pid ${PID})."
  echo "[mac-bridge] Log: ${LOG_FILE}"
  tail -n 10 "${LOG_FILE}" || true
else
  echo "[mac-bridge] Failed to stay up. Last log lines:" >&2
  tail -n 40 "${LOG_FILE}" >&2 || true
  rm -f "${PID_FILE}"
  exit 1
fi
