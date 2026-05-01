#!/usr/bin/env python3
"""Local web frontend for the host RAVENNA bridge."""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
from http import HTTPStatus
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import parse_qs, urlparse


SCRIPT_DIR = Path(__file__).resolve().parent
BRIDGECTL = SCRIPT_DIR / "bridgectl.py"
INDEX_HTML = SCRIPT_DIR / "ravennakit_bridge_web.html"


def run_ctl(*args: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        [sys.executable, str(BRIDGECTL), *args],
        text=True,
        capture_output=True,
    )


def json_response(handler: BaseHTTPRequestHandler, payload: dict, status: int = HTTPStatus.OK) -> None:
    body = json.dumps(payload, indent=2).encode("utf-8")
    handler.send_response(status)
    handler.send_header("Content-Type", "application/json; charset=utf-8")
    handler.send_header("Content-Length", str(len(body)))
    handler.end_headers()
    handler.wfile.write(body)


class BridgeHandler(BaseHTTPRequestHandler):
    def log_message(self, fmt: str, *args) -> None:
        # Keep the terminal clean; the UI itself exposes state and logs.
        return

    def do_GET(self) -> None:
        parsed = urlparse(self.path)
        if parsed.path == "/":
            self._serve_index()
            return
        if parsed.path == "/api/config":
            self._serve_config()
            return
        if parsed.path == "/api/status":
            self._serve_status()
            return
        if parsed.path == "/api/logs":
            params = parse_qs(parsed.query)
            lines = params.get("lines", ["80"])[0]
            self._serve_logs(lines)
            return
        json_response(self, {"error": "not found"}, HTTPStatus.NOT_FOUND)

    def do_POST(self) -> None:
        parsed = urlparse(self.path)
        if parsed.path == "/api/config":
            self._update_config()
            return
        if parsed.path == "/api/start":
            self._run_action("start")
            return
        if parsed.path == "/api/stop":
            self._run_action("stop")
            return
        if parsed.path == "/api/wait":
            self._run_action("wait", "30")
            return
        if parsed.path == "/api/restart":
            self._run_action("restart", "30")
            return
        json_response(self, {"error": "not found"}, HTTPStatus.NOT_FOUND)

    def _read_json_body(self) -> dict:
        length = int(self.headers.get("Content-Length", "0"))
        raw = self.rfile.read(length) if length > 0 else b"{}"
        try:
            return json.loads(raw.decode("utf-8"))
        except json.JSONDecodeError:
            return {}

    def _serve_index(self) -> None:
        body = INDEX_HTML.read_bytes()
        self.send_response(HTTPStatus.OK)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def _serve_config(self) -> None:
        proc = run_ctl("config", "dump", "--json")
        if proc.returncode != 0:
            json_response(self, {"ok": False, "stdout": proc.stdout, "stderr": proc.stderr}, HTTPStatus.INTERNAL_SERVER_ERROR)
            return
        config = json.loads(proc.stdout)
        path_proc = run_ctl("config", "path")
        json_response(self, {"ok": True, "config": config, "path": path_proc.stdout.strip()})

    def _serve_status(self) -> None:
        proc = run_ctl("status", "--json")
        if proc.returncode not in (0, 1):
            json_response(self, {"ok": False, "stdout": proc.stdout, "stderr": proc.stderr}, HTTPStatus.INTERNAL_SERVER_ERROR)
            return
        payload = json.loads(proc.stdout)
        payload["ok"] = True
        json_response(self, payload)

    def _serve_logs(self, lines: str) -> None:
        proc = run_ctl("logs", lines)
        json_response(
            self,
            {
                "ok": proc.returncode == 0,
                "stdout": proc.stdout,
                "stderr": proc.stderr,
                "returncode": proc.returncode,
            },
        )

    def _update_config(self) -> None:
        payload = self._read_json_body()
        config = payload.get("config", {})
        pairs = [f"{key}={value}" for key, value in config.items()]
        proc = run_ctl("config", "set-many", *pairs)
        status = HTTPStatus.OK if proc.returncode == 0 else HTTPStatus.INTERNAL_SERVER_ERROR
        json_response(
            self,
            {
                "ok": proc.returncode == 0,
                "stdout": proc.stdout,
                "stderr": proc.stderr,
                "returncode": proc.returncode,
            },
            status,
        )

    def _run_action(self, action: str, *extra: str) -> None:
        proc = run_ctl(action, *extra)
        status = HTTPStatus.OK if proc.returncode == 0 else HTTPStatus.INTERNAL_SERVER_ERROR
        json_response(
            self,
            {
                "ok": proc.returncode == 0,
                "stdout": proc.stdout,
                "stderr": proc.stderr,
                "returncode": proc.returncode,
            },
            status,
        )


def main() -> int:
    parser = argparse.ArgumentParser(description="Local web UI for the host RAVENNA bridge.")
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8765)
    args = parser.parse_args()

    server = ThreadingHTTPServer((args.host, args.port), BridgeHandler)
    print(f"[bridge-web] Open http://{args.host}:{args.port}", flush=True)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n[bridge-web] Stopped.", flush=True)
    finally:
        server.server_close()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
