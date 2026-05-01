#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RUNTIME_DIR="${RAVENNAKIT_BRIDGE_RUNTIME_DIR:-${SCRIPT_DIR}/.runtime/ravennakit_bridge_mac}"
LOG_FILE="${RUNTIME_DIR}/bridge.log"
LINES="${1:-40}"

if [ ! -f "${LOG_FILE}" ]; then
  echo "[mac-bridge] No log file yet: ${LOG_FILE}"
  exit 0
fi

tail -n "${LINES}" "${LOG_FILE}"
