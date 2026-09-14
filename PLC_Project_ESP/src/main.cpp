#include "DHT11.h"
#include "HCSensor.h"
#include "Presistor.h"
#include "WiFiPart.h"
#include "Outputs.h"
#include "ModbusPart.h"
#include <Arduino.h>

TaskHandle_t Task1Handle;

// function to test all sensors and outputs(it use core 0 on ESP32)
void sensors(void *parameter)
{
    for(;;)
    {
       static float temp = 0.0f;
       static float hum = 0.0f;

        float distance = readHCSensor();
        if(distance < 0) {
            Serial.println(">> EROARE! Citire de la HCSensor esuata.");
        } else {
            Serial.printf(">> SUCCES! Distanta: %.2f cm\n", distance);
        }

        Serial.println("\nIncerc citire de la DHT11...");
        if (readDHT11(&temp, &hum)) {
            Serial.printf(">> SUCCES! Temperatura: %.1f C | Umiditate: %.1f %%\n", temp, hum);
        }

        uint16_t raw = readPresistor();
        uint16_t percent = readPresistorTask();
        Serial.printf("Raw ADC Value %d %d: \n", raw, percent);

        updateModbusRegisters(temp,hum,distance,raw);


        //conditions for LED and buzzer
        if(raw < 2000 && temp > 25)
        {
            setBuzzer(true);
            LEDState(true);
        }else{
            setBuzzer(false);
            LEDState(false);            
        }

        vTaskDelay(2000/portTICK_PERIOD_MS);    
            
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    initHCSensor();
    initPresistor();
    initBuzzer();
    initLED();

    initWiFi();
    initModbusPart();
    
    xTaskCreatePinnedToCore(
        sensors,
        "Task1Handle",
        4096,
        NULL,
        1,
        &Task1Handle,
        0   //fixed on core 0
    );

}

void loop()
{
    wifiConnect();
    handleModbusRequest();

    vTaskDelay(10 / portTICK_PERIOD_MS); // Delay for 1 second before checking again
}