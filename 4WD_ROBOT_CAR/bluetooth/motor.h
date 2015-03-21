#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"


void initMotor(void);
void setMotor(char BT_rcv_char);
void setLeft(bool DIR, float SPEED);
void setRight(bool DIR, float SPEED);
void brakeMotor(void);
void freeRunMotor(void);
void Blink(int times);
void SpeedLEDs(float SPEED);

#endif