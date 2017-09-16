#include "TimerOne.h"

#define PWMDACpin 10
#define PWMperiod 32
#define PWMVALUE(v) ((1024/5)*v)
#define DACOUT(n) Timer1.pwm(PWMDACpin, n)

//#define WHEEL_NONE 820
//const uint16_t WHEEL_NONE = PWMVALUE(4);
#define WHEEL_NONE 820
#define VOL_DOWN 24
#define VOL_UP 102
#define TRK_UP 184
#define TRK_DOWN 307
#define MUTE 614
#define MODE 450


void setup() {
  pinMode(PWMDACpin, OUTPUT);
  Timer1.initialize(PWMperiod);
}

void loop() {
  DACOUT(WHEEL_NONE);
  delay(4000);
  DACOUT(VOL_DOWN);
  delay(4000);
  DACOUT(VOL_UP);
  delay(4000);
  DACOUT(TRK_UP);
  delay(4000);
  DACOUT(TRK_DOWN);
  delay(4000);
  DACOUT(MUTE);
  delay(4000);
  DACOUT(MODE);
  delay(4000);
}
