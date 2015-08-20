unsigned long lastzerotime;
unsigned long lastzerowait;

void setup()  {
   Serial.begin(9600);
   //attachInterrupt(0, inter, CHANGE);
}

void loop()  {
  lastzerowait = millis()-lastzerotime;
  if (analogRead(0) < 10) {  
    lastzerotime = millis();
    if (lastzerowait > 10)  {
      Serial.println(lastzerowait);
    }
  }

  //Serial.println(millis()-lastzerotime);
  delay(1);
}

//void inter()  {
//  Serial.println("INTERRUPT");
//}
