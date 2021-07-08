#include <Wire.h>
#include <EnableInterrupt.h>

//Declare Constants & Variables
#define HYPNOS3 5
const byte VR_ADDR = 0x2E;  //I2C address for the AD5246 (variable resistor)
const byte VR_VAL = 63;

#define pin_select 12

int measure = 0;




//Create the function that occurs when the interrupt triggers
void interruptFunction() {
  if (measure == 0)
  {
    Serial.println("\nButton Pressed Test");
    //Loom.measure();
    //Loom.package();
    measure = 1;
  }
}




//General Setup
void setup() {
  //Enable correct power rails from Hypnos
  Serial.begin(9600);
  pinMode(HYPNOS3, OUTPUT);
  digitalWrite(HYPNOS3, LOW);
  Wire.begin();
  

  //Interrupt stuff
  pinMode(pin_select, INPUT);
  enableInterrupt(pin_select, interruptFunction, FALLING); //CHANGE works

  //Send data to AD5246 Variable Resistor

  //LPrintln("\n** SETUP COMPLETE **");

}


void loop() {

  if (measure == 1)
  {
    digitalWrite(HYPNOS3, LOW);
    delay(50);
    //measure = 0;
    delay(50);//was 400

    delay(1000);
    measure = 0;
  }

  digitalWrite(HYPNOS3, HIGH);


}
