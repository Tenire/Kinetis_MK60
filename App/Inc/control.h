#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "include.h"
#include "SEEKFREE_MPU6050 2.h"
#include "SEEKFREE_IIC.h"

extern float angle;
extern float out[6];
extern float speed_target;

void balance(float angle,int gyro);
void getAngle();
void velocity();

void setSpeed(float speedd);

#endif