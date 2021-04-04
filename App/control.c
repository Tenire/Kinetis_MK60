#include "control.h"


float out[6];

//��ͬ�ٶ���ת�����,p,i
float turn_params[3][2]={{0.05,0},{0.05,0},{0.05,0}};

//�ٶȵ�λ
int velocity_gear=0;

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
static float speed_target=20;

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
	
	//�ж��ٶȵ�λ
	/*
	if(abs(speed)<3)
	{
		velocity_gear=0;
	}else if(abs(speed)<=20)
	{
		velocity_gear=1;
	}else if(abs(speed)<=40)
	{
		velocity_gear=2;
	}
	*/
	
	
	
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
	if(abs(speed_bias)<8.0)
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
	
	//�����ٶȵ�λ����p,d
	/*
	kp=turn_params[velocity_gear][0];
	kd=turn_params[velocity_gear][1];
	*/	

	float turn;
	
	//adc��ֵ+����
	float adc_weight=0.5;
	adc[0]=adc_weight*getADC(ADC1_DM1)+(1-adc_weight)*adc[0];
	adc[1]=adc_weight*getADC(ADC1_DP1)+(1-adc_weight)*adc[1];
	adc[2]=adc_weight*getADC(ADC0_DP0)+(1-adc_weight)*adc[2];
	adc[3]=adc_weight*getADC(ADC0_DM0)+(1-adc_weight)*adc[3];
	adc[4]=adc_weight*getADC(ADC0_DM1)+(1-adc_weight)*adc[4];
	
	//��һ��
	float adc_1[4];
	adc_1[1]=100*adc[1]/4095;
	adc_1[2]=100*adc[2]/3000;
	
	vcan_sendware(adc,sizeof(adc));
	vcan_sendware(adc_1,sizeof(adc_1));
	
	//�Ƿ���������
	if((adc[0]+adc[1]+adc[2]+adc[3])/4>150)
	{
		onTheTrack=1;
	}else
	{
		onTheTrack=0;
	}
	
	//ƫ��
	//turn_bias=(adc[1]-adc[2])/(adc[1]+adc[2]+1)*100;
	
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
/*
#define No_Ring            0
#define Find_Right_Ring    1
#define Find_Left_Ring     2
#define Ready_Right_Ring   3
#define Ready_Left_Ring    4
#define Start_Right_Ring   5
#define Start_Left_Ring    6
#define In_Ring            7
#define Out_Ring           8
#define Real_Out_Ring      9

//�궨����
float angle_start;
float Dis_Ring = 0;
u8 Ring_state=0;
void Ring_Control()
{
	switch(Ring_state)
	{
	case No_Ring:
		//�ж��Ƿ���ֻ�
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		//�󻷵�  ���ҵ�д������� Ȼ�����ҵ��ֵ���
		if(mid_adc>=230 && right_adc - left_adc>=15 &&right_adc >170 && left_adc >170 
		   && LL_adc - RR_adc>30 && RR_adc < 10)  
		{
			Ring_state  = Find_Left_Ring;
			Dis_Ring = Distance;
		}
		//�һ���  �����д����ҵ�� Ȼ�����ҵ��ֵ���
		else if(mid_adc>=230 &&left_adc - right_adc>=15 &&right_adc >170 && left_adc >170 
			&& RR_adc - LL_adc>30 &&  LL_adc < 10)
		{
			Ring_state  = Find_Right_Ring;
			Dis_Ring = Distance;
		}
		else
			Ring_state  = No_Ring;
		break;
		
	case Find_Right_Ring:
		//�жϽ�����
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		
		if(ABS(left_adc - right_adc)<=10||(Distance - Dis_Ring >10&&Distance - Dis_Ring <30)) 
		{
			Dis_Ring = Distance;
			Ring_state  = Ready_Right_Ring;
		}
		break;
	case Find_Left_Ring:
		//�жϽ�����
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		
		if(ABS(left_adc - right_adc)<=10||(Distance - Dis_Ring >10&&Distance - Dis_Ring <30)) 
		{
			Dis_Ring = Distance;
			Ring_state  = Ready_Left_Ring;
		}
		break;
	case Ready_Right_Ring:
		//׼������
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		if(right_adc > left_adc) //&&(Distance - Dis_Ring >30&&Distance - Dis_Ring <60)
		{
			Ring_state  = Start_Right_Ring;
			angle_start = imu_data.yaw;
		}
		break;	
	case Ready_Left_Ring:
		//׼������
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		if(left_adc > right_adc )
		{
			Ring_state  = Start_Left_Ring;
			imu_data.inter_yaw = 0;
		}
		break;					
	case Start_Left_Ring:
		//ȷ�Ͻ���,��ʼ��¼��ǰ�Ƕ�
		
		Middle_Err = 70;
		imu_data.inter_yaw += GYRO_Real.Z*0.02;
		if(ABS(imu_data.inter_yaw)>=50.0f && (right_adc<160 && left_adc<160))//&& ((RR_adc>10 || LL_adc>10) && (right_adc<170 && left_adc<170))
		{
			Dis_Ring = Distance;
			Ring_state = In_Ring;
			imu_data.inter_yaw = 0;
		}
		break;	
	case Start_Right_Ring:
		//ȷ�Ͻ���,��ʼ��¼��ǰ�Ƕ�
		
		Middle_Err = -25;
		imu_data.inter_yaw += GYRO_Real.Z*0.02;
		if(ABS(imu_data.inter_yaw)>=50.0f && (right_adc<160 && left_adc<160))//&& ((RR_adc>10 || LL_adc>10) && (right_adc<170 && left_adc<170))
		{
			Dis_Ring = Distance;
			Ring_state = In_Ring;
			imu_data.inter_yaw = 0;
		}
		break;
	case In_Ring:
		//������,��¼�����ǻ��ֽǶȱ仯����360��
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		imu_data.inter_yaw += GYRO_Real.Z*0.02;
		if(ABS(imu_data.inter_yaw)>=360.0f )//&&Distance - Dis_Ring >200&&Distance - Dis_Ring <250
		{
			Ring_state = Out_Ring;
			imu_data.inter_yaw= 0;
		}
		break;	
	case Out_Ring:
		//����,�޻�״̬
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		//��������
		if(mid_adc<=240 &&mid_adc>=150 && ABS(left_adc - right_adc)<=20&&left_adc<=150 &&right_adc<=150 )
			Ring_state  = No_Ring;
		
		imu_data.inter_yaw= 0;
		
		break;
	default:break;
	
	}
	
}
*/











