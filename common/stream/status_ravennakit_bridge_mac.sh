#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RUNTIME_DIR="${RAVENNAKIT_BRIDGE_RUNTIME_DIR:-${SCRIPT_DIR}/.runtime/ravennakit_bridge_mac}"
PID_FILE="${RUNTIME_DIR}/bridge.pid"
LOG_FILE="${RUNTIME_DIR}/bridge.log"
BRIDGE_BASENAME="pisound_ravennakit_bridge"
BRIDGE_PATTERN="[p]isound_ravennakit_bridge"
PIDS="$(pgrep -x "${BRIDGE_BASENAME}" 2>/dev/null || pgrep -f "${BRIDGE_PATTERN}" 2>/dev/null || true)"
TRACKED_PID=""

if [ -f "${PID_FILE}" ]; then
  TRACKED_PID="$(cat "${PID_FILE}" 2>/dev/null || true)"
  if [ -n "${TRACKED_PID}" ] && kill -0 "${TRACKED_PID}" 2>/dev/null; then
    case " ${PIDS} " in
      *" ${TRACKED_PID} "*) ;;
      *) PIDS="${TRACKED_PID}${PIDS:+ ${PIDS}}" ;;
    esac
  fi
fi

if [ -z "${PIDS}" ]; then
  echo "[mac-bridge] inactive"
  echo "[mac-bridge] Log: ${LOG_FILE}"
  exit 0
fi

echo "[mac-bridge] active"
if [ -n "${TRACKED_PID}" ]; then
  echo "[mac-bridge] Tracked PID: ${TRACKED_PID}"
fi
echo "[mac-bridge] Live PID(s): ${PIDS}"
echo "[mac-bridge] Log: ${LOG_FILE}"
ps -p ${PIDS} -o pid=,etime=,command= 2>/dev/null || true

if [ -f "${LOG_FILE}" ]; then
  echo
  echo "[mac-bridge] Recent log"
  tail -n 20 "${LOG_FILE}" || true
fi
