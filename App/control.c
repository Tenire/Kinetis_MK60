#include "control.h"


float out[6];

//�ĸ�adc��ֵ
float adc[4];

//ȥƫ����ٶ�
float balance_Gyro;

//���ٶȽǶ�
float accel_Angle;

//��ǰ�Ƕ�
float angle;

//ƽ��Ƕ�
float balance_angle=41.5;

//Ŀ��Ƕ�
float angle_target=41.5;

//�����ٶ�
float speed_left;

//�����ٶ�
float speed_right;

//��ǰ�ٶ�
float speed;

//�ٶ�������
float speed_sum;

//ת�����
float turn_error;
float turn_error_last;

//Ŀ���ٶ�
static float speed_target=0;

//ֱ��pwm
float balance_pwm;
//ת��pwm
float turn_pwm=0;

//�Ƿ���������
int8 onTheTrack;

void balance(float angle,int gyro)
{
	//P,D
	float bias,kp=8,kd=0.125;
	//�Ƕ�ƫ��
	bias=angle_target-angle;
	//float balance;
	balance_pwm=kp*bias+kd*gyro;
	
	/*
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
		balance1=-balance;
	}
	*/
	
	
}

void getAngle()
{
	//���ٶ�ƫ����
	float gyro_offset=0;
	//���ٶȻ���ϵ��
	float gyro_dt=0.0035;
	//�˲�Ȩ��
	float Filter_Weight=0.02;
	
	Get_AccData();
	Get_Gyro();
	
	//ԭʼ���ٶȼ�ȥƫ����
	balance_Gyro=mpu_gyro_x-gyro_offset;
	
	accel_Angle=(float)atan2(mpu_acc_y,-mpu_acc_z)*57.296;
	
	//һ�׻����˲�
	angle=Filter_Weight*accel_Angle+(1-Filter_Weight)*(angle-balance_Gyro*gyro_dt);
	
	//�ٶ�
	speed_right=0.7*ftm_quad_get(FTM1)+0.3*speed_right;
	ftm_quad_clean(FTM1);
	speed_left=0.7*(-ftm_quad_get(FTM2))+0.3*speed_left;
	ftm_quad_clean(FTM2);
	speed=(speed_left+speed_right)/2;
	
	
	out[0]=accel_Angle;
	out[1]=angle;
	out[2]=balance_pwm;
	out[3]=speed_sum;
	out[4]=speed;
	out[5]=speed_target;
	
	vcan_sendware(out,sizeof(out));
	
}

void velocity()
{
	//,P,I
	float speed_bias,kp=0.15,ki=0.0075;
	//�ٶ�ƫ��
	speed_bias=speed_target-speed;
	//����
	speed_sum+=speed_bias;
	if(speed_sum>1000)
	{
		speed_sum=1000;
	}
	if(speed_sum<-1000)
	{
		speed_sum=-1000;
	}
	
	float val=kp*speed_bias+ki*speed_sum;
	
	if(val>25)
	{
		val=25;
	}
	if(val<-7)
	{
		val=-7;
	}
	angle_target=balance_angle+val;
}

void turn()
{
	float kp,kd;
	float turn;
	float adc_weight=0.5;
	adc[0]=adc_weight*adc_once(ADC0_DP0,ADC_16bit)+(1-adc_weight)*adc[0];
	adc[1]=adc_weight*adc_once(ADC1_DM1,ADC_16bit)+(1-adc_weight)*adc[1];
	adc[2]=adc_weight*adc_once(ADC0_DM1,ADC_16bit)+(1-adc_weight)*adc[2];
	adc[3]=adc_weight*adc_once(ADC0_DP1,ADC_16bit)+(1-adc_weight)*adc[3];
	vcan_sendware(adc,sizeof(adc));
	if((adc[0]+adc[1]+adc[2]+adc[3])/4>1200)
	{
		onTheTrack=1;
	}else
	{
		onTheTrack=0;
	}
	if(adc[1]+adc[2]!=0)
	{
		turn_error=-(adc[1]-adc[2])/(adc[1]+adc[2]);
	}
	else
	{
		turn_error=-(adc[1]-adc[2])/1;
	}
	turn=kp*turn_error+kd*(turn_error-turn_error_last);
	
}

void motorControl(float left_pwm,float right_pwm)
{
	
	//���޷�
	if(left_pwm>0)
	{
		if(left_pwm>99)
		{
			left_pwm=99;
		}
		
		ftm_pwm_duty(FTM0,FTM_CH5,0);
		ftm_pwm_duty(FTM0,FTM_CH4,left_pwm);
		
	}else
	{
		left_pwm=-left_pwm;
		if(left_pwm>99)
		{
			left_pwm=99;
		}
		ftm_pwm_duty(FTM0,FTM_CH4,0);
		ftm_pwm_duty(FTM0,FTM_CH5,left_pwm);
		
	}
	
	//���޷�
	if(right_pwm>0)
	{
		if(right_pwm>99)
		{
			right_pwm=99;
		}
		
		ftm_pwm_duty(FTM0,FTM_CH7,0);
		ftm_pwm_duty(FTM0,FTM_CH6,right_pwm);
		
	}else
	{
		right_pwm=-right_pwm;
		if(right_pwm>99)
		{
			right_pwm=99;
		}
		ftm_pwm_duty(FTM0,FTM_CH6,0);
		ftm_pwm_duty(FTM0,FTM_CH7,right_pwm);
	}
}











