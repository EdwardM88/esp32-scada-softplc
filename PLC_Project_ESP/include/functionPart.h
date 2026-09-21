#ifndef FUNCTIONPART_H_
#define FUNCTIONPART_H_

#include<Arduino.h>
#include<driver/gpio.h>

static uint32_t transportStart = 0;
static uint32_t transportTimeOut = 3000;
static bool transportProgress = false;

void handleIDLE(void);
void handleTransportQC(void);
void handleInspection();
void handleSort();

#endif