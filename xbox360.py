#!/usr/bin/env python3
"""
xbox360.py — Xbox 360 controller bridge for Tricycle Tank.

Reads the controller with evdev and writes the current state to
`xbox360.txt` in the game's working directory, using the protocol
the C program expects:

    <teclaXbox> <teclaTorreta> <posX> <posY>

  teclaXbox    : T=fire, X=reload, A=play sound, B=stop sound,
                 Y=end game, E=start game, l/r/u/d=song select, N=none
  teclaTorreta : U/D/L/R (right stick), N=none
  posX, posY   : left joystick, normalized to [-1.0, 1.0]

Usage:
    sudo python3 xbox360.py                 # auto-detect the controller
    sudo python3 xbox360.py /dev/input/event3   # specific device
    sudo python3 xbox360.py --out /path/xbox360.txt

Requirements: pip3 install evdev
"""

import argparse
import os
import sys
import time

try:
    import evdev
except ImportError:
    print("Missing dependency: run  pip3 install evdev", file=sys.stderr)
    sys.exit(1)

# ---------------------------------------------------------------------------
# evdev codes for the Xbox 360 controller
# ---------------------------------------------------------------------------
BTN_A, BTN_B, BTN_X, BTN_Y = 0x130, 0x131, 0x133, 0x134
BTN_START, BTN_SELECT = 0x13B, 0x13A

ABS_LX, ABS_LY = 0x00, 0x01   # left stick
ABS_RX, ABS_RY = 0x03, 0x04   # right stick
ABS_RT = 0x05                 # right trigger
ABS_HAT_X, ABS_HAT_Y = 0x10, 0x11  # D-pad

DEADZONE = 0.25   # joystick deadzone (normalized)
STICK_MAX = 32767.0

SONG_DPAD = {(-1, 0): "l", (1, 0): "r", (0, -1): "u", (0, 1): "d"}  # D-pad -> song


def normalize(value):
    """Map an ABS axis value (-32768..32767) to [-1.0, 1.0]."""
    return max(-1.0, min(1.0, value / STICK_MAX))


def turret_dir(rx, ry):
    """Map the right stick to a turret direction character (deadzone applied)."""
    if abs(rx) < DEADZONE and abs(ry) < DEADZONE:
        return "N"
    if abs(rx) > abs(ry):
        return "R" if rx > 0 else "L"
    return "D" if ry > 0 else "U"


def find_controller():
    """Return the first Xbox 360 controller event device, or None."""
    for path in evdev.list_devices():
        dev = evdev.InputDevice(path)
        name = dev.name or ""
        if "xbox" in name.lower() or "360" in name.lower():
            dev.close()
            return path
        dev.close()
    return None


def main():
    parser = argparse.ArgumentParser(description="Xbox 360 controller bridge for Tricycle Tank")
    parser.add_argument("device", nargs="?", help="event device path (auto-detected if omitted)")
    parser.add_argument("--out", default="xbox360.txt", help="output file (default: xbox360.txt)")
    args = parser.parse_args()

    device_path = args.device or find_controller()
    if not device_path:
        print("No Xbox 360 controller found. Plug it in and retry.", file=sys.stderr)
        sys.exit(1)

    dev = evdev.InputDevice(device_path)
    print(f"Controller: {dev.name} ({dev.path})")
    print(f"Writing state to {args.out} — press Ctrl+C to stop.")

    buttons = {BTN_A: "A", BTN_B: "B", BTN_X: "X", BTN_Y: "Y",
               BTN_START: "E",  # START begins the game
               }
    axes = {ABS_LX: 0.0, ABS_LY: 0.0, ABS_RX: 0.0, ABS_RY: 0.0}
    hat = (0, 0)
    key = "N"
    turret = "N"
    rt_pressed = False

    try:
        dev.grab()
    except OSError:
        pass  # already grabbed / permission issue — best effort

    try:
        while True:
            events = dev.read()
            for e in events:
                if e.type == evdev.ecodes.EV_KEY:
                    if e.value == 1:  # pressed
                        if e.code in buttons:
                            key = buttons[e.code]
                        elif e.code == ABS_RT:
                            rt_pressed = True
                elif e.type == evdev.ecodes.EV_ABS:
                    if e.code in axes:
                        axes[e.code] = normalize(e.value)
                    elif e.code in (ABS_HAT_X, ABS_HAT_Y):
                        idx = 0 if e.code == ABS_HAT_X else 1
                        hat = hat[:idx] + (e.value,) + hat[idx + 1 :]

            if rt_pressed:
                key = "T"
                rt_pressed = False

            # D-pad song selection takes precedence when no button is held
            if key == "N":
                key = SONG_DPAD.get(tuple(hat), "N")

            posX = axes[ABS_LX]
            posY = axes[ABS_LY]
            turret = turret_dir(axes[ABS_RX], axes[ABS_RY])

            line = f"{key} {turret} {posX:.2f} {posY:.2f}\n"
            with open(args.out, "w") as f:
                f.write(line)

            key = "N"  # one-shot buttons are consumed each cycle
            time.sleep(0.02)
    except KeyboardInterrupt:
        print("\nBridge stopped.")
    finally:
        try:
            dev.ungrab()
        except OSError:
            pass
        dev.close()


if __name__ == "__main__":
    main()
