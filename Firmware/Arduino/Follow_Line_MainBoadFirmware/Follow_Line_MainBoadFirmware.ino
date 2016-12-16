#include "pins.h"

void setup() {
  /* Motor PWM pins set */
  pinMode(PWM_PIN_1, OUTPUT);
  pinMode(PWM_PIN_2, OUTPUT);
  pinMode(PWM_PIN_3, OUTPUT);
  pinMode(PWM_PIN_4, OUTPUT);
  
  /* Button input pin set */
  pinMode(BUTTON_PIN, INPUT);
  
  /* Sensors Input pins set */  
  pinMode(SENSOR_PIN_FR, INPUT);
  pinMode(SENSOR_PIN_R, INPUT);
  pinMode(SENSOR_PIN_M, INPUT);
  pinMode(SENSOR_PIN_L, INPUT);
  pinMode(SENSOR_PIN_FL, INPUT);
  
  /* Battery level input set */
  pinMode(BATTERY_LEVEL_IN, INPUT);
  
  /* Low battery led pin set */
  pinMode(LOW_BATTERY_LED_PIN, OUTPUT);

}


void loop() {
  

}
