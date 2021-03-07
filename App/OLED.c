
/* @file       OLED.c
 * @brief      OLED�����ļ�
 * @author     ZCW
 * @version    v7.9
 * @date       2016-7-9
 */


//#include "common.h"
#include "include.h"
#include "OLED.h"





unsigned char str[10];


//�����ֿ�����
const unsigned char F6x8[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp 0
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *  10
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4  20
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >  30
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @  
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A  33
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G   
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H  40
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q   
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R  50
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [   
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55 60
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a  65
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f  70
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p  80
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z  90
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 },   // horiz lines
    { 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 },   //�ϻ���
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }    //ȫ��
};
 
 byte F16x16_Idx[] = 
{
   "sss" 
};

 byte F16x16[]=
{
0x12
};
/*************************************************************************
*  �������ƣ�delay_ms
*  ����˵������ʱ����
*  ����˵����ms
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/

void delay_ms(uint8 ms)      
{   
   int ii,jj;
   if (ms<1) ms=1;          
   for(ii=0;ii<ms;ii++)     
     for(jj=0;jj<1335;jj++);
}
/*************************************************************************
*  �������ƣ�OLED_WrCmd(unsigned char cmd)
*  ����˵����ģ��SPI����д������
*  ����˵����cmd
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/
void OLED_WrCmd(unsigned char cmd)
{
  unsigned char i;
  OLED_DC=0;
  for(i=0;i<8;i++) //����һ����λ���� 
  {
    if((cmd << i) & 0x80)
       {
          OLED_SDA  = 1;
       }
    else  
          OLED_SDA  = 0;
    
    OLED_SCL = 0;
    OLED_SCL = 1;
  }

  
}




/*************************************************************************
*  �������ƣ�OLED_WrDat(unsigned char dat)
*  ����˵����ģ��SPI����д������
*  ����˵����dat
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/



void OLED_WrDat(unsigned char dat)	 
{
  unsigned char i;
  OLED_DC=1;  
  for(i=0;i<8;i++) //����һ����λ���� 
  {
    if((dat << i) & 0x80)
       {
          OLED_SDA  = 1;
       }
    else  
          OLED_SDA  = 0;
    
    OLED_SCL = 0;
    OLED_SCL = 1;
  }
}



/*************************************************************************
*  �������ƣ�OLED_Set_Pos����
*  ����˵��������OLED��ʾ��λ��
*  ����˵����x,y
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
  OLED_WrCmd(0xb0+y);
  OLED_WrCmd(((x&0xf0)>>4)|0x10);
  OLED_WrCmd((x&0x0f));//|0x01); 
} 


/*************************************************************************
*  �������ƣ�OLED_Fill
*  ����˵������ʼ��ȫ��
*  ����˵����bmp_dat
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/

void OLED_Fill(unsigned char bmp_dat) 
{
  unsigned char y,x;
  for(y=0;y<8;y++)
  {
    OLED_WrCmd(0xb0+y);
    OLED_WrCmd(0x01);
    OLED_WrCmd(0x10);
    for(x=0;x<128;x++)
        OLED_WrDat(bmp_dat);
  }
}

/*************************************************************************
*  �������ƣ�OLED_clear()
*  ����˵������ʼ������ 
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/

void OLED_clear(void)
{
    unsigned char y,x;	
    for(y=0;y<8;y++)
    {
        OLED_WrCmd(0xb0+y);
        OLED_WrCmd(0x01);
        OLED_WrCmd(0x10); 
        for(x=0;x<128;x++)
            OLED_WrDat(0);
    }			
}



/*************************************************************************
*  �������ƣ�OLED_init������
*  ����˵����OLED��ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/

void OLED_init(void)
{
    gpio_init(PTD0,GPO,0);
    gpio_init(PTD1,GPO,0);
    gpio_init(PTD2,GPO,0);
    gpio_init(PTD3,GPO,0);
//    gpio_init(PTB9,GPO,0);
    OLED_SCL=1;
    OLED_RST=0;
    delay_ms(50);
    OLED_RST=1;       //���ϵ絽���濪ʼ��ʼ��Ҫ���㹻��ʱ�䣬���ȴ�RC��λ���   
    OLED_WrCmd(0xae);
    OLED_WrCmd(0x00);
    OLED_WrCmd(0x10);
    OLED_WrCmd(0x40);
    OLED_WrCmd(0x81);
    OLED_WrCmd(0xcf); 
    OLED_WrCmd(0xa1);
    OLED_WrCmd(0xc8);
    OLED_WrCmd(0xa6);
    OLED_WrCmd(0xa8);
    OLED_WrCmd(0x3f);
    OLED_WrCmd(0xd3);
    OLED_WrCmd(0x00);
    OLED_WrCmd(0xd5);
    OLED_WrCmd(0x80);
    OLED_WrCmd(0xd9);
    OLED_WrCmd(0xf1);
    OLED_WrCmd(0xda);
    OLED_WrCmd(0x12);
    OLED_WrCmd(0xdb);
    OLED_WrCmd(0x40);
    OLED_WrCmd(0x20);
    OLED_WrCmd(0x02);
    OLED_WrCmd(0x8d);
    OLED_WrCmd(0x14);
    OLED_WrCmd(0xa4);
    OLED_WrCmd(0xa6);
    OLED_WrCmd(0xaf);
    OLED_Fill(0x00);  
    OLED_Set_Pos(0,0);
}


/*************************************************************************
*  �������ƣ�OLED_P6x8Str����
*  ����˵������ʾ6*8���ַ�����
*  ����˵����x,y,ch[]
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/

void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char c=0,i=0,j=0;      
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>126){x=0;y++;}
        OLED_Set_Pos(x,y);    
    for(i=0;i<6;i++) 
    {
      OLED_WrDat(F6x8[c][i]);
    }
    x+=6;
    j++;
  }
}

void OLED_P1Int(unsigned char x,unsigned char y,int a)
{
  unsigned char i=0;      
   
    if(x>126){x=0;y++;}
        OLED_Set_Pos(x,y);    
    for(i=0;i<6;i++) 
    {
      OLED_WrDat(F6x8[abs(a)+16][i]);
    }
  
}

/*************************************************************************
*  �������ƣ�OLED_P14*16����
*  ����˵������ʾ14*16�ĺ���
*  ����˵����x,y,ch[]
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��
************************************************************************/

