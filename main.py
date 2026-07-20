import sys
import time
import numpy as np
import pyaudio
import serial

# ==========================================
# CONFIGURATION CONSTANTS
# ==========================================
# Set your active ESP32 COM Port here (Check Device Manager / Arduino IDE)
COM_PORT = "COM3"
BAUD_RATE = 115200

# Audio Capture Pipeline Parameters
CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100

# Establish high-speed hardware serial communication link
try:
    esp32 = serial.Serial(COM_PORT, BAUD_RATE, timeout=0.1)
    print(f"[SUCCESS] Connected to ESP32 on port {COM_PORT}")
    time.sleep(2)  # Allow hardware module connection initialization buffer
except Exception as error:
    print(
        f"[ERROR] Could not connect to COM Port {COM_PORT}. Check connection configurations.\nDetails: {error}"
    )
    sys.exit()

# Initialize audio parsing device layer stream capture loop
p = pyaudio.PyAudio()
stream = p.open(
    format=FORMAT,
    channels=CHANNELS,
    rate=RATE,
    input=True,
    frames_per_buffer=CHUNK,
)

print(
    "\n🎵 [READY] Music Capture Pipeline Online! Streaming data... (Press Ctrl+C to terminate)"
)

try:
    while True:
        # Capture raw byte stream chunks from default sound input hardware
        raw_data = stream.read(CHUNK, exception_on_overflow=False)

        # Convert the audio buffer chunks directly to signed numerical arrays
        audio_array = np.frombuffer(raw_data, dtype=np.int16)

        # Skip execution routines if completely silent audio stream detected
        if np.max(audio_array) == 0:
            continue

        # Execute Fast Fourier Transform (FFT) to convert signals to Frequency domain
        fft_data = np.abs(np.fft.rfft(audio_array))
        fft_freq = np.fft.rfftfreq(CHUNK, d=1.0 / RATE)

        # Separate frequency spectrum bands into individual sound targets
        bass_band = np.mean(fft_data[(fft_freq >= 20) & (fft_freq <= 250)])
        mids_band = np.mean(fft_data[(fft_freq > 250) & (fft_freq <= 4000)])
        treb_band = np.mean(fft_data[(fft_freq > 4000) & (fft_freq <= 20000)])

        # Scale and clip calculated values to hardware limits
        bass_score = int(np.clip(bass_band / 50000, 0, 4))
        mid_score = int(np.clip(mids_band / 30000, 0, 3))
        treb_score = int(np.clip(treb_band / 15000, 0, 2))

        # Format custom high-efficiency packet footprint: "Bass,Mids,Treble\n"
        data_packet = f"{bass_score},{mid_score},{treb_score}\n"

        # Push data payload directly to ESP32 board over USB link interface
        esp32.write(data_packet.encode())

        # Brief dynamic delay buffer window framework (~30 FPS pipeline optimization)
        time.sleep(0.03)

except KeyboardInterrupt:
    print("\n[INFO] Terminating visualizer execution pipeline loop...")
finally:
    # Safely close down active hardware and system capture resources
    stream.stop_stream()
    stream.close()
    p.terminate()
    esp32.close()
    print("[INFO] System resource cleanup completed safely.")