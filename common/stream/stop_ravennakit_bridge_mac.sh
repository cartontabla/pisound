#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RUNTIME_DIR="${RAVENNAKIT_BRIDGE_RUNTIME_DIR:-${SCRIPT_DIR}/.runtime/ravennakit_bridge_mac}"
PID_FILE="${RUNTIME_DIR}/bridge.pid"
BRIDGE_BASENAME="pisound_ravennakit_bridge"
BRIDGE_PATTERN="[p]isound_ravennakit_bridge"

stop_pid() {
  local pid="$1"
  local label="$2"

  if [ -z "${pid}" ]; then
    return 0
  fi
  if ! kill -0 "${pid}" 2>/dev/null; then
    return 0
  fi

  echo "[mac-bridge] Stopping ${label} pid ${pid}..."
  kill "${pid}" 2>/dev/null || true

  for _ in 1 2 3 4 5 6 7 8 9 10; do
    if ! kill -0 "${pid}" 2>/dev/null; then
      return 0
    fi
    sleep 1
  done

  echo "[mac-bridge] Process ${pid} did not stop in time." >&2
  return 1
}

RESULT=0

if [ -f "${PID_FILE}" ]; then
  PID="$(cat "${PID_FILE}")"
  if [ -n "${PID}" ]; then
    stop_pid "${PID}" "tracked" || RESULT=1
  fi
  rm -f "${PID_FILE}"
fi

EXTRA_PIDS="$(pgrep -x "${BRIDGE_BASENAME}" 2>/dev/null || pgrep -f "${BRIDGE_PATTERN}" 2>/dev/null || true)"
if [ -n "${EXTRA_PIDS}" ]; then
  for pid in ${EXTRA_PIDS}; do
    stop_pid "${pid}" "stray" || RESULT=1
  done
fi

if [ "${RESULT}" -eq 0 ]; then
  echo "[mac-bridge] Stopped."
  exit 0
fi

exit 1
