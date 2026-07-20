#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// Initialize 1.3 inch OLED (SH1106 Driver) using Hardware I2C
// SCL Pin -> GPIO 22, SDA Pin -> GPIO 21
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 21);

// Define LED Pin Array
const int ledPins[] = {13, 12, 14, 27, 26, 25, 33, 32, 19, 18};
const int numLeds = 10;

void setup() {
  // Initialize Serial Communication at 115200 Baud Rate
  Serial.begin(115200);
  
  // Set all LED pins as OUTPUT and initialize to LOW
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Initialize and clear OLED Display
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(15, 35, "Visualizer Ready!");
  u8g2.sendBuffer();
  delay(1500);
}

void loop() {
  // Check if incoming serial data is available from Python script
  if (Serial.available() > 0) {
    // Read the incoming data packet until newline character
    String data = Serial.readStringUntil('\n');
    data.trim();

    // Parse the data comma separated format (Example: "4,3,2" -> Bass,Mids,Treble)
    int firstComma = data.indexOf(',');
    int secondComma = data.indexOf(',', firstComma + 1);

    if (firstComma != -1 && secondComma != -1) {
      int bassVal = data.substring(0, firstComma).toInt();
      int midVal = data.substring(firstComma + 1, secondComma).toInt();
      int trebVal = data.substring(secondComma + 1).toInt();

      // ==========================================
      // 1. LED VU METER CONTROL ACTION
      // ==========================================
      
      // Control Bass LEDs (Pins: 13, 12, 14, 27) -> Range 0 to 4
      for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], (i < bassVal) ? HIGH : LOW);
      }
      
      // Control Mid LEDs (Pins: 26, 25, 33) -> Range 0 to 3
      for (int i = 0; i < 3; i++) {
        digitalWrite(ledPins[4 + i], (i < midVal) ? HIGH : LOW);
      }
      
      // Control Treble LEDs (Pins: 32, 19) -> Range 0 to 2
      for (int i = 0; i < 2; i++) {
        digitalWrite(ledPins[7 + i], (i < trebVal) ? HIGH : LOW);
      }
      
      // Control Blue Peak LED (Pin: 18) -> Flash on max bass thresholds
      digitalWrite(ledPins[9], (bassVal >= 4) ? HIGH : LOW);

      // ==========================================
      // 2. OLED GRAPH RENDERING
      // ==========================================
      u8g2.clearBuffer();
      
      // Draw Graphic Frequency Bars (X, Y, Width, Height)
      u8g2.drawBox(15, 60 - (bassVal * 12), 20, (bassVal * 12)); // Bass Visual Bar
      u8g2.drawBox(55, 60 - (midVal * 16), 20, (midVal * 16));   // Mids Visual Bar
      u8g2.drawBox(95, 60 - (trebVal * 24), 20, (trebVal * 24)); // Treble Visual Bar

      // Render Text Content Labels
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.drawStr(12, 10, "BASS");
      u8g2.drawStr(53, 10, "MIDS");
      u8g2.drawStr(93, 10, "TREB");
      
      u8g2.sendBuffer();
    }
  }
}