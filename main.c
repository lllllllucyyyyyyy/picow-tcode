#include <stdio.h>
#include "pico/stdlib.h"

#include "bluetooth_manager.h"
#include "tcode_processor.h"

// Pico headers

int main()
{
    stdio_init_all();
    
    if (cyw43_arch_init())
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
    //btstack_run_loop_execute();

    void route_data(uint8_t* buffer, uint8_t length) {
        struct tcode_command_t command = process_tcode(buffer, length);
        printf("ran callback \n");
        printf("axis %d \n", command.axis);
        printf("channel %d \n", command.channel);
        printf("magnitude %f \n", command.magnitude);
        printf("time command %d \n", command.time_command);
        printf("time %d \n", command.time);
    }
    bt_string_get_callback = *route_data;

    while(true) {      
        sleep_ms(1000);
    }
}