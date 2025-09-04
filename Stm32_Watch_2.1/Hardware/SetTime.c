/*
设置界面
这个文件是设置时间的

退出
日期时间设置

年
月
日
时
分
秒

*/

#include "stm32f10x.h"                  // Device header
#include "MyRTC.h"
#include "OLED.h"
#include "KEY.h"

int set_time_flag=1;		//当前设置选项的选择设置第几个，默认是1，1是退回上一个界面
	
void show_SetTime_DateUIMenu(void)		//设置Date日期
{
	OLED_ShowImage(0,0,16,16,Enter_Return);
	MTimer Ti;
	MyRTC_ReadTime(&Ti);
	OLED_Printf(0,16,OLED_8X16,"年%4d",Ti.year);
	OLED_Printf(0,32,OLED_8X16,"月%2d",Ti.month);
	OLED_Printf(0,48,OLED_8X16,"日%2d",Ti.day);
}

MTimer Tim;
void show_SetTime_TimeUIMenu(void)			//设置Time时间
{
	OLED_Printf(0,0,OLED_8X16,"时%2d",Tim.hour);
	OLED_Printf(0,16,OLED_8X16,"分%2d",Tim.minute);
	OLED_Printf(0,32,OLED_8X16,"秒%2d",Tim.second);
}

void Set_Date_Year(void)
{
	while(1)
	{
		if(Key_Check(0,KEY_SINGLE))	//数值+1
		{
			Tim.year++;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(1,KEY_SINGLE))	//数值-1
		{
			Tim.year--;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(0,KEY_REPEAT))		//长按，快速+1
		{
			Tim.year++;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(1,KEY_REPEAT))		//长按，快速-1
		{
			Tim.year--;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(2,KEY_SINGLE))	//确认，保存退出
		{
			return;
		}

		//显示界面
		show_SetTime_DateUIMenu();
		OLED_ReverseArea(16,16,32,16);
		OLED_Update();
	}
}

void Set_Date_Month(void)
{
	while(1)
	{
		if(Key_Check(0,KEY_SINGLE))	//数值+1
		{
			Tim.month++;
			if(Tim.month>=13)	Tim.month=1;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(1,KEY_SINGLE))	//数值-1
		{
			Tim.month--;
			if(Tim.month<=0)	Tim.month=12;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(0,KEY_REPEAT))		//长按，快速+1
		{
			Tim.month++;
			if(Tim.month>=13)	Tim.month=1;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(1,KEY_REPEAT))	//长按，快速-1
		{
			Tim.month--;
			if(Tim.month<=0)	Tim.month=12;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(2,KEY_SINGLE))	//确认，保存退出
		{
			return;
		}

		//显示界面
		show_SetTime_DateUIMenu();
		OLED_ReverseArea(16,32,16,16);
		OLED_Update();
	}
}

void Set_Date_Day(void)
{
	while(1)
	{
		if(Key_Check(0,KEY_SINGLE))	//数值+1
		{
			Tim.day++;
			if(Tim.day>=32)		Tim.day=1;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(1,KEY_SINGLE))	//数值-1
		{
			Tim.day--;
			if(Tim.day<=0)	Tim.day=31;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(0,KEY_REPEAT))		//长按，快速+1
		{
			Tim.day++;
			if(Tim.day>=32)		Tim.day=1;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(1,KEY_REPEAT))	//长按，快速-1
		{
			Tim.day--;
			if(Tim.day<=0)	Tim.day=31;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(2,KEY_SINGLE))	//确认，保存退出
		{
			return;
		}
		

		show_SetTime_DateUIMenu();
		OLED_ReverseArea(16,48,16,16);
		OLED_Update();
	}
}

void Set_Time_Hour(void)
{
	while(1)
	{
		if(Key_Check(0,KEY_SINGLE))	//数值+1
		{
			Tim.hour++;
			if(Tim.hour>=24)		Tim.hour=0;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(1,KEY_SINGLE))	//数值-1
		{
			Tim.hour--;
			if(Tim.hour<0)	Tim.hour=23;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(0,KEY_REPEAT))		//长按，快速+1
		{
			Tim.hour++;
			if(Tim.hour>=24)		Tim.hour=0;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(1,KEY_REPEAT))	//长按，快速-1
		{
			Tim.hour--;
			if(Tim.hour<0)	Tim.hour=23;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(2,KEY_SINGLE))	//确认，保存退出
		{
			return;
		}
		
		//显示界面
		show_SetTime_TimeUIMenu();
		OLED_ReverseArea(16,0,16,16);
		OLED_Update();
	}
}

