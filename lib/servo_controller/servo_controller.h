#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H
void servo_loop();
void servo_move(int channel, float mag, int time);
void servo_init();
#endif