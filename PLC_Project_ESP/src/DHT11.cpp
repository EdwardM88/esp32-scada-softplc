#include "DHT11.h"

static portMUX_TYPE dhtMux = portMUX_INITIALIZER_UNLOCKED;

static int dht11LowLeve(gpio_num_t pin,int level,int timeout)
{
    uint64_t start = esp_timer_get_time();
    while(gpio_get_level(pin) == level)
    {
        if((uint32_t)(esp_timer_get_time() - start) > timeout)
        {
            return -1;
        }
    }
    return 0;
}

bool readDHT11(float *temperature, float *humidity) {
    uint8_t data[5] = {0, 0, 0, 0, 0};

    //set pin to output mode and send start signal
    gpio_set_direction(DHT11_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT11_PIN, 0); // Trage linia in LOW
    delay(20);                  // delay for at least 18ms to ensure DHT11 detects the start signal

    portENTER_CRITICAL(&dhtMux);//critical zone for rtos 

    //change pin to input to read data from DHT11
    gpio_set_direction(DHT11_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(DHT11_PIN, GPIO_PULLUP_ONLY);

    // wait for the sensor to pull the line LOW (response signal)
    if (dht11LowLeve(DHT11_PIN, 1, 100) < 0) {
        Serial.println("[EROARE] Senzorul nu a raspuns (linia a ramas in HIGH). Lipsa conectare/alimentare?");
        return false;
    }

    // wait for the sensor to pull the line LOW (80us)
    if (dht11LowLeve(DHT11_PIN, 0, 100) < 0) {
        Serial.println("[EROARE] Timeout la pulsul LOW de raspuns al senzorului.");
        return false;
    }

    // wait for the HIGH pulse that indicates the start of data transmission (80us)
    if (dht11LowLeve(DHT11_PIN, 1, 100) < 0) {
        Serial.println("[EROARE] Timeout la pulsul HIGH de raspuns al senzorului.");
        return false;
    }

    // read 40 bits of data (5 bytes)
    for (int i = 0; i < 40; i++) {
        if (dht11LowLeve(DHT11_PIN, 0, 150) < 0) {
            portEXIT_CRITICAL(&dhtMux); // unlock port before exit
            Serial.printf("[EROARE] Timeout LOW la bitul %d\n", i);
            return false;
        }

        uint32_t start_high = esp_timer_get_time();
        if (dht11LowLeve(DHT11_PIN, 1, 150) < 0) {
            portEXIT_CRITICAL(&dhtMux);
            Serial.printf("[EROARE] Timeout HIGH la bitul %d\n", i);
            return false;
        }

        uint32_t high_duration = esp_timer_get_time() - start_high;

        
        if (high_duration > 40) {
            data[i / 8] |= (1 << (7 - (i % 8)));
        }
    }

    portEXIT_CRITICAL(&dhtMux);

    // 5. check checksum
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4]) {
        Serial.printf("[EROARE] Checksum gresit! Calculat: 0x%02X | Primit: 0x%02X\n", checksum, data[4]);
        Serial.printf("Raw Bytes: [0]=%d [1]=%d [2]=%d [3]=%d [4]=%d\n", data[0], data[1], data[2], data[3], data[4]);
        return false;
    }

    *humidity = (float)data[0] + (float)data[1] * 0.1f;
    *temperature = (float)data[2] + (float)data[3] * 0.1f;

    return true;
}
