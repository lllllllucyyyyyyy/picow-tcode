#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <tusb.h>

#include "lib/bluetooth/bluetooth_manager.h"
#include "lib/tcode/tcode_processor.h"
//#include "lib/vibecontrol/vibecontrol.h"
#include "lib/servo_controller/servo_controller.h"

char message[15];
unsigned int message_pos = 0;

//this gets called whenever we receive a command, either from serial or bluetooth
//since the format from either is the same, we can use the same processing.
void string_get_callback(uint8_t *buffer, uint8_t length)
{
    struct tcode_command_t command = process_tcode(buffer, length);
    if (command.axis == LINEAR)
    {
        servo_move(command.channel, command.magnitude, command.time);
    }
}

int main()
{
    //set the write callback for bluetooth
    //through c fuckery, the bluetooth library calls this function on a write request

    //initialization stuff
    stdio_init_all();
    //ble_init();
    //vibe_init();

    printf("running");


    while (true)
    {
        //getchar, annoyingly, waits for a char to become available, and thus normally blocks the code.
        //tud cdc only runs when serial data is actually available, preventing blocking
        while (tud_cdc_available())
        {
            char inByte = getchar();
            if (inByte == '\n')
            {
                message[message_pos] = '\0';
                string_get_callback((uint8_t *)message, message_pos);
                message_pos = 0;
            }
            else
            {
                message[message_pos] = inByte;
                message_pos++;
            }
        }
        servo_loop();
    }
}