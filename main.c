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
    while(true) {      
        sleep_ms(1000);
    }
}