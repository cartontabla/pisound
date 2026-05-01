#!/usr/bin/env python3
"""Minimal RTSP server that advertises the Pi return stream as a RAVENNA session."""

from __future__ import annotations

import argparse
import json
import socket
import socketserver
import urllib.request


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--api-base", default="http://127.0.0.1:8090")
    p.add_argument("--bind-host", default="0.0.0.0")
    p.add_argument("--bind-port", type=int, default=8554)
    p.add_argument("--origin-address", default="auto")
    p.add_argument("--session-name", default="Pisound_Out")
    p.add_argument("--sample-rate", type=int, default=0)
    p.add_argument("--channels", type=int, default=0)
    p.add_argument("--framecount", type=int, default=0)
    p.add_argument("--ttl", type=int, default=0)
    p.add_argument("--multicast-address", default="")
    p.add_argument("--multicast-port", type=int, default=0)
    p.add_argument("--payload-type", type=int, default=0)
    return p.parse_args()


def http_json(url: str) -> dict:
    with urllib.request.urlopen(url, timeout=5.0) as resp:
        return json.load(resp)


def ptp_gmid(api_base: str) -> str:
    try:
        status = http_json(f"{api_base.rstrip('/')}/api/ptp/status")
    except Exception:
        return "00-00-00-00-00-00-00-00"
    return str(status.get("gmid", "00-00-00-00-00-00-00-00")).lower()


def local_ipv4() -> str:
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
            sock.connect(("239.255.255.250", 1900))
            host = sock.getsockname()[0]
        if host and not host.startswith("127."):
            return host
    except OSError:
        pass
    try:
        host = socket.gethostbyname(socket.gethostname())
        if host and not host.startswith("127."):
            return host
    except OSError:
        pass
    return "127.0.0.1"


def source_config(api_base: str, session_name: str) -> dict:
    try:
        payload = http_json(f"{api_base.rstrip('/')}/api/streams")
    except Exception:
        return {}
    for source in payload.get("sources", []):
        if source.get("name") == session_name:
            return source
    return {}


def source_audio_format(source: dict, args: argparse.Namespace) -> tuple[int, int]:
    codec = str(source.get("codec", ""))
    sample_rate = args.sample_rate or 48000
    channels = args.channels or 2
    if codec.startswith("L24/"):
        parts = codec.split("/")
        if len(parts) >= 2:
            try:
                sample_rate = int(parts[1])
            except ValueError:
                pass
        if len(parts) >= 3:
            try:
                channels = int(parts[2])
            except ValueError:
                pass
    if "map" in source and isinstance(source["map"], list) and source["map"]:
        channels = len(source["map"])
    return sample_rate, channels


def build_sdp(args: argparse.Namespace) -> str:
    gmid = ptp_gmid(args.api_base)
    source = source_config(args.api_base, args.session_name)
    sample_rate, channels = source_audio_format(source, args)
    framecount = args.framecount or int(source.get("max_samples_per_packet", 128))
    payload_type = args.payload_type or int(source.get("payload_type", 98))
    ttl = args.ttl or int(source.get("ttl", 15))
    multicast_address = str(source.get("address", args.multicast_address or "239.69.0.3"))
    multicast_port = args.multicast_port or 5004
    origin_address = args.origin_address if args.origin_address != "auto" else local_ipv4()
    ptime = (1000.0 * framecount) / float(sample_rate)
    ptime_text = f"{ptime:.2f}".rstrip("0").rstrip(".")
    return (
        "v=0\r\n"
        f"o=- 1 0 IN IP4 {origin_address}\r\n"
        f"s={args.session_name}\r\n"
        "t=0 0\r\n"
        "a=clock-domain:PTPv2 0\r\n"
        f"a=ts-refclk:ptp=IEEE1588-2008:{gmid}:0\r\n"
        "a=mediaclk:direct=0\r\n"
        f"m=audio {multicast_port} RTP/AVP {payload_type}\r\n"
        f"c=IN IP4 {multicast_address}/{ttl}\r\n"
        f"a=rtpmap:{payload_type} L24/{sample_rate}/{channels}\r\n"
        f"a=ptime:{ptime_text}\r\n"
        "a=recvonly\r\n"
        f"a=ts-refclk:ptp=IEEE1588-2008:{gmid}:0\r\n"
        "a=clock-domain:PTPv2 0\r\n"
        "a=sync-time:0\r\n"
        f"a=source-filter: incl IN IP4 {multicast_address} {origin_address}\r\n"
        f"a=framecount:{framecount}\r\n"
    )


class RtspHandler(socketserver.StreamRequestHandler):
    server: "RtspServer"

    def handle(self) -> None:
        request_line = self.rfile.readline().decode("utf-8", "replace").strip()
        if not request_line:
            return
        headers: dict[str, str] = {}
        while True:
            line = self.rfile.readline().decode("utf-8", "replace")
            if line in ("\r\n", "\n", ""):
                break
            key, _, value = line.partition(":")
            headers[key.strip().lower()] = value.strip()

        parts = request_line.split()
        method = parts[0] if parts else ""
        uri = parts[1] if len(parts) > 1 else "*"
        cseq = headers.get("cseq", "1")

        if method == "OPTIONS":
            self.send_response(
                200,
                "OK",
                cseq,
                extra_headers={"Public": "DESCRIBE, OPTIONS, TEARDOWN, GET_PARAMETER"},
            )
            return

        if method == "DESCRIBE" and uri.endswith(f"/by-name/{self.server.session_name}"):
            sdp = build_sdp(self.server.args)
            self.send_response(
                200,
                "OK",
                cseq,
                body=sdp,
                extra_headers={"Content-Type": "application/sdp"},
            )
            return

        if method in {"GET_PARAMETER", "TEARDOWN"}:
            self.send_response(200, "OK", cseq)
            return

        self.send_response(404, "Not Found", cseq)

    def send_response(
        self,
        status_code: int,
        reason: str,
        cseq: str,
        body: str = "",
        extra_headers: dict[str, str] | None = None,
    ) -> None:
        payload = body.encode("utf-8")
        lines = [
            f"RTSP/1.0 {status_code} {reason}",
            f"CSeq: {cseq}",
            f"Content-Length: {len(payload)}",
        ]
        if extra_headers:
            for key, value in extra_headers.items():
                lines.append(f"{key}: {value}")
        lines.append("")
        lines.append("")
        self.wfile.write("\r\n".join(lines).encode("utf-8") + payload)


class RtspServer(socketserver.ThreadingTCPServer):
    allow_reuse_address = True

    def __init__(self, server_address: tuple[str, int], handler_cls, args: argparse.Namespace):
        super().__init__(server_address, handler_cls)
        self.args = args
        self.session_name = args.session_name


def main() -> int:
    args = parse_args()
    with RtspServer((args.bind_host, args.bind_port), RtspHandler, args) as server:
        server.serve_forever()


if __name__ == "__main__":
    raise SystemExit(main())
