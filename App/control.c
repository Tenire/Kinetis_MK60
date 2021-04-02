#include "control.h"


float out[6];


//5��adc��ֵ
float adc[5];

//ȥƫ����ٶ�
float balance_Gyro;

//���ٶȽǶ�
float accel_Angle;

//��ǰ�Ƕ�
float angle;

//ƽ��Ƕ�
float balance_angle=43;

//Ŀ��Ƕ�
float angle_target=43;

//�����ٶ�
float speed_left;

//�����ٶ�
float speed_right;

//��ǰ�ٶ�
float speed;

//�ٶ�������
float speed_bias_sum;

//ת�����
float turn_bias;
float turn_bias_last;

//Ŀ���ٶ�
static float speed_target=40;

//ֱ��pwm
float balance_pwm=0;
//ת��pwm
float turn_pwm=0;

//�Ƿ���������
int8 onTheTrack=0;

float bias;

void balance()
{
	//P,D
	float kp=8,kd=0.125;
	//�Ƕ�ƫ��
	bias=angle_target-angle;
	//float balance;
	balance_pwm=kp*bias+kd*mpu_gyro_x;
}

void getAngle()
{
	//���ٶ�ƫ����
	float gyro_offset=0;
	//���ٶȻ���ϵ��
	float gyro_dt=0.004;
	//�˲�Ȩ��
	float Filter_Weight=0.02;
	
	Get_AccData();
	Get_Gyro();
	
	//ԭʼ���ٶȼ�ȥƫ����
	balance_Gyro=mpu_gyro_x-gyro_offset;
	
	accel_Angle=(float)atan2(mpu_acc_y,-mpu_acc_z)/PI*180;
	
	
	//һ�׻����˲�
	angle=Filter_Weight*accel_Angle+(1-Filter_Weight)*(angle-balance_Gyro*gyro_dt);
	
	//�ٶ�
	speed_right=0.7*(ftm_quad_get(FTM1))+0.3*speed_right;
	ftm_quad_clean(FTM1);
	
	speed_left=0.7*(-ftm_quad_get(FTM2))+0.3*speed_left;
	ftm_quad_clean(FTM2);
	
	speed=(speed_left+speed_right)/2;
	
	
	
	
	out[0]=accel_Angle;
	out[1]=angle;
	out[2]=-balance_pwm;
	out[3]=speed_bias_sum;
	out[4]=speed;
	out[5]=speed_target;
	
	vcan_sendware(out,sizeof(out));
	
}

void velocity()
{
	//,P,I
	float speed_bias,kp=0.75,ki=0.00375;
	//�ٶ�ƫ��
	speed_bias=speed_target-speed;
	//����
	speed_bias_sum+=speed_bias;
	if(speed_bias_sum>1000)
	{
		speed_bias_sum=1000;
	}
	if(speed_bias_sum<-1000)
	{
		speed_bias_sum=-1000;
	}
	
	float vel;
	if(abs(speed_bias)<8)
	{
		vel=kp*speed_bias+ki*speed_bias_sum;
	}else
	{
		vel=kp*speed_bias;
	}
	
	if(vel>25)
	{
		vel=25;
	}
	if(vel<-7)
	{
		vel=-7;
	}
	angle_target=balance_angle+vel;
}

//ת�����
void turn()
{
	
	float kp=0.05,kd=0;
	float turn;
	
	
	//adc��ֵ+����
	float adc_weight=0.5;
	adc[0]=adc_weight*getADC(ADC1_DM1)+(1-adc_weight)*adc[0];
	adc[1]=adc_weight*getADC(ADC1_DP1)+(1-adc_weight)*adc[1];
	adc[2]=adc_weight*getADC(ADC0_DP0)+(1-adc_weight)*adc[2];
	adc[3]=adc_weight*getADC(ADC0_DM0)+(1-adc_weight)*adc[3];
	adc[4]=adc_weight*getADC(ADC0_DM1)+(1-adc_weight)*adc[4];
	
	//��һ��
	/*float adc_1[4];
	adc_1[1]=100*adc[1]/4095;
	adc_1[2]=100*adc[2]/3000;*/
	
	vcan_sendware(adc,sizeof(adc));
	
	//�Ƿ���������
	if((adc[0]+adc[1]+adc[2]+adc[3])/4>150)
	{
		onTheTrack=1;
	}else
	{
		onTheTrack=0;
	}
	
	//ƫ��
	turn_bias=(adc[1]-adc[2])/(adc[1]+adc[2]+1)*100;
	
	turn=kp*turn_bias+kd*(turn_bias-turn_bias_last);
	
	turn_bias_last=turn_bias;
	turn_pwm=turn;
}


//�����ֵ����
int16 getADC(ADCn_Ch_e adcn_ch)
{
	int16 i=0,j=0,k=0,tmp=0,adc=0;
	int sum=0,num=0;
	for(num = 0; num < 5; num++)
	{
		
		i = adc_once(adcn_ch,ADC_12bit);
		j = adc_once(adcn_ch,ADC_12bit);
		k = adc_once(adcn_ch,ADC_12bit);
		
		
		if (i > j)
		{
			tmp = i; i = j; j = tmp;
		}
		if (k > j)
			tmp = j;
		else if(k > i)
			tmp = k;
		else 
			tmp = i;
		sum+=tmp;
		
	}
	
	adc=sum/5;
	sum=0;
	return adc;
}


//������
void motorControl(float left_pwm,float right_pwm)
{
	
	//���޷�
	if(left_pwm>0)
	{
		if(left_pwm>99)
		{
			left_pwm=99;
		}
		
		ftm_pwm_duty(FTM0,FTM_CH0,0);
		ftm_pwm_duty(FTM0,FTM_CH1,left_pwm);
		
	}else
	{
		left_pwm=-left_pwm;
		if(left_pwm>99)
		{
			left_pwm=99;
		}
		ftm_pwm_duty(FTM0,FTM_CH1,0);
		ftm_pwm_duty(FTM0,FTM_CH0,left_pwm);
		
	}
	
	//���޷�
	if(right_pwm>0)
	{
		if(right_pwm>99)
		{
			right_pwm=99;
		}
		
		ftm_pwm_duty(FTM0,FTM_CH2,0);
		ftm_pwm_duty(FTM0,FTM_CH3,right_pwm);
		
	}else
	{
		right_pwm=-right_pwm;
		if(right_pwm>99)
		{
			right_pwm=99;
		}
		ftm_pwm_duty(FTM0,FTM_CH3,0);
		ftm_pwm_duty(FTM0,FTM_CH2,right_pwm);
	}
}











