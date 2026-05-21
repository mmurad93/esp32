#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const int potPin = 4;
const int ledPin = 16;
const int buzzer = 17;
int potVal = 0;
//-------SCL - G22, SDA - G21--------

void setup() {
  // put your setup code here, to run once:
  Serial.begin(112500);
  Serial.println(112500);
  ledcAttach(buzzer, 2000, 8);
  pinMode(potPin, INPUT);
  pinMode(ledPin, OUTPUT);
  u8g2.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  potVal = analogRead(potPin);
  Serial.println(potVal);
  int DC = potVal / 17;
  float voltage = (float) potVal * (3.3/4095);
  analogWrite(ledPin, DC);
  
  int freq = map(potVal, 0, 4095, 1000, 10000);
  ledcWriteTone(buzzer, freq);
  Serial.println(freq);
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.setCursor(0, 28);
  u8g2.print("Voltage= ");
  u8g2.print(voltage);
  Serial.println(voltage);
  u8g2.sendBuffer();
  delay(50);
}
