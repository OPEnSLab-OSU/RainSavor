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

void setup() {
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  
  //Begin serial communication as usual
  Loom.begin_serial(true);

  Loom.parse_config(json_config);

  LPrintln("\n** SETUP COMPLETE **");

}

void loop() {
  //delay(100);


    Loom.record();
    Loom.display_data();
    Loom.pause(1000);
    //delay(50);



}
