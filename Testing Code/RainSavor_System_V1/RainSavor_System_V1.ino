 /*
 * OSU OPens Lab RainSavor 
 * Cameron Clonch and Cameron Whitlow Project Leads
 * Under Dr Selker, Dr Udell
 * 
 * This script utilizes OPEnS Loom and integrated sensing, OPEnS Hypnos Board, the Feather M0 Proto uC, and a custom designed printed PCB
 * 
 * The system will log values measuring the EC value of rainwater
 * 
 * 
 * 
 */


 //Loom Setup
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

volatile bool rtc_flag = false;

bool enableDeepSleep = false;


//Interrupt Function
void wakeISR_RTC() {
  //disable the interrupt
  detachInterrupt(12);
  rtc_flag = true;
  LPrintln("\nGot an Interrupt");
}

void setup() {
  //System Startup Voltages
  //Set 3V Rail High - this project doesn't use the 5V Rail so it is not implemented here
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  //Pinmode for RTC and for interrupts
  pinMode(12, INPUT_PULLUP);

  //Setup Loom comms
  Loom.begin_serial(true);
  Loom.parse_config(json_config);
  Loom.print_config();

  //Setup interrupt on the RTC alarm pin
  Loom.InterruptManager().register_ISR(12, wakeISR_RTC, LOW, ISR_Type::IMMEDIATE);

  LPrintln("\n**Setup Complete **");
  Serial.flush();

}

void loop() {
  // put your main code here, to run repeatedly:

}
