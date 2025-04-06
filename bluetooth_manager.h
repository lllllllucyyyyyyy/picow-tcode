#include "pico/cyw43_arch.h"

// BT stack headers
#include "btstack.h"
#include "btstack_run_loop.h"
#include "ble/gatt-service/battery_service_server.h"

//#include "gatt.h"

//#include "gatt_management.h"

#include "gatt.h"

static btstack_timer_source_t heartbeat;
static int le_notification_enabled;
static hci_con_handle_t con_handle;

static int val = 5;
static char val_str[30];
static int val_len;

void (*bt_string_get_callback)(uint8_t*, uint8_t);
//Zack Freedman reference?

#define APP_AD_FLAGS 0x06


static void heartbeat_handler(struct btstack_timer_source *ts) {
    static uint32_t counter = 0;
    counter++;

    // Update the temp every 10s
    if (counter % 10 == 0) {
        if (le_notification_enabled) {
            att_server_request_can_send_now_event(con_handle);
        }
    }

    // Invert the led
    // Restart timer
    btstack_run_loop_set_timer(ts, 1000);
    btstack_run_loop_add_timer(ts);
}

static uint8_t adv_data[] = {
    // Flags general discoverable
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, APP_AD_FLAGS,
    // Name
    0x05, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'p', 'i', 'c', 'o',
    // Custom Service UUID
    0x03, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS, 0x10, 0xff,
};
static const uint8_t adv_data_len = sizeof(adv_data);

static void packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    UNUSED(channel);
    UNUSED(size);
    bd_addr_t local_addr;

    if (packet_type != HCI_EVENT_PACKET) return;
    
    switch (hci_event_packet_get_type(packet)) {
        case BTSTACK_EVENT_STATE:
            if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING) return;
            gap_local_bd_addr(local_addr);
            printf("BTstack up and running on %s.\n", bd_addr_to_str(local_addr));

            // setup advertisements
            uint16_t adv_int_min = 800;
            uint16_t adv_int_max = 800;
            uint8_t adv_type = 0;
            bd_addr_t null_addr;
            memset(null_addr, 0, 6);
            gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
            assert(adv_data_len <= 31); // ble limitation
            gap_advertisements_set_data(adv_data_len, (uint8_t*) adv_data);
            gap_advertisements_enable(1);

            break;
        case HCI_EVENT_DISCONNECTION_COMPLETE:
            le_notification_enabled = 0;
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
            break;
        case ATT_EVENT_CAN_SEND_NOW:
            printf("notif sent");
            att_server_notify(con_handle, ATT_CHARACTERISTIC_F0DAC9F1_06B0_4725_A80A_FF083A09A857_01_VALUE_HANDLE, (uint8_t*) val_str, val_len);
            break;
        default:
            break;
    }
}

static uint16_t att_read_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{
    UNUSED(connection_handle);

    if (att_handle == ATT_CHARACTERISTIC_F0DAC9F1_06B0_4725_A80A_FF083A09A857_01_VALUE_HANDLE)
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

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
    switch (att_handle){
        case ATT_CHARACTERISTIC_F0DAC9F1_06B0_4725_A80A_FF083A09A857_01_CLIENT_CONFIGURATION_HANDLE:
            le_notification_enabled = little_endian_read_16(buffer, 0) == GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION;
            con_handle = connection_handle;
            break;
        case ATT_CHARACTERISTIC_F0DAC9F2_06B0_4725_A80A_FF083A09A857_01_VALUE_HANDLE:
            uint8_t* data;
            data = buffer;
            bt_string_get_callback(buffer, buffer_size);
            break;
        default:
            break;
    }
    return 0;
}

static btstack_packet_callback_registration_t hci_event_callback_registration;

static void ble_init(void){

    l2cap_init();

    // setup SM: Display only
    sm_init();

    // setup ATT server
    att_server_init(profile_data, att_read_callback, att_write_callback);    

    // register for HCI events
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // register for ATT event
    att_server_register_packet_handler(packet_handler);

    heartbeat.process = &heartbeat_handler;
    btstack_run_loop_set_timer(&heartbeat, 1000);
    btstack_run_loop_add_timer(&heartbeat);
    hci_power_control(HCI_POWER_ON);
}