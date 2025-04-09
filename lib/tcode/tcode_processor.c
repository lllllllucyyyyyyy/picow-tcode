#include <string.h>
#include <stdlib.h>
#include "tcode_processor.h"

struct tcode_command_t process_tcode(uint8_t* input, uint16_t input_length) {
    struct tcode_command_t command;
    char buffer0[4];
    char buffer1[6];
    bool buffer_which = false;
    char* input_buffer = input;
    char axis = input[0];
    switch(axis) {
        case 'L':
        case 'l':
            command.axis = LINEAR;
            break;
        case 'R':
        case 'r':
            command.axis = ROTATION;
            break;
        case 'V':
        case 'v':
            command.axis = VIBRATION;
            break;
        case 'A':
        case 'a':
            command.axis = AUXILLARY;
            break;
        default:
            break;
    }
    command.channel = input[1] - '0';

    int offset = 0;
    for (int i = 2; i < input_length; i++) {
        char index = input[i];
        switch(index) {
            case 'S':
            case 's':
                command.time_command = MAGNITUDE_SPEED;
                buffer_which = true;
                offset = strlen(buffer0) + 1; 
                break;
            case 'I':
            case 'i':
                command.time_command = MAGNITUDE_TIME;
                buffer_which = true;
                offset = strlen(buffer0) + 1;
                break;
            case '\n':
                break;
            default:
                if (!buffer_which){
                    buffer0[i - 2] = index;
                } else {
                    buffer1[i - offset - 2] = index;
                }
                break;
        }
    }
    command.magnitude = (atof(buffer0) / 100);
    command.time = atoi(buffer1);
    return command;
}