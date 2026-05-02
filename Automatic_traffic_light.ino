#define LED1 26  //LED1 connects to GPIO26
#define LED2 25  //LED2 connects to GPIO25
#define LED3 33  //LED3 connects to GPIO25

void setup() {
  // for serial monitor
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");


  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}

//----------Automatic Traffic Light------------

void loop() {
  digitalWrite(LED1, HIGH);
  delay(5000);    //5s delay
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  delay(2000);    //2s delay
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  delay(5000);    //5s delay
  digitalWrite(LED3, LOW);
}

