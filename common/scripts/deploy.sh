#!/usr/bin/env bash
set -euo pipefail

PROJECT="${1:-}"
if [[ -z "$PROJECT" ]]; then
  echo "Uso: $0 <PROJECT>   (ej: $0 amp)"
  exit 1
fi

SCRIPTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"   # .../common/scripts
COMMON_DIR="$(cd "${SCRIPTS_DIR}/.." && pwd)"                 # .../common
REPO_DIR="$(cd "${COMMON_DIR}/.." && pwd)"                    # .../pisound
PROJ_DIR="${REPO_DIR}/${PROJECT}"

LV2_DIR="/var/modep/lv2"
MAKEFILE="${COMMON_DIR}/Makefile.lv2"
GEN_SCRIPT="${SCRIPTS_DIR}/gen_lv2_wrapper.sh"

[[ -d "${PROJ_DIR}" ]]     || { echo "ERROR: no existe proyecto: ${PROJ_DIR}"; exit 1; }
[[ -f "${MAKEFILE}" ]]     || { echo "ERROR: falta ${MAKEFILE}"; exit 1; }
[[ -x "${GEN_SCRIPT}" ]]   || { echo "ERROR: falta o no es ejecutable ${GEN_SCRIPT}"; exit 1; }


BUNDLE_NAME="${PROJECT}.lv2"
SRC_DIR="${PROJ_DIR}/lv2"                  # layout plano en repo
DST_BUNDLE_DIR="${LV2_DIR}/${BUNDLE_NAME}" # bundle en destino

# ---- Auto-generar wrapper LV2 si falta ----
GEN_SCRIPT="${SCRIPTS_DIR}/gen_lv2_wrapper.sh"
TTL_REL="lv2/${PROJECT}.ttl"
OUT_REL="lv2"
WRAP_C="${PROJ_DIR}/${OUT_REL}/${PROJECT}_lv2.c"

if [[ ! -f "${WRAP_C}" || "${PROJ_DIR}/${TTL_REL}" -nt "${WRAP_C}" ]]; then
  echo "=== Generando wrapper: ${PROJECT}_lv2.c ==="
  ( cd "${PROJ_DIR}" && "${GEN_SCRIPT}" "${PROJECT}" "${TTL_REL}" "${OUT_REL}" )
fi

echo "=== Deploy ${BUNDLE_NAME} ==="
make -f "$MAKEFILE" PROJECT="$PROJECT" clean
make -f "$MAKEFILE" PROJECT="$PROJECT" all

[[ -f "${SRC_DIR}/manifest.ttl" ]] || { echo "ERROR: falta ${SRC_DIR}/manifest.ttl"; exit 1; }

sudo mkdir -p "$LV2_DIR"
sudo rm -rf "$DST_BUNDLE_DIR"
sudo mkdir -p "$DST_BUNDLE_DIR"

# Copiar lo esencial al bundle
sudo cp -a "${SRC_DIR}"/*.ttl "$DST_BUNDLE_DIR/" 2>/dev/null || true
sudo cp -a "${SRC_DIR}"/*.so  "$DST_BUNDLE_DIR/" 2>/dev/null || true

# MOD GUI si existe
if [[ -d "${SRC_DIR}/modgui" ]]; then
  sudo cp -a "${SRC_DIR}/modgui" "$DST_BUNDLE_DIR/"
fi

# cache + restart
if id -u modep >/dev/null 2>&1; then
  MODEP_HOME="$(getent passwd modep | cut -d: -f6)"
  sudo -u modep rm -rf "${MODEP_HOME}/.cache/lv2" 2>/dev/null || true
fi
sudo rm -rf /var/cache/mod-ui 2>/dev/null || true

sudo systemctl restart mod-ui 2>/dev/null || sudo systemctl restart modep-mod-ui 2>/dev/null || true
sudo systemctl restart mod-host 2>/dev/null || sudo systemctl restart modep-mod-host 2>/dev/null || true

echo "=== OK: ${DST_BUNDLE_DIR} ==="
