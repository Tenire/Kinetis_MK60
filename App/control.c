#include "control.h"


float out[6];

//ȥƫ����ٶ�
float balance_Gyro;

//���ٶȽǶ�
float accel_Angle;

//��ǰ�Ƕ�
float angle;

//ƽ��Ƕ�
//float balance_angle=-6;
float balance_angle=58;

//Ŀ��Ƕ�
float angle_target=58;

float balance1;

//�����ٶ�
float speed_left;

//�����ٶ�
float speed_right;

//��ǰ�ٶ�
float speed;

//�ٶ�������
float speed_sum;

//Ŀ���ٶ�
static float speed_target=0;


void balance(float angle,int gyro)
{
	//P,D
	float bias,kp=8,kd=0.1;
	//�Ƕ�ƫ��
	bias=angle_target-angle;
	float balance;
	balance=kp*bias-kd*gyro;
	
	//�޷�
	if(balance>0)
	{
		if(balance>99)
		{
			balance=99;
		}
		
		ftm_pwm_duty(FTM0,FTM_CH5,0);
		ftm_pwm_duty(FTM0,FTM_CH7,0);
		
		ftm_pwm_duty(FTM0,FTM_CH4,balance);
		ftm_pwm_duty(FTM0,FTM_CH6,balance);
		
		
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
		balance1=balance;
	}
	
	
	
}

void getAngle()
{
	//���ٶ�ƫ����
	float gyro_offset=0;
	//���ٶȻ���ϵ��
	float gyro_dt=0.003;
	//�˲�Ȩ��
	float Filter_Weight=0.02;
	
	Get_AccData();
	Get_Gyro();
	
	//ԭʼ���ٶȼ�ȥƫ����
	balance_Gyro=mpu_gyro_y-gyro_offset;
	
	accel_Angle=(float)atan2(mpu_acc_x,-mpu_acc_z)*57.296;
	
	//һ�׻����˲�
	angle=Filter_Weight*accel_Angle+(1-Filter_Weight)*(angle+balance_Gyro*gyro_dt);
	
	//�ٶ�
	speed_right=ftm_quad_get(FTM1);
	ftm_quad_clean(FTM1);
	speed_left=-ftm_quad_get(FTM2);
	ftm_quad_clean(FTM2);
	//speed=(speed_left+speed_right)/2;
	speed=speed_right;
	
	out[0]=accel_Angle;
	out[1]=angle;
	out[2]=balance1;
	out[3]=speed_left;
	out[4]=speed_right;
	out[5]=speed_target;
	
	vcan_sendware(out,sizeof(out));
	
}

void velocity()
{
	//P,D
	float speed_bias,kp=0.05,ki=0;
	//�ٶ�ƫ��
	speed_bias=speed_target-speed;
	speed_sum+=speed_bias;
	angle_target=balance_angle+(kp*speed_bias+ki*speed_sum);
}

void setSpeed(float speedd)
{
	speed_target=speedd;
}











