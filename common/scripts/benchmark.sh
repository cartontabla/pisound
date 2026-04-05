#!/usr/bin/env bash
# benchmark.sh — Mide rendimiento en tiempo real de un efecto pisound
#
# Uso:
#   ./benchmark.sh <ProjectName>
#
# Ejecutar mientras el efecto está corriendo en la Pi:
#   ./benchmark.sh bypass
#
# Mide:
#   - Carga de CPU del proceso <project>_app
#   - Xruns JACK (sobrecargas de tiempo real)
#   - Latencia del sistema JACK (buffer size / sample rate)

set -euo pipefail

if [ $# -lt 1 ]; then
  echo "Usage: $0 <ProjectName>"
  exit 1
fi

PROJECT="$1"
APP="${PROJECT}_app"
DURATION=10  # segundos de medición

echo "============================================"
echo " benchmark.sh — pisound DSP benchmark"
echo " Project : $PROJECT"
echo " Duration: ${DURATION}s"
echo "============================================"

# --- Localizar proceso
PID=$(pgrep -x "$APP" 2>/dev/null || true)
if [ -z "$PID" ]; then
  echo "ERROR: '$APP' no está corriendo. Arráncalo primero."
  exit 1
fi
echo "PID: $PID"
echo ""

# --- Parámetros JACK
echo "[ JACK ]"
JACK_SR=$(jack_samplerate 2>/dev/null || echo "?")
JACK_BUF=$(jack_bufsize 2>/dev/null || echo "?")
echo "  Sample rate : $JACK_SR Hz"
echo "  Buffer size : $JACK_BUF frames"
if [ "$JACK_SR" != "?" ] && [ "$JACK_BUF" != "?" ]; then
  LATENCY_MS=$(echo "scale=2; $JACK_BUF * 1000 / $JACK_SR" | bc)
  echo "  Block latency: ${LATENCY_MS} ms  (${JACK_BUF}/${JACK_SR})"
fi
echo ""

# --- Carga CPU
echo "[ CPU — ${DURATION}s average ]"
if command -v pidstat &>/dev/null; then
  pidstat -u -p "$PID" 1 "$DURATION" | tail -2
else
  # Fallback: /proc
  CPU_SAMPLES=()
  for i in $(seq 1 "$DURATION"); do
    CPU=$(ps -p "$PID" -o %cpu= 2>/dev/null || echo "0")
    CPU_SAMPLES+=("$CPU")
    sleep 1
  done
  # Media simple
  TOTAL=0
  for v in "${CPU_SAMPLES[@]}"; do TOTAL=$(echo "$TOTAL + $v" | bc); done
  AVG=$(echo "scale=1; $TOTAL / ${#CPU_SAMPLES[@]}" | bc)
  echo "  CPU avg: ${AVG}%"
fi
echo ""

# --- Xruns JACK
echo "[ JACK xruns ]"
if command -v jack_iodelay &>/dev/null; then
  echo "  Midiendo xruns durante ${DURATION}s..."
  XRUNS_BEFORE=$(jack_lsp --statistics 2>/dev/null | grep -c xrun || echo "n/a")
  sleep "$DURATION"
  XRUNS_AFTER=$(jack_lsp --statistics 2>/dev/null | grep -c xrun || echo "n/a")
  echo "  Xruns detectados: $XRUNS_BEFORE → $XRUNS_AFTER"
else
  echo "  (jack_iodelay no disponible — instala jack-tools)"
fi

echo ""
echo "============================================"
echo " Benchmark completado."
echo "============================================"
