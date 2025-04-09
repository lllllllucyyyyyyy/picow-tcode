#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

//the zero value is the minimum value needed to spin up the motor.
static const int zero = 40;
uint slice_num;

void vibe_init()
{
    //initialize our 2 pwm channels
    gpio_set_function(2, GPIO_FUNC_PWM);
    gpio_set_function(3, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(2);
    pwm_set_wrap(slice_num, 100);
    pwm_set_enabled(slice_num, true);
}

void vibe_command(int channel, float input)
{
    int mag = (100 - zero) * input;
    if (input == 0)
    {
        mag = 0;
    }
    switch (channel)
    {
    case 0:
        pwm_set_chan_level(slice_num, PWM_CHAN_A, mag);
        break;
    case 1:
        pwm_set_chan_level(slice_num, PWM_CHAN_B, mag);
        break;
    }
}