void OLED_P14x16(uint8 x,uint8 y,unsigned char a[])
{
  uint8 adder=0,wm=0;
  OLED_Set_Pos(x , y);
  for(wm = 0;wm < 14;wm++)
 {
   OLED_WrDat(a[adder]);
   adder += 1;
  }
   OLED_Set_Pos(x,y+1);
  for(wm = 0;wm < 14;wm++)
  {
    OLED_WrDat(a[adder]);
    adder += 1;
  }
}


/*************************************************************************
*  �������ƣ�OLED_P16*16����
*  ����˵������ʾ16*16�ĺ���
*  ����˵����x,y,ch[]
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��
************************************************************************/
void OLED_P16x16(uint8 x,uint8 y,unsigned char a[])
{
  uint8 adder=0,wm=0;
  OLED_Set_Pos(x , y);
  for(wm = 0;wm < 16;wm++)
 {
   OLED_WrDat(a[adder]);
   adder += 1;
  }
   OLED_Set_Pos(x,y+1);
  for(wm = 0;wm < 16;wm++)
  {
    OLED_WrDat(a[adder]);
    adder += 1;
  }
}


/*************************************************************************
*  �������ƣ�OLED_P16*16����
*  ����˵������ʾ16*16���ַ���
*  ����˵����x,y,ch[]
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��
************************************************************************/
void OLED_P16x16Str(byte x,byte y,byte ch[])
{
	byte wm=0,ii = 0;
	word adder=1; 
	
	while(ch[ii] != '\0')
	{
  	wm = 0;
  	adder = 1;
  	while(F16x16_Idx[wm] > 127)
  	{
  		if(F16x16_Idx[wm] == ch[ii])
  		{
  			if(F16x16_Idx[wm + 1] == ch[ii + 1])
  			{
  				adder = wm * 16;
  				break;
  			}
  		}
  		wm += 2;			
  	}
  	if(x>118){x=0;y+=2;}
  	OLED_Set_Pos(x , y); 
  	if(adder != 1)// ��ʾ����					
  	{
  		OLED_Set_Pos(x , y);
  		for(wm = 0;wm < 16;wm++)               
  		{
  			OLED_WrDat(F16x16[adder]);	
  			adder += 1;
  		}      
  		OLED_Set_Pos(x,y + 1); 
  		for(wm = 0;wm < 16;wm++)          
  		{
  			OLED_WrDat(F16x16[adder]);
  			adder += 1;
  		}   		
  	}
  	else			  //��ʾ�հ��ַ�			
  	{
  		ii += 1;
      OLED_Set_Pos(x,y);
  		for(wm = 0;wm < 16;wm++)
  		{
  				OLED_WrDat(0);
  		}
  		OLED_Set_Pos(x,y + 1);
  		for(wm = 0;wm < 16;wm++)
  		{   		
  				OLED_WrDat(0);	
  		}
  	}
  	x += 16;
  	ii += 2;
	}
}
/**=============================================================
*�������� void LCD_PutPixel(u8 x,u8 y)
*��������������һ���㣨x,y��
*��������ʵ����ֵ(x,y),x�ķ�Χ0��127��y�ķ�Χ0��64
*���أ���
===========================================================**/
void OLED_PutPixel(uint8 x,uint8 y)
{
	uint8 data1;  //data1��ǰ�������

         OLED_Set_Pos(x,y);
	data1 = 0x01<<(y%8);
	OLED_WrCmd(0xb0+(y>>3));
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f));//|0x00);
	OLED_WrDat(data1);
        
}
 


