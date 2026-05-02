int ledPins[] = {25, 26, 27};  //LED 1, 2, 3
int btnPins[] = {16, 17, 18};  //button 1, 2, 3
bool b[3];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  for (int i = 0; i < 3; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
  }
}

void loop() {

  for (int i = 0; i < 3; i++) {
    b[i] = digitalRead(btnPins[i]);
  }

  if (b[0] == LOW && b[1] == HIGH && b[2] == HIGH) {
    seq_1();
  }
  else if (b[0] == HIGH && b[1] == LOW && b[2] == HIGH) {
    seq_2();
  }
  else if (b[0] == HIGH && b[1] == HIGH && b[2] == LOW){
    seq_3();
  }
}

void seq_1() {
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(ledPins[i], HIGH); delay(500);
      digitalWrite(ledPins[i], LOW);
    }
  }
}

void seq_2() {
  for (int i = 0; i < 3; i++) {
        digitalWrite(ledPins[0], HIGH);
        digitalWrite(ledPins[2], HIGH); delay(500);
        digitalWrite(ledPins[0], LOW);
        digitalWrite(ledPins[2], LOW);
        digitalWrite(ledPins[1], HIGH); delay(500);
        digitalWrite(ledPins[1], LOW);
  }
}

void seq_3() {
  for (int i = 0; i < 3; i++) {
      digitalWrite(ledPins[0], HIGH); delay(500);
      digitalWrite(ledPins[0], LOW);  delay(500);
      digitalWrite(ledPins[0], HIGH); delay(250);
      digitalWrite(ledPins[1], HIGH); delay(500);
      digitalWrite(ledPins[0], LOW);
      digitalWrite(ledPins[1], LOW);  delay(500);
      digitalWrite(ledPins[0], HIGH); delay(250);
      digitalWrite(ledPins[1], HIGH); delay(250);
      digitalWrite(ledPins[2], HIGH); delay(500);
      digitalWrite(ledPins[0], LOW);
      digitalWrite(ledPins[1], LOW);
      digitalWrite(ledPins[2], LOW);  delay(500);
    }
}
