int volts = 0;

void setup()  {
   //Serial.begin(9600);
   pinMode(13, OUTPUT); 
}

void loop()  {

      //Serial.println(analogRead(0));
      //int starttime = micros();
      volts = digitalRead(3);
      if (volts == LOW){
        digitalWrite(13, HIGH);
        //Serial.println(volts);
      }
      else {
        digitalWrite(13, LOW);
        //Serial.println(volts);
      }

  //Serial.println(millis()-lastzerotime);
  //Serial.println(micros()-starttime);
  delay(10);
}
