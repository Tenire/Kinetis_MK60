#include "OLED_list.h"
#include "string.h"
#include "SEEKFREE_OLED.h"
/*在一级或部分菜单中去除右边五向导航的位置标志*/

double motor_P;
double motor_I;
double motor_D;
double servo_P;
double servo_D;
int desired_value;
int servo_mid;



void Clean_Key_Position()
{
  uint8 y=0,i=0;
  for(y=0;y<=7;y++)
  {
    OLED_Set_Pos(120,y);
    for(i=0;i<6;i++) 
    {
      OLED_WrDat(F6x8[0][i]);
    }
  }
}

/************************************************
  功能说明：uint8类型字符串的赋值函数
  参数说明：uint8形字符串首地址，常字符串首地址
  制作者：YB
  修改日期：17/12/01
  参考调用：strcpy_rewrite(str,"asd");
           可将字符串asd赋值给字符串变量str
*************************************************/
void strcpy_rewrite(uint8* ch,const char* _ch)
{
  while((*ch++ = *_ch++)!='\0');
}


/*在修改PID参数时显示五向导航光标*/
void Show_Key_Position(uint8 x,uint8 y)
{
  uint8 i=0;
  OLED_Set_Pos(x,y);
    for(i=0;i<6;i++) 
    {
      OLED_WrDat(F6x8[93][i]);
    }
}

/*在修改PID参数时改变五向导航光标*/
void Change_Key_Position(uint8 x,uint8 y)
{
    uint8 i;
    uint8 a=0,b=1;
    
    for(a=0;a<=24;a+=6)
    {
      OLED_Set_Pos(a,b);
      for(i=0;i<6;i++) 
      {
        OLED_WrDat(F6x8[0][i]);
      }
    }
    Show_Key_Position(x,y);
}


/*一级菜单*/
void firstlist()
{
  uint8 i=0,j=0;
  OLED_clear();
  uint8 str[22]="Wellcome";
  uint8 x=0,y=0;
  OLED_P6x8Str(x,y,str);
  //1
  x=0;y++;
  strcpy_rewrite(str,"1.Set Motor_PID");
  OLED_P6x8Str(x,y,str);
  //2
  x=0;y++;
  strcpy_rewrite(str,"2.Set Servo_PD");
  OLED_P6x8Str(x,y,str);
  //3
  x=0;y++;
  strcpy_rewrite(str,"3.Set Servo Mid");
  OLED_P6x8Str(x,y,str);
  //4
  x=0;y++;
  strcpy_rewrite(str,"4.Not used");
  OLED_P6x8Str(x,y,str);
  //5
  x=0;y++;
  strcpy_rewrite(str,"5.Go!");
  OLED_P6x8Str(x,y,str);
  
  DELAY_MS(500);
  
  while(i!=1)
  {
    DELAY_MS(200);
    if(key_check(KEY_U)==KEY_DOWN && j!=0)
    {
      j--;
    }
    else if(key_check(KEY_U)==KEY_DOWN && j==0)
    {
      j+=4;
    }
    if(key_check(KEY_D)==KEY_DOWN && j!=4)
    {
      j++;
    }
    else if(key_check(KEY_D)==KEY_DOWN && j==4)
    {
      j=0;
    }
    
    Clean_Key_Position();
    strcpy_rewrite(str,"<");
    OLED_P6x8Str(120,j+1,str);
    
    if(key_check(KEY_A)==KEY_DOWN)
      switch (j){
      case 0:Set_Mt_PID_list();break;
      case 1:Set_Sv_PID_list();break;
      case 2:Set_Sv_MID_list();break;
      case 3:break;
      case 4:i=1;OLED_clear();DELAY_MS(1000);break;
      }
  }
}
void Set_Sv_MID_list()
{
  Set_Parameters_int(&servo_mid);
  ftm_pwm_duty(FTM2, FTM_CH0,servo_mid);
  int x=0,y=0;
  OLED_clear();
  strcpy_rewrite(str,"Wellcome");
  OLED_P6x8Str(x,y,str);
  //1
  x=0;y++;
  strcpy_rewrite(str,"1.Set Motor_PID");
  OLED_P6x8Str(x,y,str);
  //2
  x=0;y++;
  strcpy_rewrite(str,"2.Set Servo_PD");
  OLED_P6x8Str(x,y,str);
  //3
  x=0;y++;
  strcpy_rewrite(str,"3.Set Servo Mid");
  OLED_P6x8Str(x,y,str);
  //4
  x=0;y++;
  strcpy_rewrite(str,"4.Not used");
  OLED_P6x8Str(x,y,str);
  //5
  x=0;y++;
  strcpy_rewrite(str,"5.Go!");
  OLED_P6x8Str(x,y,str);
}

