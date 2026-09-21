#include "header.h"

TaskHandle_t Task1Handle;

static const char *TAG_SENSORS = "SENSORS_TASK";

// function to test all sensors and outputs(it use core 0 on ESP32)
void sensors(void *pvParameters)
{
    for (;;) 
    {
        // read data from all sensors
        uint16_t rldr = (uint16_t)readPresistor();
        float temp = 0.0f;
        float hum = 0.0f;

        bool dhtOK = readDHT11(&temp,&hum);
        
        mb_inputs.ldr_intensity = rldr;
        if(dhtOK)
        {
            mb_inputs.dht11_hum = (uint16_t)hum;
            mb_inputs.dht11_temp = (uint16_t)temp;

            ESP_LOGD(TAG_SENSORS,"LDR %u | Temp: %d C | Hum: %u %%",
                    rldr,(int)temp,(unsigned int)hum);

            updateModbusRegisters(temp, hum, mb_inputs.dist_hc_sr04, rldr);
        }else
        {
            ESP_LOGW(TAG_SENSORS, "Error on reading from DHT11");
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }

    vTaskDelete(NULL);
}


void setup()
{
    Serial.begin(115200);
    delay(500);

    // init al sensors,Led and buzzer
    Serial.println("Initialize all peripheral!");
    initHCSensor();
    initPresistor();
    initBuzzer();
    initLED();

    // init uart part
    Serial.println("Initialize UART part");
    initUART();

    Serial.println("Initialize wifi and modbus");
    initWiFi();
    initModbusPart();
    
    Serial.println("Create Task 0");
    xTaskCreatePinnedToCore(
        sensors,
        "Task1Handle",
        4096,
        NULL,
        1,
        &Task1Handle,
        0   //fixed on core 0
    );

    mb_coils.sys_run_stop = 1;
    mb_holding.operation_mode = 1;

}

void loop()
{
    wifiConnect();
    handleModbusRequest();

    // verify emergency stop
    if(!mb_coils.sys_run_stop)
    {
        uartSendSignal(CMD_EMERGENCY,0);
        vTaskDelay(pdMS_TO_TICKS(100));
        return;
    }

    // verify modbus manual vs auto
    if(mb_holding.operation_mode == 0)
    {
        uartSendSignal(CMD_SERVO_ANGLE,(uint8_t)mb_holding.manual_servo_angle);
        vTaskDelay(pdMS_TO_TICKS(200));
        return;
    }

    switch (currentState)
    {
    case STATE_IDLE:
        handleIDLE();
        break;
    case STATE_TRANSPORT_TO_QC:
        handleTransportQC();
        break;
    case STATE_QC_INSPECTION:
        handleInspection();
        break;
    case STATE_TRANSPORT_TO_SORT:
        uartSendSignal(CMD_STEPPER_RUN,20);
        vTaskDelay(pdMS_TO_TICKS(4500));
        currentState = STATE_SORT;
        break;
    case STATE_SORT:
        handleSort();
        break;
    case STATE_EMERGENCY:
        uartSendSignal(CMD_EMERGENCY,0);
        break;
    default:
        break;
    }

    vTaskDelay(10 / portTICK_PERIOD_MS); // Delay for 1 second before checking again
}