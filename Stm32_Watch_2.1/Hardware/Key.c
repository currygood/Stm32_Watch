#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
#include "Delay.h"


uint8_t Key_Flag[KEY_NUM];
/*8位Flag  高到低：
7  6   5   4   3   2    1     0
无 重复 长按 双击 单击  松开  按下  按住不放
*/
/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */
void Key_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						
	
	GPIO_InitStructure.GPIO_Pin = KEY_2 | KEY_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*EXTI线路映射：GPIOA的Pin5 → EXTI_Line5（核心修改：PA5对应Line5）*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);	
	
	/*EXTI初始化：配置EXTI_Line5（对应PA5）*/
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;			//PA5对应EXTI线路5
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//中断模式（用于唤醒停止模式）
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿触发（按键按下时高→低）
	EXTI_Init(&EXTI_InitStructure);
	
	/*NVIC配置：对应EXTI_Line5的中断通道（核心修改）*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;		//EXTI_Line5对应的中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

/*中断服务程序：对应EXTI_Line5（KEY_2按键）   用于退出停止模式*/
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) == SET)	//检查PA5对应的EXTI线路5中断标志
    {
        // 按键消抖（避免机械抖动误触发）
        for(uint16_t i=0;i<65000;++i);
        EXTI_ClearITPendingBit(EXTI_Line5);  //清除中断标志（必须执行，否则会重复触发）
    }
}

/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
  */

uint8_t Key_GetState(uint8_t n)
{
	if(n==0)
	{
		if(GPIO_ReadInputDataBit(GPIOB,KEY_1)==0)
		{
			return KEY_PRESSED;
		}
	}
	else if(n==1)
	{
		if(GPIO_ReadInputDataBit(GPIOA,KEY_2)==0)
		{
			return KEY_PRESSED;
		}
	}
	else if(n==2)
	{
		if(GPIO_ReadInputDataBit(GPIOA,KEY_3)==0)
		{
			return KEY_PRESSED;
		}
	}

	return KEY_UNPRESSED;
}

uint8_t Key_Check(uint8_t n,uint8_t Flag)
{
	if(Key_Flag[n]&Flag)
	{
		if(Flag!=KEY_HOLD)
			Key_Flag[n]&=(~Flag);
		return 1;
	}
	return 0;
}

void Key_Tick(void)
{
	static uint8_t key_count;
	static uint8_t prestate[KEY_NUM],curstate[KEY_NUM];
	static uint8_t KS[KEY_NUM];
	static uint16_t Time[KEY_NUM];
	
	for(uint8_t i=0;i<KEY_NUM;++i)
	{
		if(Time[i]>0)		Time[i]--;
	}
	
	key_count++;
	if(key_count>=20)
	{
		key_count=0;
		for(uint8_t i=0;i<KEY_NUM;++i)
		{
			prestate[i]=curstate[i];
			curstate[i]=Key_GetState(i);
			
			if(curstate[i]==KEY_PRESSED)	//HOLD=1
			{
				Key_Flag[i]|=KEY_HOLD;
			}
			else			//HOLD=0
			{
				Key_Flag[i]&=(~KEY_HOLD);
			}
			
			if(curstate[i]==KEY_PRESSED && prestate[i]==KEY_UNPRESSED)		//DOWN=1
			{
				Key_Flag[i]|=KEY_DOWN;
			}
		
			if(curstate[i]==KEY_UNPRESSED && prestate[i]==KEY_PRESSED)			//UP=1
			{
				Key_Flag[i]|=KEY_UP;
			}
			
			
			if(KS[i]==0)
			{
				if(curstate[i]==KEY_PRESSED)
				{
					Time[i]=KEY_TIME_LONG;		//2s长按时间阈值
					KS[i]=1;
				}
			}
			else if(KS[i]==1)
			{
				if(curstate[i]==KEY_UNPRESSED)
				{
					Time[i]=KEY_TIME_DOUBLE;
					KS[i]=2;
				}
				else if(Time[i]==0)		//长按时间到
				{
					Time[i]=KEY_TIME_REPEAT;		//检测重复的
					//LONG=1
					Key_Flag[i]|=KEY_LONG;
					KS[i]=4;
				}
			}
			else if(KS[i]==2)
			{
				if(curstate[i]==KEY_PRESSED)
				{
					//DOUBLE=1
					Key_Flag[i]|=KEY_DOUBLE;
					KS[i]=3;
				}
				else if(Time[i]==0)
				{
					//SINGLE=1
					Key_Flag[i]|=KEY_SINGLE;
					KS[i]=0;
				}
			}
			else if(KS[i]==3)
			{
				if(curstate[i]==KEY_UNPRESSED)
				{
					KS[i]=0;
				}
			}
			else if(KS[i]==4)
			{
				if(curstate[i]==KEY_UNPRESSED)
				{
					KS[i]=0;
				}
				else if(Time[i]==0)
				{
					Time[i]=KEY_TIME_REPEAT;		//REPEAT=1
					Key_Flag[i]|=KEY_REPEAT;
					KS[i]=4;
				}
			}
		}
	}
}
