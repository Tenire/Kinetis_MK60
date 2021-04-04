#include "control.h"


float out[6];

//不同速度下转向参数,p,i
float turn_params[3][2]={{0.05,0},{0.05,0},{0.05,0}};

//速度档位
int velocity_gear=0;

//5个adc的值
float adc[5];

//去偏差角速度
float balance_Gyro;

//加速度角度
float accel_Angle;

//当前角度
float angle;

//平衡角度
float balance_angle=43;

//目标角度
float angle_target=43;

//左轮速度
float speed_left;

//右轮速度
float speed_right;

//当前速度
float speed;

//速度误差积分
float speed_bias_sum;

//转向误差
float turn_bias;
float turn_bias_last;

//目标速度
static float speed_target=20;

//直立pwm
float balance_pwm=0;
//转向pwm
float turn_pwm=0;

//是否在赛道中
int8 onTheTrack=0;

float bias;

void balance()
{
	//P,D
	float kp=8,kd=0.125;
	//角度偏差
	bias=angle_target-angle;
	//float balance;
	balance_pwm=kp*bias+kd*mpu_gyro_x;
}

void getAngle()
{
	//角速度偏移量
	float gyro_offset=0;
	//角速度积分系数
	float gyro_dt=0.004;
	//滤波权重
	float Filter_Weight=0.02;
	
	Get_AccData();
	Get_Gyro();
	
	//原始角速度减去偏移量
	balance_Gyro=mpu_gyro_x-gyro_offset;
	
	accel_Angle=(float)atan2(mpu_acc_y,-mpu_acc_z)/PI*180;
	
	
	//一阶互补滤波
	angle=Filter_Weight*accel_Angle+(1-Filter_Weight)*(angle-balance_Gyro*gyro_dt);
	
	//速度
	speed_right=0.7*(ftm_quad_get(FTM1))+0.3*speed_right;
	ftm_quad_clean(FTM1);
	
	speed_left=0.7*(-ftm_quad_get(FTM2))+0.3*speed_left;
	ftm_quad_clean(FTM2);
	
	speed=(speed_left+speed_right)/2;
	
	//判断速度档位
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
	//速度偏差
	speed_bias=speed_target-speed;
	//积分
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

//转向控制
void turn()
{
	
	float kp=0.05,kd=0;
	
	//根据速度档位更改p,d
	/*
	kp=turn_params[velocity_gear][0];
	kd=turn_params[velocity_gear][1];
	*/	

	float turn;
	
	//adc读值+处理
	float adc_weight=0.5;
	adc[0]=adc_weight*getADC(ADC1_DM1)+(1-adc_weight)*adc[0];
	adc[1]=adc_weight*getADC(ADC1_DP1)+(1-adc_weight)*adc[1];
	adc[2]=adc_weight*getADC(ADC0_DP0)+(1-adc_weight)*adc[2];
	adc[3]=adc_weight*getADC(ADC0_DM0)+(1-adc_weight)*adc[3];
	adc[4]=adc_weight*getADC(ADC0_DM1)+(1-adc_weight)*adc[4];
	
	//归一化
	float adc_1[4];
	adc_1[1]=100*adc[1]/4095;
	adc_1[2]=100*adc[2]/3000;
	
	vcan_sendware(adc,sizeof(adc));
	vcan_sendware(adc_1,sizeof(adc_1));
	
	//是否在赛道上
	if((adc[0]+adc[1]+adc[2]+adc[3])/4>150)
	{
		onTheTrack=1;
	}else
	{
		onTheTrack=0;
	}
	
	//偏差
	//turn_bias=(adc[1]-adc[2])/(adc[1]+adc[2]+1)*100;
	
	turn_bias=(adc[1]-adc[2])/(adc[1]+adc[2]+1)*100;
	
	turn=kp*turn_bias+kd*(turn_bias-turn_bias_last);
	
	turn_bias_last=turn_bias;
	turn_pwm=turn;
}


//电感数值处理
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


//电机输出
void motorControl(float left_pwm,float right_pwm)
{
	
	//左限幅
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
	
	//右限幅
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

//标定距离
float angle_start;
float Dis_Ring = 0;
u8 Ring_state=0;
void Ring_Control()
{
	switch(Ring_state)
	{
	case No_Ring:
		//判断是否出现环
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		//左环岛  先右电感大于左电感 然后左右电感值相等
		if(mid_adc>=230 && right_adc - left_adc>=15 &&right_adc >170 && left_adc >170 
		   && LL_adc - RR_adc>30 && RR_adc < 10)  
		{
			Ring_state  = Find_Left_Ring;
			Dis_Ring = Distance;
		}
		//右环岛  先左电感大于右电感 然后左右电感值相等
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
		//判断进环点
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		
		if(ABS(left_adc - right_adc)<=10||(Distance - Dis_Ring >10&&Distance - Dis_Ring <30)) 
		{
			Dis_Ring = Distance;
			Ring_state  = Ready_Right_Ring;
		}
		break;
	case Find_Left_Ring:
		//判断进环点
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		
		if(ABS(left_adc - right_adc)<=10||(Distance - Dis_Ring >10&&Distance - Dis_Ring <30)) 
		{
			Dis_Ring = Distance;
			Ring_state  = Ready_Left_Ring;
		}
		break;
	case Ready_Right_Ring:
		//准备进环
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		if(right_adc > left_adc) //&&(Distance - Dis_Ring >30&&Distance - Dis_Ring <60)
		{
			Ring_state  = Start_Right_Ring;
			angle_start = imu_data.yaw;
		}
		break;	
	case Ready_Left_Ring:
		//准备进环
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		if(left_adc > right_adc )
		{
			Ring_state  = Start_Left_Ring;
			imu_data.inter_yaw = 0;
		}
		break;					
	case Start_Left_Ring:
		//确认进环,开始记录当前角度
		
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
		//确认进环,开始记录当前角度
		
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
		//环岛中,记录陀螺仪积分角度变化大于360度
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		imu_data.inter_yaw += GYRO_Real.Z*0.02;
		if(ABS(imu_data.inter_yaw)>=360.0f )//&&Distance - Dis_Ring >200&&Distance - Dis_Ring <250
		{
			Ring_state = Out_Ring;
			imu_data.inter_yaw= 0;
		}
		break;	
	case Out_Ring:
		//出环,无环状态
		Middle_Err=80.0f*(left_adc-right_adc)/(left_adc+right_adc+mid_adc);
		//出环条件
		if(mid_adc<=240 &&mid_adc>=150 && ABS(left_adc - right_adc)<=20&&left_adc<=150 &&right_adc<=150 )
			Ring_state  = No_Ring;
		
		imu_data.inter_yaw= 0;
		
		break;
	default:break;
	
	}
	
}
*/











