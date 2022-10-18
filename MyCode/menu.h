/*!
 * @file    menu.h
 * @author  Baohan
 * @date    2022/06/18
 */

#ifndef _menu_h_
#define _menu_h_

#define SCANNER_PERIOD 10

#include "zf_device_tft180.h"
#include "zf_driver_pit.h"
#include "zf_driver_gpio.h"
#include "zf_device_mt9v03x.h"
#include "image_process.h"
#include "ctrl.h"
#include "easy_key.h"
#include "user_tft180.h"

extern uint8_t runFlag;

extern int Menu_array[7];
#define Distance1 Menu_array[0] 
#define Distance2 Menu_array[1]
#define Distance3 Menu_array[2] 
#define Distance4 Menu_array[3]
#define Distance5 Menu_array[4]
#define T_flag Menu_array[5]
#define THR_flag Menu_array[6]


void MenuInit(void);

void ShowMenuMainPage(void);

void ConfigFloat(float *val);
void ConfigInt(int *val);
void RSP(void);
void PRP(void);


void TAT(void);
void ShowImage(void);
// void BackUP_Recover(uint8_t BR);
void MenuOperator(void);

void TestBench(void);

#endif