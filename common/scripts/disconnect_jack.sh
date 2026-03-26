#!/usr/bin/env bash
set -u

CLIENT="${1:-}"
if [[ -z "$CLIENT" ]]; then
  echo "Uso: $0 <jack_client_name>   (ej: $0 amp)"
  exit 1
fi

# Puertos system por defecto (puedes sobreescribir con args 2..5)
CAP_L="${2:-system:capture_1}"
CAP_R="${3:-system:capture_2}"
PB_L="${4:-system:playback_1}"
PB_R="${5:-system:playback_2}"

# Detectar nombres de puertos del plugin (in_l/in_r/out_l/out_r o in_1/in_2/out_1/out_2)
IN_L=""; IN_R=""; OUT_L=""; OUT_R=""

PORTS="$(jack_lsp 2>/dev/null || true)"

if printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:in_l";  then IN_L="${CLIENT}:in_l";  fi
if printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:in_r";  then IN_R="${CLIENT}:in_r";  fi
if printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:out_l"; then OUT_L="${CLIENT}:out_l"; fi
if printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:out_r"; then OUT_R="${CLIENT}:out_r"; fi

if [[ -z "$IN_L"  ]] && printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:in_1";  then IN_L="${CLIENT}:in_1";  fi
if [[ -z "$IN_R"  ]] && printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:in_2";  then IN_R="${CLIENT}:in_2";  fi
if [[ -z "$OUT_L" ]] && printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:out_1"; then OUT_L="${CLIENT}:out_1"; fi
if [[ -z "$OUT_R" ]] && printf '%s\n' "$PORTS" | grep -qx "${CLIENT}:out_2"; then OUT_R="${CLIENT}:out_2"; fi

# Desconectar cliente (si no existen puertos, no pasa nada)
[[ -n "$IN_L"  ]] && jack_disconnect "$CAP_L" "$IN_L"  2>/dev/null || true
[[ -n "$IN_R"  ]] && jack_disconnect "$CAP_R" "$IN_R"  2>/dev/null || true
[[ -n "$OUT_L" ]] && jack_disconnect "$OUT_L" "$PB_L"  2>/dev/null || true
[[ -n "$OUT_R" ]] && jack_disconnect "$OUT_R" "$PB_R"  2>/dev/null || true

# Restaurar passthrough
jack_connect mod-monitor:out_1 system:playback_1 2>/dev/null || true
jack_connect mod-monitor:out_2 system:playback_2 2>/dev/null || true

echo "[disconnect_jack] OK: ${CLIENT} desconectado; mod-monitor restaurado"

if [[ ! -x "$0" ]]; then
  chmod +x "$0" 2>/dev/null || true
fi