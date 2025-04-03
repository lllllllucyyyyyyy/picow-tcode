#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pico headers
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

// BT stack headers
#include "btstack.h"
#include "btstack_run_loop.h"
#include "ble/gatt-service/battery_service_server.h"

//#include "gatt.h"

//#include "gatt_management.h"

#include "gatt.h"

static int le_notification_enabled;
static hci_con_handle_t con_handle;

static int val = 5;
static char val_str[30];
static int val_len;

#define APP_AD_FLAGS 0x06

static uint8_t adv_data[] = {
    // Flags general discoverable
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, APP_AD_FLAGS,
    // Name
    0x05, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'L', 'V', 'S', '-',
    // Custom Service UUID
    0x03, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS, 0x10, 0xff,
};
static const uint8_t adv_data_len = sizeof(adv_data);

static void packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    UNUSED(channel);
    UNUSED(size);

    if (packet_type != HCI_EVENT_PACKET) return;
    
    switch (hci_event_packet_get_type(packet)) {
        case HCI_EVENT_DISCONNECTION_COMPLETE:
            le_notification_enabled = 0;
            break;
        case ATT_EVENT_CAN_SEND_NOW:
            printf("notif sent");
            att_server_notify(con_handle, ATT_CHARACTERISTIC_50300003_0023_4bd4_bbd5_a6920e4c5653_01_VALUE_HANDLE, (uint8_t*) val_str, val_len);
            break;
        default:
            break;
    }
}

static uint16_t att_read_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{
    UNUSED(connection_handle);

    if (att_handle == ATT_CHARACTERISTIC_50300003_0023_4bd4_bbd5_a6920e4c5653_01_VALUE_HANDLE)
    {
        val_len = snprintf(val_str, sizeof(val_str), "BTstack counter %04u", val);
        printf("sent packet");
        return att_read_callback_handle_blob((const uint8_t *)val, val_len, offset, buffer, buffer_size);
    }
    return 0;
}

static int att_write_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{
    UNUSED(transaction_mode);
    UNUSED(offset);
    UNUSED(buffer_size);

    // Enable/disable notifications
    if (att_handle == ATT_CHARACTERISTIC_50300003_0023_4bd4_bbd5_a6920e4c5653_01_CLIENT_CONFIGURATION_HANDLE)
    {
        le_notification_enabled = little_endian_read_16(buffer, 0);
        con_handle = connection_handle; 
    }

    if (att_handle == ATT_CHARACTERISTIC_50300002_0023_4bd4_bbd5_a6920e4c5653_01_VALUE_HANDLE)
    {
        printf_hexdump(buffer, buffer_size);
    }
    return 0;
}

static btstack_packet_callback_registration_t hci_event_callback_registration;

static void le_counter_setup(void){

    l2cap_init();

    // setup SM: Display only
    sm_init();

    // setup ATT server
    att_server_init(profile_data, att_read_callback, att_write_callback);    

    // setup advertisements
    uint16_t adv_int_min = 0x0030;
    uint16_t adv_int_max = 0x0030;
    uint8_t adv_type = 0;
    bd_addr_t null_addr;
    memset(null_addr, 0, 6);
    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
    gap_advertisements_set_data(adv_data_len, (uint8_t*) adv_data);
    gap_advertisements_enable(1);

    // register for HCI events
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // register for ATT event
    att_server_register_packet_handler(packet_handler);
}

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
    
    le_counter_setup();

    // turn on!
	hci_power_control(HCI_POWER_ON);
	    
    // Run our Bluetooth app
    btstack_run_loop_execute();
    printf("running");
}