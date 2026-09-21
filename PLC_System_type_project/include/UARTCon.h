#ifndef UARTCON_H_
#define UARTCON_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>

// binar protocols
#define PKT_HEADER        0xAA
#define CMD_STEPPER_RUN   0x01
#define CMD_STEPPER_STOP  0x02
#define CMD_SERVO_ANGLE   0x03
#define CMD_EMERGENCY     0x05
#define CMD_ACK_DONE      0x06

// steps configuration
#define STEPS_PER_UNIT    100

void initUART(void);
uint8_t readUART(void);
void writeUART(uint8_t data);
bool availableUART(void);
void sendACKDone(void);

#endif