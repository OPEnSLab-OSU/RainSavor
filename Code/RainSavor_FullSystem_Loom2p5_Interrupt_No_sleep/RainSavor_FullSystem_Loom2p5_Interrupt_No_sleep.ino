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
const byte VR_VAL = 36; //out of 127, 1 being the lowest value

#define pin_select 12

volatile bool measure = false;

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

//Create the function that occurs when the interrupt triggers
void interruptFunction() {
  if (!measure){
    LPrintln("\nInterrupt");
    measure = true;
  }
}

//Custom function to communicate with I2C devices on the bus
void send_data(byte addr, byte data) {
  Wire.beginTransmission(addr);
  Wire.write(data);
  Wire.endTransmission(true);
}

//General Setup
void setup() {
  //Enable correct power rails from Hypnos
  pinMode(HYPNOS3, OUTPUT);
  digitalWrite(HYPNOS3, LOW);
  Wire.begin();

  Loom.begin_LED();
  
  //Begin serial communication as usual
  Loom.begin_serial(true);

  //Setup Loom
  Loom.parse_config(json_config);

  //Interrupt stuff
  pinMode(pin_select, INPUT);
  attachInterrupt( digitalPinToInterrupt(pin_select), interruptFunction, RISING );
  attachInterrupt( digitalPinToInterrupt(pin_select), interruptFunction, RISING );

  LPrintln("\n** SETUP COMPLETE - Loom 2.5.1 Enable interrupt input interrupt rising no standby **");
}

void loop() {

  if (measure)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(HYPNOS3, LOW);
    delay(50);
    
    send_data(VR_ADDR, VR_VAL);
    delay(50);//was 400

    // initialize SD
    pinMode(23, OUTPUT);
    pinMode(24, OUTPUT);
    pinMode(10, OUTPUT);

    Loom.power_up(); //only thing doing is sd.begin?
    
    Loom.record();
    Loom.display_data();
    Loom.SDCARD().log();
    delay(1000);

    measure = false;
  }

  digitalWrite(LED_BUILTIN, LOW);

  // turn off 3V rail, other Hypnos features
  digitalWrite(HYPNOS3, HIGH);
 // Disables SPI communication to SD before going to sleep
  pinMode(23, INPUT);
  pinMode(24, INPUT);
  pinMode(10, INPUT);
}
