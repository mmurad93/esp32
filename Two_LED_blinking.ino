#define LED1 26  //LED1 connects to GPIO26
#define LED2 25  //LED2 connects to GPIO25


void setup() {
  // for serial monitor
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");


  pinMode(LED1, OUTPUT);  //assigned as output
  pinMode(LED2, OUTPUT);  //assigned as output
}


void loop() {
  digitalWrite(LED2, LOW);
  digitalWrite(LED1, HIGH);
  delay(300);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  delay(300);
}
