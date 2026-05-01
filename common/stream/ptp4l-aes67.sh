#!/usr/bin/env bash
set -euo pipefail

AES67_IFACE="${AES67_IFACE:-eth0}"
AES67_PTP_EXTRA_IFACES="${AES67_PTP_EXTRA_IFACES:-}"
AES67_PTP_SOFTWARE_TS="${AES67_PTP_SOFTWARE_TS:-0}"

args=()

if [[ "${AES67_PTP_SOFTWARE_TS}" == "1" ]]; then
  args+=(-S)
fi

args+=(-i "${AES67_IFACE}")

if [[ -n "${AES67_PTP_EXTRA_IFACES}" ]]; then
  normalized_extra_ifaces="${AES67_PTP_EXTRA_IFACES//,/ }"
  for iface in ${normalized_extra_ifaces}; do
    [[ -n "${iface}" ]] || continue
    args+=(-i "${iface}")
  done
fi

exec /usr/sbin/ptp4l "${args[@]}" -f /etc/pisound/ptp4l-aes67.conf
