# Tricycle Tank 🚀🔫

[![C](https://img.shields.io/badge/C-gnu99-3776AB?style=flat-square)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Hardware](https://img.shields.io/badge/Raspberry%20Pi-wiringPi-C51A4A?style=flat-square)](https://github.com/WiringPi/WiringPi)
[![License](https://img.shields.io/badge/license-MIT-yellow.svg?style=flat-square)](LICENSE)
[![Build](https://img.shields.io/badge/build-make%20%7C%20make%20test-10B981?style=flat-square)](Makefile)

A C-based robotic combat game for the **Raspberry Pi**, built as a university
project (SDG2). Players control a **laser-equipped tricycle tank** with an
**Xbox 360 controller**: drive with the left stick, aim the turret with the
right stick, and fire an IR "laser" at opponents. Each tank has limited lives —
the last one standing wins.

![Architecture: FSM-driven](docs/fsm_arch.png)

## ✨ Features

- 🎮 **Xbox 360 controller** input (Python bridge script → C program)
- 🛞 **Hardware PWM wheel control** (tricycle steering)
- 🎯 **Dual-servo turret** (software PWM) with IR laser shooting
- 🔊 **SoftTone sound effects** — Despacito, Game of Thrones, Tetris, Star Wars
- ⚙️ **4 parallel finite-state machines** (FSM) running on a shared 10 ms clock
- 💥 **IR hit detection** with debounce and shoot-timeout logic

## 📋 Hardware Requirements

| Component | Detail |
|---|---|
| Raspberry Pi | Any model with 40-pin GPIO (wiringPi compatible) |
| Xbox 360 controller | Wired or wireless receiver |
| Motor driver | Dual H-bridge (e.g. L298N) for the two drive wheels |
| 2× servos | Pan/tilt turret (SG90-class) |
| IR emitter + receiver | Laser firing & hit detection (pin 9 out, pin 11 in) |
| Buzzer / speaker | Sound effects via softTone (pin 23) |
| Laser diode | The "weapon" (activated on pin 9) |

## 🔌 GPIO Pin Configuration

| Pin | Function |
|-----|----------|
| 18, 19 | Hardware PWM — wheel control |
| 17, 27 | Software PWM — turret servos (horizontal / vertical) |
| 23 | SoftTone — sound effects playback |
| 9 | High — laser shooting activation |
| 11 | Rising-edge interrupt — hit detection |

## 🛠️ Build & Run

### 1. Install dependencies (Raspberry Pi OS)

```bash
sudo apt update
sudo apt install -y git build-essential python3 python3-pip wiringpi \
    libwiringpi-dev
pip3 install evdev
```

> WiringPi is preinstalled on Raspberry Pi OS. On other distros, install it
> from https://github.com/WiringPi/WiringPi.

### 2. Build

```bash
git clone https://github.com/alejp1998/tricycle-tank.git
cd tricycle-tank
make            # Debug/piTankGo_1
```

Other targets:

| Target | Purpose |
|---|---|
| `make debug` | Debug build (`-g -O0`, `DPRINTF` enabled) |
| `make run` | Build and launch with `sudo` |
| `make check` | Syntax-check every source on **any** host (no Pi needed — uses `tests/stubs/` wiringPi headers) |
| `make test` | Run the hardware-free **FSM unit tests** |
| `make clean` | Remove `Debug/` |

### 3. Start the Xbox controller bridge (background)

The controller bridge lives in its own repository —
**[alejp1998/xbox360controller](https://github.com/alejp1998/xbox360controller)**:

```bash
git clone https://github.com/alejp1998/xbox360controller.git
cd xbox360controller
pip3 install evdev
sudo python3 xbox360.py --out ../tricycle-tank/xbox360.txt
```

The Python bridge reads the controller with `evdev` and writes the current
button/joystick state to `xbox360.txt` (the file the C program polls every
10 ms through its xbox360 FSM). A copy of the bridge is also included here at
[`xbox360.py`](xbox360.py) for convenience.

### 4. Start the game

```bash
cd piTankEx
sudo ./Debug/piTankGo_1   # or just: sudo make run
```

### 5. End the game

- Press **Y** on the controller, or
- Land **10 hits** on your opponent 🏆

## 🎮 Xbox 360 Controller Mapping

| Input | Action |
|---|---|
| Left Joystick | Move wheels |
| Right Joystick | Move turret servos |
| D-Pad | Select sound effect (Despacito / GOT / Tetris / Star Wars) |
| Right Trigger | Fire laser |
| A | Play selected sound effect |
| B | Stop sound effect |
| X | Reload (max 10 bullets) |
| Y | End game |
| START | Begin game |

## 🧠 Architecture

Four finite-state machines (implemented by the tiny reusable engine in
`fsm.c`) run in a shared main loop clocked every `CLK_MS = 10 ms`:

```
                ┌──────────────────────────┐
                │      main loop (10ms)     │
                └───────┬─────┬─────┬───────┘
        ┌───────────────┘     │     └────────────────┐
        ▼                     ▼                      ▼
 ┌─────────────┐      ┌──────────────┐      ┌──────────────┐
 │ xbox360 FSM │      │  player FSM  │      │  ruedas FSM  │
 │ (input/keys)│      │ (sound fx)   │      │  (wheels)    │
 └─────────────┘      └──────────────┘      └──────────────┘
        │                     ▲                      ▲
        ▼                     │                      │
 ┌─────────────┐              │                      │
 │ torreta FSM │──────────────┘                      │
 │ (turret+laser)                                     │
 └─────────────┘                                      │
        │                                             │
        ▼                                             │
 ┌─────────────┐  GPIO interrupts (hit detection)     │
 │ wiringPi ISR│───────────────────────────────────────┘
 └─────────────┘
```

Module map:

| File | Responsibility |
|---|---|
| `piTankGo_1.c` | System setup, FSM wiring, main loop, sound-effect tables |
| `fsm.c` / `fsm.h` | Reusable finite-state-machine engine |
| `tmr.c` / `tmr.h` | POSIX timer helpers |
| `xbox360.c` | Controller input, debounce, joystick parsing |
| `ruedas.c` | Wheel PWM + tricycle steering |
| `torreta.c` | Turret servos, IR shooting, hit detection ISR |
| `player.c` | SoftTone melody player |
| `piTankGoLib.h` | Shared flags, mutex keys, clock constant |

## 🧪 Testing

The FSM engine and timer module are pure C — they run on any machine:

```bash
make test     # FSM unit tests (transition firing, reset, destroy)
make check    # syntax-check all sources with wiringPi stubs
```

## 🌿 Branches

- `master` — canonical, "final version" of the project
- `wheelscontrol` — experimental wheel-control refactor (unmerged)
- `xbox360` — experimental controller rework (unmerged)

## 📜 License

MIT — see [LICENSE](LICENSE). Built by **Alicia & Alejandro** for the SDG2
course project.
