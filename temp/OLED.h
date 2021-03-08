/* @file       OLED.h
 * @brief      OLED驱动头文件
 * @author     ZCW
 * @version    v7.9
 * @date       2016-7-9
 */
/*
    字模取模：从左到右，从上到下 纵向8点下位 

*/




#ifndef OLED_H_
#define OLED_H_

   

#include "include.h"


typedef unsigned char byte;
typedef unsigned int word;




#define OLED_SCL PTB20_OUT
#define OLED_SDA PTB19_OUT
#define OLED_RST PTB18_OUT
#define OLED_DC  PTB17_OUT
//#define OLED_CS  PTB9_OUT

void delay_ms(uint8 ms);
void OLED_WrCmd(uint8 cmd);
void OLED_WrDat(uint8 dat);
void OLED_Set_Pos(uint8 x, uint8 y);
void OLED_Fill(uint8 bmp_dat);
void OLED_clear(void);
void OLED_init(void);
void OLED_P6x8Str(uint8 x,uint8 y,uint8 ch[]);
void OLED_P1Int(uint8 x,uint8 y,int a);
void OLED_P14x16(uint8 x,uint8 y,uint8 a[]);
void OLED_P16x16(uint8 x,uint8 y,uint8 a[]);
void OLED_P16x16Str(byte x,byte y,byte ch[]);
void OLED_PutPixel(uint8 x,uint8 y);

void DisplayInt(uint8 x,uint8 y,int a);
void DisplayDou(uint8 x,uint8 y,double a);

void OLED_CAMERA(uint8 *imgaddr,uint32 camera_w,uint32 camera_h);
void OLED_Rectangle(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 gif);
void OLED_DrawDot(uint8 (*imgaddr)[60],uint8 camera_w,uint8 camera_h);

void OLED_PrintImage(uint8 *pucTable, uint16 usRowNum, uint16 usColumnNum);
extern unsigned char str[10];



#endif 