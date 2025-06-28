# 🚦 Arduino Traffic‑Light & Countdown Display  
A mini traffic‑light simulator for Arduino that drives three LEDs (red, yellow, green) **and** a 4‑digit TM1637 seven‑segment display.  
It can run automatically or be interrupted with a push‑button to trigger a quick “Ready… GO!” sequence.

---

## Table of Contents
1. [Features](#features)  
2. [Bill of Materials](#bill-of-materials)  
3. [Wiring Guide](#wiring-guide)  
4. [Installation](#installation)  
5. [How It Works](#how-it-works)  
6. [Customization Tips](#customization-tips)  
7. [License](#license)  

---

## Features
| Mode | What Happens | Display | LEDs |
|------|--------------|---------|------|
| **Automatic** | Cycles **STOP → YELLOW → GO** repeatedly | “STOP” / countdown / “GO” | Red → Yellow → Green |
| **Button Override** | Press the button at any time to start an 8‑second countdown and fast “GO” cycle | Countdown 8 → 0, then “GO” | Yellow (countdown) → Green |
| **Visual Feedback** | Each phase does a short LED blink pattern before switching | N/A | All LEDs blink in their phase color |

---

## Bill of Materials
| Qty | Item | Notes |
|-----|------|-------|
| 1 | **Arduino** (UNO/Nano/Pro Mini…) | Any 5 V board with ≥ 14 I/O |
| 1 | **TM1637 4‑digit display** | Common inexpensive module |
| 3 | **5 mm LEDs** (Red, Yellow, Green) | Use 220 Ω – 330 Ω series resistors |
| 1 | **Momentary push‑button** + 10 kΩ pull‑up/down | Active‑LOW in sketch |
| Jumper wires, breadboard, USB cable |

> **Library**  
> [`TM1637Display`](https://github.com/avishorp/TM1637) by @avishorp

---

## Wiring Guide
| Arduino Pin | Connected To | Purpose |
|-------------|--------------|---------|
| **D2** | TM1637 **CLK** | Display clock |
| **D3** | TM1637 **DIO** | Display data |
| **D4** | Push‑button (active‑LOW) | Manual override |
| **D11** | Green LED (+ resistor → GND) | GO |
| **D12** | Yellow LED (+ resistor → GND) | Countdown / caution |
| **D13** | Red LED (+ resistor → GND) | STOP |
| **5 V / GND** | Display VCC/GND, LED cathodes, button reference | Power |

```ascii
           +5 V           GND
            │              │
   ┌──TM1637─┴───────┐     │
   │ VCC  CLK  DIO GND │   │
   │  |    |    |   |  │   │
   │  |    |    |   |  │   │
   └──┘    |    |   └──┴───┘
           |    |
           |    └───► D3
           └────────► D2

Push‑button: D4 ───►◉◄─── GND
(resistor optional if INPUT_PULLUP is enabled)

LEDs:  D13─►[220 Ω]─►▶Red  
       D12─►[220 Ω]─►▶Yellow  
       D11─►[220 Ω]─►▶Green
