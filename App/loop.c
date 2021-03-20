#include "loop.h"



int i=0;
int out2[3];
int j=0;

void duty_4ms()
{
	getAngle();
}

void duty_8ms()
{
	balance(angle,mpu_gyro_y);
}
void duty_40ms()
{
	velocity();;
	out[5]=i;
}

void duty_500ms()
{
	out2[0]=adc_once(ADC1_DM1,ADC_16bit);
	out2[1]=adc_once(ADC1_DP1,ADC_16bit);
	if(out2[0]+out2[1]==0)
	{
		out2[2]=out2[0]*out2[1];
	}else
	{
		out2[2]=(out2[0]*out2[1])/(out2[0]+out2[1]);
	}
	out2[0]=0;
	out2[1]=0;
	out2[2]=0;
	vcan_sendware(out2,sizeof(out2));
	printf("%d\n",out2[0]);
	printf("%d\n",out2[1]);
	printf("%d\n",out2[2]);
}

void loop_run()
{
	if(i%4==0)
	{
		duty_4ms();
	}
	if(i%8==0)
	{
		duty_8ms();
	}
	if(i%40==0)
	{
		duty_40ms();
	}
	if(i>=39)
	{
		i=0;
	}else
	{
		i++;
	}
	
	if(j==0)
	{
		setSpeed(0);
	}
	if(j>10000)
	{
		setSpeed(50);
	}
	if(j>12000)
	{
		setSpeed(-25);
	}
	if(j>13000)
	{
		setSpeed(0);
	}
	if(j>18000)
	{
		setSpeed(-50);
	}
	if(j>20000)
	{
		setSpeed(0);
	}
	j++;
}










