# 🎵 Real-Time Audio Spectrum Visualizer (ESP32 + Python + OLED)

A real-time, hardware-based Audio Spectrum Visualizer that dynamically reacts to desktop audio or microphone input. Built using **Python (PyAudio + NumPy)** for audio processing/FFT and an **ESP32 Microcontroller** to drive an **I2C OLED Display** and a multi-frequency LED array over serial communication.

## ✨ Features
* **Real-Time FFT Processing:** Uses NumPy's Fast Fourier Transform (`rfft`) to analyze incoming audio streams in real-time.
* **3-Band Frequency Separation:** Automatically categorizes audio into **Bass** (20-250 Hz), **Mids** (250-4000 Hz), and **Treble** (4000-20000 Hz).
* **OLED Bar Graph:** Displays dynamic vertical visualizer bars on a 1.3-inch SH1106 OLED Display using the `U8g2` library.
* **10-Channel LED VU Meter:** 
  * 🟢 **4 Green LEDs:** Low frequencies (Bass)
  * 🟡 **3 Yellow LEDs:** Mid frequencies (Vocals/Beats)
  * 🔴 **2 Red LEDs:** High frequencies (Treble)
  * 🔵 **1 Blue LED:** Peak detector (Flashes on heavy bass drops)
* **Low Latency Communication:** Transmits processed data packets from laptop to ESP32 over USB Serial at `115200` baud rate (~30 FPS update rate).

## 🛠️ Hardware Requirements
* **Microcontroller:** ESP32 Development Board
* **Display:** 1.3-inch OLED Display (I2C / SH1106 Driver)
* **LEDs:** 4x Green, 3x Yellow, 2x Red, 1x Blue (Total 10)
* **Resistors:** 10x 220 Ω Resistors (Crucial for current limiting)
* **Other:** Breadboard, Jumper Wires, Micro-USB Data Cable

## 🔌 Circuit & Pin Mapping (ESP32)

| Component | ESP32 Pin | Notes |
| :--- | :--- | :--- |
| **OLED VCC** | `3V3` | 3.3V Power Rail |
| **OLED GND** | `GND` | Ground Rail |
| **OLED SDA** | `GPIO 21` | I2C Data |
| **OLED SCL** | `GPIO 22` | I2C Clock |
| **Bass LEDs (Green x4)** | `GPIO 13, 12, 14, 27`| Connect via 220Ω resistor to GND |
| **Mid LEDs (Yellow x3)**| `GPIO 26, 25, 33` | Connect via 220Ω resistor to GND |
| **Treble LEDs (Red x2)**| `GPIO 32, 19` | Connect via 220Ω resistor to GND |
| **Peak LED (Blue x1)** | `GPIO 18` | Connect via 220Ω resistor to GND |

## 💻 Software Setup

### 1. Microcontroller Setup (ESP32)
1. Open Arduino IDE and install the **U8g2** library by *oliver* from the Library Manager.
2. Select your ESP32 board and the correct COM Port.
3. Upload `sketch_jul19a.ino` to your ESP32.

### 2. Python Environment Setup (PC/Laptop)
1. Ensure Python 3 is installed. Open Terminal/PowerShell and install required dependencies:
   ```bash
   pip install pyaudio numpy pyserial
Open main.py and update the COM_PORT variable to match your ESP32 port (e.g., "COM3" on Windows or "/dev/ttyUSB0" on Linux/Mac).

Run the script:

Bash
python main.py
Play any music on your PC and watch the hardware come alive! 🚀

👨‍💻 Author
[Awaish Lakhani]

Feel free to star ⭐ this repository if you found it helpful!
