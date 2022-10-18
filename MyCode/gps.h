/*
 * @file    gps.h
 * @author  Baohan
 * @date    2022/04/01
 */

#ifndef _gps_h_
#define _gps_h_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zf_device_gps_tau1201.h"
#include "zf_device_tft180.h"
#include "zf_driver_flash.h"

void ShowAllGpsInfo(void);
void LocateGps(void);

void DoubleToInt(double val, uint32_t *arr);

void SavePoints(uint8_t state);

#endif