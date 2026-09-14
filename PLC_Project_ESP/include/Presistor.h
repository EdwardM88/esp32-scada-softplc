#ifndef Presistor_H
#define Presistor_H

#include <stdint.h>
#include "driver/adc.h"

#define PRESISTOR_ADC_CHANNEL ADC1_CHANNEL_4 // GPIO 34

void initPresistor();
uint16_t readPresistor(void);
uint16_t readPresistorTask(void);

#endif // Presistor_H