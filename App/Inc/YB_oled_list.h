

#ifndef _YB_OLED_LIST_H_
#define _YB_OLED_LIST_H_

#include "include.h"

void Clean_Key_Position();
void firstlist();               /*һ���˵�*/
void Set_Mt_PID_list();          /*�����˵���һ�����޸�pid����*/
void Set_Sv_PID_list();
void Set_Parameters_list();
void Set_Parameters_int();
void Set_Sv_MID_list();

extern double motor_P;                                        //���pֵ
extern double motor_I;                                        //���iֵ
extern double motor_D;                                        //���dֵ
extern double servo_P;
extern double servo_D;
extern int desired_value;
extern int servo_mid;


extern const unsigned char F6x8[][6];
/*
extern uint8_t DMAFinishFlag;
extern uint8 *imgbuff;
extern uint8 imgbuff1[CAMERA_SIZE];                             //����洢����ͼ�������
extern uint8 imgbuff2[CAMERA_SIZE];
extern uint8 img[CAMERA_H][CAMERA_W];
*/
#endif