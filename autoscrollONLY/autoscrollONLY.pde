// The setup() method runs once, when the sketch starts

void setup()   {
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
}

// the loop() method runs over and over again,
// as long as the Arduino has power

void left()  {
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  delay(200);
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);
  delay(200);
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  delay(200);
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  delay(200);
}
void right()  {
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  delay(200);
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  delay(200);
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  delay(200);
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);
  delay(200);
}

void loop()  {
  for (int x = 0; x <32; x++) {
    right();
  }
  for (int x = 0; x <32; x++) {
    left();
  }
}
