// The setup() method runs once, when the sketch starts

//to program select
//Board:"Arduino Duemilanove or Diecimila" and Processor:"ATmega168"

//#define DEBUG

  const int scrollspeed = 75;
  
  unsigned long lastDebounceTime = 0;
  int lastButtonState = 815;
  unsigned long debounceDelay = 30;

  unsigned long fmAmCount_lastTime = 0;
  unsigned int fmAmCount = 0;
  int amFmreading_temp = 0;
  int amFmlastButtonState = 0;
  bool amFmbuttonPressed = false;
  bool amFmbuttonPressed_last = false;
  
  unsigned long lastscrolltime = 0;
  unsigned long trackupsignalstart;
  unsigned long trackdownsignalstart;
  unsigned long volupsignalstart;
  unsigned long voldownsignalstart;
  unsigned long mutesignalstart;
  unsigned long outofmp3timeindex = 0;
  unsigned long mutewaspressedtime = 0;
  unsigned long lastbuslow = 0;
  unsigned long firstbuslow = 0;
  
  unsigned long delaylength = 4000000000;
  
  boolean isscrolling = false;
  boolean restart = false;
  boolean trackupwaspressed = false;
  boolean trackdownwaspressed = false;
  boolean volupwaspressed = false;
  boolean voldownwaspressed = false;
  boolean mutewaspressed = false;
  
  boolean firstbuscheck = false;

/* PINS
    Right knob                   digital 8
    wheel input                  analog 0
    AM/FM                        analog 4
    Bus                          digital 10
    
    wheel CMD outputs            digital 7,6,5,4,3,2
    				vol+	7
    				vol-	6
    				T-up	5
    				T-dn	4
    				mode	3
    				mute	2
    
    Scroll outputs               digital 11,12
*/

  int scrollpin1 = 12;
  int scrollpin2 = 11;
  int rtknobpin = 8;
  int wheelreadpin = 0;  //analog pin
  int fmampin = 4;  //analog pin
  int buspin = 10;

  int rknob = HIGH;
  int wheel = 0;
  int amfm;
  int bus = HIGH;

void setup()   {
  //delay(8000);
  pinMode(scrollpin1, OUTPUT);
  pinMode(scrollpin2, OUTPUT);
  
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  digitalWrite(3, LOW);
  digitalWrite(2, LOW);
  digitalWrite(buspin, HIGH);
  //digitalWrite(3, HIGH);
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
}

void left()  {
  checkdelay();
  digitalWrite(scrollpin1, LOW);
  digitalWrite(scrollpin2, LOW); lastscrolltime = millis();
  checkdelay();
  digitalWrite(scrollpin1, HIGH);
  digitalWrite(scrollpin2, LOW); lastscrolltime = millis();
  checkdelay();
  digitalWrite(scrollpin1, HIGH);
  digitalWrite(scrollpin2, HIGH); lastscrolltime = millis();
  checkdelay();
  digitalWrite(scrollpin1, LOW);
  digitalWrite(scrollpin2, HIGH); lastscrolltime = millis();
}
void right()  {
  checkdelay();
  digitalWrite(scrollpin1, LOW);
  digitalWrite(scrollpin2, LOW); lastscrolltime = millis();
  checkdelay();
  digitalWrite(scrollpin1, LOW);
  digitalWrite(scrollpin2, HIGH); lastscrolltime = millis();
  checkdelay();
  digitalWrite(scrollpin1, HIGH);
  digitalWrite(scrollpin2, HIGH); lastscrolltime = millis();
  checkdelay();
  digitalWrite(scrollpin1, HIGH);
  digitalWrite(scrollpin2, LOW); lastscrolltime = millis();
}



void loop()  {
  start:
  for (int x = 0; x <20; x++) {
    if (restart == true) {restart=false; /*delay(10000);*/ goto start;}
    right();
  }
  for (int x = 0; x <20; x++) {
    if (restart == true) {restart=false; /*delay(10000);*/ goto start;}
    left();
  }
}


