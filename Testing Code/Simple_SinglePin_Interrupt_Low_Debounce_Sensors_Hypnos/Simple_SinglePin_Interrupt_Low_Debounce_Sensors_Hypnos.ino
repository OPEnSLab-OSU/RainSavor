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
#define pin_select 11

int measure = 0;

//Create the function that occurs when the interrupt triggers
void interruptFunction() {
  sensor_measure();
}

void sensor_measure() {
  if (measure == 0)
  {
    LPrintln("\nButton Pressed");
    //Loom.measure();
    //Loom.package();
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
  enableInterrupt(pin_select, interruptFunction, LOW);
  //can use either CHANGE, RISING, FALLING, LOW - determined by hardware

  //Hypnos Setup
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
}

void loop() {
  digitalWrite(5, HIGH); //< Disabling all pins before going to sleep.
  digitalWrite(6, LOW);
  pinMode(23, INPUT);
  pinMode(24, INPUT);
  pinMode(10, INPUT);

  Loom.SleepManager().sleep();
  
  if (measure == 1)
  {
    
    Loom.record();
    Loom.display_data();
    //Loom.pause();
    delay(50);
    measure = 0;
  }
  //5ms delay for debouncing
}
