// The setup() method runs once, when the sketch starts

  const int scrollspeed = 200;
  
  unsigned long time;
  
  int delaylength = scrollspeed;

void setup()   {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(8, INPUT);
  digitalWrite(8, HIGH);
  Serial.begin(9600);
}

// the loop() method runs over and over again,
// as long as the Arduino has power

void loop()  {
  checkdelay();
  time = micros();
  Serial.println(0);
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);
  Serial.println(micros() - time);
  checkdelay();
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  checkdelay();
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  checkdelay();
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
}

void checkdelay()                     
{
  unsigned long starttime = millis();
         while( (millis() - starttime) < delaylength ) {
                 if (digitalRead(8) == LOW) {
                    starttime = millis();
                    delaylength = 5000;
                 }
         delay(1);
	}
  delaylength = scrollspeed;
}
