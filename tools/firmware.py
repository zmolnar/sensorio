#!/usr/bin/env python3
"""Firmware workflow for Sensorio."""

from __future__ import annotations

import argparse
import hashlib
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
HOST_VENV_DIR = ROOT / ".venv"
HOST_REQUIREMENTS = ROOT / "tools" / "host-requirements.txt"
DEFAULT_IMAGE = "sensorio-firmware:idf-v4.4.8"
DEFAULT_PLATFORM = "linux/amd64"
DEFAULT_BAUD = 115200
DEFAULT_USB_VID = 0x0403
DEFAULT_USB_PID = 0x6015
DOCKERFILE_HASH_LABEL = "sensorio.firmware.dockerfile-sha256"


def env_value(name: str, default: str) -> str:
    return os.environ.get(name, default)


def usb_id(value: str) -> int:
    try:
        base = 16 if not value.lower().startswith("0x") else 0
        return int(value, base)
    except ValueError as exc:
        raise argparse.ArgumentTypeError(f"invalid USB id: {value}") from exc


def format_usb_id(value: int) -> str:
    return f"{value:04x}"


def docker_platform() -> str:
    return env_value("SENSORIO_DOCKER_PLATFORM", DEFAULT_PLATFORM)


def docker_image() -> str:
    return env_value("SENSORIO_IDF_IMAGE", DEFAULT_IMAGE)


def run(
    cmd: list[str],
    check: bool = True,
    cwd: Path | None = None,
    label: str | None = None,
    quiet: bool = False,
) -> int:
    print("+ " + (label or shlex.join(cmd)), flush=True)
    completed = subprocess.run(
        cmd,
        check=False,
        cwd=cwd,
        stdout=subprocess.PIPE if quiet else None,
        stderr=subprocess.STDOUT if quiet else None,
        text=quiet,
    )
    if quiet and completed.returncode != 0 and completed.stdout:
        print(completed.stdout, end="")
    if check and completed.returncode != 0:
        raise SystemExit(completed.returncode)
    return completed.returncode


def file_sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as file:
        for chunk in iter(lambda: file.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


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
    ensure_firmware_image()
    args = docker_base_args()
    args.append(docker_image())
    args.extend(command)
    return run(args, label=shlex.join(command))


def build_image() -> int:
    dockerfile_hash = file_sha256(DOCKERFILE)
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
            "--label",
            f"{DOCKERFILE_HASH_LABEL}={dockerfile_hash}",
            str(ROOT),
        ],
        label="prepare firmware build environment",
        quiet=True,
    )


def image_dockerfile_hash() -> str | None:
    completed = subprocess.run(
        ["docker", "image", "inspect", docker_image()],
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        text=True,
        check=False,
    )
    if completed.returncode != 0:
        return None

    try:
        image_info = json.loads(completed.stdout)
        labels = image_info[0]["Config"].get("Labels") or {}
        value = labels.get(DOCKERFILE_HASH_LABEL)
    except (KeyError, IndexError, TypeError, json.JSONDecodeError):
        return None

    return value if isinstance(value, str) else None


def ensure_firmware_image() -> None:
    expected_hash = file_sha256(DOCKERFILE)
    if image_dockerfile_hash() == expected_hash:
        return

    build_image()


def host_python_candidates() -> list[Path]:
    if os.name == "nt":
        return [HOST_VENV_DIR / "Scripts" / "python.exe"]

    return [
        HOST_VENV_DIR / "bin" / "python",
        HOST_VENV_DIR / "bin" / "python3",
    ]


def create_host_venv(recreate: bool) -> None:
    if recreate and HOST_VENV_DIR.exists():
        current = Path(sys.executable).resolve()
        venv = HOST_VENV_DIR.resolve()
        if current == venv or venv in current.parents:
            raise RuntimeError("cannot recreate the venv while running from it")
        print(f"removing {HOST_VENV_DIR}")
        shutil.rmtree(HOST_VENV_DIR)

    if any(candidate.exists() for candidate in host_python_candidates()):
        print(f"host venv already exists: {HOST_VENV_DIR}")
        return

    run([sys.executable, "-m", "venv", str(HOST_VENV_DIR)])


def install_host_requirements() -> None:
    run(
        [
            host_python(),
            "-m",
            "pip",
            "install",
            "--disable-pip-version-check",
            "--no-cache-dir",
            "-r",
            str(HOST_REQUIREMENTS),
        ]
    )


def setup(args: argparse.Namespace) -> int:
    create_host_venv(args.recreate_venv)
    install_host_requirements()
    ensure_firmware_image()

    return 0


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
    for candidate in host_python_candidates():
        if candidate.exists():
            return str(candidate)

    raise RuntimeError(
        f"host venv is required for serial tools.\n{host_setup_hint()}"
    )


