/*
这是谷歌恐龙游戏的文件
*/
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"
#include <math.h>
#include <stdlib.h>
#include "dino.h"
#include "Delay.h"
#include "Slip_Menu.h"

/*
	显示分数
*/
int score;
void Show_Score(void)
{
	OLED_ShowNum(98,0,score,5,OLED_6X8);
}

/*
	显示地面
*/
uint16_t ground_pos;
extern uint8_t OLED_DisplayBuf[8][128];    //这是定义在OLED.c的OLED缓存数组
void Show_Ground(void)
{
	if(ground_pos<128)
	{
		for(uint8_t i=0;i<128;++i)
		{
			OLED_DisplayBuf[7][i]=Ground[i+ground_pos];
		}
	}
	else
	{
		for(uint8_t i=0;i<255-ground_pos;++i)
		{
			OLED_DisplayBuf[7][i]=Ground[i+ground_pos];
		}
		for(uint8_t i=255-ground_pos;i<128;++i)
		{
			OLED_DisplayBuf[7][i]=Ground[i-(255-ground_pos)];
		}
	}
}

/*
	显示障碍
*/
uint8_t barrier_choice;
uint8_t barrier_pos=0;
Object_Position barrier_p;
void Show_Barrier(void)
{
	if(barrier_pos>=143)	//等上一个障碍物完全移出屏幕外才取随机数
		barrier_choice=rand()%3;
	OLED_ShowImage(127-barrier_pos,44,16,18,Barrier[barrier_choice]);
	
	barrier_p.minX=127-barrier_pos;
	barrier_p.maxX=barrier_p.minX+16;
	barrier_p.minY=44;
	barrier_p.maxY=62;
}

/*
	显示云朵
*/
uint8_t cloud_pos;
void Show_Cloud(void)
{
	OLED_ShowImage(127-cloud_pos,9,16,8,Cloud);
}

/*
	显示恐龙
*/
uint8_t dino_jump_flag=0;   //0:奔跑，1:跳跃
uint16_t jump_t;	//整个在空中的时间为1s，通过TIM计时
uint8_t jump_pos;	//恐龙跳跃高度
Object_Position dino_p;
void Show_Dino(void)
{
	if(dino_jump_flag)	jump_pos=28*sin((float)(PI*jump_t/1000));		//跳跃高度通过正弦函数算28sin(wt)
	if(dino_jump_flag==0)
	{
		if(cloud_pos%2)		OLED_ShowImage(0,44,16,18,Dino[1]);
		else	OLED_ShowImage(0,44,16,18,Dino[0]);
	}
	else
	{
		OLED_ShowImage(0,44-jump_pos,16,18,Dino[2]);
	}
	
	dino_p.minX=0;
	dino_p.maxX=16;
	dino_p.minY=44-jump_pos;
	dino_p.maxY=dino_p.minY+18;
}

/*
	判断恐龙是否撞上了障碍物
*/
uint8_t isloss=0;
void IsColliding(Object_Position*a,Object_Position*b)
{
	if((a->maxX>b->minX)&&(a->maxY>b->minY))
	{
		OLED_Clear();
		OLED_ShowString(28,24,"Game Over",OLED_8X16);
		OLED_Update();
		Delay_s(1);
		isloss=1;
	}
}

/*
	整个游戏的实现
*/
extern uint8_t KeyNum;
void DinoGame_Animation(void)
{
	barrier_pos=0;
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)	dino_jump_flag=1;		//按Key1就是跳跃
		if(KeyNum==3)
		{
			OLED_Clear();
			OLED_ShowString(28,24,"Game Over",OLED_8X16);
			OLED_Update();
			Delay_s(1);
			isloss=0;
			score=0;
			barrier_pos=0;
			return;
		}
		OLED_Clear();
		Show_Score();
		Show_Ground();
		Show_Barrier();
		Show_Cloud();
		Show_Dino();
		OLED_Update();
		IsColliding(&dino_p,&barrier_p);
		if(isloss==1)
		{
			isloss=0;
			score=0;
			barrier_pos=0;
			return;
		}
	}
	
}

/*
	游戏各种东西移动的关键
*/
void Dino_Tick(void)
{
	static uint8_t score_count,ground_count,cloud_count;
	score_count++;
	ground_count++;
	cloud_count++;
	if(score_count>=100)
	{
		score_count=0;
		score++;		//100ms加一次分数
		if(score>99999)
			score=0;
	}
	if(ground_count>=20)
	{
		ground_count=0;
		ground_pos++;
		barrier_pos++;
		if(ground_pos>=256)	ground_pos=0;
		if(barrier_pos>=144)	barrier_pos=0;
	}
	if(cloud_count>=50)
	{
		cloud_count=0;
		cloud_pos++;
		if(cloud_pos>=200)	cloud_pos=0;
	}
	if(dino_jump_flag==1)		//按下了跳跃
	{
		jump_t++;
		if(jump_t>=1000)		//1s，跳跃时间在空中为1s
		{
			jump_t=0;
			dino_jump_flag=0;
		}
	}
}

