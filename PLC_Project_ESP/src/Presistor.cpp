#include "Presistor.h"
#include <Arduino.h>

void initPresistor() {
    adc1_config_width(ADC_WIDTH_BIT_12); // Set ADC width to 12 bits

    adc1_config_channel_atten(PRESISTOR_ADC_CHANNEL, ADC_ATTEN_DB_12); // Set attenuation to 11 dB
}

uint16_t readPresistor(void)
{
    return (uint16_t)adc1_get_raw(PRESISTOR_ADC_CHANNEL);
}

uint16_t readPresistorTask(void)
{
    int raw = readPresistor();
    uint16_t percentage = (uint16_t)(raw * 100.0 / 4095.0); // Convert to percentage (0-100)

    //filter the raw value to reduce noise
    if(percentage <0) percentage = 0;
    if(percentage > 100) percentage = 100;

    return percentage;
}

