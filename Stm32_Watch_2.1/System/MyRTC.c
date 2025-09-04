#include "stm32f10x.h"                  // Device header
#include "MyRTC.h"
#include <time.h>

/*
	实时时钟初始化
*/
void MyRTC_Init(void)
{
	//bkp是为了主电源掉电后，bkp没掉电就可以继续运行时间
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);	//开启电源时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);	//开启bkp时钟
	PWR_BackupAccessCmd(ENABLE);
	
	if(BKP_ReadBackupRegister(BKP_DR1)!=0xA5A5)		//bkp掉电没了
	{
		RCC_LSEConfig(RCC_LSE_ON);	//开启外部低速时钟
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);		//等待时钟开启完成
		
		//选择RTCCLK时钟源
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);
		
		//等待上面的配置完成
		RTC_WaitForLastTask();		//等待上一部写入操作完成 
		RTC_WaitForSynchro();		//等待同步完成
		
		RTC_SetPrescaler(32768-1);
		RTC_WaitForLastTask();
		
		RTC_SetCounter(1755849870);
		RTC_WaitForLastTask();
		
		BKP_WriteBackupRegister(BKP_DR1,0xA5A5);		//写入 写入标志位（自己定义的）
	}
	else		//如果已经有时间写进去了
	{
		RTC_WaitForLastTask();
		RTC_WaitForSynchro();
	}
}

/*
	设置时间戳
*/
void MyRTC_SetTime(MTimer t)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_date.tm_year=t.year-1900;
	time_date.tm_mon=t.month-1;
	time_date.tm_mday=t.day;
	time_date.tm_hour=t.hour;
	time_date.tm_min=t.minute;
	time_date.tm_sec=t.second;
	
	time_cnt = mktime(&time_date) - 8 * 60 * 60;			//转成0时区的
	
	RTC_SetCounter(time_cnt);
}

/*
	读取时间戳
*/
void MyRTC_ReadTime(MTimer *t)
{
	time_t time_cnt;
	struct tm time_date;
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;		//转成8时区（北京时间）
	time_date = *localtime(&time_cnt);	//这个stm芯片离线，不知道是在哪个时区，所以还是会用伦敦的0时区，上面就加回来了
	
	t->year=time_date.tm_year+1900;
	t->month=time_date.tm_mon+1;
	t->day=time_date.tm_mday;
	t->hour=time_date.tm_hour;
	t->minute=time_date.tm_min;
	t->second=time_date.tm_sec;
}
	

