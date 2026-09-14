#ifndef DHT11_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <rom/ets_sys.h>
#include <esp_timer.h>
#include <Arduino.h>

#define DHT11_PIN GPIO_NUM_18  //port 18 on ESP32

static int dht11LowLeve(gpio_num_t pin,int level,int timeout);

bool readDHT11(float *temperature, float *humidity);


#endif // DHT11_H