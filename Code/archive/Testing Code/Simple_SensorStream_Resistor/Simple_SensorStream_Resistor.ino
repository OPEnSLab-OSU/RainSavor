#include <Loom.h>
#include <Wire.h>

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

void send_data(byte addr, byte data) {
  Wire.beginTransmission(addr);

  Wire.write(data);

  Wire.endTransmission(true);
}

void setup() {
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  Wire.begin();
  
  //Begin serial communication as usual
  Loom.begin_serial(true);

  Loom.parse_config(json_config);

  LPrintln("\n** SETUP COMPLETE **");

}

byte i = 20;

void loop() {
  //delay(100);

   send_data(0x2E, i);
   Loom.record();
   Loom.display_data();
   LPrintln("\nVariable resistor D = \d", i);

   //Loom.SDCARD().log();
      
   Loom.pause(1000);




}
