#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RUNTIME_DIR="${RAVENNAKIT_BRIDGE_RUNTIME_DIR:-${SCRIPT_DIR}/.runtime/ravennakit_bridge_mac}"
PID_FILE="${RUNTIME_DIR}/bridge.pid"
LOG_FILE="${RUNTIME_DIR}/bridge.log"
TIMEOUT_SECONDS="${1:-30}"
BRIDGE_PATTERN="[p]isound_ravennakit_bridge"

deadline=$((SECONDS + TIMEOUT_SECONDS))

if [ ! -f "${LOG_FILE}" ]; then
  echo "[mac-bridge] Waiting failed: no log file yet: ${LOG_FILE}" >&2
  exit 1
fi

while [ "${SECONDS}" -lt "${deadline}" ]; do
  if [ -f "${PID_FILE}" ]; then
    PID="$(cat "${PID_FILE}" 2>/dev/null || true)"
    if [ -n "${PID}" ] && ! kill -0 "${PID}" 2>/dev/null && ! pgrep -f "${BRIDGE_PATTERN}" >/dev/null 2>&1; then
      echo "[mac-bridge] Waiting failed: pid ${PID} is no longer alive." >&2
      tail -n 40 "${LOG_FILE}" >&2 || true
      exit 1
    fi
  fi

  if grep -Eq 'ravennakit rx: ptp_calibrated=1|Switching port [0-9]+ to slave' "${LOG_FILE}"; then
    echo "[mac-bridge] Ready: PTP calibrated."
    tail -n 20 "${LOG_FILE}" || true
    exit 0
  fi

  sleep 1
done

echo "[mac-bridge] Timeout waiting for PTP/calibration." >&2
tail -n 40 "${LOG_FILE}" >&2 || true
exit 1
