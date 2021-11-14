#include "customDebug.h"

void lightUp(){
  dbmsg("light up");
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
}

void lightDown(){
  dbmsg("light down");
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void lightBlink(int numberOfBlinks, int delayDuration){
  dbmsg("blink light");

  for (int i = 0; i < numberOfBlinks; i++)
  {
    delay(delayDuration);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delayDuration);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delayDuration);
    digitalWrite(LED_BUILTIN, LOW);
  }
}