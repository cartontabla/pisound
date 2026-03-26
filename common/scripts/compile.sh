#!/usr/bin/env bash
set -euo pipefail

if [ $# -lt 1 ]; then
  echo "Usage: $0 <ProjectName>"
  exit 1
fi

PROJECT="$1"

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

MODEL_SRC="${RTW_DIR}/${PROJECT}.c"
DATA_SRC="${RTW_DIR}/${PROJECT}_data.c"
NONFINITE_SRC="${RTW_DIR}/rt_nonfinite.c"

# R2025b+ genera rtGetNaN.c y rtGetInf.c como archivos separados
EXTRA_SRCS=()
[ -f "${RTW_DIR}/rtGetNaN.c" ] && EXTRA_SRCS+=("${RTW_DIR}/rtGetNaN.c")
[ -f "${RTW_DIR}/rtGetInf.c" ] && EXTRA_SRCS+=("${RTW_DIR}/rtGetInf.c")

MAIN_SRC="${COMMON_DIR}/custom_main.c"
PISOUND_IN="${COMMON_DIR}/pisound_in.c"
PISOUND_OUT="${COMMON_DIR}/pisound_out.c"
CTRL_IN_SRC="${COMMON_DIR}/ctrl_in.c"
CTRL_OUT_SRC="${COMMON_DIR}/ctrl_out.c"

FLAGS_FILE="${PROJECT_DIR}/${PROJECT}_compile_flags.txt"

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

# Solo habilitar tuning si hay flags (opt-in)
PARAM_DEFS=()
if [ ${#EXTRA_FLAGS[@]} -gt 0 ]; then
  # Auto-define struct/var si el usuario no los puso explícitos
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
  -ljack -lasound -lpthread -lm -lrt -O3

echo "Build SUCCESS: ${PROJECT_DIR}/${PROJECT}_app"
