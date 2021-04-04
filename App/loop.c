#include "loop.h"



int i=0;
float out2[5]={0};
int j=0;

void duty_4ms()
{
	getAngle();
}

void duty_8ms()
{
	balance();
	turn();
	
	if(onTheTrack)
	{
		motorControl(balance_pwm+turn_pwm,balance_pwm-turn_pwm);
	}
	else
	{
		motorControl(0,0);
	}
	
}
void duty_80ms()
{
	if(j>2000)
	{
		velocity();
	}
}

void duty_500ms()
{
	
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
	if(i%80==0)
	{
		duty_80ms();
	}
	if(i&500==0)
	{
		duty_500ms();
	}
	if(i>=79)
	{
		i=0;
	}else
	{
		i++;
	}
	j++;
}










