/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             main
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ3184284598)
* @version          查看doc内version文件 版本说明
* @Software         IAR 8.32.4 or MDK 5.28
* @Target core      MM32F327X_G9P
* @Taobao           https://seekfree.taobao.com/
* @date             2022-04-11
********************************************************************************************************************/

#include "zf_common_headfile.h"

int main (void)
{
    // Clock init, working frequency:120MHz
	clock_init(SYSTEM_CLOCK_120M);
	// Debug output uart init
	// debug_init();
	// interrupt_global_disable();
	uart_init(UART_1, 115200, UART1_TX_A9, UART1_RX_A10);
	// pit_disable(TIM7_PIT);

	// Device init
	// Screen init
	tft180_init();
	tft180_show_string(0, 0 * 16, "initializing...");

	// Coreboard led init(For test usage)
	gpio_init(H2, GPO, GPIO_HIGH, GPO_PUSH_PULL);
	gpio_init(B13, GPO, GPIO_HIGH, GPO_PUSH_PULL);
	gpio_init(D12, GPO, GPIO_HIGH, GPO_PUSH_PULL);
	gpio_set_level(D12, 0);

	// BLDC and servo motor init
	MotorInit();

	// Switch init
	MenuInit();
    system_delay_ms(100);

	// Camera init
	mt9v03x_init();
    system_delay_ms(100);
	tft180_show_string(0, 3 * 16, "camera: === [done]");

	GrayScaleSensorInit();
	DipHandler();

	tft180_show_string(0, 7 * 16, "all completed!");
    system_delay_ms(100);

	// Clear screen(If the screen is cleared means all the inits were done)
	tft180_clear();


	// pit_init(TIM6_PIT, 5);

	MenuOperator();
    pit_ms_init(TIM7_PIT, 5);                  // Use TIM7 timer

	// extern int16_t leftSpeed;
	// extern int16_t rightSpeed;
	// int16_t speed[3];

	while (1)
	{
		//Flash command(Ignore this)
		//pyocd flash -t mm32f3277g9p -f 20M ~/Desktop/pyocdbuild/mm32f3270.bin
		
		// If you see the screen shows /xxxx/xxxxx.c and a number below it,
		// it means that file has wrong "zf_assert();" in that certain line.

		// Test code
		// gpio_set_level(B13,1);
		// gpio_set_level(H2,0);
		// system_delay_ms(500);
		// gpio_set_level(B13,0);
		// gpio_set_level(H2,1);
		// system_delay_ms(500);

		ImageBinarization(mt9v03x_image, 140, BinarizedImage);
		FiltrateImage(&BinarizedImage[0]);
		FindBottomEdge(BinarizedImage[0],MT9V03X_W,MT9V03X_H,bottomEdge);
		tNumber=FindTSign(BinarizedImage[0],MT9V03X_W,MT9V03X_H,bottomEdge,tPosition);
		// speed[0] = leftSpeed;
		// speed[1] = rightSpeed;
		// speed[2] = spdParam[3];
		// VcanSendware(speed, sizeof(speed));
		// uart_putstr(UART_1, "hello");
		// printf("hello");
	}
}
