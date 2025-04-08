#include <stdio.h>
#include "pico/stdlib.h"
#include <tusb.h>

#include "bluetooth_manager.h"
#include "tcode_processor.h"

#include "servo.h"

bool direction = true;
int pos = 1500;
int servoPin0 = 2;
int servoPin1 = 3;

int old_timestamp;
int target_time;
int old_pos;
int target_pos;
bool movecommand = false;

int zero = 1250;
int range = 500;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main()
{
    stdio_init_all();

    /*if (cyw43_arch_init())
    {
        printf("failed to initialise cyw43_arch\n");
        return -1;
    }else{
        printf("initialization successful");
    }

    ble_init();

    // turn on!

    // Run our Bluetooth app
    printf("running");
    //btstack_run_loop_execute();*/

    void route_data(uint8_t *buffer, uint8_t length)
    {
        struct tcode_command_t command = process_tcode(buffer, length);
        /*printf("ran callback \n");
        printf("axis %d \n", command.axis);
        printf("channel %d \n", command.channel);
        printf("magnitude %f \n", command.magnitude);
        printf("time command %d \n", command.time_command);
        printf("time %d \n", command.time);*/

        if (command.axis == 0)
        {
            if (command.channel == 0)
            {
                old_pos = pos;
                target_pos = (command.magnitude * range) + zero;
                old_timestamp = (get_absolute_time() / 1000);
                target_time = command.time;
                movecommand = true;
            }
        }
    }
    // bt_string_get_callback = *route_data;

    setServo(servoPin0, zero);
    setServo(servoPin1, zero);

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

        if (movecommand)
        {
            int deltatime = (get_absolute_time() / 1000) - old_timestamp;
            if (deltatime >= target_time)
            {
                movecommand = false;
                pos = target_pos;
            }
            else
            {
                pos = map(deltatime, 0, target_time, old_pos, target_pos);
            }
            setMillis(servoPin0, pos);
            setMillis(servoPin1, pos);
        }
    }
}