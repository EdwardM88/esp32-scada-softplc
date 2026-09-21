#include "header.h"

void initUART()
{
    uart_config_t uart_conf = {};
    uart_conf.baud_rate = UART_BAUD_RATE;
    uart_conf.data_bits =UART_DATA_8_BITS;
    uart_conf.parity = UART_PARITY_DISABLE;
    uart_conf.stop_bits =  UART_STOP_BITS_1;
    uart_conf.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;

    // install UART driver
    ESP_ERROR_CHECK(uart_driver_install(UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, 0));

    // load parameters configuration
    ESP_ERROR_CHECK(uart_param_config(UART_PORT_NUM, &uart_conf));

    // add tx and rx pins
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    ESP_LOGI(TAG, "Successfully initialized TX=17, RX=16 at 9600 baud");

}


void uartSendSignal(uint16_t cmd,uint16_t val)
{   

    //format of data : header,cmd,val,checksum
    uint8_t frame[4];
    frame[0] = PKT_HEADER;
    frame[1] = cmd;
    frame[2] = val;
    frame[3] =  (uint8_t)(frame[0] ^ frame[1] ^ frame[2]); // XOR Checksum

    uart_write_bytes(UART_PORT_NUM, (const char *)frame, 4);
    ESP_LOGI(TAG, "Sent UART: CMD=0x%02X, VAL=%d, CHK=0x%02X", cmd, val, frame[3]);
}

