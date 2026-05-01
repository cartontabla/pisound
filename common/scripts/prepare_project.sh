#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: common/scripts/prepare_project.sh [options] <project>

Checks and installs the Raspberry Pi/Patchbox requirements for one generated
Simulink project. The project compile flags are the single source of truth.

Options:
  --check-only    Report missing requirements without installing anything.
  --no-deploy     Do not run the AES67/RAVENNA deployment helper.
  --compile       Compile the project after requirements pass.
  -h, --help      Show this help.

Examples:
  sudo common/scripts/prepare_project.sh amp2
  common/scripts/prepare_project.sh --check-only bypasscross
  sudo common/scripts/prepare_project.sh --compile amp2
EOF
}

CHECK_ONLY=0
DEPLOY=1
COMPILE=0
PROJECT=""

while [ $# -gt 0 ]; do
  case "$1" in
    --check-only)
      CHECK_ONLY=1
      shift
      ;;
    --no-deploy)
      DEPLOY=0
      shift
      ;;
    --compile)
      COMPILE=1
      shift
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    -*)
      echo "[prepare] Unknown option: $1" >&2
      usage >&2
      exit 2
      ;;
    *)
      if [ -n "${PROJECT}" ]; then
        echo "[prepare] Only one project can be prepared at a time." >&2
        usage >&2
        exit 2
      fi
      PROJECT="$1"
      shift
      ;;
  esac
done

if [ -z "${PROJECT}" ]; then
  usage >&2
  exit 2
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
COMMON_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
ROOT_DIR="$(cd "${COMMON_DIR}/.." && pwd)"
PROJECT_DIR="${ROOT_DIR}/${PROJECT}"
RTW_DIR="${PROJECT_DIR}/${PROJECT}_ert_rtw"
FLAGS_FILE="${PROJECT_DIR}/${PROJECT}_compile_flags.txt"

if [ ! -d "${PROJECT_DIR}" ]; then
  echo "[prepare] Project directory not found: ${PROJECT_DIR}" >&2
  exit 1
fi

if [ ! -d "${RTW_DIR}" ]; then
  echo "[prepare] Generated code directory not found: ${RTW_DIR}" >&2
  echo "[prepare] Run sbuild/MATLAB code generation before preparing this project." >&2
  exit 1
fi

if [ ! -f "${FLAGS_FILE}" ]; then
  echo "[prepare] Flags file not found: ${FLAGS_FILE}" >&2
  echo "[prepare] Create ${PROJECT}_compile_flags.txt so the framework can infer requirements." >&2
  exit 1
fi

flag_enabled() {
  local flag="$1"
  grep -Eq "^[[:space:]]*-D${flag}([[:space:]]*=|$)" "${FLAGS_FILE}"
}

MODE="pisound"
if flag_enabled STREAM_MODE; then
  MODE="stream"
fi

MODEL_SRC="${RTW_DIR}/${PROJECT}.c"
if [ -f "${MODEL_SRC}" ]; then
  MODEL_HAS_PISOUND=0
  MODEL_HAS_STREAM=0
  grep -Eq 'pisound_in|pisound_out' "${MODEL_SRC}" && MODEL_HAS_PISOUND=1 || true
  grep -Eq 'stream_in|stream_out' "${MODEL_SRC}" && MODEL_HAS_STREAM=1 || true
  if [ "${MODEL_HAS_PISOUND}" -eq 1 ] && [ "${MODEL_HAS_STREAM}" -eq 1 ]; then
    MODE="hybrid"
  fi
fi

NEEDS_RAVENNA=0
if [ "${MODE}" = "stream" ] || [ "${MODE}" = "hybrid" ]; then
  if flag_enabled STREAM_BACKEND_DEFAULT_ALSA; then
    NEEDS_RAVENNA=1
  fi
fi

echo "[prepare] Project: ${PROJECT}"
echo "[prepare] Directory: ${PROJECT_DIR}"
echo "[prepare] Flags: ${FLAGS_FILE}"
echo "[prepare] Mode: ${MODE}"
if [ "${NEEDS_RAVENNA}" -eq 1 ]; then
  echo "[prepare] Runtime: AES67/RAVENNA ALSA"
fi

if [ "$(uname -s)" != "Linux" ]; then
  echo "[prepare] This preparation must run on the Raspberry Pi/Patchbox target." >&2
  echo "[prepare] On this host I can only validate the script syntax, not apt/systemd state." >&2
  exit 1
fi

if [ "${NEEDS_RAVENNA}" -eq 1 ] && [ "${DEPLOY}" -eq 1 ]; then
  if [ "${CHECK_ONLY}" -eq 1 ]; then
    echo "[prepare] Check-only: not running deploy_aes67.sh."
  else
    echo "[prepare] Deploying AES67/RAVENNA framework requirements..."
    if [ "$(id -u)" -eq 0 ]; then
      "${SCRIPT_DIR}/deploy_aes67.sh"
    else
      sudo "${SCRIPT_DIR}/deploy_aes67.sh"
    fi
  fi
fi

REQ_ARGS=("--project" "${PROJECT}" "--flags" "${FLAGS_FILE}" "--mode" "${MODE}")
if [ "${CHECK_ONLY}" -eq 0 ]; then
  REQ_ARGS=("--install" "${REQ_ARGS[@]}")
fi

"${SCRIPT_DIR}/check_pi_requirements.sh" "${REQ_ARGS[@]}"

if [ "${COMPILE}" -eq 1 ]; then
  echo "[prepare] Compiling ${PROJECT}..."
  "${SCRIPT_DIR}/compile.sh" "${PROJECT}"
fi

echo "[prepare] ${PROJECT}: ready."
