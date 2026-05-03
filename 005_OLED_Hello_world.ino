#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

//--------------I2C 128x64 OLED---------------
//----21 to SDA
//----22 to SCL

// This line tells the library exactly which screen and pins to use
// U8G2_SH1106_128X64_NONAME_F_HW_I2C: SH1106 driver, 128x64, Full buffer, Hardware I2C
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  u8g2.begin();
}

void loop() {
  u8g2.clearBuffer();					// clear the internal memory
  
  u8g2.setFont(u8g2_font_ncenB10_tr);	// choose a suitable font
  u8g2.drawStr(0, 32, "Hello world!");	// write something to the internal memory

  u8g2.sendBuffer();					// transfer internal memory to the display
  
  delay(1000);  
}
