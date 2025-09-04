#ifndef __MENU_H
#define __MENU_H

#define SLEEP_ON 2
#define SLEEP_OFF 1

//整个项目初始化
void Menu_Init(void);
//首页时钟UI
void Show_Clock_UIMenu(void);
//首页时钟选项
int First_Page_Clock(void);
//设置UI
void Show_SettingPage_UIMenu(void);
//设置选项
void SettingPage(void);
//滑动菜单UI
int Slip_Menu(void);
//菜单选项
void Set_MoodChoice(uint8_t Choice);

#endif

