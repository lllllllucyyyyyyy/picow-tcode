#include <stdio.h>
#include "pico/stdlib.h"

#include "servo.h"
#include "servo_controller.h"

// usable range of our actuator
int zero = 1250;
int range = 500;

// hardware configs
int servoPin0 = 2;
int servoPin1 = 3;

// state values for movements
int pos = 1500;
int old_timestamp;
int target_time;
int old_pos;
int target_pos;
bool movecommand = false;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// set our start and end points, and start and end times.
void servo_move(int channel, float mag, int time)
{
    if (channel == 0)
    {
        old_pos = pos;
        target_pos = (mag * range) + zero;
        old_timestamp = (get_absolute_time() / 1000);
        target_time = time;
        movecommand = true;
    }
}

void servo_loop()
{
    if (movecommand)
    {
        int deltatime = (get_absolute_time() / 1000) - old_timestamp;
        if (deltatime >= target_time)
        {
            movecommand = false;
            pos = target_pos;
        }
        else
        {
            // find where we are in current move
            pos = map(deltatime, 0, target_time, old_pos, target_pos);
        }
        setMillis(servoPin0, pos);
        setMillis(servoPin1, pos);
    }
}

void servo_init()
{
    setServo(servoPin0, zero);
    setServo(servoPin1, zero);
}