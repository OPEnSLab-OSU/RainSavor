/*
 * OPEnS Lab
 * Software for the RainSavor Project
 * Dr Chet Udell, Dr Segura
 * 
 * This software operates the electronics for the RainSavor project under the OPEnS lab at Oregon State University and is open source. All of the supporting files, such as electrical design, mechanical design, etc., 
 * are hosted on the OPEnS Lab wiki. 
 * The goal of this system is to sample rainfall in storms at installed locations. This software is the basis for data sampling. All of the sensing is done in hardware with a custom sensor, this software is 
 * peripheral to that sensor. This sketch will simply sample the voltages at the ADC and log it to an SD along with a timestamp. This sketch also utilizes a deep sleep mode to increase battery length
 * and field duration of the system. An interrupt system is used with the deep sleep to allow the controller to take a sample during rainfall events. 
 * 
 * This code heavily utilizes the OPEnS Loom library and software. It also assumes a OPEnS Hypnos board is being used for power control, RTC access, and SD logging. These must be present for correct operation. 
 * 
 */
  
 
//Initlialize libraries
#include <Loom.h>
#include <Wire.h>
#include <EnableInterrupt.h>


//Declare Constants & Variables
#define HYPNOS3 5
const byte VR_ADDR = 0x2E;  //I2C address for the AD5246 (variable resistor)
const byte VR_VAL = 63;

#define pin_select 12

int measure = 0;


//Initialize Loom dependencies
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




//General Setup
void setup() {
  //Enable correct power rails from Hypnos
  pinMode(HYPNOS3, OUTPUT);
  digitalWrite(HYPNOS3, LOW);
  Wire.begin();
  
  
  //Begin serial communication as usual
  Loom.begin_serial(true);

  //Setup Loom
  Loom.parse_config(json_config);

  LPrintln("\n** SETUP COMPLETE **");

}


void loop() {


    Loom.record();
    Loom.display_data();
    //Loom.pause();
    delay(400);



}
