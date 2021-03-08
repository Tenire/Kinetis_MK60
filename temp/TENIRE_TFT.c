#include "TENIRE_TFT.h"



void draw_picture(uint8 x,uint8 y,uint16 w,uint16 h,const uint16 *pimage_io)
{
	unsigned int i,j;
	if(w>TFT18W||h>TFT18H)
	{
		TFTSPI_Set_Pos(x,y,x+TFT18W-1,y+TFT18H);
	}else
	{
		TFTSPI_Set_Pos(x,y,x+w-1,y+h);
	}
	for(i=0;i<w*h;i++)
	{ 			
		//TFTSPI_Write_Word(pimage_io[i]); //高位在前
	}
	for(i=0;i<h;i++)
	{
		if(i>=TFT18H-y)
		{
			break;
		}
		for(j=0;j<w;j++)
		{
			if(j>=TFT18W-x)
			{
				break;
			}
			TFTSPI_Write_Word(pimage_io[i*w+j]); //高位在前
		}
	}
}