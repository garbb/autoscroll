int volts = 0;

void setup()  {
   //Serial.begin(9600);
   pinMode(13, OUTPUT); 
}

void loop()  {

      //Serial.println(analogRead(0));
      //int starttime = micros();
      volts = analogRead(0);
      if (volts > 15 && volts <= 25){
        digitalWrite(13, LOW);
        //Serial.println(volts);
      }
      else if (volts > 55 && volts <= 65) {
        digitalWrite(13, HIGH);
        //Serial.println(volts);
      }

  //Serial.println(millis()-lastzerotime);
  //Serial.println(micros()-starttime);
  delay(10);
}
