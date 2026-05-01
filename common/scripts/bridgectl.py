#!/usr/bin/env python3
"""Thin control surface for the Mac/host RAVENNA bridge.

The real backend remains the existing shell-based runtime under common/stream.
This script centralizes:
  - config file management
  - start/stop/wait/status/logs commands
  - a stable CLI that a GUI can call
"""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path


SCRIPT_DIR = Path(__file__).resolve().parent
ROOT_DIR = SCRIPT_DIR.parent.parent
STREAM_DIR = ROOT_DIR / "common" / "stream"
ENV_PATH = STREAM_DIR / "ravennakit_bridge_mac.env"
ENV_EXAMPLE_PATH = STREAM_DIR / "ravennakit_bridge_mac.env.example"

BRIDGE_SCRIPTS = {
    "start": STREAM_DIR / "start_ravennakit_bridge_mac.sh",
    "stop": STREAM_DIR / "stop_ravennakit_bridge_mac.sh",
    "wait": STREAM_DIR / "wait_ravennakit_bridge_mac.sh",
    "status": STREAM_DIR / "status_ravennakit_bridge_mac.sh",
    "logs": STREAM_DIR / "logs_ravennakit_bridge_mac.sh",
}

ENV_KEY_RE = re.compile(r"^([A-Za-z_][A-Za-z0-9_]*)=(.*)$")

DEFAULTS = {
    "INPUT_DEVICE": "BlackHole 16ch",
    "OUTPUT_DEVICE": "BlackHole 16ch",
    "INPUT_START_CHANNEL": "1",
    "OUTPUT_START_CHANNEL": "3",
    "NETWORK_INTERFACE": "en0",
    "TX_DESTINATION": "239.69.0.2",
    "TX_PORT": "5004",
    "RX_PORT": "5004",
    "SAMPLE_RATE": "48000",
    "BLOCK_SIZE": "128",
    "CHANNELS": "2",
    "PAYLOAD_TYPE": "98",
    "SESSION_NAME": "pisound",
    "TX_SESSION_NAME": "Pisound_In",
    "RX_SESSION_NAME": "",
    "PI_SOURCE_NAME": "Pisound_Out",
    "PI_API_BASE": "http://patchbox.local:8090",
    "RX_REQUIRE_DELAY_FRAMES": "1024",
    "TX_TIMESTAMP_OFFSET_FRAMES": "480",
}


def parse_env_text(text: str) -> tuple[list[str], dict[str, str]]:
    lines = text.splitlines()
    data: dict[str, str] = {}
    for raw_line in lines:
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        match = ENV_KEY_RE.match(line)
        if not match:
            continue
        key, raw_value = match.groups()
        value = raw_value.strip()
        if len(value) >= 2 and value[0] == value[-1] and value[0] in {"'", '"'}:
            value = value[1:-1]
        data[key] = value
    return lines, data


def load_defaults() -> tuple[list[str], dict[str, str]]:
    if ENV_EXAMPLE_PATH.exists():
        example_lines, example_data = parse_env_text(ENV_EXAMPLE_PATH.read_text())
        merged = DEFAULTS.copy()
        merged.update(example_data)
        return example_lines, merged
    return [], DEFAULTS.copy()


def load_env() -> tuple[list[str], dict[str, str], bool]:
    example_lines, defaults = load_defaults()
    if not ENV_PATH.exists():
        return example_lines, defaults, False
    lines, data = parse_env_text(ENV_PATH.read_text())
    merged = defaults.copy()
    merged.update(data)
    return lines, merged, True


def shell_quote(value: str) -> str:
    escaped = value.replace("\\", "\\\\").replace('"', '\\"')
    return f'"{escaped}"'


def update_lines(existing_lines: list[str], merged: dict[str, str], keys_to_update: set[str]) -> list[str]:
    if not existing_lines:
        if ENV_EXAMPLE_PATH.exists():
            existing_lines = ENV_EXAMPLE_PATH.read_text().splitlines()
        else:
            existing_lines = []

    output: list[str] = []
    seen: set[str] = set()
    for line in existing_lines:
        match = ENV_KEY_RE.match(line.strip())
        if not match:
            output.append(line)
            continue
        key = match.group(1)
        if key in keys_to_update:
            output.append(f"{key}={shell_quote(merged[key])}")
            seen.add(key)
        else:
            output.append(line)
            seen.add(key)

    missing = [key for key in keys_to_update if key not in seen]
    if missing:
        if output and output[-1].strip():
            output.append("")
        for key in missing:
            output.append(f"{key}={shell_quote(merged[key])}")
    return output


def save_env(updates: dict[str, str]) -> None:
    lines, merged, exists = load_env()
    merged.update(updates)
    final_lines = update_lines(lines if exists else lines, merged, set(updates))
    ENV_PATH.write_text("\n".join(final_lines).rstrip() + "\n")


def run_bridge_script(name: str, args: list[str]) -> int:
    script = BRIDGE_SCRIPTS[name]
    env = os.environ.copy()
    env["RAVENNAKIT_BRIDGE_ENV"] = str(ENV_PATH)
    proc = subprocess.run([str(script), *args], env=env)
    return proc.returncode


def capture_bridge_script(name: str, args: list[str]) -> subprocess.CompletedProcess[str]:
    env = os.environ.copy()
    env["RAVENNAKIT_BRIDGE_ENV"] = str(ENV_PATH)
    return subprocess.run(
        [str(BRIDGE_SCRIPTS[name]), *args],
        env=env,
        text=True,
        capture_output=True,
    )