// all wheel readings are with external ?? pullup
void checkdelay()
{
  if (isscrolling == false) delaylength = 4000000000;
  unsigned long starttime = millis();
         while( (millis() - starttime) < delaylength ) {
           rknob = digitalRead(rtknobpin);
           
           int reading = analogRead(wheelreadpin);    //debounce
           //Serial.print(reading);
           //Serial.print("\t");
           //Serial.print(lastButtonState);
           if (abs(reading - lastButtonState) > 3) {    //value fluctuates a bit so if it changes by less than 3 then consider it unchanging
            // reset the debouncing timer
            lastDebounceTime = millis();
           }
           if ((millis() - lastDebounceTime) > debounceDelay) {
          // whatever the reading is at, it's been there for longer
          // than the debounce delay, so take it as the actual current state:
          wheel = reading;
           }
          //Serial.print("\t");
          //Serial.println(wheel);

           
           //debounce amfm button reading
           amFmreading_temp = analogRead(fmampin);
           if (amFmreading_temp > 54 && amFmreading_temp < 63) {
            #ifdef DEBUG
              Serial.print(millis()); Serial.print(" amFmreading_temp = "); Serial.println(amFmreading_temp);
            #endif
            fmAmCount++;
            fmAmCount_lastTime = millis();
           }
           if (fmAmCount > 0 && (millis() - fmAmCount_lastTime) > 10) {
            fmAmCount = 0;
            amFmbuttonPressed = false;
           }
           if (fmAmCount >= 2) amFmbuttonPressed = true;
           #ifdef DEBUG
            if (amFmbuttonPressed_last != amFmbuttonPressed) {Serial.print("amFmbuttonPressed = "); Serial.println(amFmbuttonPressed);}
           #endif

           
           bus = digitalRead(buspin);
           //Serial.println(bus);
           //Serial.println(delaylength);
           if (isscrolling == true) {
//                #ifdef DEBUG
//                 Serial.println("scrolling = true");
//                #endif
                 //if (millis()-lastbuslow>1000) {Serial.println("TO");}
             
                 if (rknob == LOW) {                // Right knob delay
                    starttime = millis();
                    //digitalWrite(scrollpin1, LOW);
                    //digitalWrite(scrollpin2, LOW);
                    delaylength = 5000;
                 }
                 
                 if (wheel > 80 && wheel <= 120) {           // wheel vol+
                    volupwaspressed = true;
                    starttime = millis();
                    delaylength = 500;
                 }
                 if (volupwaspressed == true && millis()-lastscrolltime>100) {
                      digitalWrite(7, HIGH);
                      volupsignalstart = millis();
                      volupwaspressed = false;
                 }
                 
                 if (wheel <= 50) {                   // wheel vol-
                    voldownwaspressed = true;
                    starttime = millis();
                    delaylength = 500;
                 }
                 if (voldownwaspressed == true && millis()-lastscrolltime>100) {
                      digitalWrite(6, HIGH);
                      voldownsignalstart = millis();
                      voldownwaspressed = false;
                 }
                 
                 if (wheel > 170 && wheel <= 210) {                                 // wheel T-UP delay
                    trackupwaspressed = true;
                    starttime = millis();
                    //digitalWrite(scrollpin1, LOW);
                    //digitalWrite(scrollpin2, LOW);
                    restart = true;
                    delaylength = 11000;
                 }
                 if (trackupwaspressed == true && millis()-lastscrolltime>200) {
                      digitalWrite(5, HIGH);
                      trackupsignalstart = millis();
                      trackupwaspressed = false;
                 }
                 
                 if (wheel > 290 && wheel <= 330) {                                // wheel T-DN delay
                    trackdownwaspressed = true;
                    starttime = millis();
                    //digitalWrite(scrollpin1, LOW);
                    //digitalWrite(scrollpin2, LOW);
                    restart = true;
                    delaylength = 11000;
                 }
                 if (trackdownwaspressed == true && millis()-lastscrolltime>200) {
                      digitalWrite(4, HIGH);
                      trackdownsignalstart = millis();
                      trackdownwaspressed = false;
                 }

//                 if (amfm > 54 && amfm < 63) {   // AM/FM button stop.   got 59 once when button pressed
                 if (amFmbuttonPressed) {   // AM/FM button stop.   got 59 once when button pressed
//                   #ifdef DEBUG
//                    Serial.print("amfm pin = "); Serial.println(amfm);
//                   #endif
                    restart = false;
                    isscrolling = false;          // WILL NOT READ PROPERLY WITH
                    delaylength = 4000000000;        // LAPTOP cig lighter adaptor plugged in
                    //digitalWrite(scrollpin1, LOW);
                    //digitalWrite(scrollpin2, LOW);
                    outofmp3timeindex = millis();
                 } 
                 if (wheel > 437 && wheel <= 477) {   // MODE button stop
                    restart = false;
                    isscrolling = false;
                    delaylength = 4000000000;
                    //digitalWrite(scrollpin1, LOW);
                    //digitalWrite(scrollpin2, LOW);
                    outofmp3timeindex = millis();
                    digitalWrite(3, HIGH);
                 }
                 
                 if (wheel > 608 && wheel <= 648) {   // wheel mute
                      if (millis()-mutewaspressedtime>100) {
                        mutewaspressed = true;
                        if (delaylength == 3000001) delaylength = scrollspeed;
                        else if (delaylength != 3000001) delaylength = 3000001;
                      }
                      mutewaspressedtime = millis();
                 }
                 if (mutewaspressed == true && millis()-lastscrolltime>200) {
                      digitalWrite(2, HIGH);
                      mutesignalstart = millis();
                      mutewaspressed = false;
                 }
                 
                 if (wheel > 800 && wheel <= 830) {   // wheel none pressed
                      if (millis()-volupsignalstart>50) digitalWrite(7, LOW);
                      if (millis()-voldownsignalstart>50) digitalWrite(6, LOW);
                      if (millis()-trackupsignalstart>100) digitalWrite(5, LOW);
                      if (millis()-trackdownsignalstart>100) digitalWrite(4, LOW);
                      digitalWrite(3, LOW);
                      if (millis()-mutesignalstart>100) digitalWrite(2, LOW);
                  }
          lastButtonState = reading;
          //wheel = 815;      //needed to reset once per loop because debounce code above 'holds' wheel at a value
          }
           if (isscrolling == false) {
//                #ifdef DEBUG
//                 Serial.println("scrolling = false");
//                #endif
                 
                 if (bus == LOW) {
                   if (millis()-lastbuslow>1000) {firstbuscheck=false;}  //timeout so restart firstbuslow
                   if (firstbuscheck==false) {firstbuslow=millis(); firstbuscheck=true;}
                   lastbuslow=millis();
                 }
                 //Serial.print(millis()-firstbuslow); Serial.print("\t"); Serial.print(millis()-lastbuslow);
                 if (bus == LOW && millis()-firstbuslow>11000 && millis()-lastbuslow<1000 && millis()-outofmp3timeindex>11000 && millis() > 11000) {       // Bus check
                 //if (bus == LOW && millis()-outofmp3timeindex>11000 && millis() > 11000) {       // Bus check
                    isscrolling = true;
                    delaylength = scrollspeed;
                    restart = true;
                    firstbuscheck = false;
                 }
                 
//                 if (amfm > 56 && amfm < 63) {   // AM/FM button stop
                 if (amFmbuttonPressed) {   // AM/FM button stop
                                                  // WILL NOT READ PROPERLY WITH
                                                  // LAPTOP cig lighter adaptor plugged in
                    outofmp3timeindex = millis();
                 } 
                 
                 if (wheel > 80 && wheel <= 120) {           // wheel vol+
                    digitalWrite(7, HIGH);
                 }
                 if (wheel <= 50) {                   // wheel vol-  should read 36
                    digitalWrite(6, HIGH);
                 }
                 if (wheel > 170 && wheel <= 210) {   // wheel T-UP
                      digitalWrite(5, HIGH);
                 }
                 if (wheel > 290 && wheel <= 330) {   // wheel T-DN
                      digitalWrite(4, HIGH);
                 }
                 if (wheel > 608 && wheel <= 648) {   // wheel mute
                      digitalWrite(2, HIGH);
                 }
                 if (wheel > 437 && wheel <= 477) {   // wheel mode
                 digitalWrite(3, HIGH);
                 outofmp3timeindex = millis();
                 }
                 if (wheel > 800 && wheel <= 830) {   // wheel none pressed
                      digitalWrite(7, LOW);
                      digitalWrite(6, LOW);
                      digitalWrite(5, LOW);
                      digitalWrite(4, LOW);
                      digitalWrite(3, LOW);
                      digitalWrite(2, LOW);
                 }
                 lastButtonState = reading;
           }
           amFmbuttonPressed_last = amFmbuttonPressed;
         delay(0);  // i *think* this stopped the volume up/dn from randomly changing the track
                    // but could also be making it miss AMFM button detection
                    // might need to use delayMicroseconds() ??

	}
  delaylength = scrollspeed;
}
