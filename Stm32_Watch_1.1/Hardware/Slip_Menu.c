/*
这是滑动菜单的页面

*/

#include "stm32f10x.h"                  // Device header
#include "MyRTC.h"
#include "OLED.h"
#include "KEY.h"
#include "Slip_Menu.h"
#include "Menu.h"
#include "Dino.h"
#include <math.h>
#include "Delay.h"
#include "MPU6050.h"
#include "LED.h"
#include "Timer.h"

extern uint8_t KeyNum;	//KeyNum在Menu.c中定义

/*
	菜单第一页UI
*/
void Slip_FirstMenu(void)		//32*3=96  退出，秒表，心情   放在第一页
{
	OLED_ShowImage(5,15,32,32,Slip_Return);
	OLED_ShowImage(45,15,32,32,Slip_Clock);
	OLED_ShowImage(83,15,32,32,Slip_Mood);
}

/*
	菜单第二页UI
*/
void Slip_SecondMenu(void)		//32*2=64  游戏，水平仪 手电筒 放在右边第二页
{
	OLED_ShowImage(5,15,32,32,Slip_Game);
	OLED_ShowImage(45,15,32,32,Level);
	OLED_ShowImage(83,15,32,32,Light);
}

/*-------------------------秒表-----------------------*/

/*
	秒表UI
*/
uint8_t watch_hour,watch_min,watch_sec;
void Slip_Show_StopWatch_Menu(void)
{
	OLED_ShowImage(0,0,16,16,Enter_Return);
	OLED_Printf(32,20,OLED_8X16,"%02d:%02d:%02d",watch_hour,watch_min,watch_sec);
	OLED_ShowString(8,44,"开始",OLED_8X16);
	OLED_ShowString(48,44,"停止",OLED_8X16);
	OLED_ShowString(88,44,"清除",OLED_8X16);
}

/*
	秒表计数实现
*/
uint8_t start_timing_flag;   //1:开始 0:停止
void StopWatch_Tick(void)
{
	static uint16_t watch_count;
	watch_count++;
	if(watch_count>=1000)
	{
		watch_count=0;
		if(start_timing_flag==1)
		{
			watch_sec++;
			if(watch_sec>=60)	
			{
				watch_min++;
				if(watch_min>=60)
				{
					watch_hour++;
					if(watch_hour>99)	watch_hour=0;
				}
			}
		}
	}
}

/*
	秒表选项
*/
uint8_t watch_flag=1;
void Slip_StopWatch(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		uint8_t watch_setflag_temp=0;
		if(KeyNum==1)		//上一项
		{
			watch_flag--;
			if(watch_flag<=0)
				watch_flag=4;
		}
		else if(KeyNum==2)		//下一项
		{
			watch_flag++;
			if(watch_flag>=5)
				watch_flag=1;
		}
		else if(KeyNum==3)		//确认
		{
			OLED_Clear();
			OLED_Update();
			watch_setflag_temp=watch_flag;
		}
		
		if(watch_setflag_temp==1)
		{
			return;
		}
		else if(watch_setflag_temp==2)
		{
			start_timing_flag=1;
		}
		else if(watch_setflag_temp==3)
		{
			start_timing_flag=0;
		}
		else if(watch_setflag_temp==4)
		{
			start_timing_flag=0;
			watch_hour=watch_min=watch_sec=0;
		}
		
		switch(watch_flag)
		{
			case 1:
				Slip_Show_StopWatch_Menu();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			case 2:
				Slip_Show_StopWatch_Menu();
				OLED_ReverseArea(8,44,32,16);
				OLED_Update();
				break;
			case 3:
				Slip_Show_StopWatch_Menu();
				OLED_ReverseArea(48,44,32,16);
				OLED_Update();
				break;
			case 4:
				Slip_Show_StopWatch_Menu();
				OLED_ReverseArea(88,44,32,16);
				OLED_Update();
				break;
		}
	}
}


/*-------------------------设置首页心情-----------------------*/

/*
	心情UI
*/
void Slip_Show_MoodMenu(void)
{
	OLED_ShowImage(5,15,32,32,Big_Simle);
	OLED_ShowImage(45,15,32,32,Big_Unhappy);
	OLED_ShowImage(83,15,32,32,Big_Anxiety);
}

/*
	心情选项
*/
uint8_t mood_flag=1;
void Slip_SetMood(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		uint8_t mood_setflag_temp=0;
		if(KeyNum==1)		//上一项
		{
			mood_flag--;
			if(mood_flag<=0)
				mood_flag=3;
		}
		else if(KeyNum==2)		//下一项
		{
			mood_flag++;
			if(mood_flag>=4)
				mood_flag=1;
		}
		else if(KeyNum==3)		//确认
		{
			OLED_Clear();
			OLED_Update();
			mood_setflag_temp=mood_flag;
		}
		
		if(mood_setflag_temp==1)
		{
			Set_MoodChoice(1);
			return;
		}
		else if(mood_setflag_temp==2)
		{
			Set_MoodChoice(2);
			return;
		}
		else if(mood_setflag_temp==3)
		{
			Set_MoodChoice(3);
			return;
		}
		
		switch(mood_flag)
		{
			case 1:
				OLED_Clear();
				OLED_ShowImage(3,13,36,36,Frame);
				Slip_Show_MoodMenu();
				OLED_Update();
				break;
			case 2:
				OLED_Clear();
				OLED_ShowImage(42,13,36,36,Frame);
				Slip_Show_MoodMenu();
				OLED_Update();
				break;
			case 3:
				OLED_Clear();
				OLED_ShowImage(81,13,36,36,Frame);
				Slip_Show_MoodMenu();
				OLED_Update();
				break;
		}
	}
}


/*-------------------------玩游戏-----------------------*/

