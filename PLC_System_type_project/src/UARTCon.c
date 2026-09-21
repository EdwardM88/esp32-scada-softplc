#include "UARTCon.h"

 void initUART()
{
    // UBRR = (16000000 / (16 * 9600)) - 1 = 103
    uint16_t ubrr = 103;
    UBRR1H = (uint8_t)(ubrr >> 8);
    UBRR1L = (uint8_t)(ubrr);

    // activate transmitter and receiver on UART
    UCSR1B = (1 << RXEN1) | (1 << TXEN1);

    // format: b bites of data ,one bit for stop
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

 uint8_t readUART(void)
{   
    // wait to get a byte
    while(!(UCSR1A & (1 << RXC1)));
    return UDR1;
}

 void writeUART(uint8_t data)
{
    while(!(UCSR1A & (1 << UDRE1)));
    UDR1 = data;
}

 bool availableUART(void)
{   
    // return true if are non-read data in register (RXC1 = 1)
    return (UCSR1A & (1 << RXC1));
}

void sendACKDone(void)
{
    uint8_t frame[4];
    frame[0] = PKT_HEADER;
    frame[1] = CMD_ACK_DONE;
    frame[2] = 0x01;
    frame[3]  = (uint8_t)(frame[0] ^ frame[1] ^ frame[2]); // XOR Checksum

    for (uint8_t i = 0;i < 4;i++)
    {
        writeUART(frame[i]);
    }
}

