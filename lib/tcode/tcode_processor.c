#include <string.h>
#include <stdlib.h>
#include "tcode_processor.h"

struct tcode_command_t process_tcode(uint8_t* input, uint16_t input_length) {

    //init our in-progress tcode command, see header file for type explanation
    struct tcode_command_t command;

    //create our buffers for magnitude and time
    char buffer0[4];
    char buffer1[6];
    //buffer_which just decides which buffer we're appending to
    bool buffer_which = false;

    //copies the input buffer
    char* input_buffer = input;

    //sets a temp variable to the first character of the input so it can be used in a switch function
    char axis = input[0];

    //sets our axis depending on the first character
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
    //channel is always the second character
    //idk how/why, but the - '0' makes the char into an integer
    command.channel = input[1] - '0';

    //offset for how many characters come before the speed characters, used later
    int offset = 0;

    //loop through every character in the input except the first 2
    for (int i = 2; i < input_length; i++) {
        char index = input[i];

        //detect possible time commands, or just append to buffer
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

    //convert time and magnitude buffers to their correct formats.
    command.magnitude = (atof(buffer0) / 100);
    command.time = atoi(buffer1);

    //submit the final command
    return command;
}