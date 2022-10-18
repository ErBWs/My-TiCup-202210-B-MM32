/*!
 * @file    grayscale_sensor.c
 * @author  Baohan
 * @date    2022/10/09
 */

#include "grayscale_sensor.h"

uint8_t pinLevel[4] = {0, 0, 0, 0};

void GrayScaleSensorInit(void)
{
    gpio_init(B13, GPI, GPIO_LOW, GPI_PULL_UP);    // Left back
    gpio_init(B14, GPI, GPIO_LOW, GPI_PULL_UP);    // Right back
    gpio_init(B14, GPI, GPIO_LOW, GPI_PULL_UP);    // Left front
    gpio_init(B12, GPI, GPIO_LOW, GPI_PULL_UP);    // Right front
}

void ReadGrayScaleSensor(void)
{
    pinLevel[0] = gpio_get_level(B13);
    pinLevel[1] = gpio_get_level(B14);
    pinLevel[2] = gpio_get_level(B15);
    pinLevel[3] = gpio_get_level(B12);
}