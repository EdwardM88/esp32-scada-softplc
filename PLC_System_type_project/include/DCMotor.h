#ifndef DCMOTOR_H
#include <util/delay.h>
#include <avr/io.h>

void init_DCMotor();

void setMotorSpeed(uint8_t speed);

#endif