def host_setup_hint() -> str:
    if os.name == "nt":
        return (
            "Run the one-time setup:\n"
            "  py -3 tools\\firmware.py setup"
        )
    return (
        "Run the one-time setup:\n"
        "  python3 tools/firmware.py setup"
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


def host_serial_ports() -> list[dict[str, object]]:
    ensure_host_module("serial", "pyserial")

    completed = subprocess.run(
        [
            host_python(),
            "-c",
            (
                "import json; "
                "from serial.tools import list_ports; "
                "print(json.dumps([{"
                "'device': p.device, "
                "'description': p.description, "
                "'hwid': p.hwid, "
                "'vid': p.vid, "
                "'pid': p.pid, "
                "'serial_number': p.serial_number, "
                "'location': p.location"
                "} for p in list_ports.comports()]))"
            ),
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        check=False,
    )
    if completed.returncode != 0:
        raise RuntimeError(completed.stderr.strip() or "failed to list serial ports")

    return json.loads(completed.stdout)


def serial_port_description(port: dict[str, object]) -> str:
    vid = port.get("vid")
    pid = port.get("pid")
    usb_id = (
        f" VID:PID={format_usb_id(vid)}:{format_usb_id(pid)}"
        if isinstance(vid, int) and isinstance(pid, int)
        else ""
    )
    description = port.get("description") or "n/a"
    hwid = port.get("hwid") or "n/a"
    return f"  {port.get('device')} - {description}{usb_id} ({hwid})"


def auto_detect_port(vid: int, pid: int) -> str:
    ports = host_serial_ports()
    matches = [
        port
        for port in ports
        if port.get("vid") == vid and port.get("pid") == pid
    ]

    cu_matches = [
        port
        for port in matches
        if isinstance(port.get("device"), str)
        and str(port.get("device")).startswith("/dev/cu.")
    ]
    if len(cu_matches) == 1:
        matches = cu_matches

    usb_name = f"{format_usb_id(vid)}:{format_usb_id(pid)}"
    if not matches:
        available = "\n".join(serial_port_description(port) for port in ports)
        raise RuntimeError(
            f"no serial port found for USB VID:PID={usb_name}.\n"
            f"Available serial ports:\n{available or '  none'}"
        )
    if len(matches) > 1:
        available = "\n".join(serial_port_description(port) for port in matches)
        raise RuntimeError(
            f"multiple serial ports match USB VID:PID={usb_name}; "
            f"use --port to choose one.\n{available}"
        )

    port = matches[0]
    device = port.get("device")
    if not isinstance(device, str):
        raise RuntimeError(f"invalid serial port entry: {port!r}")

    print(
        "auto-detected serial port "
        f"{device} for USB VID:PID={usb_name}",
        flush=True,
    )
    return device


def serial_port(args: argparse.Namespace) -> str:
    if args.port:
        return str(args.port)
    return auto_detect_port(args.usb_vid, args.usb_pid)


def flash(args: argparse.Namespace) -> int:
    ensure_host_module("esptool", "esptool")
    port = serial_port(args)
    build(args)
    return host_esptool_flash(args, port)


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


def host_esptool_flash(args: argparse.Namespace, port: str) -> int:
    command = esptool_flash_command(args, port, [host_python(), "-m", "esptool"])
    return run(command, cwd=FIRMWARE_DIR)


def monitor(args: argparse.Namespace) -> int:
    ensure_host_module("serial", "pyserial")
    port = serial_port(args)
    return run(
        [
            host_python(),
            "-m",
            "serial.tools.miniterm",
            "--raw",
            port,
            str(args.baud),
        ]
    )


def add_serial_args(parser: argparse.ArgumentParser) -> None:
    parser.add_argument(
        "--port",
        default=None,
        help=(
            "Serial device override, for example /dev/cu.usbserial-0001. "
            "If omitted, the Sensorio USB serial port is auto-detected."
        ),
    )
    parser.add_argument(
        "--usb-vid",
        type=usb_id,
        default=DEFAULT_USB_VID,
        help=(
            "USB VID used for serial port auto-detection, "
            f"default {format_usb_id(DEFAULT_USB_VID)}"
        ),
    )
    parser.add_argument(
        "--usb-pid",
        type=usb_id,
        default=DEFAULT_USB_PID,
        help=(
            "USB PID used for serial port auto-detection, "
            f"default {format_usb_id(DEFAULT_USB_PID)}"
        ),
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
    subcommands = parser.add_subparsers(dest="command", required=True)

    setup_parser = subcommands.add_parser(
        "setup", help="Prepare the local firmware workflow"
    )
    setup_parser.add_argument(
        "--recreate-venv",
        action="store_true",
        help="Remove and recreate the host venv before installing tools",
    )
    setup_parser.set_defaults(func=setup)

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

    return parser.parse_args()


def main() -> int:
    args = parse_args()
    try:
        return int(args.func(args) or 0)
    except RuntimeError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
