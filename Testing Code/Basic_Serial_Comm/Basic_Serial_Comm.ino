//Always include the following

#include <Loom.h>

//config file sets up the sensors and what data is being pulled from those ports/wires
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

 //Always include up to here

 void setup()
 {
  Loom.begin_serial(true); //Turn on serial

  Loom.parse_config(json_config); //Uses config file to determine sensors & data needed for measurements

  LPrintln("Setup Complete *****************");
 }

 void loop()
 {
  Loom.measure(); //Measure data of all managed sensors. Doesn't prep for output

  Loom.package(); //Packages data of all modules into JSON object
  
  Loom.display_data(); //displays data from the measured sensors listed in the config/LoomFactory setup (prints out internal JSON object)
  //When called solo has an empty JSON object

  Loom.pause(); //simply pauses Loom for a small amount of time. Makes serial output much more readable
 }
