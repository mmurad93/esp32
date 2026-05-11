const int potPin = 4;
const int led1 = 16;
const int led2 = 15;
const int led3 = 17;
const int led4 = 18;
const int led5 = 19;
int potValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
}
  
void loop() {
  potValue = analogRead(potPin);
  Serial.println(potValue);
  if (potValue > 800) {
    digitalWrite(led1, HIGH);
    if (potValue > 1600) {
      digitalWrite(led2, HIGH);
      if (potValue > 2400) {
        digitalWrite(led3, HIGH);
        if (potValue > 3200) {
          digitalWrite(led4, HIGH);
          if (potValue > 4000) {
            digitalWrite(led5, HIGH);
          }
          else digitalWrite(led5, LOW);
        }
        else digitalWrite(led4, LOW);
      }
      else digitalWrite(led3, LOW);
    }
    else digitalWrite(led2, LOW);
  }
  else digitalWrite(led1, LOW);
  
  delay(500); // this speeds up the simulation
}