void Set_Time_Minute(void)
{
	while(1)
	{
		if(Key_Check(0,KEY_SINGLE))	//数值+1
		{
			Tim.minute++;
			if(Tim.minute>=60)		Tim.minute=0;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(1,KEY_SINGLE))	//数值-1
		{
			Tim.minute--;
			if(Tim.minute<0)	Tim.minute=59;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(0,KEY_REPEAT))		//长按，快速+1
		{
			Tim.minute++;
			if(Tim.minute>=60)		Tim.minute=0;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(1,KEY_REPEAT))	//长按，快速-1
		{
			Tim.minute--;
			if(Tim.minute<0)	Tim.minute=59;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(2,KEY_SINGLE))	//确认，保存退出
		{
			return;
		}
		
		//显示界面
		show_SetTime_TimeUIMenu();
		OLED_ReverseArea(16,16,16,16);
		OLED_Update();
	}
}

void Set_Date_Second(void)
{
	while(1)
	{
		if(Key_Check(0,KEY_SINGLE))	//数值+1
		{
			Tim.second++;
			if(Tim.second>=60)		Tim.second=0;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(1,KEY_SINGLE))	//数值-1
		{
			Tim.second--;
			if(Tim.second<0)	Tim.second=59;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(0,KEY_REPEAT))		//长按，快速+1
		{
			Tim.second++;
			if(Tim.second>=60)		Tim.second=0;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(1,KEY_REPEAT))	//长按，快速-1
		{
			Tim.second--;
			if(Tim.second<0)	Tim.second=59;
			MyRTC_SetTime(Tim);
		}
		else if(Key_Check(2,KEY_SINGLE))	//确认，保存退出
		{
			return;
		}
		
		//显示界面
		show_SetTime_TimeUIMenu();
		OLED_ReverseArea(16,32,16,16);
		OLED_Update();
	}
}

int SetTime(void)
{
	while(1)
	{
		uint8_t set_time_flag_temp=0;
		if(Key_Check(0,KEY_SINGLE))	//上一项
		{
			set_time_flag--;
			if(set_time_flag<=0)
				set_time_flag=7;
		}
		else if(Key_Check(1,KEY_SINGLE))	//下一项
		{
			set_time_flag++;
			if(set_time_flag>=8)
				set_time_flag=1;
		}
		else if(Key_Check(2,KEY_SINGLE))	//确认
		{
			OLED_Clear();
			OLED_Update();
			set_time_flag_temp=set_time_flag;
		}
		
		
		if(set_time_flag_temp==1)
		{
			return 0;
		}
		else if(set_time_flag_temp==2)		//设置年
		{
			Set_Date_Year();
		}
		else if(set_time_flag_temp==3)		//设置月
		{
			Set_Date_Month();
		}
		else if(set_time_flag_temp==4)		//设置日
		{
			Set_Date_Day();
		}
		else if(set_time_flag_temp==5)		//设置时
		{
			Set_Time_Hour();
		}
		else if(set_time_flag_temp==6)		//设置分
		{
			Set_Time_Minute();
		}
		else if(set_time_flag_temp==7)		//设置秒
		{
			Set_Date_Second();
		}
		
		//显示界面
		MyRTC_ReadTime(&Tim);
		switch(set_time_flag)
		{
			case 1:			//年月日在第一页显示
				OLED_Clear();
				show_SetTime_DateUIMenu();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			case 2:
				OLED_Clear();
				show_SetTime_DateUIMenu();
				OLED_ReverseArea(0,16,16,16);
				OLED_Update();
				break;
			case 3:
				OLED_Clear();
				show_SetTime_DateUIMenu();
				OLED_ReverseArea(0,32,16,16);
				OLED_Update();
				break;
			case 4:
				OLED_Clear();
				show_SetTime_DateUIMenu();
				OLED_ReverseArea(0,48,16,16);
				OLED_Update();
				break;
			case 5:			//时分秒在第二页显示
				OLED_Clear();
				show_SetTime_TimeUIMenu();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			case 6:			//时分秒在第二页显示
				OLED_Clear();
				show_SetTime_TimeUIMenu();
				OLED_ReverseArea(0,16,16,16);
				OLED_Update();
				break;
			case 7:			//时分秒在第二页显示
				OLED_Clear();
				show_SetTime_TimeUIMenu();
				OLED_ReverseArea(0,32,16,16);
				OLED_Update();
				break;
		}
	}
}


