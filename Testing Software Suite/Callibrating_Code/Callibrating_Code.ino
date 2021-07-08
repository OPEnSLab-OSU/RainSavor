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

const byte VR_ADDR = 0x2E;  //I2C address for the AD5246 (variable resistor)
const byte VR_VAL = 40;     //Tunable value to change the resolution of the sensor (value 1-127)

//Custom function to communicate with I2C devices on the bus
void send_data(byte addr, byte data) {
  Wire.beginTransmission(addr);

  Wire.write(data);

  Wire.endTransmission(true);
  
}

void setup() {
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  
  //Begin serial communication as usual
  Loom.begin_serial(true);

  Loom.parse_config(json_config);

  send_data(VR_ADDR, VR_VAL);

  LPrintln("\n** SETUP COMPLETE **");

}

void loop() {
  //delay(100);


    Loom.record();
    Loom.display_data();
    Loom.pause(1000);
    //delay(50);



}
