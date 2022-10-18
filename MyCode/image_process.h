/*
 * @file    image_process.h
 * @author  Baohan
 * @date    2022/10/14
 */

#ifndef _IMAGE_PROCESS_H
#define _IMAGE_PROCESS_H

#include "zf_device_mt9v03x.h"

#define HEIGHT  MT9V03X_H
#define WIDTH   MT9V03X_W

extern uint8_t BinarizedImage[HEIGHT][WIDTH];
extern float tPosition[20];
extern int16_t tNumber;
extern int16 bottomEdge[MT9V03X_W];

#define WIDTH_THRESHOLD     10    //识别T字的底线宽度阈值

void FindBottomEdge(uint8 *image, uint16_t col, uint16_t row ,int16_t *bottomEdge);
int16_t FindTSign(uint8 *image, uint16_t col, uint16_t row ,int16_t *bottomEdge,float *tPosition);
void ImageBinarization(uint8_t imageIn[HEIGHT][WIDTH], uint8_t threshold, uint8_t imageOut[HEIGHT][WIDTH]);
void FiltrateImage(uint8_t image[HEIGHT][WIDTH]);

#endif