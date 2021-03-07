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
	//角度偏差
	bias=angle_target-angle;
	float balance;
	balance=kp*bias-kd*gyro;
	
	//限
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
	//角速度偏移量
	float gyro_offset=0;
	//角速度积分系数
	float gyro_dt=0.005;
	//滤波权重
	float Filter_Weight=0.01;
	
	Get_AccData();
	Get_Gyro();
	
	//原始角速度减去偏移量
	balance_Gyro=mpu_gyro_y-gyro_offset;
	
	accel_Angle=(float)atan2(mpu_acc_x,-mpu_acc_z)*57.296;
	
	//一阶互补滤波
	angle=Filter_Weight*accel_Angle+(1-Filter_Weight)*(angle+balance_Gyro*gyro_dt);
	
	out[0]=accel_Angle;
	out[1]=angle;
	vcan_sendware(out,sizeof(out));
	return angle;
}