/*
	游戏UI
*/
void Slip_ShowGmaeUI(void)
{
	OLED_ShowImage(0,0,16,16,Enter_Return);
	OLED_ShowString(0,20,"谷歌恐龙小游戏",OLED_8X16);
}

/*
	选择游戏玩（目前只有一个）
	1.谷歌恐龙小游戏（不行玩了，退出可以按KEY3）
*/
uint8_t game_flag=1;
void Slip_PlayGame(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		uint8_t game_flag_temp=0;
		if(KeyNum==1)
		{
			game_flag--;
			if(game_flag<=0)	game_flag=2;
		}
		else if(KeyNum==2)
		{
			game_flag++;
			if(game_flag>=3)	game_flag=1;
		}
		else if(KeyNum==3)
		{
			game_flag_temp=game_flag;
			OLED_Clear();
		}
		
		if(game_flag_temp==1)
		{
			return;
		}
		else if(game_flag_temp==2)
		{
			DinoGame_Animation();
		}
		switch(game_flag)
		{
			case 1:
				Slip_ShowGmaeUI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			case 2:
				Slip_ShowGmaeUI();
				OLED_ReverseArea(0,20,112,16);
				OLED_Update();
				break;
		}
	}
	
}


/*-------------------------玩水平仪-----------------------*/

int16_t ax,ay,az,gx,gy,gz;//MPU6050测得的三轴加速度和角速度
float roll_g,pitch_g,yaw_g;//陀螺仪解算的欧拉角
float roll_a,pitch_a;//加速度计解算的欧拉角
float Roll,Pitch,Yaw;//互补滤波后的欧拉角
float a=0.9;//互补滤波器系数
float Delta_t=0.005;//采样周期

//通过MPU6050的数据进行姿态解算的函数
void MPU6050_Calculation(void)
{
	Delay_ms(5);
	MPU6050_GetData(&ax,&ay,&az,&gx,&gy,&gz);
	
	//通过陀螺仪解算欧拉角
	roll_g=Roll+(float)gx*Delta_t;
	pitch_g=Pitch+(float)gy*Delta_t;
	yaw_g=Yaw+(float)gz*Delta_t;
	
	//通过加速度计解算欧拉角
	pitch_a=atan2((-1)*ax,az)*180/PI;
	roll_a=atan2(ay,az)*180/PI;
	
	//通过互补滤波器进行数据融合
	Roll=a*roll_g+(1-a)*roll_a;
	Pitch=a*pitch_g+(1-a)*pitch_a;
	Yaw=a*yaw_g;
	
}

/*
	水平仪UI
*/
void Slip_ShowGradienter_UIMenu(void)
{
	MPU6050_Calculation();
	OLED_DrawCircle(64,32,30,0);
	OLED_DrawCircle(64-Roll,32+Pitch,4,1);
}

/*
	玩水平仪
	如果想要退出按Key3
*/
void Slip_PlayGradienter(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==3)
		{
			OLED_Clear();
			OLED_Update();
			return;
		}
		OLED_Clear();
		Slip_ShowGradienter_UIMenu();
		OLED_Update();
	}
}


/*-------------------------手电-----------------------*/

/*
	手电UI第一页
*/
void Show_LightUI1(void)
{
	OLED_ShowImage(0,0,16,16,Enter_Return);
	OLED_ShowString(0,20,"常亮模式开关",OLED_8X16);
	OLED_ShowString(0,40,"呼吸灯模式开关",OLED_8X16);
}

/*
	手电UI第二页
*/
void Show_LightUI2(void)
{
	OLED_ShowString(0,0,"闪烁开关",OLED_8X16);
}

/*
	手电选项
*/
uint8_t light_mode=1;  //1:没开  2:开了呼吸    3:开了闪烁
uint8_t light_flag=1;
void Slip_Light(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	while(1)
	{
		KeyNum=Key_GetNum();
		uint8_t light_flag_temp=0;
		if(KeyNum==1)
		{
			light_flag--;
			if(light_flag<=0)	light_flag=4;
		}
		else if(KeyNum==2)
		{
			light_flag++;
			if(light_flag>=5)	light_flag=1;
		}
		else if(KeyNum==3)
		{
			light_flag_temp=light_flag;
		}
		
		if(light_flag_temp==1)
		{
			return;
		}
		else if(light_flag_temp==2)
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			LED1_Turn();
		}
		else if(light_flag_temp==3)
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			if(light_mode==1)
				light_mode=2;
			else if(light_mode==2)
			{
				light_mode=1;
				PWN_SetCompare1(0);
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			}
		}
		else if(light_flag_temp==4)
		{
			if(light_mode==1)
			{
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				light_mode=3;
			}
			else if(light_mode==3)
			{
				LED1_OFF();
				LED1_OFF();
				light_mode=1;
			}
		}
		
		switch(light_flag)
		{
			case 1:
				OLED_Clear();
				Show_LightUI1();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			case 2:
				OLED_Clear();
				Show_LightUI1();
				OLED_ReverseArea(0,20,96,16);
				OLED_Update();
				break;
			case 3:
				OLED_Clear();
				Show_LightUI1();
				OLED_ReverseArea(0,40,112,16);
				OLED_Update();
				break;
			case 4:
				OLED_Clear();
				Show_LightUI2();
				OLED_ReverseArea(0,0,64,16);
				OLED_Update();
				break;
			
		}
			
	}
}

/*
	灯闪烁模式实现
*/
void Flash_Tick(void)
{
	static uint16_t flash_count;
	flash_count++;
	flash_count%=1000;		//这是到1000就会回到0了，利用了取模的特性
	if(flash_count>500)
	{
		LED1_OFF();
	}
	else
	{
		LED1_ON();
	}
}

