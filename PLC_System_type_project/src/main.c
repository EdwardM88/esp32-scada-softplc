#include "header.h"

int main() {
    init_DCMotor(); // initialize the DC motor
    initStepperMotor(); // initialize the stepper motor
    initUART();

    DDRB |= (1 << DDB7);
    PORTB &= ~(1 << PB7); // LED-ul "L" se stinge garantat aici

    sei();

    while (1) {
        if(availableUART())
        {
            uint8_t header = readUART();
            if(header == PKT_HEADER)
            {
                uint8_t cmd = readUART();
                uint8_t val = readUART();
                uint8_t chk = readUART();

                if(chk == (uint8_t)(PKT_HEADER ^ cmd ^ val))
                {
                    PORTB |= (1 << PB7); // <-- APRINDE LED-ul "L": Pachetul este 100% valid!
                    switch (cmd)
                    {
                    case CMD_STEPPER_RUN:
                        PORTB ^= (1 << PB7); // Toggle LED 13
                        stepper_rotate_steps((uint16_t)val * 50,1);
                        break;
                    case CMD_SERVO_ANGLE:
                        setMotorSpeed(val);
                        break;
                    case CMD_STEPPER_STOP:
                    case CMD_EMERGENCY:
                        stepper_stop();
                        break;
                    default:
                        break;
                    }
                }
            }
            
        }

        if(stepper_motion_done)
        {
            stepper_motion_done = false;
            sendACKDone(); 
        }
    }

   

    return 0;
}