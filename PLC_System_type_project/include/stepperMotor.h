#ifndef STEPPERMOTOR_H
#include <avr/io.h>
#include <util/delay.h>

void initStepperMotor();

void stepper_step(uint8_t step_index);

void stepper_stop(void);

void stepper_rotate_steps(uint16_t steps, uint8_t dir);


#endif