#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// Initialize the OLED (SH1106 128x64 I2C)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Variables to store time
int hours = 0;
int minutes = 0;
int seconds = 0;

// Variables for the "millis" timer
unsigned long previousMillis = 0;
const long interval = 1000; // 1000 milliseconds = 1 second

void setup() {
  u8g2.begin();
}

void loop() {
  // Get current time since start
  unsigned long currentMillis = millis();

  // Check if 1 second has passed
  if (currentMillis - previousMillis >= interval) {
    // Save the last time we updated
    // Using += interval instead of = currentMillis prevents "time drift"
    previousMillis += interval;

    // 1. Increment seconds
    seconds++;

    // 2. Logic to handle rollover (60s -> 1m, etc.)
    if (seconds >= 60) {
      seconds = 0;
      minutes++;
    }
    if (minutes >= 60) {
      minutes = 0;
      hours++;
    }
    if (hours >= 24) {
      hours = 0;
    }

    // 3. Update the OLED Display
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr); // Slightly larger font for the clock
    
    // Starting position (X=25, Y=40)
    u8g2.setCursor(25, 40);

    // Print Hours
    if (hours < 10) u8g2.print("0");
    u8g2.print(hours);
    u8g2.print(":");

    // Print Minutes
    if (minutes < 10) u8g2.print("0");
    u8g2.print(minutes);
    u8g2.print(":");

    // Print Seconds
    if (seconds < 10) u8g2.print("0");
    u8g2.print(seconds);

    u8g2.sendBuffer();
  }

  // Because we used millis(), the code is "non-blocking"
  // You can add other code here (like reading sensors) and the clock won't stop!
}

