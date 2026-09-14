#include"stepperMotor.h"

void initStepperMotor(void) {
    //set port 8 & 9 as output
    DDRA = (1 << DDA0) | (1 << DDA1) |  (1 << DDA2) | (1 << DDA3);
}

void stepper_step(uint8_t step_index) {
    uint8_t current_porta = PORTA & 0xF0;

    switch (step_index % 4) {
        case 0:
            // coil A + B (Pin 22 + Pin 23)
            PORTA = current_porta | (1 << PA0) | (1 << PA1);
            break;
        case 1:
            // coil B + C (Pin 23 + Pin 24)
            PORTA = current_porta | (1 << PA1) | (1 << PA2);
            break;
        case 2:
            // coil C + D (Pin 24 + Pin 25)
            PORTA = current_porta | (1 << PA2) | (1 << PA3);
            break;
        case 3:
            // coil D + A (Pin 25 + Pin 22)
            PORTA = current_porta | (1 << PA3) | (1 << PA0);
            break;
    }
}

void stepper_stop(void) {
    PORTA &= 0xF0; // Oprește toate LED-urile (Pin 22, 23, 24, 25)
}


void stepper_rotate_steps(uint16_t steps, uint8_t dir) {
    static int8_t step_pos = 0;

    for (uint16_t i = 0; i < steps; i++) {
        if (dir) {
            step_pos = (step_pos + 1) % 4;
        } else {
            step_pos = (step_pos - 1 + 4) % 4;
        }

        stepper_step(step_pos);
        _delay_ms(4); // 4ms oferă cuplu bun și viteză stabilă
    }

    stepper_stop(); // Stingem bobinele după terminarea mișcării
}
