#!/usr/bin/env bash
set -euo pipefail

if [ $# -lt 1 ]; then
  echo "Usage: $0 <ProjectName> [--stream]"
  exit 1
fi

PROJECT="$1"
# --stream puede venir como argumento O declararse en el flags file con -DSTREAM_MODE
STREAM_MODE=0
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

# R2025b+ genera rtGetNaN.c y rtGetInf.c como archivos separados
EXTRA_SRCS=()
[ -f "${RTW_DIR}/rtGetNaN.c" ] && EXTRA_SRCS+=("${RTW_DIR}/rtGetNaN.c")
[ -f "${RTW_DIR}/rtGetInf.c" ] && EXTRA_SRCS+=("${RTW_DIR}/rtGetInf.c")

if [ "$STREAM_MODE" -eq 1 ]; then
  MAIN_SRC="${COMMON_DIR}/stream_main.c"
  PISOUND_IN="${COMMON_DIR}/stream_in.c"
  PISOUND_OUT="${COMMON_DIR}/stream_out.c"
  echo "Mode: stream (AES67/ALSA)"
else
  MAIN_SRC="${COMMON_DIR}/custom_main.c"
  PISOUND_IN="${COMMON_DIR}/pisound_in.c"
  PISOUND_OUT="${COMMON_DIR}/pisound_out.c"
  echo "Mode: pisound (JACK)"
fi
CTRL_IN_SRC="${COMMON_DIR}/ctrl_in.c"
CTRL_OUT_SRC="${COMMON_DIR}/ctrl_out.c"

CTRL_DIR="${COMMON_DIR}/control"
CTRL_SRCS=()
EXTRA_DEFS=()
EXTRA_FLAGS=()

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
  "${MODEL_SRC}" "${DATA_SRC}" "${NONFINITE_SRC}" "${EXTRA_SRCS[@]}" \
  "${MAIN_SRC}" "${PISOUND_IN}" "${PISOUND_OUT}" \
  "${CTRL_IN_SRC}" "${CTRL_OUT_SRC}" \
  "${CTRL_SRCS[@]}" \
  -I"${PROJECT_DIR}" -I"${RTW_DIR}" -I"${COMMON_DIR}" -I"${CTRL_DIR}" \
  -DMODEL_HEADER="\"${PROJECT}.h\"" \
  -DMODEL_INITIALIZE=${PROJECT}_initialize \
  -DMODEL_STEP=${PROJECT}_step \
  -DMODEL_TERMINATE=${PROJECT}_terminate \
  -DMODEL_FRAME_LENGTH=128 \
  -DMODEL_RT_TYPE=RT_MODEL_${PROJECT}_T \
  -DMODEL_RT_PTR=${PROJECT}_M \
  "${PARAM_DEFS[@]}" \
  "${EXTRA_FLAGS[@]}" \
  $([ "$STREAM_MODE" -eq 0 ] && echo "-ljack") \
  -lasound -lpthread -lm -lrt -O3

echo "Build SUCCESS: ${PROJECT_DIR}/${PROJECT}_app"

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
  mv "${PROJECT_DIR}/${PROJECT}_app" "${PROJECT_DIR}/${PROJECT}_app_bin"

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
  elif echo "\$log" | grep -q "not found\|No module\|ModuleNotFoundError\|ImportError"; then
    echo "[ble] Dependencias Python no instaladas. Ejecuta:" >&2
    echo "[ble]   sudo apt install python3-dbus python3-gi" >&2
  elif echo "\$log" | grep -q "Permission denied\|Operation not permitted"; then
    echo "[ble] Sin permisos para acceder a BlueZ. ¿Está ejecutando con sudo?" >&2
  else
    echo "[ble] El canal BLE no ha podido arrancar. Log:" >&2
    echo "\$log" | head -5 | sed 's/^/[ble]   /' >&2
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

for i in \$(seq 1 40); do
  grep -q "Running" /tmp/ble_bridge.log 2>/dev/null && break
  grep -q "^ERROR:" /tmp/ble_bridge.log 2>/dev/null && break
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
  vir_client=\$(aconnect -l 2>/dev/null | grep -m1 'Virtual Raw MIDI' | sed 's/client \([0-9]*\).*/\1/')
  if [ -n "\$ctrl_client" ] && [ -n "\$vir_client" ]; then
    if aconnect "\${vir_client}:0" "\${ctrl_client}:0" 2>/tmp/ble_aconn.err; then
      echo "[ble] VirMIDI connected to pisound-control (\${vir_client}:0 → \${ctrl_client}:0)."
      exit
    fi
    if grep -q "already subscribed" /tmp/ble_aconn.err 2>/dev/null; then
      echo "[ble] VirMIDI already connected to pisound-control (\${vir_client}:0 → \${ctrl_client}:0)."
      exit
    fi
  fi
  sleep 0.5
done
echo "[ble] WARNING: VirMIDI → pisound-control routing failed." >&2
[ -s /tmp/ble_aconn.err ] && echo "[ble]   aconnect error: \$(cat /tmp/ble_aconn.err)" >&2
echo "[ble]   Manual fix: aconnect \"\$(aconnect -l | grep -m1 'Virtual Raw MIDI' | sed 's/client \([0-9]*\).*/\1/'):0\" \$(aconnect -l | grep -m1 'pisound-control' | sed 's/client \([0-9]*\).*/\1/'):0" >&2) &

trap 'sudo kill \$BRIDGE_PID 2>/dev/null' EXIT INT TERM

exec "\${SCRIPT_DIR}/${PROJECT}_app_bin" "\$@"
WRAPPER

  chmod +x "${PROJECT_DIR}/${PROJECT}_app"
  echo "BLE MIDI wrapper generated (${PROJECT}_app → ${PROJECT}_app_bin)"
fi
