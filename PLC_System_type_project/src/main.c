#include "DCMotor.h"
#include "stepperMotor.h"

int main() {
    init_DCMotor(); // initialize the DC motor
    initStepperMotor(); // initialize the stepper motor

    while (1) {
        // make 2048 steps in one direction
        stepper_rotate_steps(2048, 1);
        _delay_ms(1000);

        // make 2048 steps in opposite direction
        stepper_rotate_steps(2048, 0);
        _delay_ms(1000);

        setMotorSpeed(180);
        _delay_ms(100);

        setMotorSpeed(90);
        _delay_ms(100);
    }

   

    return 0;
}