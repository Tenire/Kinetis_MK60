#include "loop.h"


int i=0;
float out2[3]={0};

void duty_8ms()
{
	balance(getAngle(),mpu_gyro_y);
}
void duty_50ms()
{
	
}

void duty_500ms()
{
	/*out2[0]=adc_once(ADC0_DP0,ADC_16bit);
	out2[1]=adc_once(ADC0_DM0,ADC_16bit);
	if(out2[0]+out2[1]==0)
	{
		out2[2]=out2[0]*out2[1];
	}else
	{
		out2[2]=(out2[0]*out2[1])/(out2[0]+out2[1]);
	}
	vcan_sendware(out2,sizeof(out2));
	printf("%f\n",out2[0]);
	printf("%f\n",out2[1]);
	printf("%f\n",out2[2]);*/
}

void loop_run()
{
	if(i%8==0)
	{
		duty_8ms();
	}
	if(i%500==0)
	{
		duty_500ms();
	}
	if(i%50==0)
	{
		duty_50ms();
	}
	if(i>=500)
	{
		i=0;
	}else
	{
		i++;
	}
}