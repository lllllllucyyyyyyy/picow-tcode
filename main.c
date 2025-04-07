#include <stdio.h>
#include "pico/stdlib.h"

#include "bluetooth_manager.h"
#include "tcode_processor.h"

#include "hardware/pwm.h"

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

    uint slice_num = pwm_gpio_to_slice_num(2);
    pwm_set_wrap(slice_num, 100);

    ble_init();

    printf("running");

    void route_data(uint8_t *buffer, uint8_t length)
    {
        struct tcode_command_t command = process_tcode(buffer, length);
        printf("ran callback \n");
        printf("axis %d \n", command.axis);
        printf("channel %d \n", command.channel);
        printf("magnitude %f \n", command.magnitude);
        printf("time command %d \n", command.time_command);
        printf("time %ds \n", command.time);

        if (command.axis == VIBRATION)
        {
            switch (command.channel)
            {
            case 0:
                pwm_set_chan_level(slice_num, PWM_CHAN_A, command.magnitude * 100);
                break;
            case 1:
                pwm_set_chan_level(slice_num, PWM_CHAN_B, command.magnitude * 100);
                break;
            }
        }
    }
    bt_string_get_callback = *route_data;

    while (true)
    {
        sleep_ms(1000);
    }
}