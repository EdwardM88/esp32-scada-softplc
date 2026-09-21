#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>

extern volatile bool stepper_is_busy;
extern volatile bool stepper_motion_done;

void initStepperMotor();

void stepper_step(uint8_t step_index);

void stepper_stop(void);

void stepper_rotate_steps(uint16_t steps, uint8_t dir);


#endif