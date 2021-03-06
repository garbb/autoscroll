#include "TimerOne.h"

//to program select
//Board:"Arduino Duemilanove or Diecimila" and Processor:"ATmega168"

//#define DEBUG

//DAC setup
#define PWMDACpin 9
#define PWMperiod 32
#define DACOUT(n) Timer1.pwm(PWMDACpin, n)
//DAC values to get proper output voltages for wheel button simulation
#define WHEEL_NONE 820
#define VOL_DOWN 24
#define VOL_UP 102
#define TRK_UP 184
#define TRK_DOWN 307
#define MUTE 614
#define MODE 450

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

  #define scrollpin1 11
  #define scrollpin2 12
  int rtknobpin = 8;
  int wheelreadpin = 0;  //analog pin
  int fmampin = 4;  //analog pin
  int buspin = 10;

  int rknob = HIGH;
  int wheel = 0;
  int amfm;
  int bus = HIGH;

  //thresholds for wheel buttons
  #define volUp_min 80
  #define volUp_max 120
  #define volDn_min 0
  #define volDn_max 50
  #define trkUp_min 170
  #define trkUp_max 210
  #define trkDn_min 290
  #define trkDn_max 330
  #define mode_min 437
  #define mode_max 477
  #define mute_min 608
  #define mute_max 648
  #define none_min 800
  #define none_max 830

void setup()   {
  //delay(8000);
  pinMode(scrollpin1, OUTPUT);
  pinMode(scrollpin2, OUTPUT);
  digitalWrite(scrollpin1, LOW);
  digitalWrite(scrollpin2, LOW);
  
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

  //setup DAC output (pin and timer we need)
  pinMode(PWMDACpin, OUTPUT);
  Timer1.initialize(PWMperiod);
  
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
}

//simulate closing of one of the "switches" inside the encoder
//short encoder pin to GND
void scrollPinClose(int scrollpin) {
//  digitalWrite(scrollpin, LOW);
  pinMode(scrollpin, OUTPUT);
}

//simulate opening of one of the "switches" inside the encoder
//set pin to INPUT mode (high impedance ~100mohm)
void scrollPinOpen(int scrollpin) {
//  digitalWrite(scrollpin, HIGH);
  pinMode(scrollpin, INPUT);
}

void left()  {
  checkdelay();
  scrollPinClose(scrollpin1);
  scrollPinClose(scrollpin2);
  lastscrolltime = millis();
  
  checkdelay();
  scrollPinOpen(scrollpin1);
  lastscrolltime = millis();
  
  checkdelay();
  scrollPinOpen(scrollpin2);
  lastscrolltime = millis();
  
  checkdelay();
  scrollPinClose(scrollpin1);
  lastscrolltime = millis();
}
void right()  {
  checkdelay();
  scrollPinClose(scrollpin1);
  scrollPinClose(scrollpin2);
  lastscrolltime = millis();
  
  checkdelay();
  scrollPinOpen(scrollpin2);
  lastscrolltime = millis();
  
  checkdelay();
  scrollPinOpen(scrollpin1);
  lastscrolltime = millis();
  
  checkdelay();
  scrollPinClose(scrollpin2);
  lastscrolltime = millis();
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
                #ifdef DEBUG
                 Serial.println("scrolling = true");
                #endif
                 //if (millis()-lastbuslow>1000) {Serial.println("TO");}
             
                 if (rknob == LOW) {                // Right knob delay
                    starttime = millis();
                    delaylength = 5000;
                 }
                 
                 if (wheel > volUp_min && wheel <= volUp_max) {           // wheel vol+
                    volupwaspressed = true;
                    starttime = millis();
                    delaylength = 500;
                 }
                 if (volupwaspressed == true && millis()-lastscrolltime>100) {
                      digitalWrite(7, HIGH);
                      volupsignalstart = millis();
                      volupwaspressed = false;
                 }
                 
                 if (wheel <= volDn_max) {                   // wheel vol-
                    voldownwaspressed = true;
                    starttime = millis();
                    delaylength = 500;
                 }
                 if (voldownwaspressed == true && millis()-lastscrolltime>100) {
                      digitalWrite(6, HIGH);
                      voldownsignalstart = millis();
                      voldownwaspressed = false;
                 }
                 
                 if (wheel > trkUp_min && wheel <= trkUp_max) {                                 // wheel T-UP delay
                    trackupwaspressed = true;
                    starttime = millis();
                    restart = true;
                    delaylength = 11000;
                 }
                 if (trackupwaspressed == true && millis()-lastscrolltime>200) {
                      digitalWrite(5, HIGH);
                      trackupsignalstart = millis();
                      trackupwaspressed = false;
                 }
                 
                 if (wheel > trkDn_min && wheel <= trkDn_max) {                                // wheel T-DN delay
                    trackdownwaspressed = true;
                    starttime = millis();
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
                    outofmp3timeindex = millis();
                    scrollPinOpen(scrollpin1);
                    scrollPinOpen(scrollpin2);
                 } 
                 if (wheel > mode_min && wheel <= mode_max) {   // MODE button stop
                    restart = false;
                    isscrolling = false;
                    delaylength = 4000000000;
                    outofmp3timeindex = millis();
                    scrollPinOpen(scrollpin1);
                    scrollPinOpen(scrollpin2);
                    digitalWrite(3, HIGH);
                 }
                 
                 if (wheel > mute_min && wheel <= mute_max) {   // wheel mute
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
                 
                 if (wheel > none_min && wheel <= none_max) {   // wheel none pressed
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
                #ifdef DEBUG
                 Serial.println("scrolling = false");
                #endif
                 
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
                 
                 if (wheel > volUp_min && wheel <= volUp_max) {           // wheel vol+
                    digitalWrite(7, HIGH);
                 }
                 if (wheel <= volDn_max) {                   // wheel vol-  should read 36
                    digitalWrite(6, HIGH);
                 }
                 if (wheel > trkUp_min && wheel <= trkUp_max) {   // wheel T-UP
                      digitalWrite(5, HIGH);
                 }
                 if (wheel > trkDn_min && wheel <= trkDn_max) {   // wheel T-DN
                      digitalWrite(4, HIGH);
                 }
                 if (wheel > mute_min && wheel <= mute_max) {   // wheel mute
                      digitalWrite(2, HIGH);
                 }
                 if (wheel > mode_min && wheel <= mode_max) {   // wheel mode
                 digitalWrite(3, HIGH);
                 outofmp3timeindex = millis();
                 }
                 if (wheel > none_min && wheel <= none_max) {   // wheel none pressed
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
