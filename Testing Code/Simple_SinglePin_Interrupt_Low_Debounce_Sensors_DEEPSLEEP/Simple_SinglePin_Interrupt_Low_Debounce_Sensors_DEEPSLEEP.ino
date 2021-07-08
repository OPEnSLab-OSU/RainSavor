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
  //Wake up the board and turn on flag to make measurements in main loop
  digitalWrite(5, HIGH); //turn on 3v rail
  pinMode(10, OUTPUT); //Enables SPI communication to SD after going to sleep
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);

  
  if (measure == 0)
  {
    LPrintln("\nButton Pressed Test");
    //Loom.measure();
    //Loom.package();
    measure = 1;
  }
}

void setup() {
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  
  //Begin serial communication as usual
  Loom.begin_serial(true);

  Loom.parse_config(json_config);

  LPrintln("\n** SETUP COMPLETE **");

  //Setup the chosen pin as an input with the pullup resistor enabled (this can also be done with register allocations)
  pinMode(pin_select, INPUT_PULLUP);
  //Connect the pin to the interrupt function with what causes the interrupt
  enableInterrupt(pin_select, interruptFunction, LOW);
  //can use either CHANGE, RISING, FALLING, LOW - determined by hardware

}

void loop() {
  
  //delay(100);
  if (measure == 1)
  {
    //measure = 0;
    Loom.record();
    Loom.display_data();
    //Loom.pause();
    delay(50);
    measure = 0;
  } else {
    //Sleep and wait for an interrupt
    digitalWrite(5, HIGH); //turn off 3v rail
    pinMode(23, INPUT); //Disables SPI communication to SD before going to sleep
    pinMode(24, INPUT);
    pinMode(10, INPUT);
  }


}
