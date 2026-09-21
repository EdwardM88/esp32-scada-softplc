#include"Outputs.h"

void initBuzzer()
{
    gpio_reset_pin((gpio_num_t)Buzzer_Pin);

    gpio_config_t buzzer_conf{};
    buzzer_conf.pin_bit_mask = (1ULL << Buzzer_Pin); //mask on 23 pin
    buzzer_conf.mode = GPIO_MODE_OUTPUT;
    buzzer_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    buzzer_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    buzzer_conf.intr_type = GPIO_INTR_DISABLE;

    gpio_config(&buzzer_conf);
    gpio_set_level(Buzzer_Pin,0);
}

void setBuzzer(bool state)
{
    gpio_set_level(Buzzer_Pin,state ? 1: 0);
}

void initLED()
{
    gpio_config_t LED_conf{};
    LED_conf.pin_bit_mask = (1ULL << Buzzer_Pin); //mask on 4 pin
    LED_conf.mode = GPIO_MODE_OUTPUT;
    LED_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    LED_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    LED_conf.intr_type = GPIO_INTR_DISABLE;

    gpio_config(&LED_conf);
    gpio_set_level(LED_Pin,0);
}

void LEDState(bool state)
{
    gpio_set_level(LED_Pin,state ? 1 : 0);
}