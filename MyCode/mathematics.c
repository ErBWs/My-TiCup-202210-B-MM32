/*
 * @file    mathematics.c
 * @author  Baohan
 * @date    2022/04/09
 */

#include "mathematics.h"



//-------------------------------------------------------------------------------------------------------------------
//  @brief      求方差
//  @param      data[]      求方差的数组
//  @param      count       数组长度
//  @param      startX      开始点x坐标
//  @param      line[]      目标直线
//  @return     方差
//-------------------------------------------------------------------------------------------------------------------
float GetVariance(uint16_t data[], uint16_t count, uint16_t startX, uint16_t target_line[])
{
    float sum = 0;      //和
    uint16_t i;

    //遍历求数组元素与目标直线差值
    for(i = 0 ; i < count ; i++)
    {
        sum += (data[i] - target_line[i+startX])*(data[i] - target_line[i+startX]);
    }

    //求总体方差，并且返回
    return sum / count;
}


uint16_t line[120] = {0};
//-------------------------------------------------------------------------------------------------------------------
//  @brief      最小二乘法
//  @param      data[]      求直线的数组
//  @param      count       数组长度
//  @param      startX      开始点x坐标
//  @return     NULL
//-------------------------------------------------------------------------------------------------------------------
void OrdinaryLeastSquares(uint16_t data[], uint16_t count, uint16_t startX)
{
    uint16_t i = 0;
    float sumX = 0;
    float sumY = 0;
    float averageX = 0;
    float averageY = 0;
    float sumUp = 0;    //系数k公式的分子
    float sumDown = 0;  //系数k公式的分母
    float k;
    float b;    //直线方程y=kx+b

    //求x与y的平均值
    while(i < count-1)
    {
        sumX += i+startX;
        sumY += data[i];
        i++;
    }
    averageX = sumX/count;
    averageY = sumY/count;

    //求k
    do
    {
        sumUp += (data[i]-averageY) * (i+startX-averageX);
        sumDown += (i+startX-averageX)*(i+startX-averageX);
    } while(--i >= 0);
    if(sumDown == 0)    //防止分母为0
    {
        k = 0;
    }
    else
    {
        k = sumUp/sumDown;
    }

    //求b
    b = averageY-k*averageX;

    //将方程输入line函数中

}