/**=============================================================
**�������� int_to_str(int a)
**������������һ����������ת�����ַ�����ʾ��
**����������a
**���أ��ַ����׵�ַ
=============================================================**/
void DisplayInt(uint8 x,uint8 y,int a)
{
   int i=0,j,m=1,n=1;
   
   if(a<0)
     a=-a;      //ֻ��ʾ����
   
   while(m!=0)
   {
     n=n*10;
     m=a/n;
     i++;
   }
   for(j=0;j<i;j++)
   {
      n=n/10;
      str[j]=(uint8)(a/n+48);
      a=a%n;
   }
   str[j]='\0';
   OLED_P6x8Str(x,y,str);
   
}


/**=============================================================
**�������� int_to_float(float a)
**������������һ��˫���ȸ���������ת�����ַ�����ʾ��
**����������a
**���أ��ַ����׵�ַ
=============================================================**/
void DisplayDou(uint8 x,uint8 y,double a)
{
  int temp;
  double temp1;
  int i=1,j=0,m=1,n=1;
  temp=abs((int)a);
  temp1=fabs(a)-temp;
  temp1=temp1*1000;
    
  while(m!=0)
  {
    n=n*10;
    m=temp/n;
    i++;
  }
  DisplayInt(x,y,temp);
  temp=(int)temp1;
  OLED_P6x8Str(x+(i-1)*6,y,".");
  n=1000;
  for(j=0;j<3;j++)
  {
    n=n/10;
    str[j]=(uint8)(temp/n+48);
    temp=temp%n;
   }
   str[j]='\0';
   OLED_P6x8Str(x+i*6,y,str);
}

/**=============================================================
**�������� OLED_CAMERA(uint8 *imgaddr,uint32 camera_w,uint32 camera_h)
**������������ͼ����ʾ��OLED��
**������
**���أ���
=============================================================**/
void OLED_CAMERA(uint8 *imgaddr,uint32 camera_w,uint32 camera_h)
{
    uint8 data,tmp;
    uint32 i,j,maj;
    uint8 n;
    //OLED_clear();
    OLED_Set_Pos(0,0);
    for(j=0;j<camera_h/8+1;j++)
    {
        OLED_Set_Pos(24,j);
        for(i=0;i<camera_w;i++)
        {
            data=0x00;
            for(n=0;n<8;n++)
            {
                maj=(j*8+n)*10+i/8;
                if(maj < camera_w*camera_h/8 )
                {    tmp=(imgaddr[maj] & ( 0x80 >> (i%8)))<<(i%8);}
                else
                {   tmp=(0x00 & ( 0x80 >> (i%8)))<<(i%8) ;}
                data=data | ((tmp >> 7) << n);
            }
            OLED_WrDat(data);
        }
    }
}



