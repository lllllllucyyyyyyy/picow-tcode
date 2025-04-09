#ifndef tcode_h
#define tcode_h

#include "pico/stdlib.h"

//define our axes
enum axis_t {
    LINEAR,
    ROTATION,
    VIBRATION,
    AUXILLARY
};

//define possible time commands
enum time_command_t {
    MAGNITUDE_TIME,
    MAGNITUDE_SPEED
};

//our custom type, containing openings for all the traits of a command
struct tcode_command_t {
    enum axis_t axis;
    int channel;
    float magnitude;
    enum time_command_t time_command;
    int time;
};


struct tcode_command_t process_tcode(uint8_t* input, uint16_t input_length);

#endif