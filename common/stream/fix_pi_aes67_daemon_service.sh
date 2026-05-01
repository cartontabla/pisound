#!/usr/bin/env bash
set -euo pipefail

if [ "$(id -u)" -ne 0 ]; then
  echo "Run with sudo."
  exit 1
fi

UNIT=/etc/systemd/system/aes67-daemon.service

if [ ! -f "${UNIT}" ]; then
  echo "Missing ${UNIT}"
  exit 1
fi

sed -i 's/^Type=notify$/Type=simple/' "${UNIT}"
sed -i '/^WatchdogSec=10$/d' "${UNIT}"

systemctl daemon-reload
systemctl restart aes67-daemon.service

echo "Patched ${UNIT}:"
grep -E '^(Type|WatchdogSec)' "${UNIT}" || true
