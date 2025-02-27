# Tricycle Tank 🚀🔫

Tricycle Tank is a C-based robotic combat game developed for the university course SDG2. Players control a laser-equipped tricycle tank using an Xbox controller, maneuvering to shoot opponents while avoiding hits. Each tank has limited lives, and the last one standing wins.

## Project Overview
This project controls a turret and two wheels while also handling sound effects using the Raspberry Pi GPIO pins.

## GPIO Pin Configuration

| Pin  | Function                      |
|------|--------------------------------|
| 18, 19  | Hardware PWM – Wheel control |
| 17, 27  | Software PWM – Servo control |
| 23   | SoftTone – Sound effects playback |
| 9    | High – Shooting activation |
| 11   | Rising Edge Detection – Hit detection |

## Xbox 360 Controller Mapping 🎮

- Left Joystick → Move wheels  
- Right Joystick → Move turret servos  
- D-Pad → Select sound effect  
- Right Trigger → Fire laser  
- A Button → Play selected sound effect  
- B Button → Stop sound effect  
- X Button → Reload (max 10 bullets)  
- Y Button → End game  
- Start Button → Begin game  

## Instructions 🛠️

### 1️⃣ Start Xbox Controller Input in the Background
```bash
cd xbox360controller  
sudo python xbox360.py
```

### 2️⃣ Start the Game
```bash
cd piTankEx  
sudo ./piTankGo_1
```

### 3️⃣ End the Game
- Press **START** on the Xbox 360 controller  
- Or, get hit **10 times**  
