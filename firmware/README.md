# Firmware Workflow

The firmware builds with ESP-IDF in Docker. Flashing and monitoring run on the
host through the repo venv.

One-time setup:

```sh
python3 -m venv .venv
.venv/bin/python -m pip install -r tools/host-requirements.txt
.venv/bin/python tools/firmware.py image
```

Build:

```sh
.venv/bin/python tools/firmware.py build
```

Flash:

```sh
.venv/bin/python tools/firmware.py flash
```

On Sensorio hardware, keep the power button pressed while flashing.

Monitor:

```sh
.venv/bin/python tools/firmware.py monitor
```

Notes:

- Docker is used only for `image` and `build`.
- `flash` uses host `esptool` from `.venv` on every platform, so USB serial
  control pins are handled by the host.
- `monitor` uses host `pyserial` from `.venv`.
- `flash` and `monitor` auto-detect the Sensorio USB serial port by USB
  VID:PID `0403:6015`. Pass `--port` to override it.
- `SENSORIO_IDF_IMAGE` and `SENSORIO_DOCKER_PLATFORM` can override the default
  Docker image and platform.