/*二级菜单（一），修改pid参数*/
void Set_Mt_PID_list()
{
  uint8 i=0,j=0;
  OLED_clear();
  uint8 str[22];
  uint8 x=0,y=0;
  
  strcpy_rewrite(str,"P=");
  OLED_P6x8Str(x,y,str);
  DisplayDou(x+12,y,motor_P);
  
  x=0;y++;
  strcpy_rewrite(str,"I=");
  OLED_P6x8Str(x,y,str);
  DisplayDou(x+12,y,motor_I);
  
  x=0;y++;
  strcpy_rewrite(str,"D=");
  OLED_P6x8Str(x,y,str);
  DisplayDou(x+12,y,motor_D);
  //3
  x=0;y++;
  strcpy_rewrite(str,"1.Set P Control");
  OLED_P6x8Str(x,y,str);
  //4
  x=0;y++;
  strcpy_rewrite(str,"2.Set I Control");
  OLED_P6x8Str(x,y,str);
  //5
  x=0;y++;
  strcpy_rewrite(str,"3.Set D Control");
  OLED_P6x8Str(x,y,str);
  //6
  x=0;y++;
  strcpy_rewrite(str,"4.back");
  OLED_P6x8Str(x,y,str);
  
  DELAY_MS(500);
  
  while(i!=4)
  {
    if(key_check(KEY_U)==KEY_DOWN && j!=0)
      j--;
    else if(key_check(KEY_U)==KEY_DOWN && j==0)
      j+=3;
    if(key_check(KEY_D)==KEY_DOWN && j!=3)
      j++;
    else if(key_check(KEY_D)==KEY_DOWN && j==3)
      j-=3;
    
    Clean_Key_Position();
    strcpy_rewrite(str,"<");
    OLED_P6x8Str(120,j+3,str);
    
    if(key_check(KEY_A)==KEY_DOWN)
    {
      switch (j){
      case 0:Set_Parameters_list(&motor_P);break;
      case 1:Set_Parameters_list(&motor_I);break;
      case 2:Set_Parameters_list(&motor_D);break;
      case 3:i=3;break;
      }
      x=0,y=0;
      OLED_clear();
      strcpy_rewrite(str,"Wellcome");
      OLED_P6x8Str(x,y,str);
      //1
      x=0;y++;
      strcpy_rewrite(str,"1.Set Motor_PID");
      OLED_P6x8Str(x,y,str);
      //2
      x=0;y++;
      strcpy_rewrite(str,"2.Set Servo_PD");
      OLED_P6x8Str(x,y,str);
      //3
      x=0;y++;
      strcpy_rewrite(str,"3.Set Servo Mid");
      OLED_P6x8Str(x,y,str);
      //4
      x=0;y++;
      strcpy_rewrite(str,"4.Not used");
      OLED_P6x8Str(x,y,str);
      //5
      x=0;y++;
      strcpy_rewrite(str,"5.Go!");
      OLED_P6x8Str(x,y,str);
      break;
    } 
  
    DELAY_MS(200);
   }
}

/*二级菜单（一），修改舵机参数*/
void Set_Sv_PID_list()
{
  uint8 i=0,j=0;
  OLED_clear();
  uint8 str[22];
  uint8 x=0,y=0;
  
  strcpy_rewrite(str,"P=");
  OLED_P6x8Str(x,y,str);
  DisplayDou(x+12,y,servo_P);
  
  x=0;y++;
  strcpy_rewrite(str,"S=");
  OLED_P6x8Str(x,y,str);
  DisplayDou(x+12,y,desired_value);
  
  x=0;y++;
  strcpy_rewrite(str,"D=");
  OLED_P6x8Str(x,y,str);
  DisplayDou(x+12,y,servo_D);
  //3
  x=0;y++;
  strcpy_rewrite(str,"1.Set P Control");
  OLED_P6x8Str(x,y,str);
  //4
  x=0;y++;
  strcpy_rewrite(str,"2.Set Speed");
  OLED_P6x8Str(x,y,str);
  //5
  x=0;y++;
  strcpy_rewrite(str,"3.Set D Control");
  OLED_P6x8Str(x,y,str);
  //6
  x=0;y++;
  strcpy_rewrite(str,"4.back");
  OLED_P6x8Str(x,y,str);
  
  DELAY_MS(500);
  
  while(i!=4)
  {
    if(key_check(KEY_U)==KEY_DOWN && j!=0)
      j--;
    else if(key_check(KEY_U)==KEY_DOWN && j==0)
      j+=3;
    if(key_check(KEY_D)==KEY_DOWN && j!=3)
      j++;
    else if(key_check(KEY_D)==KEY_DOWN && j==3)
      j-=3;
    
    Clean_Key_Position();
    strcpy_rewrite(str,"<");
    OLED_P6x8Str(120,j+3,str);
    
    if(key_check(KEY_A)==KEY_DOWN)
    {
      switch (j){
      case 0:Set_Parameters_list(&servo_P);break;
      case 1:Set_Parameters_int(&desired_value);break;
      case 2:Set_Parameters_list(&servo_D);break;
      case 3:i=3;break;
      }
      x=0,y=0;
      OLED_clear();
      strcpy_rewrite(str,"Wellcome");
      OLED_P6x8Str(x,y,str);
      //1
      x=0;y++;
      strcpy_rewrite(str,"1.Set Motor_PID");
      OLED_P6x8Str(x,y,str);
      //2
      x=0;y++;
      strcpy_rewrite(str,"2.Set Servo_PD");
      OLED_P6x8Str(x,y,str);
      //3
      x=0;y++;
      strcpy_rewrite(str,"3.Set Servo Mid");
      OLED_P6x8Str(x,y,str);
      //4
      x=0;y++;
      strcpy_rewrite(str,"4.Not used");
      OLED_P6x8Str(x,y,str);
      //5
      x=0;y++;
      strcpy_rewrite(str,"5.Go!");
      OLED_P6x8Str(x,y,str);
      break;
    } 
  
    DELAY_MS(200);
   }
}

