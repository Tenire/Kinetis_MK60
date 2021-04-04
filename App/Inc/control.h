#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "include.h"
#include "SEEKFREE_MPU6050 2.h"
#include "SEEKFREE_IIC.h"

extern float angle;
extern float out[6];
extern float speed_target;
extern float balance_pwm;
extern float turn_pwm;
extern int8 onTheTrack;

void balance();
void getAngle();
void velocity();
void turn();
void motorControl(float left_pwm,float right_pwm);
int16 getADC(ADCn_Ch_e adcn_ch);
void Ring_Control();

#endif