def cmd_config(args: argparse.Namespace) -> int:
    if args.config_action == "path":
        print(ENV_PATH)
        return 0
    if args.config_action == "init":
        if ENV_PATH.exists() and not args.force:
            print(f"[bridgectl] Config already exists: {ENV_PATH}")
            return 0
        if ENV_EXAMPLE_PATH.exists():
            shutil.copyfile(ENV_EXAMPLE_PATH, ENV_PATH)
        else:
            save_env(DEFAULTS)
        print(f"[bridgectl] Created config: {ENV_PATH}")
        return 0
    if args.config_action == "dump":
        _, data, _ = load_env()
        if args.json:
            print(json.dumps(data, indent=2, sort_keys=True))
        else:
            for key in sorted(data):
                print(f"{key}={data[key]}")
        return 0
    if args.config_action == "get":
        _, data, _ = load_env()
        value = data.get(args.key)
        if value is None:
            print(f"[bridgectl] Unknown key: {args.key}", file=sys.stderr)
            return 1
        print(value)
        return 0
    if args.config_action == "set":
        save_env({args.key: args.value})
        print(f"[bridgectl] Updated {args.key} in {ENV_PATH}")
        return 0
    if args.config_action == "set-many":
        updates = {}
        for pair in args.pairs:
            if "=" not in pair:
                print(f"[bridgectl] Expected KEY=VALUE, got: {pair}", file=sys.stderr)
                return 1
            key, value = pair.split("=", 1)
            updates[key] = value
        save_env(updates)
        print(f"[bridgectl] Updated {len(updates)} value(s) in {ENV_PATH}")
        return 0
    return 1


def cmd_start(_: argparse.Namespace) -> int:
    if not ENV_PATH.exists() and ENV_EXAMPLE_PATH.exists():
        shutil.copyfile(ENV_EXAMPLE_PATH, ENV_PATH)
    return run_bridge_script("start", [])


def cmd_stop(_: argparse.Namespace) -> int:
    return run_bridge_script("stop", [])


def cmd_wait(args: argparse.Namespace) -> int:
    return run_bridge_script("wait", [str(args.timeout)])


def cmd_status(args: argparse.Namespace) -> int:
    proc = capture_bridge_script("status", [])
    if args.json:
        lines = [line.rstrip() for line in proc.stdout.splitlines()]
        payload = {
            "active": any(line.strip() == "[mac-bridge] active" for line in lines),
            "tracked_pid": "",
            "live_pids": [],
            "log": "",
            "stdout": proc.stdout,
            "stderr": proc.stderr,
            "returncode": proc.returncode,
        }
        for line in lines:
            if line.startswith("[mac-bridge] Tracked PID:"):
                payload["tracked_pid"] = line.split(":", 1)[1].strip()
            elif line.startswith("[mac-bridge] Live PID(s):"):
                pids = line.split(":", 1)[1].strip()
                payload["live_pids"] = [pid for pid in pids.split() if pid]
            elif line.startswith("[mac-bridge] Log:"):
                payload["log"] = line.split(":", 1)[1].strip()
        print(json.dumps(payload, indent=2))
        return proc.returncode
    sys.stdout.write(proc.stdout)
    sys.stderr.write(proc.stderr)
    return proc.returncode


def cmd_logs(args: argparse.Namespace) -> int:
    return run_bridge_script("logs", [str(args.lines)])


def cmd_restart(args: argparse.Namespace) -> int:
    code = cmd_stop(args)
    if code != 0:
        return code
    code = cmd_start(args)
    if code != 0:
        return code
    return cmd_wait(args)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Control the host RAVENNA bridge.")
    sub = parser.add_subparsers(dest="command", required=True)

    sub.add_parser("start").set_defaults(func=cmd_start)
    sub.add_parser("stop").set_defaults(func=cmd_stop)

    wait = sub.add_parser("wait")
    wait.add_argument("timeout", nargs="?", type=int, default=30)
    wait.set_defaults(func=cmd_wait)

    restart = sub.add_parser("restart")
    restart.add_argument("timeout", nargs="?", type=int, default=30)
    restart.set_defaults(func=cmd_restart)

    status = sub.add_parser("status")
    status.add_argument("--json", action="store_true")
    status.set_defaults(func=cmd_status)

    logs = sub.add_parser("logs")
    logs.add_argument("lines", nargs="?", type=int, default=40)
    logs.set_defaults(func=cmd_logs)

    config = sub.add_parser("config")
    config_sub = config.add_subparsers(dest="config_action", required=True)

    config_sub.add_parser("path")

    init = config_sub.add_parser("init")
    init.add_argument("--force", action="store_true")

    dump = config_sub.add_parser("dump")
    dump.add_argument("--json", action="store_true")

    get_cmd = config_sub.add_parser("get")
    get_cmd.add_argument("key")

    set_cmd = config_sub.add_parser("set")
    set_cmd.add_argument("key")
    set_cmd.add_argument("value")

    set_many = config_sub.add_parser("set-many")
    set_many.add_argument("pairs", nargs="+")

    config.set_defaults(func=cmd_config)

    gui = sub.add_parser("gui")
    gui.set_defaults(func=lambda _: subprocess.call([sys.executable, str(SCRIPT_DIR / "ravennakit_bridge_web.py")]))

    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    raise SystemExit(main())