/**=============================================================
**�������� OLED_DrawDot(uint8 *imgaddr,uint32 camera_w,uint32 camera_h)
**������������ͼ����ʾ��OLED��
**������
**���أ���
=============================================================**/

void OLED_DrawDot(uint8 (*imgaddr)[60],uint8 camera_w,uint8 camera_h)
{

  uint8 w,h,data1=0x00,i;
  
  for(h=0;h<camera_h/8+1;h++){
  for(w=0;w<camera_w+1;w++){
    for(i=0;i<8;i++){
      if(imgaddr[h*8+i][w]<255){
        data1 = ((data1<<1)|0x01);
      }else{
       data1= (data1<<1);
      }
    }
    DisplayInt(0,0,data1);
    OLED_Set_Pos(w,h);
    OLED_WrDat(data1);
    data1=0x00;
  }
 }

}
//==============================================================
//�������� void LCD_Rectangle(u8 x1,u8 y1,
//                   u8 x2,u8 y2,u8 color,u8 gif)
//��������������һ��ʵ�ľ���
//���������Ͻ����꣨x1,y1��,���½����꣨x2��y2��
//      ����x1��x2�ķ�Χ0��127��y1��y2�ķ�Χ0��63������ʵ����ֵ
//���أ���
//==============================================================
void OLED_Rectangle(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 gif)
{
	uint8 n;

	OLED_Set_Pos(x1,y1>>3);
	for(n=x1;n<=x2;n++)
	{
		OLED_WrDat(~(0x00|(y1%8)));
		if(gif == 1) 	delay_ms(100);
	}
	OLED_Set_Pos(x1,y2>>3);
  for(n=x1;n<=x2;n++)
	{
		OLED_WrDat(~(0x00|(y1%8)));
		if(gif == 1) 	delay_ms(5);
	}

}


//*****************************************************************************/

//OLED��ʾ��ѹ��ͼ��

//*****************************************************************************/


void OLED_PrintImage(uint8 *pucTable, uint16 usRowNum, uint16 usColumnNum)
{
    uint8 ucData;
    uint16 i,j,k,m,n;
    uint16 usRowTmp;

    m = usRowNum >> 3;   //����ͼƬ������8λΪһ������������
    n = usRowNum % 8;    //����������ʣ�µ�����
    
    for(i = 0; i < m; i++) //��������ɨ��
    {
        OLED_Set_Pos(24,i);
        usRowTmp = i << 3;    //���㵱ǰ�����е��±�                  
        for(j = 0; j < usColumnNum; j++) //��ɨ��        
        {
            ucData = 0;
            for(k = 0; k < 8; k++) //��i���ж���8��ɨ��
            {
                ucData = ucData >> 1;
                if((pucTable + (usRowTmp + k) * usColumnNum)[j] == 0) ;
                else
                    ucData = ucData | 0x80;
            }
            OLED_WrDat(ucData);
        }
    }
    
    OLED_Set_Pos(24,i); //����ʣ�µ�����ʾ����ʼ����
    usRowTmp = i << 3;       //���㵱ǰ�����е��±�                  
    for(j = 0; j < usColumnNum; j++) //��ɨ��        
    {
        ucData = 0;
        for(k = 0; k < n; k++) //��ʣ�µ���ɨ��
        {
            ucData = ucData >> 1;
            if((pucTable + (usRowTmp + k) * usColumnNum)[j] == 0)
            {
                ucData = ucData | 0x80;
            }
            
        }
        ucData = ucData >> (8 - n);
        OLED_WrDat(~ucData);
    }

    return;
}



