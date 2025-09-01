#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Menu.h"
#include "LED.h"
#include "Timer.h"
#include "Key.h"
#include "Slip_Menu.h"
#include "Dino.h"

int main(void)
{
	Menu_Init();
	Timer_Init();
	int clkflaghome;
	
	while (1)
	{
		clkflaghome=First_Page_Clock();
		if(clkflaghome==1)
		{
			Slip_Menu();
		}
		else if(clkflaghome==2)
		{
			SettingPage();
		}
	}
}

extern uint8_t light_mode;
static uint8_t now_ccr=0;	//PWM输出的CCR
static uint8_t now_add=2;	//1:ccr-   2:ccr+
static uint8_t ccr_count;	//ccr计数，用于减慢呼吸速率
void TIM2_IRQHandler(void)		//1ms进入一次中断的，具体看Timer.h
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		Key_Tick();
		StopWatch_Tick();
		Dino_Tick();
		if(light_mode==2)	//灯呼吸模式
		{
			ccr_count++;
			if(ccr_count>=20)	//20ms ccr改变一次，减少呼吸速度，改小可以变快，改大可以变慢
			{
				ccr_count=0;
				if(now_add==2)
					now_ccr++;
				else
					now_ccr--;
				if(now_ccr>100)	now_add=1;
				if(now_ccr<=0)	now_add=2;
				PWN_SetCompare1(now_ccr);
			}
		}
		else if(light_mode==3)	//灯闪烁模式
		{
			Flash_Tick();
		}
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
