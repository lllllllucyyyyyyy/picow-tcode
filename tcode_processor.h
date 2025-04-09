#ifndef tcode_h
#define tcode_h

#include "pico/stdlib.h"

enum axis_t {
    LINEAR,
    ROTATION,
    VIBRATION,
    AUXILLARY
};

enum time_command_t {
    MAGNITUDE_TIME,
    MAGNITUDE_SPEED
};

struct tcode_command_t {
    enum axis_t axis;
    int channel;
    float magnitude;
    enum time_command_t time_command;
    int time;
};


struct tcode_command_t process_tcode(uint8_t* input, uint16_t input_length);

#endif