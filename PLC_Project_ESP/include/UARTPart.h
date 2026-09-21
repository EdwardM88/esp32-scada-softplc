#ifndef UARTPART_H_
#define UARTPART_H_

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/adc.h"

#define UART_PORT_NUM      UART_NUM_2
#define UART_BAUD_RATE     9600
#define TXD_PIN            GPIO_NUM_17
#define RXD_PIN            GPIO_NUM_16
#define BUF_SIZE           1024

static const char *TAG = "ESP_UART";

void initUART();

void uartSendSignal(uint16_t cmd,uint16_t val);

#endif