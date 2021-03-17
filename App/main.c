/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2013,山外科技
*     All rights reserved.
*     技术讨论：山外论坛 http://www.vcan123.com
*
*     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
*     修改内容时必须保留山外科技的版权声明。
*
* @file       main.c
* @brief      山外K60 平台主程序
* @author     山外科技
* @version    v5.0
* @date       2013-08-28
*/

#include "common.h"
#include "include.h"
#include "math.h"
#include "SEEKFREE_OLED.h"
#include "loop.h"
#include "OLED_list.h"
#include "SEEKFREE_MPU6050 2.h"
#include "SEEKFREE_IIC.h"

void PIT0_IRQHandler();

void main()
{

	IIC_init();
	InitMPU6050();
	
	//串口
	uart_init(UART1,115200);
	
	//编码器
	ftm_quad_init(FTM1);
	ftm_quad_init(FTM2);
	
	//PIT
	pit_init_ms(PIT0,1);	
	set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);
	enable_irq (PIT0_IRQn);
	
	//电机
	ftm_pwm_init(FTM0,FTM_CH4,13*1000,0);//左反
	ftm_pwm_init(FTM0,FTM_CH5,13*1000,0);//左正
	
	ftm_pwm_init(FTM0,FTM_CH6,13*1000,0);//右反
	ftm_pwm_init(FTM0,FTM_CH7,13*1000,0);//右正
	
	//OLED
	OLED_Init();
	firstlist();
	
	//adc
	adc_init(ADC1_DP1);
	adc_init(ADC1_DM1);
	while(1)
	{
		
	}
}


void PIT0_IRQHandler()
{
	loop_run();
	
	PIT_Flag_Clear(PIT0);
}









