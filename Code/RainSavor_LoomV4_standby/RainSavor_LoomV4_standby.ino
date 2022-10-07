/**
 * OPEnS Lab
 * Software for the RainSavor Project
 * Dr Chet Udell, Dr Segura
 * 
 * This software operates the electronics for the RainSavor project under the OPEnS lab at Oregon State University and is open source. All of the supporting files, such as electrical design, mechanical design, etc., 
 * are hosted on the OPEnS Lab wiki. 
 * The goal of this system is to sample the electrical conductivity of rainfall in storms at installed locations. This software is the basis for data sampling. All of the sensing is done in hardware with a custom sensor, this software is 
 * peripheral to that sensor. 
 * 
 * This project uses an I2C AD5246 variable resistor and an SHT31 sensor to measure environment data and logs it to both the SD card 
 * 
 * MANAGER MUST BE INCLUDED FIRST IN ALL CODE
 * Loom version 4.0.5 (https://github.com/OPEnSLab-OSU/Loom-V4/releases/tag/v4.0.5) with modified Hypnos files in this folder
 */

#include <Loom_Manager.h>

#include <Hardware/Loom_Hypnos/Loom_Hypnos.h>

#include <Sensors/Loom_Analog/Loom_Analog.h>
#include <Sensors/I2C/Loom_SHT31/Loom_SHT31.h>
#include <Sensors/I2C/Loom_ADS1115/Loom_ADS1115.h>

Manager manager("RainSavor", 1);

// Analog for reading battery voltage
Loom_Analog analog(manager);

// Create a new Hypnos object - need Will's version of hypnos.cpp and .h
// Manager& man, HYPNOS_VERSION version, TIME_ZONE zone, bool use_custom_time, bool useSD, bool useRTC, bool use5V) 
Loom_Hypnos hypnos(manager, HYPNOS_VERSION::V3_2, TIME_ZONE::PST, true, true, false, false);

// Create the SHT classes
Loom_SHT31 sht(manager);

// Create ADS1115
Loom_ADS1115 ads(manager, 0x48, true, false, adsGain_t::GAIN_TWOTHIRDS);

//Declare Constants & Variables
const byte VR_ADDR = 0x2E;  //I2C address for the AD5246 (variable resistor)
const byte VR_VAL = 23; //out of 127, 1 being the lowest value

#define pin_select 12
volatile bool wakeup = false;

// Called when the interrupt is triggered 
void isrTrigger(){
   wakeup = true;
}

//Custom function to communicate with I2C devices on the bus
void send_data(byte addr, byte data) {
  Wire.beginTransmission(addr);
  Wire.write(data);
  Wire.endTransmission();
}

void setup() {

  // Wait 20 seconds for the serial console to open
  manager.beginSerial();

  // Enable the hypnos rails, 3 SD pins, sd begin
  hypnos.enable();

  // Initialize all in-use modules
  manager.initialize();

   //Interrupt stuff
  pinMode(pin_select, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin_select), isrTrigger, LOW);
}

void loop() {
  
  if(wakeup){
    // Enable variable resistor
    //have to set each time it wakes up
    send_data(VR_ADDR, VR_VAL);
    Serial.print("Resistor value: ");
    Serial.println(VR_VAL);
    manager.pause(50);
    
    // Measure and package the data
    manager.measure();
    manager.package();   

    //manager.addData("Analog Values", "Conductivity", ads.getAnalog(1));
    manager.addData("VariableResistor","SetValue",VR_VAL);
  
    // Log the data to the SD card              
    hypnos.logToSD();

    // Print the current JSON packet
    manager.pause(1000);
    manager.display_data();
  
    wakeup = false;
  }

  // Put the device into a deep sleep, operation HALTS here until the interrupt is triggered, don't wait for serial
  hypnos.sleep(false);
}
