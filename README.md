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

For detailed instructions on Arduino IDE setup, library installation, and panel pinout connections, refer to the following document:

[View PDF](https://github.com/project-madhava/AriesDMD/blob/main/Tutorial%20for%20Aries-Panel.pdf)
