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
#define pin_select 12

//Create the function that occurs when the interrupt triggers
void interruptFunction() {
  LPrintln("\nButton Pressed");
}

void setup() {
  //Begin serial communication as usual
  Loom.begin_serial(true);

  Loom.parse_config(json_config);

  LPrintln("\n** SETUP COMPLETE **");

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);

  //Setup the chosen pin as an input with the pullup resistor enabled (this can also be done with register allocations)
  pinMode(pin_select, INPUT_PULLUP);
  //Connect the pin to the interrupt function with what causes the interrupt
  enableInterrupt(pin_select, interruptFunction, CHANGE);
  //can use either CHANGE, RISING, FALLING, LOW - determined by hardware

}

void loop() {
  // put your main code here, to run repeatedly:
  LPrintln("\nValue: %f", digitalRead(pin_select));

}
