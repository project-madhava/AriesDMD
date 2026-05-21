# AriesDMD Library

**Author:** Nishil Patel  
**Platform:** RISC-V (Vega Aries V3 – C-DAC)  
**Project Type:** Academic & Experimental  
**Status:** Stable / Actively Maintained  
---

<img width="4160" height="1553" alt="WhatsApp Image 2026-05-19 at 2 49 28 PM" src="https://github.com/user-attachments/assets/1de92896-8314-4caf-8864-0078f18bb237" />

# 🚀 Overview

AriesDMD is a modular display driver library designed specifically for P10 and HUB75 LED matrix panels on RISC-V based Vega Aries V3 boards.
It supports:
- 🟢 Monochrome P10 panels (32×16, 1/4 scan)
- 🌈 RGB HUB75 panels (32×16, 1/8 scan and beyond)

Most existing LED matrix libraries were written for AVR-based Arduino boards like Arduino UNO. Unlike traditional Arduino libraries designed for AVR microcontrollers, AriesDMD is built specifically for the Vega Aries architecture to provide:
- Stable scanning
- Correct pixel mapping
- Flicker-free refresh
- Multi-panel support
- Clean framebuffer rendering

---

# 🎯 Key Design Goals
- Deterministic and timing-safe scanning
- RISC-V friendly architecture
- Clean pixel mapping independent of wiring
- Modular RGB / Mono architecture
- Large multi-panel scalability
- Future-ready for IoT text streaming

---

**🟢 Supported Hardware:** Microcontroller Vega Aries V3 (C-DAC)

# 🖥 Supported Displays

| **Description**                 | **Interface** | **Pixels** |   **Scan**  | **Code module**                              |
|---------------------------------|:-------------:|:----------:|:-----------:|----------------------------------------------|
| Monochrome P10 panels           |     HUB12     |    32x16   |     1/4     | AriesDMD.h, AriesDMD.cpp                     |
| Indoor RGB                      |     HUB75     |    32x16   |     1/8     | AriesRGB32x16.h, AriesRGB32x16.cpp           |
|                                 |               |    80x40   |     1/20    | AriesRGB.h, AriesRGB.cpp                     |
  
**Features:**
- Multi-panel cascading
- Text rendering
- 8 colour options
- Pixel drawing

---

 # Pixel Coordinate System

1) Single Panel:
```text
(0,0) -----------------> X
  |
  |
  |
  v
  Y
```

- Top left corner: (0,0)
- Bottom right corner: (W-1,H-1)

2) Multi Panel: Multiple panels are treated as one continuous display canvas.
- Example: AriesDMD dmd(4,1,...);
- Creates: 128 × 16 display 

---
# User Guide:

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
<table>
<tr>

<td valign="top">

<table>
<tr>
<th>HUB75 Pin</th>
<th>Aries Pin</th>
</tr>

<tr><td>R1</td><td>GPIO3</td></tr>
<tr><td>G1</td><td>GPIO4</td></tr>
<tr><td>B1</td><td>GPIO5</td></tr>
<tr><td>R2</td><td>GPIO6</td></tr>
<tr><td>G2</td><td>GPIO7</td></tr>
<tr><td>B2</td><td>GPIO8</td></tr>
<tr><td>CLK</td><td>GPIO9</td></tr>
<tr><td>LAT</td><td>GPIO10</td></tr>
<tr><td>OE</td><td>GPIO11</td></tr>
<tr><td>A</td><td>GPIO12</td></tr>
<tr><td>B</td><td>GPIO13</td></tr>
<tr><td>C</td><td>GPIO14</td></tr>

</table>

</td>

<td valign="top">
<img width="594" height="764" alt="image" src="https://github.com/user-attachments/assets/34f9d9e6-a49e-4df3-b4c9-80233f6dc7fa" />
</td>

</tr>
</table>
---

### 2) 80x40 Panel:
<table>
<tr>

<td valign="top">

<table>
<tr>
<th>HUB75 Pin</th>
<th>Aries Pin</th>
</tr>

<tr><td>R0</td><td>GPIO3</td></tr>
<tr><td>G0</td><td>GPIO4</td></tr>
<tr><td>B0</td><td>GPIO5</td></tr>
<tr><td>R1</td><td>GPIO6</td></tr>
<tr><td>G1</td><td>GPIO7</td></tr>
<tr><td>B1</td><td>GPIO8</td></tr>
<tr><td>CLK</td><td>GPIO14</td></tr>
<tr><td>LAT</td><td>GPIO15</td></tr>
<tr><td>OE</td><td>GPIO16</td></tr>
<tr><td>A</td><td>GPIO10</td></tr>
<tr><td>B</td><td>GPIO11</td></tr>
<tr><td>C</td><td>GPIO12</td></tr>
<tr><td>D</td><td>GPIO13</td></tr>
<tr><td>E</td><td>GPIO9</td></tr>

</table>

</td>

<td valign="top">

<img width="503" height="673" alt="image" src="https://github.com/user-attachments/assets/361cc5c4-57f6-44da-91cb-c2c3f3b2b76b" />

</td>

</tr>
</table>

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
<table>
<tr>

<td valign="top">

<table>
<tr>
<th>P10 Panel</th>
<th>Aries Pin</th>
</tr>

<tr><td>OE/EN</td><td>GPIO9</td></tr>
<tr><td>A</td><td>GPIO6</td></tr>
<tr><td>B</td><td>GPIO7</td></tr>
<tr><td>CLK</td><td>SCLK</td></tr>
<tr><td>Lat/SCLK</td><td>GPIO8</td></tr>
<tr><td>Data</td><td>MOSI</td></tr>

</table>

</td>

<td valign="top">

<img width="486" height="667" alt="image" src="https://github.com/user-attachments/assets/22131709-f7d9-4a65-bc8b-a7cb2a42befd" />

</td>

</tr>
</table>

---
