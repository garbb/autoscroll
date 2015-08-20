// The setup() method runs once, when the sketch starts

  const int scrollspeed = 75;
  
  unsigned long lastscrolltime = 0;
  unsigned long trackupsignalstart;
  unsigned long trackdownsignalstart;
  unsigned long mutesignalstart;
  unsigned long outofmp3timeindex = 0;
  unsigned long mutewaspressedtime = 0;
  
  unsigned long delaylength = 4000000000;
  
  boolean isscrolling = false;
  boolean restart = false;
  boolean trackupwaspressed = false;
  boolean trackdownwaspressed = false;
  boolean mutewaspressed = false;

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
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  
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
  //Serial.begin(9600);
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
           wheel = analogRead(wheelreadpin);
           amfm = analogRead(fmampin);
           bus = digitalRead(buspin);
           //Serial.println(delaylength);
           if (isscrolling == true) {
                 if (rknob == LOW) {                // Right knob delay
                    starttime = millis();
                    //digitalWrite(scrollpin1, LOW);
                    //digitalWrite(scrollpin2, LOW);
                    delaylength = 5000;
                 }
                 
                 if (wheel > 80 && wheel <= 120) {           // wheel vol+
                    digitalWrite(7, HIGH);
                    starttime = millis();
                    delaylength = 500;
                 }
                 if (wheel <= 50) {                   // wheel vol-
                    digitalWrite(6, HIGH);
                    starttime = millis();
                    delaylength = 500;
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

                 if (amfm > 56 && amfm < 63) {   // AM/FM button stop
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
                      digitalWrite(7, LOW);
                      digitalWrite(6, LOW);
                      if (millis()-trackupsignalstart>100) digitalWrite(5, LOW);
                      if (millis()-trackdownsignalstart>100) digitalWrite(4, LOW);
                      digitalWrite(3, LOW);
                      if (millis()-mutesignalstart>100) digitalWrite(2, LOW);
                  }
          }
           if (isscrolling == false) {
             //Serial.println(bus);
                 if (bus == LOW && millis()-outofmp3timeindex>5000 && millis() > 11000) {       // Bus check
                    isscrolling = true;
                    delaylength = scrollspeed;
                    restart = true;
                 }
                 
                 if (amfm > 56 && amfm < 63) {   // AM/FM button stop
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
           }
         delay(0);  // i *think* this stopped the volume up/dn from randomly changing the track
                    // but could also be making it miss AMFM button detection
                    // might need to use delayMicroseconds() ??

	}
  delaylength = scrollspeed;
}