#include"stepperMotor.h"

//variables for asyncron control
static volatile uint16_t stepper_steps_left = 0;
static volatile uint8_t  stepper_direction = 1;
volatile bool stepper_is_busy = false;
volatile bool stepper_motion_done = false;

void initStepperMotor(void) {
    //set port 22,23 24 & w5 as output
    DDRA = (1 << DDA0) | (1 << DDA1) |  (1 << DDA2) | (1 << DDA3);
    PORTA &= 0xF0; //coils interrupted

    // configure Timer 1 in CTC Mode (Mode 4) with 64 prescaler
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // CTC + Prescaler 64
    OCR1A  = 999;                                       //  4.0 ms
    TCNT1  = 0;

    TIMSK1 &= ~(1 << OCIE1A);

    // global activation for interrups
    sei();

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
    TIMSK1 &= ~(1 << OCIE1A); // stop all interruptions for Timer1
    PORTA &= 0xF0; // stop all ports(22 23 24 25)
    stepper_is_busy = false;
}


void stepper_rotate_steps(uint16_t steps, uint8_t dir) {
   if(steps == 0) return;

    stepper_direction = dir;
    stepper_steps_left = steps;
    stepper_is_busy = true;
    stepper_motion_done = false;

    TCNT1 = 0;
    TIMSK1 |= (1 << OCIE1A); // activate interruption 

}

// interruption routine
ISR(TIMER1_COMPA_vect)
{
    static uint8_t stepP = 0;

    if(stepper_steps_left > 0)
    {
        if(stepper_direction )
        {
            stepP = (stepP + 1) % 4;
        }else
        {
            stepP = (stepP - 1 + 4) % 4;
        }

        stepper_step(stepP);
        stepper_steps_left--;

         // verify if the process is done
        if(stepper_steps_left == 0)
        {
            TIMSK1 &= ~(1 << OCIE1A); // stop interruption
            PORTA &= 0xF0; // stop coils
            stepper_is_busy = false;
            stepper_motion_done =  true;
        }
    }

   
}