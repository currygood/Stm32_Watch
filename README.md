# Stm32_Watch
Stm32_Watch OR Desktop decoration And Other things. Whatever you want to become, it is.
一. Version 1.1
I developed using ST's standard library.
1.My hardware:
(1)stm32: STM32F103C8T6
(2)OLED: 0.96inch SSD1306MCU
(3)MPU6050: 6 axes MPU6050

2.How to use:
(1)Key1 Pre
(2)Key2 Next
(3)Key3 Confirm
About game: If you lose, you will automatically quit. If you want to quit but you not lose:press Key3
About gradienter: press Key3 quit.
About SetTime: Key1 is add Key2 is subtract.
That's all simple use.

3.Plug in the wire
(1)Key1:B0
(2)Key2:A5
(3)Key3:A2
(4)LED:A0
(5)OLED_SCL:B8
(6)OLED_SDA:B9
.......
![Uploading 742f12e0e4ba2bfadca12c6bc5e9a00f.jpg…]()

具体的看我b站：
https://www.bilibili.com/video/BV1EPhvzxEYv/?vd_source=c9924bda4031caf6f7caa15a01d9be91#reply273445220865

二. Version 2.1
Version 2.1 major update
1. Get the key values updated, various key functions (long press, double click, click, repeat).
2. Added stop mode (one of the STM32 low power modes) for power saving; On the home page, long press KEY3 to enter (OLED is no longer displayed). If you want to exit, press KEY_2.
3. Dinosaur Minigame Update: There is a tall cactus that requires a double click KEY_1 (that is, a big jump) to jump over.
4. When setting the time, press and hold to speed up (using the repeat function of the button).
