#!/usr/bin/env bash
set -euo pipefail

CLIENT="${1:-}"
if [[ -z "${CLIENT}" ]]; then
  echo "Uso: $0 <jack_client_name>   (ej: $0 amp)"
  exit 1
fi

CAP_L="${2:-system:capture_1}"
CAP_R="${3:-system:capture_2}"
PB_L="${4:-system:playback_1}"
PB_R="${5:-system:playback_2}"

# Garantizar que existen aunque haya 'set -u'
: "${IN_L:=}" "${IN_R:=}" "${OUT_L:=}" "${OUT_R:=}"

for _ in $(seq 1 40); do
  PORTS="$(jack_lsp 2>/dev/null || true)"

  if [[ -z "${IN_L:-}"  ]] && printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:in_l";  then IN_L="${CLIENT}:in_l";  fi
  if [[ -z "${IN_R:-}"  ]] && printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:in_r";  then IN_R="${CLIENT}:in_r";  fi
  if [[ -z "${OUT_L:-}" ]] && printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:out_l"; then OUT_L="${CLIENT}:out_l"; fi
  if [[ -z "${OUT_R:-}" ]] && printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:out_r"; then OUT_R="${CLIENT}:out_r"; fi

  if [[ -z "${IN_L:-}"  ]] && printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:in_1";  then IN_L="${CLIENT}:in_1";  fi
  if [[ -z "${IN_R:-}"  ]] && printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:in_2";  then IN_R="${CLIENT}:in_2";  fi
  if [[ -z "${OUT_L:-}" ]] && printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:out_1"; then OUT_L="${CLIENT}:out_1"; fi
  if [[ -z "${OUT_R:-}" ]] && printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:out_2"; then OUT_R="${CLIENT}:out_2"; fi

  [[ -n "${IN_L:-}" && -n "${IN_R:-}" && -n "${OUT_L:-}" && -n "${OUT_R:-}" ]] && break
  sleep 0.05
done

if [[ -z "${IN_L:-}" || -z "${IN_R:-}" || -z "${OUT_L:-}" || -z "${OUT_R:-}" ]]; then
  echo "[connect_jack] ERROR: no veo todos los puertos de ${CLIENT} (¿jalv -n ${CLIENT} está corriendo?)"
  exit 1
fi

# Quitar passthrough
jack_disconnect mod-monitor:out_1 system:playback_1 2>/dev/null || true
jack_disconnect mod-monitor:out_2 system:playback_2 2>/dev/null || true

# Limpiar conexiones previas del cliente
jack_disconnect "$CAP_L" "$IN_L" 2>/dev/null || true
jack_disconnect "$CAP_R" "$IN_R" 2>/dev/null || true
jack_disconnect "$OUT_L" "$PB_L" 2>/dev/null || true
jack_disconnect "$OUT_R" "$PB_R" 2>/dev/null || true

# Conectar
jack_connect "$CAP_L" "$IN_L" 2>/dev/null || true
jack_connect "$CAP_R" "$IN_R" 2>/dev/null || true
jack_connect "$OUT_L" "$PB_L" 2>/dev/null || true
jack_connect "$OUT_R" "$PB_R" 2>/dev/null || true

echo "[connect_jack] OK: ${CAP_L}/${CAP_R} -> ${CLIENT} -> ${PB_L}/${PB_R}"

if [[ ! -x "$0" ]]; then
  chmod +x "$0" 2>/dev/null || true
fi