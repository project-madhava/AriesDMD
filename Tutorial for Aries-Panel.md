# Setting up VEGA Ecosystem on Arduino IDE

Open Arduino IDE

Open **File->Preferences**, Add below JSON in "Additional Boards Manager URL", Press OK

```
https://gitlab.com/riscv-vega/vega-arduino/-/raw/main/package_vega_index.json
```
<img width="900" height="601" alt="image" src="https://github.com/user-attachments/assets/9327e487-5ce4-485b-9644-0d5b7840e357" />

Open **Tools->Boards->Board manager**, Search for "vega"

Install "VEGA ARIES Boards" with latest version

Select **Tools->Board->VEGA Processor: ARIES Boards->ARIES v3**
<img width="858" height="568" alt="image" src="https://github.com/user-attachments/assets/aa2728a0-ffed-43e1-824b-e92c959bfcae" />

---

Check your ARIES board whether the **BOOT SEL** pin is shorted using a jumper or not.
<img width="795" height="564" alt="image" src="https://github.com/user-attachments/assets/76dc573f-229c-488d-8448-ece324b1ac42" />

**Important:**

> If  
> BOOT-SEL jumper (J12) is shorted :  
> a) Select **Flash Mode** -> **Enabled**  
> b) Select Tools -> **Programmer** -> **VEGA FLASHER**
>
> Else  
> a) Select **Flash Mode** -> **Disabled**  
> b) Select Tools -> **Programmer** -> **VEGA XMODEM**
>
> Here the BOOT SEL pin is shorted, so we have **enabled** the flash mode and selected **VEGA FLASHER** from programmer option.

Connect the ARIES board to PC

Select **Tools -> Port -> COM\* (ARIES Board)**

---

## Aries-RGB Panel Connections : (Used in given examples)

### 1) 32x16 Panel:

| HUB75 Pin | Aries Pin |
|-----------|-----------|
| R1        | GPIO3     |
| G1        | GPIO4     |
| B1        | GPIO5     |
| R2        | GPIO6     |
| G2        | GPIO7     |
| B2        | GPIO8     |
| CLK       | GPIO9     |
| LAT       | GPIO10    |
| OE        | GPIO11    |
| A         | GPIO12    |
| B         | GPIO13    |
| C         | GPIO14    |

<img width="594" height="764" alt="image" src="https://github.com/user-attachments/assets/34f9d9e6-a49e-4df3-b4c9-80233f6dc7fa" />

---

### 2) 80x40 Panel:

| HUB75 Pin | Aries Pin |
|-----------|-----------|
| R0        | GPIO3     |
| G0        | GPIO4     |
| B0        | GPIO5     |
| R1        | GPIO6     |
| G1        | GPIO7     |
| B1        | GPIO8     |
| CLK       | GPIO14    |
| LAT       | GPIO15    |
| OE        | GPIO16    |
| A         | GPIO10    |
| B         | GPIO11    |
| C         | GPIO12    |
| D         | GPIO13    |
| E         | GPIO9     |

<img width="503" height="673" alt="image" src="https://github.com/user-attachments/assets/361cc5c4-57f6-44da-91cb-c2c3f3b2b76b" />

---

## 🎨 RGB Color Table

RGB library supports 8 basic colors using ON/OFF RGB channels.

```
AriesDrawString(display,1,0,"HELLO",1,1,0);
                                    ↑
                          Change color from here
```

| R | G | B | Color   |
|---|---|---|---------|
| 0 | 0 | 0 | Black   |
| 1 | 0 | 0 | Red     |
| 0 | 1 | 0 | Green   |
| 0 | 0 | 1 | Blue    |
| 1 | 1 | 0 | Yellow  |
| 1 | 0 | 1 | Magenta |
| 0 | 1 | 1 | Cyan    |
| 1 | 1 | 1 | White   |

---

## Aries-Monochrome P10 Connections:

| P10 Panel | Aries Pin |
|-----------|-----------|
| OE/EN     | GPIO9     |
| A         | GPIO6     |
| B         | GPIO7     |
| CLK       | SCLK      |
| Lat/SCLK  | GPIO8     |
| Data      | MOSI      |

<img width="486" height="667" alt="image" src="https://github.com/user-attachments/assets/22131709-f7d9-4a65-bc8b-a7cb2a42befd" />

---
