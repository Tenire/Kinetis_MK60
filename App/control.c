#include "control.h"


float out[6];

//去偏差角速度
float balance_Gyro;

//加速度角度
float accel_Angle;

//当前角度
float angle;

//平衡角度
float balance_angle=-6;
//float balance_angle=52.5;

//目标角度
//float angle_target=52.5;
float angle_target=-6;

float balance1;

//左轮速度
float speed_left;

//右轮速度
float speed_right;

//当前速度
float speed;

//速度误差积分
float speed_sum;

//目标速度
float speed_target=0;


void balance(float angle,int gyro)
{
	//P,D
	float bias,kp=12,kd=0.1;
	//角度偏差
	bias=angle_target-angle;
	float balance;
	balance=kp*bias-kd*gyro;
	
	//限幅
	if(balance>0)
	{
		if(balance>99)
		{
			balance=99;
		}
		
		ftm_pwm_duty(FTM0,FTM_CH4,balance);
		ftm_pwm_duty(FTM0,FTM_CH6,balance);
		
		ftm_pwm_duty(FTM0,FTM_CH5,0);
		ftm_pwm_duty(FTM0,FTM_CH7,0);
		balance1=balance;
	}else
	{
		balance=-balance;
		if(balance>99)
		{
			balance=99;
		}
		ftm_pwm_duty(FTM0,FTM_CH4,0);
		ftm_pwm_duty(FTM0,FTM_CH6,0);
		
		ftm_pwm_duty(FTM0,FTM_CH5,balance);
		ftm_pwm_duty(FTM0,FTM_CH7,balance);
		balance1=-balance;
	}
	
	
	
}

void getAngle()
{
	//角速度偏移量
	float gyro_offset=0;
	//角速度积分系数
	float gyro_dt=0.003;
	//滤波权重
	float Filter_Weight=0.02;
	
	Get_AccData();
	Get_Gyro();
	
	//原始角速度减去偏移量
	balance_Gyro=mpu_gyro_y-gyro_offset;
	
	accel_Angle=(float)atan2(mpu_acc_x,-mpu_acc_z)*57.296;
	
	//一阶互补滤波
	angle=Filter_Weight*accel_Angle+(1-Filter_Weight)*(angle+balance_Gyro*gyro_dt);
	
	//速度
	speed_left=ftm_quad_get(FTM1);
	ftm_quad_clean(FTM1);
	speed_right=-ftm_quad_get(FTM2);
	ftm_quad_clean(FTM2);
	speed=(speed_left+speed_right)/2;
	
	out[0]=accel_Angle;
	out[1]=angle;
	out[2]=balance1;
	out[3]=speed_left;
	out[4]=speed_right;
	
	vcan_sendware(out,sizeof(out));
	
}

void velocity()
{
	//P,D
	float speed_bias,kp=0.2,ki=0;
	//速度偏差
	speed_bias=speed_target-speed;
	speed_sum+=speed_bias;
	angle_target=balance_angle-(kp*speed_bias+ki*speed_sum);
}







