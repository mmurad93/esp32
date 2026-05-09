int ledPin = 16; 
int btn1 = 25;
int btn2 = 26;
int DC = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
}

void loop() {

  if (digitalRead(btn1) == LOW && DC <= 245) {
    DC = DC + 10;
    delay(150); // Simple "debounce" to prevent value flying to 255 instantly
  }
  if (digitalRead(btn2) == LOW && DC >= 10) {
    DC = DC - 10;
    delay(150); // Simple debounce
  }

  analogWrite(ledPin, DC);
}
