#ifndef Outputs_h_
#define Outputs_h_

#include<Arduino.h>
#include<driver/gpio.h>

#define Buzzer_Pin GPIO_NUM_25
#define LED_Pin GPIO_NUM_4

void initBuzzer();
void setBuzzer(bool state);

void initLED();
void LEDState(bool state);


#endif