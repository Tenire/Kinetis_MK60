#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "include.h"
#include "SEEKFREE_MPU6050 2.h"
#include "SEEKFREE_IIC.h"


void balance(float angle,int gyro);
float getAngle();

#endif