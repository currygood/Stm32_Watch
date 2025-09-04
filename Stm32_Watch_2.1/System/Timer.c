#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_InternalClockConfig(TIM2);//可以不写这个，因为定时器上电后默认是开启内部时钟

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=100-1;		//arr
	TIM_TimeBaseInitStructure.TIM_Prescaler=720-1;		//PSC
	//最终1ms中断一次
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//防止刚初始化完就中断了
	/*刚初始化完就中断的原因，TIM_TimeBaseInit函数内部最后面更新了 预分频器（PSC），因为PSC有一个缓冲机制，我们写入的值不会立刻起作用，只有在更新事件时才起作用
	为了立刻起作用就手动生成了一个更新时间。因为更新时间和更新中断同时发生，所以中断了，所以Num一开始就+1了
	*/
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;      //CCR
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
	
}

/*
	这是PWM改变CCR的函数.PWM是实现呼吸灯的关键
*/
void PWN_SetCompare1(uint16_t compare)
{
	TIM_SetCompare1(TIM2,compare);
}

