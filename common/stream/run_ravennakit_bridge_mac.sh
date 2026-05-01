#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/../.." && pwd)"
BRIDGE_ENV="${RAVENNAKIT_BRIDGE_ENV:-${SCRIPT_DIR}/ravennakit_bridge_mac.env}"

if [ -f "${BRIDGE_ENV}" ]; then
  # shellcheck disable=SC1090
  source "${BRIDGE_ENV}"
fi

BRIDGE_BIN="${BRIDGE_BIN:-${ROOT_DIR}/build/ravennakit_bridge/pisound_ravennakit_bridge}"
INPUT_DEVICE="${INPUT_DEVICE:-BlackHole 16ch}"
OUTPUT_DEVICE="${OUTPUT_DEVICE:-BlackHole 16ch}"
INPUT_START_CHANNEL="${INPUT_START_CHANNEL:-1}"
OUTPUT_START_CHANNEL="${OUTPUT_START_CHANNEL:-3}"
NETWORK_INTERFACE="${NETWORK_INTERFACE:-en0}"
TX_DESTINATION="${TX_DESTINATION:-239.69.0.2}"
TX_PORT="${TX_PORT:-5004}"
RX_PORT="${RX_PORT:-5004}"
SAMPLE_RATE="${SAMPLE_RATE:-48000}"
BLOCK_SIZE="${BLOCK_SIZE:-128}"
CHANNELS="${CHANNELS:-2}"
PAYLOAD_TYPE="${PAYLOAD_TYPE:-98}"
SESSION_NAME="${SESSION_NAME:-pisound}"
TX_SESSION_NAME="${TX_SESSION_NAME:-Pisound_In}"
RX_SESSION_NAME="${RX_SESSION_NAME:-}"
PI_SOURCE_NAME="${PI_SOURCE_NAME:-Pisound_Out}"
RX_REQUIRE_DELAY_FRAMES="${RX_REQUIRE_DELAY_FRAMES:-1024}"
TX_TIMESTAMP_OFFSET_FRAMES="${TX_TIMESTAMP_OFFSET_FRAMES:-480}"
PI_API_BASE="${PI_API_BASE:-http://patchbox.local:8090}"

if [ ! -x "${BRIDGE_BIN}" ]; then
  echo "Missing bridge binary: ${BRIDGE_BIN}" >&2
  echo "Build it with:" >&2
  echo "  cmake -S common/stream/ravennakit_bridge -B build/ravennakit_bridge \\" >&2
  echo "    -DPISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO=ON \\" >&2
  echo "    -DPISOUND_RAVENNAKIT_BRIDGE_ENABLE_RAVENNAKIT=ON" >&2
  echo "  cmake --build build/ravennakit_bridge" >&2
  exit 1
fi

echo "pisound_ravennakit_bridge"
echo "  input:  ${INPUT_DEVICE} ch ${INPUT_START_CHANNEL}-$((INPUT_START_CHANNEL + CHANNELS - 1))"
echo "  output: ${OUTPUT_DEVICE} ch ${OUTPUT_START_CHANNEL}-$((OUTPUT_START_CHANNEL + CHANNELS - 1))"
echo "  iface:  ${NETWORK_INTERFACE}"
echo "  tx dst: ${TX_DESTINATION}:${TX_PORT}"
echo "  rx port:${RX_PORT}"

if [ -z "${RX_SESSION_NAME}" ] && command -v python3 >/dev/null 2>&1; then
  RX_SESSION_NAME="$(python3 - "${PI_API_BASE}" "${PI_SOURCE_NAME}" "${SESSION_NAME}-rx" <<'PY'
import json
import sys
import urllib.request

api_base = sys.argv[1].rstrip("/")
preferred_name = sys.argv[2]
legacy_suffix = sys.argv[3]
try:
    with urllib.request.urlopen(f"{api_base}/api/streams", timeout=3.0) as resp:
        payload = json.load(resp)
except Exception:
    sys.exit(0)

matches = []
for source in payload.get("sources", []):
    name = source.get("name", "")
    if not name:
        continue
    if name == preferred_name:
        print(name)
        break
    if name == legacy_suffix or name.endswith(f" {legacy_suffix}"):
        matches.append(name)
else:
    if matches:
        print(matches[0])
PY
)"
fi

if [ -n "${RX_SESSION_NAME}" ]; then
  echo "  rx ses: ${RX_SESSION_NAME}"
else
  RX_SESSION_NAME="${PI_SOURCE_NAME}"
  echo "  rx ses: ${RX_SESSION_NAME} (fallback)"
fi

echo "  tx ses: ${TX_SESSION_NAME}"
echo

exec "${BRIDGE_BIN}" \
  --audio-backend portaudio \
  --aoip-backend ravennakit \
  --input-device "${INPUT_DEVICE}" \
  --output-device "${OUTPUT_DEVICE}" \
  --input-start-channel "${INPUT_START_CHANNEL}" \
  --output-start-channel "${OUTPUT_START_CHANNEL}" \
  --network-interface "${NETWORK_INTERFACE}" \
  --tx-destination "${TX_DESTINATION}" \
  --tx-port "${TX_PORT}" \
  --rx-port "${RX_PORT}" \
  --sample-rate "${SAMPLE_RATE}" \
  --block-size "${BLOCK_SIZE}" \
  --channels "${CHANNELS}" \
  --payload-type "${PAYLOAD_TYPE}" \
  --session-name "${SESSION_NAME}" \
  --tx-session-name "${TX_SESSION_NAME}" \
  --rx-session-name "${RX_SESSION_NAME}" \
  --rx-require-delay-frames "${RX_REQUIRE_DELAY_FRAMES}" \
  --tx-timestamp-offset-frames "${TX_TIMESTAMP_OFFSET_FRAMES}" \
  --verbose
