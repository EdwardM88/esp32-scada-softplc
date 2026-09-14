#ifndef HCSENSOR_H
#define HCSENSOR_H

#include <stdbool.h>
#include "driver/gpio.h"
#include <Arduino.h>

#define TrigPin  GPIO_NUM_5     // port 5 for trig
#define EchoPin  GPIO_NUM_19    //port 19 for echo

void initHCSensor();
float readHCSensor();

#endif // HCSENSOR_H