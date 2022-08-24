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
#define HYPNOS3 5 //Hypnos 3V Rail
#define HYPNOS5 6 //Hypnos 5V Rail
#define RTC_INT_PIN 12

int measure = 0;

//Create the function that occurs when the interrupt triggers
void interruptFunction() {
  //Exit Deep Sleep
  digitalWrite(HYPNOS3, LOW);
  pinMode(10, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);

  
  if (measure == 0)
  {
    LPrintln("\nButton Pressed");
    //Loom.measure();
    //Loom.package();
    measure = 1;
  }
}

void setup() {
  //Setting up hypnos stuff
  pinMode(HYPNOS3, OUTPUT);
  digitalWrite(HYPNOS3, LOW); //Enables 3V rail

  pinMode(RTC_INT_PIN, INPUT_PULLUP);
  
  
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
  if (measure == 0)
  {
    //Go into Deep Sleep
    digitalWrite(HYPNOS3, HIGH);
    pinMode(23, INPUT); //Turn off SPI for SD
    pinMode(24, INPUT);
    pinMode(10, INPUT);
  } else if (measure == 1)
  {
    //measure = 0;
    Loom.record();
    Loom.display_data();
    //Loom.pause();
    Loom.SDCARD().log();
    delay(2000);
    measure = 0;
  }


}
