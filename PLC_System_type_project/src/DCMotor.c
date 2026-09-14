#include"DCMotor.h"

void init_DCMotor() {
    // configure the motor control pins as output
    DDRH |= (1 << DDH3);

    TCCR4A = (1 << COM4A1) | (1 << WGM41); // non-inverting mode, fast PWM

    TCCR4B = (1 << WGM43) | (1 << WGM42) | (1 << CS41); // prescaler 8, fast PWM mode

    ICR4 = 4000; // set the top value for 8-bit resolution

    OCR4A = 3000;
}

void setMotorSpeed(uint8_t angle) {
    if(angle > 180)
    {
        angle = 180;
    }

    OCR4A = 2000 + ((uint32_t)angle * 2000) / 100;
}