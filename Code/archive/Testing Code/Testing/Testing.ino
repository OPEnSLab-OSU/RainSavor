
//select the pin that triggers the interrupt
#define pin_select 12

int measure = 0;

//Create the function that occurs when the interrupt triggers
void interruptFunction() {
  if (measure == 0)
  {
    Serial.print("\nButton Pressed");
    //Loom.measure();
    //Loom.package();
    measure = 1;
  }
}

void setup() {
  //Begin serial communication as usual
  Serial.begin(9600);

  //Setup the chosen pin as an input with the pullup resistor enabled (this can also be done with register allocations)
  pinMode(pin_select, INPUT);
  //Connect the pin to the interrupt function with what causes the interrupt
  attachInterrupt(digitalPinToInterrupt(pin_select), interruptFunction, CHANGE);
  //can use either CHANGE, RISING, FALLING, LOW - determined by hardware

}

void loop() {
  delay(100);
  if (measure == 1)
  {
    measure = 0;
    
  }
  //Serial.println(digitalRead(pin_select));
}
