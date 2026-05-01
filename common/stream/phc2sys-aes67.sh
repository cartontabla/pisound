#!/usr/bin/env bash
set -euo pipefail

AES67_IFACE="${AES67_IFACE:-eth0}"
AES67_PHC_DEVICE="${AES67_PHC_DEVICE:-}"
AES67_PTP_SOFTWARE_TS="${AES67_PTP_SOFTWARE_TS:-0}"
PHC_PATH="/sys/class/net/${AES67_IFACE}/device/ptp"

if [ -n "${AES67_PHC_DEVICE}" ]; then
  PHC_DEV="${AES67_PHC_DEVICE#/dev/}"
elif [ -d "${PHC_PATH}" ]; then
  PHC_DEV="$(basename "$(readlink -f "${PHC_PATH}")")"
elif [ -e /dev/ptp0 ]; then
  # Some kernels expose the NIC timestamp device without the sysfs ptp symlink.
  PHC_DEV="ptp0"
else
  echo "phc2sys-aes67: cannot resolve PHC for interface ${AES67_IFACE}" >&2
  exit 1
fi

if [ -z "${PHC_DEV}" ]; then
  echo "phc2sys-aes67: empty PHC device for interface ${AES67_IFACE}" >&2
  exit 1
fi

if [[ "${AES67_PTP_SOFTWARE_TS}" == "1" ]]; then
  # In software timestamp mode ptp4l uses CLOCK_REALTIME as the PTP clock.
  # There is no PHC discipline loop to run here; trying to steer CLOCK_REALTIME
  # from /dev/ptp0 (or the reverse) can create huge offset/frequency logs on
  # Patchbox kernels. Keep the service active as an intentional no-op so the
  # runtime target remains simple and deterministic.
  echo "phc2sys-aes67: software timestamp mode; phc2sys not required for ${AES67_IFACE}" >&2
  exec /bin/sleep infinity
fi

exec /usr/sbin/phc2sys -s "/dev/${PHC_DEV}" -c CLOCK_REALTIME -O 0 -m
