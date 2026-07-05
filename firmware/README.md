# Firmware Workflow

The firmware builds with ESP-IDF in Docker. Flashing and monitoring run on the
host through the repo venv.

One-time setup:

```sh
python3 tools/firmware.py setup
```

Build:

```sh
python3 tools/firmware.py build
```

Flash:

```sh
python3 tools/firmware.py flash
```

On Sensorio hardware, keep the power button pressed while flashing.

Monitor:

```sh
python3 tools/firmware.py monitor
```

Notes:

- `setup` creates `.venv`, installs the host serial tools, and prepares the
  firmware build environment.
- Docker is required for firmware builds, but the script manages the build
  environment automatically.
- `flash` uses host `esptool` from `.venv` on every platform, so USB serial
  control pins are handled by the host.
- `monitor` uses host `pyserial` from `.venv`.
- `flash` and `monitor` auto-detect the Sensorio USB serial port by USB
  VID:PID `0403:6015`. Pass `--port` to override it.
