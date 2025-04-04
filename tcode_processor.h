#include <string.h>

enum axis_t {
    LINEAR,
    ROTATION,
    VIBRATION,
    AUXILLARY
};

enum time_command_t {
    MAG_TIME,
    MAG_SPEED
};

struct tcode_command_t {
    enum axis_t axis;
    int channel;
    double mag;
    enum time_command_t time_command;
    int time;
};


struct tcode_command_t process_tcode(uint8_t* input, uint16_t input_length) {
    struct tcode_command_t command;
    char* buffer0 = "";
    char* buffer1 = "";
    int buffer_which = 0;
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
    command.channel = (double)input[1];

    int offset = 0;
    for (int i = 2; i < input_length; i++) {
        char index = input[i];
        switch(index) {
            case 'S':
            case 's':
                command.time_command = MAG_SPEED;
                buffer_which = 1;
                offset = strlen(buffer0) + 1; 
                break;
            case 'I':
            case 'i':
                command.time_command = MAG_TIME;
                buffer_which = 1;
                offset = strlen(buffer0) + 1;
                break;
            default:
                if (!buffer_which) {
                    buffer0[i] = index;
                } else {
                    buffer1[i - offset] = index;
                }
                break;
        }
    }
}