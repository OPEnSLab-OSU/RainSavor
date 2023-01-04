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
 * Loom version 4.0.6 (https://github.com/OPEnSLab-OSU/Loom-V4/releases/tag/v4.0.6)
 */

#include <Loom_Manager.h>

#include <Hardware/Loom_Hypnos/Loom_Hypnos.h>

#include <Sensors/Loom_Analog/Loom_Analog.h>
#include <Sensors/I2C/Loom_SHT31/Loom_SHT31.h>
#include <Sensors/I2C/Loom_ADS1115/Loom_ADS1115.h>

Manager manager("RS2_", 2);

// Analog for reading battery voltage
Loom_Analog analog(manager); //add , A0 if reading analog pin

// Create a new Hypnos object
// Manager& man, HYPNOS_VERSION version, TIME_ZONE zone, bool use_custom_time, bool useSD) 
Loom_Hypnos hypnos(manager, HYPNOS_VERSION::V3_2, TIME_ZONE::PST, true, true);

// Create the SHT classes
Loom_SHT31 sht(manager);

// Create ADS1115
Loom_ADS1115 ads(manager);

//Declare Constants & Variables
const byte VR_ADDR = 0x2E;  //I2C address for the AD5246 (variable resistor)
const byte VR_VAL = 16; //1 = 150 Ohms, 127 = 400 Ohms; lower value = larger maximum reading
const float EC_SLOPE = 0.013973; //RS1: 0.013814; RS2: 0.013973; RS3: 0.013253; RS4: 0.015562; RS5: 0.014423; RS6: 0.013836 
const float EC_INTERCEPT = -51.2529; // RS1:-51.8934; RS2: -51.2529; RS3: -51.6423; RS4: -65.2051; RS5: -62.0963; RS6: -55.3435 
const float TEMP_COEFF = 0.02;

// Called when the interrupt is triggered 
void isrTrigger(){
  hypnos.wakeup();
}

//Custom function to communicate with I2C devices on the bus for the AD5246 variable resistor
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

  // Register the ISR and attach to the interrupt
  hypnos.registerInterrupt(isrTrigger);
}

void loop() {
  
    // Enable variable resistor
    //have to set each time it wakes up
    send_data(VR_ADDR, VR_VAL);
    Serial.print("Resistor value: ");
    Serial.println(VR_VAL);
    manager.pause(50);
    
    // Measure and package the data
    manager.measure();
    manager.package();   

    // add in resistor value to sd log
    manager.addData("VariableResistor","SetValue",VR_VAL);
    // add EC value to sd log
    float ECatObsT = ads.getAnalog(1)*EC_SLOPE + EC_INTERCEPT;
    manager.addData("ElectricalConductivity","ECatObsT",ECatObsT);
    // Add temperature corrected EC value to sd log
    float ECat25C = ECatObsT /(1+TEMP_COEFF*(sht.getTemperature()-25));
    manager.addData("ElectricalConductivity","ECat25C",ECat25C);
  
    // Log the data to the SD card              
    hypnos.logToSD();

    // Print the current JSON packet
    manager.display_data();

// Set the RTC interrupt alarm to wake the device in 5 seconds
  hypnos.setInterruptDuration(TimeSpan(0, 0, 0, 5));

  // Reattach to the interrupt after we have set the alarm so we can have repeat triggers
  hypnos.reattachRTCInterrupt();
  
  // Put the device into a deep sleep, operation HALTS here until the interrupt is triggered, don't wait for serial
  hypnos.sleep(false);
  //manager.pause(5000);

}
