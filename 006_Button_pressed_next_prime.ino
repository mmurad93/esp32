#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Preferences.h> // Include Preferences library

#define BTN 25

Preferences preferences; // Create instance of Preferences
int a = 1; 
int b = 0;

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /*reset*/ U8X8_PIN_NONE);

void setup() {
  pinMode(BTN, INPUT_PULLUP);
  u8g2.begin();

  // Open "storage" namespace. false = read/write mode
  preferences.begin("prime-data", false);

  // Load the last saved value of 'a'. If no value exists, default to 1.
  a = preferences.getInt("last_prime", 1);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.setCursor(5, 20);
  u8g2.print("Last Saved:");
  u8g2.setCursor(5, 45);
  u8g2.print(a);
  u8g2.sendBuffer();
}

void loop() {
  if (digitalRead(BTN) == LOW) {
    a++; 
    while (primeFind(a) == 0) {
      a++;
    }
    
    b = primeFind(a);

    // Save the new prime to flash memory
    preferences.putInt("last_prime", a);

    u8g2.clearBuffer();
    u8g2.setCursor(5, 32);
    if (a == b) {
      u8g2.print(a);
      u8g2.print(" is a prime!");
    }
    u8g2.sendBuffer();

    delay(300); 
  }
}

int primeFind (int c) {
  if (c < 2) return 0;
  for (int i = 2; i * i <= c; i++) {
    if (c % i == 0) return 0;
  }
  return c;
}
