#ifndef __MYRTC_H
#define __MYRTC_H

//时间结构体
typedef struct
{
	int year,month,day,hour,minute,second;
}MTimer;


//实时时钟初始化
void MyRTC_Init(void);
//改变实时时钟
void MyRTC_SetTime(MTimer t);
//读取实时时钟
void MyRTC_ReadTime(MTimer *t);

#endif
