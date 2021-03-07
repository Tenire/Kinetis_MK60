

#ifndef _YB_OLED_LIST_H_
#define _YB_OLED_LIST_H_

#include "include.h"

void Clean_Key_Position();
void firstlist();               /*一级菜单*/
void Set_Mt_PID_list();          /*二级菜单（一），修改pid参数*/
void Set_Sv_PID_list();
void Set_Parameters_list();
void Set_Parameters_int();
void Set_Sv_MID_list();

extern double motor_P;                                        //电机p值
extern double motor_I;                                        //电机i值
extern double motor_D;                                        //电机d值
extern double servo_P;
extern double servo_D;
extern int desired_value;
extern int servo_mid;


extern const unsigned char F6x8[][6];
/*
extern uint8_t DMAFinishFlag;
extern uint8 *imgbuff;
extern uint8 imgbuff1[CAMERA_SIZE];                             //定义存储接收图像的数组
extern uint8 imgbuff2[CAMERA_SIZE];
extern uint8 img[CAMERA_H][CAMERA_W];
*/
#endif