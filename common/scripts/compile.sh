#!/usr/bin/env bash
set -euo pipefail

if [ $# -lt 1 ]; then
  echo "Usage: $0 <ProjectName> [--stream]"
  echo "  <ProjectName>_compile_flags.txt is the single source of truth for"
  echo "  per-project compile-time flags in both pisound and stream modes."
  exit 1
fi

PROJECT="$1"
# --stream puede venir como argumento O declararse en el flags file con -DSTREAM_MODE
STREAM_MODE=0
HYBRID_MODE=0
for arg in "$@"; do
  [ "$arg" = "--stream" ] && STREAM_MODE=1
done

# --- rutas robustas (independientes del cwd)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"   # .../common/scripts
COMMON_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"                   # .../common
ROOT_DIR="$(cd "$COMMON_DIR/.." && pwd)"                     # .../pisound

# --- localizar carpeta del proyecto
# 1) si estás dentro del proyecto (modo antiguo), úsalo
# 2) si no, asume estructura .../pisound/<PROJECT>/
if [ -d "./${PROJECT}_ert_rtw" ]; then
  PROJECT_DIR="$(pwd)"
elif [ -d "${ROOT_DIR}/${PROJECT}/${PROJECT}_ert_rtw" ]; then
  PROJECT_DIR="${ROOT_DIR}/${PROJECT}"
else
  echo "Error: no encuentro ${PROJECT}_ert_rtw."
  echo "Ejecuta desde ~/pisound/${PROJECT} o asegúrate de que existe ${ROOT_DIR}/${PROJECT}/${PROJECT}_ert_rtw"
  exit 1
fi

RTW_DIR="${PROJECT_DIR}/${PROJECT}_ert_rtw"

if [ ! -d "${RTW_DIR}" ]; then
  echo "Error: Directory ${RTW_DIR} not found!"
  echo "Run from the project folder, e.g. ~/pisound/${PROJECT}"
  exit 1
fi

# --- Pre-scan flags file para detectar STREAM_MODE antes de seleccionar main
FLAGS_FILE="${PROJECT_DIR}/${PROJECT}_compile_flags.txt"
if [ "$STREAM_MODE" -eq 0 ] && [ -f "${FLAGS_FILE}" ]; then
  grep -Eq '^[[:space:]]*-DSTREAM_MODE([[:space:]]*=|$)' "${FLAGS_FILE}" 2>/dev/null \
    && STREAM_MODE=1 || true
fi

MODEL_SRC="${RTW_DIR}/${PROJECT}.c"
DATA_SRC="${RTW_DIR}/${PROJECT}_data.c"
NONFINITE_SRC="${RTW_DIR}/rt_nonfinite.c"

MODEL_HAS_PISOUND=0
MODEL_HAS_STREAM=0
if [ -f "${MODEL_SRC}" ]; then
  grep -Eq 'pisound_in|pisound_out' "${MODEL_SRC}" && MODEL_HAS_PISOUND=1 || true
  grep -Eq 'stream_in|stream_out' "${MODEL_SRC}" && MODEL_HAS_STREAM=1 || true
fi
if [ "${MODEL_HAS_PISOUND}" -eq 1 ] && [ "${MODEL_HAS_STREAM}" -eq 1 ]; then
  HYBRID_MODE=1
  STREAM_MODE=1
fi

# R2025b+ genera rtGetNaN.c y rtGetInf.c como archivos separados
EXTRA_SRCS=()
[ -f "${RTW_DIR}/rtGetNaN.c" ] && EXTRA_SRCS+=("${RTW_DIR}/rtGetNaN.c")
[ -f "${RTW_DIR}/rtGetInf.c" ] && EXTRA_SRCS+=("${RTW_DIR}/rtGetInf.c")

if [ "$HYBRID_MODE" -eq 1 ]; then
  MAIN_SRC="${COMMON_DIR}/pisound_main.c"
  PISOUND_IN="${COMMON_DIR}/pisound_in.c"
  PISOUND_OUT="${COMMON_DIR}/pisound_out.c"
  STREAM_IN_SRC="${COMMON_DIR}/stream_in.c"
  STREAM_OUT_SRC="${COMMON_DIR}/stream_out.c"
  echo "Mode: hybrid (Pisound/JACK + AES67/ALSA)"
  echo "Using mixed heads: pisound_in.c / pisound_out.c + stream_in.c / stream_out.c"
elif [ "$STREAM_MODE" -eq 1 ]; then
  MAIN_SRC="${COMMON_DIR}/stream_main.c"
  PISOUND_IN="${COMMON_DIR}/stream_in.c"
  PISOUND_OUT="${COMMON_DIR}/stream_out.c"
  STREAM_IN_SRC=""
  STREAM_OUT_SRC=""
  echo "Mode: stream (AES67/ALSA)"
  echo "Using stream heads: stream_in.c / stream_out.c"
else
  MAIN_SRC="${COMMON_DIR}/custom_main.c"
  PISOUND_IN="${COMMON_DIR}/pisound_in.c"
  PISOUND_OUT="${COMMON_DIR}/pisound_out.c"
  STREAM_IN_SRC=""
  STREAM_OUT_SRC=""
  echo "Mode: pisound (JACK)"
fi
CTRL_IN_SRC="${COMMON_DIR}/ctrl_in.c"
CTRL_OUT_SRC="${COMMON_DIR}/ctrl_out.c"
STREAM_RT_SRCS=()
if [ "$STREAM_MODE" -eq 1 ]; then
  STREAM_RT_SRCS+=("${COMMON_DIR}/stream/stream_contract.c")
  STREAM_RT_SRCS+=("${COMMON_DIR}/stream/stream_backend.c")
fi
JACK_LIBS=()
if [ "$STREAM_MODE" -eq 0 ] || [ "$HYBRID_MODE" -eq 1 ]; then
  JACK_LIBS+=("-ljack")
fi

CTRL_DIR="${COMMON_DIR}/control"
CTRL_SRCS=()
EXTRA_DEFS=()
EXTRA_FLAGS=()
STREAM_ALSA_DEFAULT=0

if [ -f "${FLAGS_FILE}" ]; then
  echo "Found configuration file: ${FLAGS_FILE}"

  while IFS= read -r line; do
    line="${line%%$'\r'}"
    [[ -z "$line" || "$line" =~ ^[[:space:]]*# ]] && continue
    EXTRA_FLAGS+=("$line")
  done < "${FLAGS_FILE}"

  if [ -d "${CTRL_DIR}" ]; then
    if grep -Eq '^[[:space:]]*-DENABLE_CONTROL([[:space:]]*=|$)' "${FLAGS_FILE}"; then
      EXTRA_DEFS+=("-DENABLE_CONTROL")
      CTRL_SRCS+=("${CTRL_DIR}/control.c")
      CTRL_SRCS+=("${CTRL_DIR}/midi_io.c")
      CTRL_SRCS+=("${CTRL_DIR}/midi_proto.c")
      CTRL_SRCS+=("${CTRL_DIR}/ctrl_map.c")
      CTRL_SRCS+=("${CTRL_DIR}/ctrl_apply.c")
      CTRL_SRCS+=("${CTRL_DIR}/ctrl_event.c")
    fi

    if grep -Eq '^[[:space:]]*-DENABLE_OSC([[:space:]]*=|$)' "${FLAGS_FILE}"; then
      EXTRA_DEFS+=("-DENABLE_OSC")
      CTRL_SRCS+=("${CTRL_DIR}/osc_proto.c")
      CTRL_SRCS+=("${CTRL_DIR}/udp_io.c")   # osc_proto depends on udp_io
      EXTRA_FLAGS+=("-include" "osc_proto.h")
    fi

    if grep -Eq '^[[:space:]]*-DENABLE_UDP([[:space:]]*=|$)' "${FLAGS_FILE}"; then
      EXTRA_DEFS+=("-DENABLE_UDP")
      # udp_io.c already added with ENABLE_OSC; add only if OSC is not enabled
      if ! grep -Eq '^[[:space:]]*-DENABLE_OSC([[:space:]]*=|$)' "${FLAGS_FILE}"; then
        CTRL_SRCS+=("${CTRL_DIR}/udp_io.c")
      fi
    fi
  fi
fi

if [ "$STREAM_MODE" -eq 1 ] && grep -Eq '^[[:space:]]*-DSTREAM_BACKEND_DEFAULT_ALSA([[:space:]]*=|$)' "${FLAGS_FILE}" 2>/dev/null; then
  STREAM_ALSA_DEFAULT=1
fi

if [ ! -f "${FLAGS_FILE}" ]; then
  echo "No ${PROJECT}_compile_flags.txt found; building with defaults only."
fi

# Auto-inyectar PARAM_STRUCT solo si hay PARAM_P*_LABEL declarados en el flags file
# (sin labels no hay params tuneables por texto y el struct no se necesita)
PARAM_DEFS=()
if grep -Eq '^[[:space:]]*-DPARAM_P[0-9]+_LABEL=' "${FLAGS_FILE}" 2>/dev/null; then
  if ! grep -q 'PARAM_STRUCT_TYPE' "${FLAGS_FILE}" 2>/dev/null; then
    PARAM_DEFS+=("-DPARAM_STRUCT_TYPE=P_${PROJECT}_T")
  fi
  if ! grep -q 'PARAM_STRUCT_VAR' "${FLAGS_FILE}" 2>/dev/null; then
    PARAM_DEFS+=("-DPARAM_STRUCT_VAR=${PROJECT}_P")
  fi
fi

echo "Compiling ${PROJECT}..."

gcc -o "${PROJECT_DIR}/${PROJECT}_app" \
  "${MODEL_SRC}" "${DATA_SRC}" "${NONFINITE_SRC}" ${EXTRA_SRCS+"${EXTRA_SRCS[@]}"} \
  "${MAIN_SRC}" "${PISOUND_IN}" "${PISOUND_OUT}" \
  ${STREAM_IN_SRC:+"${STREAM_IN_SRC}"} ${STREAM_OUT_SRC:+"${STREAM_OUT_SRC}"} \
  ${STREAM_RT_SRCS+"${STREAM_RT_SRCS[@]}"} \
  "${CTRL_IN_SRC}" "${CTRL_OUT_SRC}" \
  ${CTRL_SRCS+"${CTRL_SRCS[@]}"} \
  -I"${PROJECT_DIR}" -I"${RTW_DIR}" -I"${COMMON_DIR}" -I"${CTRL_DIR}" \
  -DMODEL_HEADER="\"${PROJECT}.h\"" \
  -DMODEL_INITIALIZE=${PROJECT}_initialize \
  -DMODEL_STEP=${PROJECT}_step \
  -DMODEL_TERMINATE=${PROJECT}_terminate \
  -DMODEL_FRAME_LENGTH=128 \
  -DMODEL_RT_TYPE=RT_MODEL_${PROJECT}_T \
  -DMODEL_RT_PTR=${PROJECT}_M \
  ${PARAM_DEFS+"${PARAM_DEFS[@]}"} \
  ${EXTRA_FLAGS+"${EXTRA_FLAGS[@]}"} \
  ${JACK_LIBS+"${JACK_LIBS[@]}"} \
  -lasound -lpthread -lm -lrt -O3

echo "Build SUCCESS: ${PROJECT_DIR}/${PROJECT}_app"

# Remove stale generated wrappers from previous builds before recreating them.
rm -f "${PROJECT_DIR}/${PROJECT}_app_bin" "${PROJECT_DIR}/${PROJECT}_app_real"

# --- BLE MIDI wrapper (opt-in via -DBLE_MIDI in flags file)
if grep -Eq '^[[:space:]]*-DBLE_MIDI([[:space:]]*=|$)' "${FLAGS_FILE}" 2>/dev/null; then
  BRIDGE_PY="${COMMON_DIR}/control/ble_midi_bridge.py"

  # Optional MAC override: -DBLE_MIDI_MAC="xx:xx:xx:xx:xx:xx"
  BLE_MAC=""
  mac_line=$(grep -E '^[[:space:]]*-DBLE_MIDI_MAC=' "${FLAGS_FILE}" 2>/dev/null || true)
  if [ -n "$mac_line" ]; then
    BLE_MAC=$(echo "$mac_line" | sed 's/.*-DBLE_MIDI_MAC=["]*\([^"[:space:]]*\)["]*$/\1/')
  fi

  # Determine if BLE is the only active transport (to decide abort vs warn)
  BLE_ONLY=1
  grep -Eq '^[[:space:]]*-DCTRL_MIDI_PORT=' "${FLAGS_FILE}" 2>/dev/null && BLE_ONLY=0
  grep -Eq '^[[:space:]]*-DENABLE_OSC([[:space:]]*=|$)' "${FLAGS_FILE}" 2>/dev/null && BLE_ONLY=0

  # Rename real binary
  mv "${PROJECT_DIR}/${PROJECT}_app" "${PROJECT_DIR}/${PROJECT}_app_real"

  # Generate transparent wrapper
  cat > "${PROJECT_DIR}/${PROJECT}_app" << WRAPPER
#!/usr/bin/env bash
# Auto-generated by compile.sh — do not edit
SCRIPT_DIR="\$(cd "\$(dirname "\${BASH_SOURCE[0]}")" && pwd)"
BLE_ONLY=${BLE_ONLY}

_ble_fail() {
  local log
  log=\$(cat /tmp/ble_bridge.log 2>/dev/null)
  echo "" >&2
  if echo "\$log" | grep -q "not found during scan"; then
    echo "[ble] No se ha encontrado el dispositivo BLE. ¿Está encendido y cerca?" >&2
  elif echo "\$log" | grep -q "services not resolved"; then
    echo "[ble] El dispositivo BLE se encontró pero no respondió a tiempo. ¿Está ocupado con otro host?" >&2
  elif echo "\$log" | grep -q "no Bluetooth adapter"; then
    echo "[ble] No se detecta el adaptador USB Bluetooth. ¿Está conectado el dongle?" >&2
  elif echo "\$log" | grep -q "Connect() failed"; then
    echo "[ble] No se ha podido conectar. Prueba a encender y apagar el dispositivo BLE." >&2
  else
    echo "[ble] El canal BLE no ha podido arrancar." >&2
  fi
  sudo kill \$BRIDGE_PID 2>/dev/null
  if [ "\$BLE_ONLY" -eq 1 ]; then
    echo "[ble] Es el único canal de control activo — no se puede continuar." >&2
    exit 1
  else
    echo "[ble] Continuando sin BLE — los demás canales siguen activos." >&2
  fi
}

sudo modprobe snd-virmidi midi_devs=1 2>/dev/null

# Pre-check: ensure hci0 is up
if ! sudo hciconfig hci0 2>/dev/null | grep -q "UP RUNNING"; then
  echo "[ble] hci0 is down — attempting to bring up..." >&2
  sudo rfkill unblock bluetooth 2>/dev/null
  sudo hciconfig hci0 up 2>/dev/null
  sleep 1
fi

sudo python3 -u "${BRIDGE_PY}" ${BLE_MAC} < /dev/null > /tmp/ble_bridge.log 2>&1 &
BRIDGE_PID=\$!

for i in \$(seq 1 20); do
  grep -q "Running" /tmp/ble_bridge.log 2>/dev/null && break
  grep -q "ERROR" /tmp/ble_bridge.log 2>/dev/null && break
  sleep 0.5
done

if ! grep -q "Running" /tmp/ble_bridge.log 2>/dev/null; then
  _ble_fail
else
  echo "[ble] Bridge ready."
fi

# Connect VirMIDI → pisound-control once the app creates its ALSA port
(for i in \$(seq 1 30); do
  ctrl_client=\$(aconnect -l 2>/dev/null | grep -m1 'pisound-control' | sed 's/client \([0-9]*\).*/\1/')
  if [ -n "\$ctrl_client" ]; then
    aconnect 20:0 "\${ctrl_client}:0" 2>/dev/null && echo "[ble] VirMIDI connected to pisound-control." && exit
  fi
  sleep 0.5
done
echo "[ble] WARNING: BLE conectado pero el rutado MIDI a pisound-control falló." >&2
echo "[ble]          Ejecuta manualmente: aconnect 20:0 <client>:0" >&2) &

trap 'sudo kill \$BRIDGE_PID 2>/dev/null' EXIT INT TERM

exec "\${SCRIPT_DIR}/${PROJECT}_app_real" "\$@"
WRAPPER

  chmod +x "${PROJECT_DIR}/${PROJECT}_app"
  echo "BLE MIDI wrapper generated (${PROJECT}_app → ${PROJECT}_app_real)"
fi

# --- Canonical RAVENNA runtime wrapper for stream/ALSA projects
if [ "$STREAM_ALSA_DEFAULT" -eq 1 ]; then
  APP_PATH="${PROJECT_DIR}/${PROJECT}_app"
  APP_BIN="${PROJECT_DIR}/${PROJECT}_app_bin"

  mv "${APP_PATH}" "${APP_BIN}"

  cat > "${APP_PATH}" << WRAPPER
#!/usr/bin/env bash
# Auto-generated by compile.sh — do not edit
set -euo pipefail

SCRIPT_DIR="\$(cd "\$(dirname "\${BASH_SOURCE[0]}")" && pwd)"
STARTED_RUNTIME=0
SERVICE_TARGET="pisound-aes67-runtime.target"
PROJECT_RUNTIME_ENV="\${SCRIPT_DIR}/${PROJECT}_ravenna.env"

_run_systemctl() {
  if [ "\$(id -u)" -eq 0 ]; then
    systemctl "\$@"
  else
    sudo systemctl "\$@"
  fi
}

_cleanup() {
  local rc=\$?
  if [ "\$STARTED_RUNTIME" -eq 1 ]; then
    echo "[ravenna] Stopping \${SERVICE_TARGET}..."
    _run_systemctl stop "\${SERVICE_TARGET}" >/dev/null 2>&1 || true
  fi
  exit \$rc
}

trap _cleanup EXIT INT TERM

if systemctl is-active --quiet "\${SERVICE_TARGET}"; then
  :
else
  echo "[ravenna] Starting \${SERVICE_TARGET}..."
  _run_systemctl enable --now "\${SERVICE_TARGET}"
  STARTED_RUNTIME=1
fi

if [ -f "\${PROJECT_RUNTIME_ENV}" ]; then
  # shellcheck disable=SC1090
  source "\${PROJECT_RUNTIME_ENV}"
fi

: "\${STREAM_BACKEND:=alsa}"
: "\${STREAM_SAMPLE_FORMAT:=s32le}"
: "\${STREAM_DEVICE:=hw:CARD=RAVENNA,DEV=0}"
: "\${STREAM_CAPTURE_DEVICE:=\${STREAM_DEVICE}}"
: "\${STREAM_PLAYBACK_DEVICE:=\${STREAM_DEVICE}}"

export STREAM_BACKEND
export STREAM_SAMPLE_FORMAT
export STREAM_DEVICE
export STREAM_CAPTURE_DEVICE
export STREAM_PLAYBACK_DEVICE
export STREAM_CAPTURE_HW_CHANNELS="\${STREAM_CAPTURE_HW_CHANNELS:-}"
export STREAM_PLAYBACK_HW_CHANNELS="\${STREAM_PLAYBACK_HW_CHANNELS:-}"
export STREAM_CAPTURE_MAP="\${STREAM_CAPTURE_MAP:-}"
export STREAM_PLAYBACK_MAP="\${STREAM_PLAYBACK_MAP:-}"

exec "\${SCRIPT_DIR}/${PROJECT}_app_bin" "\$@"
WRAPPER

  chmod +x "${APP_PATH}"
  echo "RAVENNA runtime wrapper generated (${PROJECT}_app → ${PROJECT}_app_bin)"
fi
