#ifndef __KEY_H
#define __KEY_H

#define KEY_NUM 	3
#define KEY_1 GPIO_Pin_0   //B0			KEY1为上一项
#define KEY_2 GPIO_Pin_5	//A5		KEY2为下一项
#define KEY_3 GPIO_Pin_2	//A2  		KEY3为确定

#define KEY_PRESSED 1
#define KEY_UNPRESSED 0

#define KEY_HOLD 0x01
#define KEY_DOWN 0x02
#define KEY_UP 0x04
#define KEY_SINGLE 0x08
#define KEY_DOUBLE 0x10
#define KEY_LONG 0x20
#define KEY_REPEAT 0x40

#define KEY_TIME_DOUBLE 100
#define KEY_TIME_LONG 2000
#define KEY_TIME_REPEAT 100

void Key_Init(void);
uint8_t Key_GetNum(void);
void Key_Tick(void);
uint8_t Key_GetState(uint8_t n);
uint8_t Key_Check(uint8_t n,uint8_t Flag);

#endif
