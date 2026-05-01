#!/usr/bin/env bash
set -euo pipefail

INSTALL=0
PROJECT=""
FLAGS_FILE=""
MODE="pisound"

usage() {
  cat <<'EOF'
Usage: check_pi_requirements.sh [--install] --project <name> --flags <file> --mode <pisound|stream|hybrid>

Checks the Raspberry Pi/Patchbox requirements implied by a generated
Simulink project's compile flags. With --install, missing apt packages are
installed automatically when apt-get is available.
EOF
}

while [ $# -gt 0 ]; do
  case "$1" in
    --install)
      INSTALL=1
      shift
      ;;
    --project)
      PROJECT="${2:-}"
      shift 2
      ;;
    --flags)
      FLAGS_FILE="${2:-}"
      shift 2
      ;;
    --mode)
      MODE="${2:-}"
      shift 2
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      echo "[requirements] Unknown argument: $1" >&2
      usage >&2
      exit 2
      ;;
  esac
done

if [ -z "${PROJECT}" ]; then
  echo "[requirements] Missing --project." >&2
  usage >&2
  exit 2
fi

flag_enabled() {
  local flag="$1"
  [ -f "${FLAGS_FILE}" ] && grep -Eq "^[[:space:]]*-D${flag}([[:space:]]*=|$)" "${FLAGS_FILE}"
}

MISSING_PKGS=()
MISSING_NOTES=()

add_pkg() {
  local pkg="$1"
  local note="${2:-$1}"

  if command -v dpkg >/dev/null 2>&1; then
    if dpkg -s "${pkg}" >/dev/null 2>&1; then
      return 0
    fi
  elif command -v apt-get >/dev/null 2>&1; then
    :
  else
    MISSING_NOTES+=("${note}: apt/dpkg not available to verify package ${pkg}")
    return 0
  fi

  local existing
  for existing in "${MISSING_PKGS[@]}"; do
    if [ "${existing}" = "${pkg}" ]; then
      MISSING_NOTES+=("${note}: missing apt package ${pkg}")
      return 0
    fi
  done

  MISSING_PKGS+=("${pkg}")
  MISSING_NOTES+=("${note}: missing apt package ${pkg}")
}

need_cmd() {
  local cmd="$1"
  local pkg="$2"
  local note="$3"

  if command -v "${cmd}" >/dev/null 2>&1; then
    return 0
  fi
  add_pkg "${pkg}" "${note}"
}

need_python_module() {
  local module="$1"
  local pkg="$2"
  local note="$3"

  if command -v python3 >/dev/null 2>&1 &&
     python3 -c "import ${module}" >/dev/null 2>&1; then
    return 0
  fi
  add_pkg "${pkg}" "${note}"
}

need_file() {
  local path="$1"
  local note="$2"

  if [ ! -e "${path}" ]; then
    MISSING_NOTES+=("${note}: missing ${path}")
  fi
}

need_cmd gcc build-essential "C compiler"
add_pkg build-essential "C toolchain"
add_pkg libasound2-dev "ALSA development headers/libraries"
add_pkg alsa-utils "ALSA utilities"
add_pkg psmisc "Process/device diagnostics"

case "${MODE}" in
  pisound|hybrid)
    add_pkg libjack-jackd2-dev "JACK development headers/libraries"
    add_pkg jackd2 "JACK runtime"
    ;;
  stream)
    ;;
  *)
    MISSING_NOTES+=("unknown compile mode '${MODE}'")
    ;;
esac

if flag_enabled ENABLE_CONTROL; then
  add_pkg alsa-utils "Control/MIDI routing utilities"
fi

if flag_enabled BLE_MIDI; then
  need_cmd python3 python3 "BLE MIDI Python runtime"
  need_python_module dbus python3-dbus "BLE MIDI D-Bus bindings"
  need_python_module gi python3-gi "BLE MIDI GLib bindings"
  need_cmd modprobe kmod "BLE MIDI virtual MIDI module loader"
  need_cmd rfkill rfkill "Bluetooth unblock helper"
  need_cmd hciconfig bluez "Bluetooth adapter control"
  need_cmd bluetoothctl bluez "Bluetooth daemon tools"
  need_cmd aconnect alsa-utils "ALSA MIDI connector"
fi

if [ "${MODE}" = "stream" ] || [ "${MODE}" = "hybrid" ] || flag_enabled STREAM_MODE; then
  add_pkg linuxptp "AES67/PTP runtime"
  need_cmd python3 python3 "AES67 helper Python runtime"
  need_cmd systemctl systemd "systemd runtime control"

  if flag_enabled STREAM_BACKEND_DEFAULT_ALSA; then
    need_file /etc/systemd/system/pisound-aes67-runtime.target \
      "AES67 runtime is not deployed; run sudo common/scripts/deploy_aes67.sh"
    need_file /etc/systemd/system/pisound-stream-ravenna@.service \
      "RAVENNA project service is not deployed; run sudo common/scripts/deploy_aes67.sh"
  fi
fi

if [ "${#MISSING_PKGS[@]}" -gt 0 ] && [ "${INSTALL}" -eq 1 ]; then
  if ! command -v apt-get >/dev/null 2>&1; then
    echo "[requirements] Cannot install packages: apt-get not found." >&2
    exit 1
  fi
  echo "[requirements] Installing missing apt packages for ${PROJECT}: ${MISSING_PKGS[*]}"
  if [ "$(id -u)" -eq 0 ]; then
    apt-get update
    DEBIAN_FRONTEND=noninteractive apt-get install -y "${MISSING_PKGS[@]}"
  else
    sudo apt-get update
    sudo env DEBIAN_FRONTEND=noninteractive apt-get install -y "${MISSING_PKGS[@]}"
  fi
  MISSING_PKGS=()
  MISSING_NOTES=()
  exec "$0" --project "${PROJECT}" --flags "${FLAGS_FILE}" --mode "${MODE}"
fi

if [ "${#MISSING_NOTES[@]}" -gt 0 ]; then
  echo "[requirements] ${PROJECT}: missing framework requirements for mode '${MODE}'." >&2
  for note in "${MISSING_NOTES[@]}"; do
    echo "  - ${note}" >&2
  done
  echo >&2
  if [ "${#MISSING_PKGS[@]}" -gt 0 ]; then
    echo "Fix apt packages with:" >&2
    echo "  sudo common/scripts/check_pi_requirements.sh --install --project ${PROJECT} --flags ${FLAGS_FILE:-/dev/null} --mode ${MODE}" >&2
  fi
  echo "For AES67/RAVENNA projects, also run:" >&2
  echo "  sudo common/scripts/deploy_aes67.sh" >&2
  exit 1
fi

echo "[requirements] ${PROJECT}: OK for mode '${MODE}'."
