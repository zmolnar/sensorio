#!/usr/bin/env python3
"""GUI simulator workflow for Sensorio."""

from __future__ import annotations

import argparse
import os
import platform
import shlex
import shutil
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SIMULATOR_DIR = ROOT / "firmware" / "src" / "gui" / "simulator"
BUILD_DIR = ROOT / "firmware" / "build-simulator"
EXECUTABLE = BUILD_DIR / "sensorio-gui-simulator"


def run(
    cmd: list[str],
    cwd: Path | None = None,
    env: dict[str, str] | None = None,
) -> int:
    print("+ " + shlex.join(cmd), flush=True)
    completed = subprocess.run(cmd, cwd=cwd, env=env, check=False)
    if completed.returncode != 0:
        raise SystemExit(completed.returncode)
    return completed.returncode


def setup(_: argparse.Namespace) -> int:
    missing = [
        tool
        for tool in ("cmake", "ninja", "sdl2-config")
        if shutil.which(tool) is None
    ]
    if missing:
        hint = "brew install cmake ninja sdl2" if platform.system() == "Darwin" else ""
        message = f"missing required tool(s): {', '.join(missing)}"
        if hint:
            message += f"\nInstall them with:\n  {hint}"
        raise RuntimeError(message)

    print("simulator host tools are available")
    return 0


def configure(_: argparse.Namespace) -> int:
    setup(argparse.Namespace())
    return run(
        [
            "cmake",
            "-S",
            str(SIMULATOR_DIR),
            "-B",
            str(BUILD_DIR),
            "-G",
            "Ninja",
        ]
    )


def build(args: argparse.Namespace) -> int:
    configure(args)
    return run(["cmake", "--build", str(BUILD_DIR)])


def clean(_: argparse.Namespace) -> int:
    if BUILD_DIR.exists():
        print(f"removing {BUILD_DIR}")
        shutil.rmtree(BUILD_DIR)
    else:
        print(f"{BUILD_DIR} does not exist")
    return 0


def run_simulator(args: argparse.Namespace) -> int:
    if not args.no_build:
        build(args)
    elif not EXECUTABLE.exists():
        raise RuntimeError(f"simulator executable not found: {EXECUTABLE}")

    env = os.environ.copy()
    env.setdefault("SDL_VIDEO_CENTERED", "1")
    return run([str(EXECUTABLE)], cwd=ROOT, env=env)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    subcommands = parser.add_subparsers(dest="command", required=True)

    setup_parser = subcommands.add_parser("setup", help="Check simulator host tools")
    setup_parser.set_defaults(func=setup)

    build_parser = subcommands.add_parser("build", help="Build the GUI simulator")
    build_parser.set_defaults(func=build)

    run_parser = subcommands.add_parser("run", help="Run the GUI simulator")
    run_parser.add_argument(
        "--no-build",
        action="store_true",
        help="Run the existing simulator executable without rebuilding",
    )
    run_parser.set_defaults(func=run_simulator)

    clean_parser = subcommands.add_parser("clean", help="Remove simulator build output")
    clean_parser.set_defaults(func=clean)

    return parser.parse_args()


def main() -> int:
    args = parse_args()
    try:
        return int(args.func(args) or 0)
    except KeyboardInterrupt:
        print()
        return 130
    except RuntimeError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
