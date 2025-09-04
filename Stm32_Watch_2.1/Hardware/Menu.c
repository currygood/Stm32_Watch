/*
整个菜单
*/
#include "stm32f10x.h"                  // Device header
#include "MyRTC.h"
#include "Delay.h"
#include "Menu.h"
#include "OLED.h"
#include "Key.h"
#include "LED.h"
#include "SetTime.h"
#include "Slip_Menu.h"
#include "MPU6050.h"

int clkflag=1;		//菜单处1，设置处2

/*
	整个项目的初始化都在这里调用了
*/
void Menu_Init(void)
{
	OLED_Init();
	MyRTC_Init();
	Key_Init();
	LED_Init();
	MPU6050_Init();
}

/*---------------首页时钟----------------*/
uint8_t mood_choice=MOOD_HAPPY;

void Set_MoodChoice(uint8_t Choice)
{
	mood_choice=Choice;
}

void Show_Clock_UIMenu(void)
{
	MTimer Ti;
	MyRTC_ReadTime(&Ti);
	OLED_Printf(0,0,OLED_6X8,"%d-%d-%d",Ti.year,Ti.month,Ti.day);
	OLED_Printf(16,16,OLED_12X24,"%02d:%02d:%02d",Ti.hour,Ti.minute,Ti.second);
	OLED_Printf(5,48,OLED_8X16,"菜单");
	OLED_Printf(91,48,OLED_8X16,"设置");
	
	if(mood_choice==MOOD_HAPPY)
	{
		OLED_ShowImage(107,0,16,16,Simle);
	}
	else if(mood_choice==MOOD_UNHAPPY)
	{
		OLED_ShowImage(107,0,16,16,Unhappy);
	}
	else if(mood_choice==MOOD_ANXIETY)
	{
		OLED_ShowImage(107,0,16,16,Anxiety);
	}
}

int First_Page_Clock(void)
{
	while(1)
	{
		if(Key_Check(0,KEY_SINGLE))	//单机按下了，上一项
		{
			clkflag--;
			if(clkflag<=0)
				clkflag=2;
		}
		else if(Key_Check(1,KEY_SINGLE))	//单机按下了，下一项
		{
			clkflag++;
			if(clkflag>=3)
				clkflag=1;
		}
		else if(Key_Check(2,KEY_SINGLE))	//单机按下了，确认
		{
			OLED_Clear();
			OLED_Update();
			return clkflag;
		}
		else if(Key_Check(2,KEY_LONG))		//进入停止模式，关闭OLED屏幕
		{
			OLED_Clear();
			OLED_Update();
			//进入停止模式，降低功耗
			PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);
			SystemInit();
		}

		switch(clkflag)		//显示
		{
			case 1:
				Show_Clock_UIMenu();
				OLED_ReverseArea(5,48,32,16);
				OLED_Update();
				break;
			case 2:
				Show_Clock_UIMenu();
				OLED_ReverseArea(91,48,32,16);
				OLED_Update();
				break;
		}
	}
}

/*-----------------------设置界面-----------------------*/
/*
退出
日期时间设置
*/
int setflag=1;		//当前设置选项的选择设置第几个，默认是1，1是退回上一个界面

void Show_SettingPage_UIMenu(void)
{
	OLED_ShowImage(0,0,16,16,Enter_Return);
	OLED_ShowString(0,16,"日期时间设置",OLED_8X16);
}

/*
	设置的界面
*/
void SettingPage(void)
{
	while(1)
	{
		uint8_t setflag_temp=0;
		if(Key_Check(0,KEY_SINGLE))	//单机按下了，上一项
		{
			setflag--;
			if(setflag<=0)
				setflag=2;
		}
		else if(Key_Check(1,KEY_SINGLE))	//单机按下了，下一项
		{
			setflag++;
			if(setflag>=3)
				setflag=1;
		}
		else if(Key_Check(2,KEY_SINGLE))	//单机按下了，确认
		{
			OLED_Clear();
			OLED_Update();
			setflag_temp=setflag;
		}
		
		if(setflag_temp==1)		//确认了退出选项
		{
			return;
		}
		else if(setflag_temp==2)	//确认进入设置时间
		{
			SetTime();
		}
		
		switch(setflag)			//移动光标显示
		{
			case 1:
				Show_SettingPage_UIMenu();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			case 2:
				Show_SettingPage_UIMenu();
				OLED_ReverseArea(0,16,96,16);
				OLED_Update();
				break;
		}
	}
}


/*-----------------------滑动菜单界面-----------------------*/
/*退出 秒表 首页心情表情 游戏 水平仪*/
uint8_t slip_menu_flag=1;		//目前选择了几个

/*
	滑动菜单的界面
*/
int Slip_Menu(void)
{
	while(1)
	{
		uint8_t slip_menu_setflag_temp=0;
		if(Key_Check(0,KEY_SINGLE))	//单机按下了，上一项
		{
			slip_menu_flag--;
			if(slip_menu_flag<=0)
				slip_menu_flag=6;
		}
		else if(Key_Check(1,KEY_SINGLE))	//单机按下了，下一项
		{
			slip_menu_flag++;
			if(slip_menu_flag>=7)
				slip_menu_flag=1;
		}
		else if(Key_Check(2,KEY_SINGLE))	//单机按下了，确认
		{
			OLED_Clear();
			OLED_Update();
			slip_menu_setflag_temp=slip_menu_flag;
		}
		
		if(slip_menu_setflag_temp==1)		//确认退出
		{
			return 0;
		}
		else if(slip_menu_setflag_temp==2)		//确认进入计时器
		{
			Slip_StopWatch();
		}
		else if(slip_menu_setflag_temp==3)		//确认进入首页心情
		{
			Slip_SetMood();
		}
		else if(slip_menu_setflag_temp==4)		//确认进入游戏
		{
			Slip_PlayGame();
		}
		else if(slip_menu_setflag_temp==5)		//确认进入水平仪
		{
			Slip_PlayGradienter();
		}
		else if(slip_menu_setflag_temp==6)		//确认进入灯
		{
			Slip_Light();
		}
		
		switch(slip_menu_flag)			//移动方框显示
		{
			case 1:
				OLED_Clear();
				OLED_ShowImage(3,13,36,36,Frame);
				Slip_FirstMenu();
				OLED_Update();
				break;
			case 2:
				OLED_Clear();
				OLED_ShowImage(42,13,36,36,Frame);
				Slip_FirstMenu();
				OLED_Update();
				break;
			case 3:
				OLED_Clear();
				OLED_ShowImage(81,13,36,36,Frame);
				Slip_FirstMenu();
				OLED_Update();
				break;
			case 4:
				OLED_Clear();
				OLED_ShowImage(3,13,36,36,Frame);
				Slip_SecondMenu();
				OLED_Update();
				break;
			case 5:
				OLED_Clear();
				OLED_ShowImage(42,13,36,36,Frame);
				Slip_SecondMenu();
				OLED_Update();
				break;
			case 6:
				OLED_Clear();
				OLED_ShowImage(81,13,36,36,Frame);
				Slip_SecondMenu();
				OLED_Update();
				break;
		}
	}
}


