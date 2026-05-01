#!/usr/bin/env python3
"""Restore the Pi-side AES67 sessions for the RavennaKit bridge."""

from __future__ import annotations

import argparse
import json
import re
import sys
import time
import urllib.error
import urllib.request


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--api-base", default="http://127.0.0.1:8090")
    p.add_argument("--tx-name", default="Pisound_In")
    p.add_argument("--rx-name", default="Pisound_Out")
    p.add_argument("--rx-address", default="239.69.0.3")
    p.add_argument("--payload-type", type=int, default=98)
    p.add_argument("--channels", type=int, default=2)
    p.add_argument("--max-samples-per-packet", type=int, default=48)
    p.add_argument("--delay", type=int, default=1024)
    p.add_argument("--discover-timeout", type=float, default=60.0)
    p.add_argument("--discover-interval", type=float, default=1.0)
    p.add_argument("--watch", action="store_true")
    p.add_argument("--watch-interval", type=float, default=2.0)
    return p.parse_args()


def http_json(method: str, url: str, payload: dict | None = None) -> dict | list | None:
    data = None
    headers = {}
    if payload is not None:
        data = json.dumps(payload).encode("utf-8")
        headers["Content-Type"] = "application/json"
    req = urllib.request.Request(url, data=data, method=method, headers=headers)
    with urllib.request.urlopen(req, timeout=5.0) as resp:
        body = resp.read()
    if not body:
        return None
    return json.loads(body.decode("utf-8"))


def put_json(url: str, payload: dict) -> None:
    http_json("PUT", url, payload)


def get_json(url: str) -> dict | list | None:
    return http_json("GET", url)


def normalize_session_name(value: str) -> str:
    value = value.strip().lower()
    value = re.sub(r"[^a-z0-9]+", "", value)
    return value


def source_matches(source: dict, tx_name: str) -> bool:
    name = str(source.get("name", "") or "")
    sdp = str(source.get("sdp", "") or "")
    target = normalize_session_name(tx_name)

    if normalize_session_name(name) == target:
        return True

    if sdp:
        for line in sdp.splitlines():
            if line.startswith("s=") and normalize_session_name(line[2:]) == target:
                return True

    return False


def wait_for_remote_sdp(api_base: str, tx_name: str, timeout_s: float, interval_s: float) -> str:
    deadline = time.monotonic() + timeout_s
    browse_url = f"{api_base}/api/browse/sources/all"
    while time.monotonic() < deadline:
        try:
            payload = get_json(browse_url)
        except (urllib.error.URLError, TimeoutError):
            time.sleep(interval_s)
            continue

        if isinstance(payload, dict):
            for source in payload.get("remote_sources", []):
                if source_matches(source, tx_name) and source.get("sdp"):
                    return str(source["sdp"])
        time.sleep(interval_s)
    raise RuntimeError(f"timed out waiting for remote source '{tx_name}'")


def get_sink(api_base: str, index: int = 0) -> dict:
    streams = get_json(f"{api_base}/api/streams")
    if not isinstance(streams, dict):
        return {}
    sinks = streams.get("sinks", [])
    if not isinstance(sinks, list) or index >= len(sinks):
        return {}
    sink = sinks[index]
    return sink if isinstance(sink, dict) else {}


def get_sink_status(api_base: str, index: int = 0) -> dict:
    status = get_json(f"{api_base}/api/sink/status/{index}")
    return status if isinstance(status, dict) else {}


def put_source(api_base: str, args: argparse.Namespace) -> None:
    source_payload = {
        "enabled": True,
        "name": args.rx_name,
        "io": "Audio Device",
        "map": list(range(args.channels)),
        "max_samples_per_packet": args.max_samples_per_packet,
        "codec": "L24",
        "address": args.rx_address,
        "ttl": 15,
        "payload_type": args.payload_type,
        "dscp": 34,
        "refclk_ptp_traceable": False,
    }
    put_json(f"{args.api_base.rstrip('/')}/api/source/0", source_payload)


def put_sink(api_base: str, args: argparse.Namespace, remote_sdp: str) -> None:
    sink_payload = {
        "name": "Mac Bridge TX",
        "io": "Audio Device",
        "source": "",
        "use_sdp": True,
        "sdp": remote_sdp,
        "delay": args.delay,
        "ignore_refclk_gmid": True,
        "map": list(range(args.channels)),
    }
    put_json(f"{api_base}/api/sink/0", sink_payload)


def sink_needs_refresh(api_base: str, remote_sdp: str) -> bool:
    sink = get_sink(api_base, 0)
    sink_status = get_sink_status(api_base, 0)
    current_sdp = str(sink.get("sdp", "") or "")
    if current_sdp != remote_sdp:
        return True

    flags = sink_status.get("sink_flags", {})
    if not isinstance(flags, dict):
        return False
    return bool(flags.get("rtp_ssrc_error") or flags.get("all_muted"))


def main() -> int:
    args = parse_args()
    api_base = args.api_base.rstrip("/")
    put_source(api_base, args)

    while True:
        remote_sdp = wait_for_remote_sdp(
            api_base,
            args.tx_name,
            timeout_s=args.discover_timeout,
            interval_s=args.discover_interval,
        )

        if sink_needs_refresh(api_base, remote_sdp):
            put_sink(api_base, args, remote_sdp)

        streams = get_json(f"{api_base}/api/streams")
        sys.stdout.write(json.dumps(streams, indent=2))
        sys.stdout.write("\n")
        sys.stdout.flush()

        if not args.watch:
            return 0
        time.sleep(args.watch_interval)


if __name__ == "__main__":
    raise SystemExit(main())
