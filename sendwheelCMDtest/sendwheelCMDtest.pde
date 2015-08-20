// The setup() method runs once, when the sketch starts

void setup()   {
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(0, OUTPUT);
}

// the loop() method runs over and over again,
// as long as the Arduino has power


void loop()  {
digitalWrite(5, LOW);
digitalWrite(4, LOW);
digitalWrite(3, LOW);
digitalWrite(2, LOW);
digitalWrite(1, LOW);
digitalWrite(0, LOW);
delay(10000);
digitalWrite(5, HIGH);
delay(10000);
digitalWrite(5, LOW);
digitalWrite(4, HIGH);
delay(10000);
digitalWrite(4, LOW);
digitalWrite(3, HIGH);
delay(10000);
digitalWrite(3, LOW);
digitalWrite(2, HIGH);
delay(10000);
digitalWrite(2, LOW);
digitalWrite(1, HIGH);
delay(10000);
digitalWrite(1, LOW);
digitalWrite(0, HIGH);
delay(10000);
digitalWrite(0, LOW);
}
