#include "HCSensor.h"

void initHCSensor() {
    //set trigger pin as output and echo pin as input
    gpio_config_t trigConfig;
    trigConfig.pin_bit_mask = (1ULL << TrigPin);
    trigConfig.mode = GPIO_MODE_OUTPUT;
    trigConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    trigConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
    trigConfig.intr_type = GPIO_INTR_DISABLE;

    gpio_config(&trigConfig);
    gpio_set_level(TrigPin, 0); // Ensure the trigger pin is low initially

    gpio_config_t echoConfig;
    echoConfig.pin_bit_mask = (1ULL << EchoPin);
    echoConfig.mode = GPIO_MODE_INPUT;
    echoConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    echoConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
    echoConfig.intr_type = GPIO_INTR_DISABLE;

    gpio_config(&echoConfig);

}

float readHCSensor() {
    // Send a 10us pulse to trigger the sensor
    gpio_set_level(TrigPin, 0);
    ets_delay_us(2); // Wait for 2 microseconds
    gpio_set_level(TrigPin, 1);
    ets_delay_us(10); // Wait for 10 microseconds
    gpio_set_level(TrigPin, 0);

    // Wait for the echo pin to go high and measure the duration
    int32_t startTime = esp_timer_get_time();
    while (gpio_get_level(EchoPin) == 0) {
        if ((esp_timer_get_time() - startTime) > 50000) { // Timeout after 1 second
            return -1.0f; // Indicate an error
        }
    }

    // Measure the duration of the echo pulse
    int32_t echoStartTime = esp_timer_get_time();
    while (gpio_get_level(EchoPin) == 1) {
        if ((esp_timer_get_time() - echoStartTime) > 25000) { // Timeout after 1 second
            return -1.0f; // Indicate an error
        }
    }
    int64_t echoEndTime = esp_timer_get_time();

    int64_t duration = echoEndTime - echoStartTime;
    return (float)duration * 0.0343f / 2.0f; // Calculate distance in cm
}