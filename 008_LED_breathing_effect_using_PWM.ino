const int ledPin = 16; //PIN 16 to LED anode


void setup() {
  pinMode(ledPin, OUTPUT);  //Assigns pin 16 as output
}

void loop() { 
  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; ){   
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
    delay(10);
    dutyCycle = dutyCycle + 2;  //increase duty cycle by 2
  }

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 0; ){
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
    delay(10);
    dutyCycle = dutyCycle - 2;  //decrease duty cycle by 2
  }
}