/*三级菜单，修改P参数*/
void Set_Parameters_list(double *Par)
{
  uint8 x=0,y=0;
  uint8 i=0,j=0;
  
  OLED_clear();
  DisplayDou(x,y,*Par);
  x=0;y++;
  Change_Key_Position(x,y);
  
  DELAY_MS(500);
  
  while (j!=2)
  {
    DELAY_MS(150);
    if(key_check(KEY_U) ==  KEY_DOWN)
    {
      switch(x){
      case 0:(*Par)+=1;break;
      case 6:(*Par)+=5;break;
      case 12:(*Par)+=10;break;
      case 18:(*Par)+=0.1;break;
      case 24:(*Par)+=0.001;break;
      }
      i=x;
      x=0;y=0;
      DisplayDou(x,y,*Par);
      x=i;y++;
      Change_Key_Position(x,y);
    }
    if(key_check(KEY_D) ==  KEY_DOWN)
    {
      switch(x){
      case 0:(*Par)-=1;break;
      case 6:(*Par)-=5;break;
      case 12:(*Par)-=10;break;
      case 18:(*Par)-=0.1;break;
      case 24:(*Par)-=0.01;break;
      }
      i=x;
      x=0;y=0;
      DisplayDou(x,y,*Par);
      x=i;y++;
      Change_Key_Position(x,y);
    }
    if(key_check(KEY_L) ==  KEY_DOWN)
    {
      if(x==0)   x=24;
      else      x-=6;
      Change_Key_Position(x,y);
    }
    if(key_check(KEY_R) ==  KEY_DOWN)
    {
      if(x==24)   x=0;
      else      x+=6;
      Change_Key_Position(x,y);
    }
    
    if(key_check(KEY_A) ==  KEY_DOWN)
    {
      j=2;
    }
  }
}

void Set_Parameters_int(int *Par)
{
  uint8 x=0,y=0;
  uint8 i=0,j=0;
  
  OLED_clear();
  DisplayDou(x,y,*Par);
  x=0;y++;
  Change_Key_Position(x,y);
  
  DELAY_MS(500);
  
  while (j!=2)
  {
    DELAY_MS(150);
    if(key_check(KEY_U) ==  KEY_DOWN)
    {
      switch(x){
      case 0:(*Par)+=1;break;
      case 6:(*Par)+=5;break;
      case 12:(*Par)+=10;break;
      case 18:(*Par)+=0.1;break;
      case 24:(*Par)+=0.001;break;
      }
      i=x;
      x=0;y=0;
      DisplayDou(x,y,*Par);
      x=i;y++;
      Change_Key_Position(x,y);
    }
    if(key_check(KEY_D) ==  KEY_DOWN)
    {
      switch(x){
      case 0:(*Par)-=1;break;
      case 6:(*Par)-=5;break;
      case 12:(*Par)-=10;break;
      case 18:(*Par)-=0.1;break;
      case 24:(*Par)-=0.01;break;
      }
      i=x;
      x=0;y=0;
      DisplayDou(x,y,*Par);
      x=i;y++;
      Change_Key_Position(x,y);
    }
    if(key_check(KEY_L) ==  KEY_DOWN)
    {
      if(x==0)   x=24;
      else      x-=6;
      Change_Key_Position(x,y);
    }
    if(key_check(KEY_R) ==  KEY_DOWN)
    {
      if(x==24)   x=0;
      else      x+=6;
      Change_Key_Position(x,y);
    }
    
    if(key_check(KEY_A) ==  KEY_DOWN)
    {
      j=2;
    }
  }
}




