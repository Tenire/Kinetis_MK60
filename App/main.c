/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2013,ɽ��Ƽ�
*     All rights reserved.
*     �������ۣ�ɽ����̳ http://www.vcan123.com
*
*     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
*     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
*
* @file       main.c
* @brief      ɽ��K60 ƽ̨������
* @author     ɽ��Ƽ�
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
	
	//����
	uart_init(UART1,115200);
	
	//������
	ftm_quad_init(FTM1);
	ftm_quad_init(FTM2);
	
	//PIT
	pit_init_ms(PIT0,1);	
	set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);
	enable_irq (PIT0_IRQn);
	
	//���
	ftm_pwm_init(FTM0,FTM_CH4,13*1000,0);//��
	ftm_pwm_init(FTM0,FTM_CH5,13*1000,0);//����
	
	ftm_pwm_init(FTM0,FTM_CH6,13*1000,0);//�ҷ�
	ftm_pwm_init(FTM0,FTM_CH7,13*1000,0);//����
	
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









