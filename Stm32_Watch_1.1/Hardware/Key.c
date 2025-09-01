#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define KEY_1 GPIO_Pin_0   //B0			KEY1为上一项
#define KEY_2 GPIO_Pin_5	//A5		KEY2为下一项
#define KEY_3 GPIO_Pin_2	//A2  		KEY3为确定

uint8_t Key_Num;

/*
  按键初始化
 */
void Key_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY_2 | KEY_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
  获取按了哪个
 */
uint8_t Key_GetNum(void)
{
	uint8_t temp;
	if(Key_Num)
	{
		temp=Key_Num;
		Key_Num=0;
		return temp;
	}
	return 0;
}

uint8_t Key_GetState(void)
{
	if (GPIO_ReadInputDataBit(GPIOB, KEY_1) == 0)			
	{
		return 1;										
	}
	
	if (GPIO_ReadInputDataBit(GPIOA, KEY_2) == 0)			
	{
		return 2;											
	}
	
	if (GPIO_ReadInputDataBit(GPIOA, KEY_3) == 0)			
	{
		return 3;											
	}
	
	return 0;			//返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
}

/*
非阻塞按键的实现，原理就是通过定时器中断，20ms读一次。不同而且当前的KeyNum不是0就是按下了按键，然后按键松开后就把Key_Num=PreState  上一次按下的是哪个了
*/
void Key_Tick(void)
{
	static uint8_t Count;
	static uint8_t CurrentState,PreState;
	Count++;
	if(Count>=20)
	{
		Count=0;
		PreState=CurrentState;
		CurrentState=Key_GetState();
		if(PreState!=0&&CurrentState==0)
		{
			Key_Num=PreState;
		}
	}
}
