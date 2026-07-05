#!/usr/bin/env python3
"""Firmware workflow for Sensorio."""

from __future__ import annotations

import argparse
import json
import os
import shlex
import shutil
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
FIRMWARE_DIR = ROOT / "firmware"
DOCKERFILE = ROOT / "docker" / "firmware.Dockerfile"
DEFAULT_IMAGE = "sensorio-firmware:idf-v4.4.8"
DEFAULT_PLATFORM = "linux/amd64"
DEFAULT_BAUD = 115200


def env_value(name: str, default: str) -> str:
    return os.environ.get(name, default)


def docker_platform() -> str:
    return env_value("SENSORIO_DOCKER_PLATFORM", DEFAULT_PLATFORM)


def docker_image() -> str:
    return env_value("SENSORIO_IDF_IMAGE", DEFAULT_IMAGE)


def run(cmd: list[str], check: bool = True, cwd: Path | None = None) -> int:
    print("+ " + shlex.join(cmd), flush=True)
    completed = subprocess.run(cmd, check=False, cwd=cwd)
    if check and completed.returncode != 0:
        raise SystemExit(completed.returncode)
    return completed.returncode


def docker_base_args() -> list[str]:
    args = [
        "docker",
        "run",
        "--rm",
        "--platform",
        docker_platform(),
        "-v",
        f"{ROOT}:/workspace",
        "-w",
        "/workspace/firmware",
        "-e",
        "HOME=/tmp",
    ]

    if os.name == "posix":
        args.extend(["--user", f"{os.getuid()}:{os.getgid()}"])

    return args


def docker_run(command: list[str]) -> int:
    args = docker_base_args()
    args.append(docker_image())
    args.extend(command)
    return run(args)


def build_image(_: argparse.Namespace) -> int:
    return run(
        [
            "docker",
            "buildx",
            "build",
            "--load",
            "--platform",
            docker_platform(),
            "-f",
            str(DOCKERFILE),
            "-t",
            docker_image(),
            str(ROOT),
        ]
    )


def build(_: argparse.Namespace) -> int:
    return docker_run(["idf.py", "build"])


def clean(_: argparse.Namespace) -> int:
    build_dir = FIRMWARE_DIR / "build"
    if build_dir.exists():
        print(f"removing {build_dir}")
        shutil.rmtree(build_dir)
    else:
        print(f"{build_dir} does not exist")
    return 0


def host_python() -> str:
    if os.name == "nt":
        candidates = [ROOT / ".venv" / "Scripts" / "python.exe"]
    else:
        candidates = [
            ROOT / ".venv" / "bin" / "python",
            ROOT / ".venv" / "bin" / "python3",
        ]

    for candidate in candidates:
        if candidate.exists():
            return str(candidate)

    raise RuntimeError(
        f"host venv is required for serial tools.\n{host_setup_hint()}"
    )


def host_setup_hint() -> str:
    if os.name == "nt":
        return (
            "Create the host venv and install serial tools:\n"
            "  py -3 -m venv .venv\n"
            "  .venv\\Scripts\\python -m pip install -r tools\\host-requirements.txt"
        )
    return (
        "Create the host venv and install serial tools:\n"
        "  python3 -m venv .venv\n"
        "  .venv/bin/python -m pip install -r tools/host-requirements.txt"
    )


def ensure_host_module(module: str, package: str) -> None:
    completed = subprocess.run(
        [
            host_python(),
            "-c",
            (
                "import importlib.util, sys; "
                "sys.exit(0 if importlib.util.find_spec(sys.argv[1]) else 1)"
            ),
            module,
        ],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
        check=False,
    )
    if completed.returncode != 0:
        raise RuntimeError(
            f"host Python package '{package}' is required.\n{host_setup_hint()}"
        )


def flash(args: argparse.Namespace) -> int:
    ensure_host_module("esptool", "esptool")
    build(args)
    return host_esptool_flash(args)


def flasher_args() -> dict[str, object]:
    path = FIRMWARE_DIR / "build" / "flasher_args.json"
    with path.open("r", encoding="utf-8") as file:
        return json.load(file)


def esptool_flash_command(
    args: argparse.Namespace, port: str, executable: list[str]
) -> list[str]:
    flash = flasher_args()
    extra_args = flash["extra_esptool_args"]
    write_flash_args = flash["write_flash_args"]
    flash_files = flash["flash_files"]

    if not isinstance(extra_args, dict):
        raise RuntimeError("invalid flasher_args.json: extra_esptool_args")
    if not isinstance(write_flash_args, list):
        raise RuntimeError("invalid flasher_args.json: write_flash_args")
    if not isinstance(flash_files, dict):
        raise RuntimeError("invalid flasher_args.json: flash_files")

    command = [
        *executable,
        "--chip",
        str(extra_args.get("chip", "esp32")),
        "-p",
        port,
        "-b",
        str(args.baud),
        "--before",
        args.before,
        "--after",
        args.after,
        "write_flash",
    ]
    command.extend(str(item) for item in write_flash_args)

    for offset, filename in sorted(flash_files.items(), key=lambda item: int(item[0], 0)):
        command.extend([str(offset), f"build/{filename}"])

    return command


def host_esptool_flash(args: argparse.Namespace) -> int:
    command = esptool_flash_command(args, args.port, [host_python(), "-m", "esptool"])
    return run(command, cwd=FIRMWARE_DIR)


def monitor(args: argparse.Namespace) -> int:
    ensure_host_module("serial", "pyserial")
    return run(
        [
            host_python(),
            "-m",
            "serial.tools.miniterm",
            "--raw",
            args.port,
            str(args.baud),
        ]
    )


def add_serial_args(parser: argparse.ArgumentParser) -> None:
    parser.add_argument(
        "--port",
        required=True,
        help="Serial device, for example /dev/cu.usbserial-0001",
    )
    parser.add_argument(
        "--baud",
        type=int,
        default=DEFAULT_BAUD,
        help=f"Flash/monitor baud rate, default {DEFAULT_BAUD}",
    )


def add_flash_args(parser: argparse.ArgumentParser) -> None:
    add_serial_args(parser)
    parser.add_argument(
        "--before",
        default="default_reset",
        choices=("default_reset", "no_reset", "no_reset_no_sync"),
        help="esptool reset behavior before flashing, default default_reset",
    )
    parser.add_argument(
        "--after",
        default="hard_reset",
        choices=("hard_reset", "soft_reset", "no_reset"),
        help="esptool reset behavior after flashing, default hard_reset",
    )


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--image",
        default=None,
        help=f"Docker image override, default {DEFAULT_IMAGE}",
    )
    subcommands = parser.add_subparsers(dest="command", required=True)

    image_parser = subcommands.add_parser("image", help="Build the firmware Docker image")
    image_parser.set_defaults(func=build_image)

    build_parser = subcommands.add_parser("build", help="Build firmware")
    build_parser.set_defaults(func=build)

    clean_parser = subcommands.add_parser("clean", help="Remove firmware build output")
    clean_parser.set_defaults(func=clean)

    flash_parser = subcommands.add_parser("flash", help="Flash firmware")
    add_flash_args(flash_parser)
    flash_parser.set_defaults(func=flash)

    monitor_parser = subcommands.add_parser("monitor", help="Run firmware serial monitor")
    add_serial_args(monitor_parser)
    monitor_parser.set_defaults(func=monitor)

    args = parser.parse_args()
    if args.image:
        os.environ["SENSORIO_IDF_IMAGE"] = args.image
    return args


def main() -> int:
    args = parse_args()
    try:
        return int(args.func(args) or 0)
    except RuntimeError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
