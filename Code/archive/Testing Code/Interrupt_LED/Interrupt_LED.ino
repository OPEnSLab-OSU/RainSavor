#include <EnableInterrupt.h>

#define int_pin 7
#define LED_pin 13
#define initial_LED LOW

int LED_state = initial_LED;

void interruptFunction() {
  //LED_state = !LED_state;
}

void setup() {
  
  pinMode(int_pin, INPUT_PULLUP);
  enableInterrupt(int_pin, interruptFunction, CHANGE);
  //CHANGE, RISING, FALLING, LOW

  pinMode(LED_pin, OUTPUT);
  digitalWrite(LED_pin, initial_LED);
}

void loop() {
  digitalWrite(LED_pin, LED_state);

}
