#include "control.h"


float out[2];
float balance_Gyro;
float accel_Angle;
float angle;
float angle_target=-8.5;


void balance(float angle,int gyro)
{
	//P,D
	float bias,kp=8,kd=0.4;
	//�Ƕ�ƫ��
	bias=angle_target-angle;
	float balance;
	balance=kp*bias-kd*gyro;
	
	//��
	if(balance>0)
	{
		if(balance>60)
		{
			balance=60;
		}
		
		ftm_pwm_duty(FTM0,FTM_CH4,balance);
		ftm_pwm_duty(FTM0,FTM_CH6,balance*0.75);
	}else
	{
		balance=-balance;
		if(balance>60)
		{
			balance=60;
		}
		ftm_pwm_duty(FTM0,FTM_CH5,balance);
		ftm_pwm_duty(FTM0,FTM_CH7,balance*0.75);
	}
}

float getAngle()
{
	//���ٶ�ƫ����
	float gyro_offset=0;
	//���ٶȻ���ϵ��
	float gyro_dt=0.005;
	//�˲�Ȩ��
	float Filter_Weight=0.01;
	
	Get_AccData();
	Get_Gyro();
	
	//ԭʼ���ٶȼ�ȥƫ����
	balance_Gyro=mpu_gyro_y-gyro_offset;
	
	accel_Angle=(float)atan2(mpu_acc_x,-mpu_acc_z)*57.296;
	
	//һ�׻����˲�
	angle=Filter_Weight*accel_Angle+(1-Filter_Weight)*(angle+balance_Gyro*gyro_dt);
	
	out[0]=accel_Angle;
	out[1]=angle;
	vcan_sendware(out,sizeof(out));
	return angle;
}