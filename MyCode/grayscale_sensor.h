/*!
 * @file    grayscale_sensor.h
 * @author  Baohan
 * @date    2022/10/09
 */

#ifndef _GRAYSCALE_SENSOR_H
#define _GRAYSCALE_SENSOR_H

#include "zf_driver_gpio.h"

extern uint8_t pinLevel[4];

// enum TraceType
// {
//     NORMAL,
//     LEFT90,
//     RIGHT90,
// } traceType;

void GrayScaleSensorInit();
void ReadGrayScaleSensor();

#endif
