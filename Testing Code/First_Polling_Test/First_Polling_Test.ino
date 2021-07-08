#include <Loom.h>

const char* json_config =
#include "config.h"
;

LoomFactory<
  Enable::Internet::Disabled,
  Enable::Sensors::Enabled,
  Enable::Radios::Disabled,
  Enable::Actuators::Disabled,
  Enable::Max::Disabled
>ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

#include <EnableInterrupt.h>


//select the pin that triggers the interrupt
#define pin_select A0

void interruptFunction() {
  //UNSLEEP
  LPrintln("INTERRUPTED");
}


void setup() {
  //Begin serial communication as usual
  Loom.begin_serial(true);

  Loom.parse_config(json_config);

  LPrintln("\n** SETUP COMPLETE **");

  //Setup the chosen pin as an input with the pullup resistor enabled (this can also be done with register allocations)
  pinMode(pin_select, INPUT);
  //Connect the pin to the interrupt function with what causes the interrupt

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW); //Enable 3v Rail

  pinMode(12, INPUT);
  enableInterrupt(12, interruptFunction, FALLING);
}

void loop() {
  delay(100);
  LPrintln(digitalRead(12));
  //if (analogRead(pin_select) < 50)
  //  LPrintln("INTERRUPT");

}
