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

int measure = 0;

//Create the function that occurs when the interrupt triggers
void interruptFunction() {
  if (measure == 0)
  {
    LPrintln("\nButton Pressed");
    measure = 1;
  }
}

void setup() {
  //Begin serial communication as usual
  Loom.begin_serial(true);

  Loom.parse_config(json_config);

  LPrintln("\n** SETUP COMPLETE **");

  //Setup the chosen pin as an input with the pullup resistor enabled (this can also be done with register allocations)
  pinMode(pin_select, INPUT_PULLUP);
  //Connect the pin to the interrupt function with what causes the interrupt
  enableInterrupt(pin_select, interruptFunction, CHANGE);
  //can use either CHANGE, RISING, FALLING, LOW - determined by hardware

}

void loop() {
  delay(100);
  if (measure == 1)
  {
    measure = 0;
  }

}
