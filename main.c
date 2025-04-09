#include <stdio.h>
#include "pico/stdlib.h"

#include "bluetooth_manager.h"
#include "tcode_processor.h"

#include "hardware/pwm.h"

void route_data(uint8_t *buffer, uint8_t length)
{
    if (command.axis == VIBRATION)
    {
        if (command.magnitude == 0)
        {
            switch (command.channel)
            {
            case 0:
                pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);
                break;
            case 1:
                pwm_set_chan_level(slice_num, PWM_CHAN_B, 0);
                break;
            }
        }
        else
        {
            int vel = (command.magnitude * 60) + 40;
            switch (command.channel)
            {
            case 0:
                pwm_set_chan_level(slice_num, PWM_CHAN_A, vel;
                break;
            case 1:
                pwm_set_chan_level(slice_num, PWM_CHAN_B, vel);
                break;
            }
        }
    }
}

int main()
{
    stdio_init_all();

    if (cyw43_arch_init())
    {
        printf("failed to initialise cyw43_arch\n");
        return -1;
    }
    else
    {
        printf("initialization successful");
    }

    gpio_set_function(2, GPIO_FUNC_PWM);
    gpio_set_function(3, GPIO_FUNC_PWM);
    pwm_config config = pwm_get_default_config();

    uint slice_num = pwm_gpio_to_slice_num(2);
    pwm_set_wrap(slice_num, 100);
    pwm_set_enabled(slice_num, true);

    ble_init();

    printf("running");

    bt_string_get_callback = *route_data;

    static char message[15];
    static unsigned int message_pos = 0;

    while (true)
    {
        while (tud_cdc_available())
        {
            char inByte = getchar();
            if (inByte == '\n')
            {
                message[message_pos] = '\0';
                route_data((uint8_t *)message, message_pos);
                message_pos = 0;
            }
            else
            {
                message[message_pos] = inByte;
                message_pos++;
            }
        }
    }
}