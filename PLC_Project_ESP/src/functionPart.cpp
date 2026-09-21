#include "header.h"

volatile bool mega_reported_done;
lineState currentState = STATE_IDLE;

void handleIDLE(void)
{
    // extract data from HC sensor
    uint16_t distance = readHCSensor();
    mb_inputs.dist_hc_sr04 = distance;
    //ESP_LOGI(TAG_HC, "Distanta detectata: %u cm", distance);

    static uint8_t triggerDistance = 0;

    Serial.printf("[IDLE] Distanta senzor: %u cm\n", distance);
    if(distance > 0 && distance < 10)
    {   
        triggerDistance++;
        //if the piece is present,we send command to transport it.
        if(triggerDistance >= 2){
            mb_discrete.piece_detected = 1;
            ESP_LOGI(TAG_HC, "Piece at %u cm! Start transporting...", distance);
            currentState = STATE_TRANSPORT_TO_QC;

            transportProgress = false;
            mega_reported_done = false;
        }

    }else{
        mb_discrete.piece_detected = 0;
        triggerDistance = 0;
    }
}

void handleTransportQC(void)
{
    if(!transportProgress)
    {
        uart_flush_input(UART_PORT_NUM);

        //start movement
        uartSendSignal(CMD_STEPPER_RUN,10);
        transportProgress = true;
        mega_reported_done = false;
        transportStart = xTaskGetTickCount() * portTICK_PERIOD_MS;
        return;
    }

    uint32_t elapsed = (xTaskGetTickCount() * portTICK_PERIOD_MS) - transportStart;

    // we need to read uart port before check commands signals from arduino
    uint8_t megaBuf[16];
    int len = uart_read_bytes(UART_PORT_NUM,megaBuf, sizeof(megaBuf), 0); // non-blocked reading
    if(len > 0)
    {
        for(int i = 0;i <= len;i++)
        {
        //verifi ack packet
            if(megaBuf[i] == CMD_ACK_DONE)
            {
                mega_reported_done = true;
                
            }
        }
    }


    // case A : confirm get at time from mega with stepper movement;
    if(mega_reported_done)
    {
        mega_reported_done = false;
        transportProgress = false;
        gpio_set_level(LED_Pin,0);
        currentState = STATE_QC_INSPECTION;
        Serial.println(">>> [TRANZITIE] Trecem la Inspectia QC...");
        return;
    }

    // case B : Timeout overtaken (block detected)
    if(elapsed > transportTimeOut)
    {
        transportProgress = false;

        // error got on modbus and sensors
        gpio_set_level(LED_Pin,1);
        gpio_set_level(Buzzer_Pin,1);
        uartSendSignal(CMD_EMERGENCY,0);

        mb_coils.sys_run_stop = 0;
        currentState = STATE_EMERGENCY;
        Serial.println("!!! [AVARIE] Timeout! Mega nu a miscat sau nu a trimis ACK in 4 secunde !!!");

    }
}

void handleInspection()
{   
    // little pause between processes
    vTaskDelay(pdMS_TO_TICKS(250));

    // read data from al sensors 
    static float temp = mb_inputs.dht11_temp;
    static float hum  = mb_inputs.dht11_hum;

    static uint16_t ldr = (uint16_t)readPresistor();
    mb_inputs.ldr_intensity = ldr;

    // verify with the mandatory standards
    bool tempOK = (temp <= mb_holding.temp_threshold);
    bool ldrOK = (ldr <= mb_holding.ldr_threshold);
    bool humOK = (hum <= mb_holding.hum_threshold);

    if(tempOK && ldrOK && humOK)
    {   
        // piece is ok and goes to sorting process
        mb_discrete.qc_status = 1;
        ESP_LOGI("QC_STAGE","Piece OK! LDR: %d (Max : %d), TEMP: %d C (Max : %d C), HUM: %d % (Max: %d %)",ldr,mb_holding.ldr_threshold,temp,mb_holding.temp_threshold,hum,mb_holding.hum_threshold);
    }else
    {   
        // piese is not ok and goes to sorting process
        mb_discrete.qc_status = 0;
        ESP_LOGI("QC_STAGE","Rejected! LDR: %d (Max : %d), TEMP: %d C (Max : %d C), HUM: %d % (Max: %d %)",ldr,mb_holding.ldr_threshold,temp,mb_holding.temp_threshold,hum,mb_holding.hum_threshold
        (tempOK ? "[Temp not ok]" : ""),
        (humOK ? "[Temperatur not ok]" : "")
        (ldrOK ? "[Light parameters are not ok]"));   
    }

    currentState = STATE_TRANSPORT_TO_SORT;
}

void handleSort()
{
    if(mb_discrete.qc_status == 1)
    {   
        //if the piece is ok,it goes to the next stage
        uartSendSignal(CMD_SERVO_ANGLE,0);
        mb_inputs.total_ok_pieces++;
        ESP_LOGI("SORT","Piece OK delivered. Total pieces : %d ",mb_inputs.total_ok_pieces);
        vTaskDelay(500);
    }
    else
    {   
        //if the piece is not ok,it goes to trash

        uartSendSignal(CMD_SERVO_ANGLE,90);
        vTaskDelay(pdMS_TO_TICKS(800));//time to execute

        uartSendSignal(CMD_SERVO_ANGLE,0);
        vTaskDelay(pdMS_TO_TICKS(300));

        mb_inputs.total_rejects++;
        ESP_LOGI("SORT","Piece rejected . Total pieces : %d ",mb_inputs.total_rejects);

    }

    //reset to the initial state
    mb_discrete.piece_detected = 0;
    currentState = STATE_IDLE;
}