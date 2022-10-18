/*
 * @file    mathematics.c
 * @author  Baohan
 * @date    2022/04/09
 */

#include "mathematics.h"



//-------------------------------------------------------------------------------------------------------------------
//  @brief      �󷽲�
//  @param      data[]      �󷽲������
//  @param      count       ���鳤��
//  @param      startX      ��ʼ��x����
//  @param      line[]      Ŀ��ֱ��
//  @return     ����
//-------------------------------------------------------------------------------------------------------------------
float GetVariance(uint16_t data[], uint16_t count, uint16_t startX, uint16_t target_line[])
{
    float sum = 0;      //��
    uint16_t i;

    //����������Ԫ����Ŀ��ֱ�߲�ֵ
    for(i = 0 ; i < count ; i++)
    {
        sum += (data[i] - target_line[i+startX])*(data[i] - target_line[i+startX]);
    }

    //�����巽����ҷ���
    return sum / count;
}


uint16_t line[120] = {0};
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��С���˷�
//  @param      data[]      ��ֱ�ߵ�����
//  @param      count       ���鳤��
//  @param      startX      ��ʼ��x����
//  @return     NULL
//-------------------------------------------------------------------------------------------------------------------
void OrdinaryLeastSquares(uint16_t data[], uint16_t count, uint16_t startX)
{
    uint16_t i = 0;
    float sumX = 0;
    float sumY = 0;
    float averageX = 0;
    float averageY = 0;
    float sumUp = 0;    //ϵ��k��ʽ�ķ���
    float sumDown = 0;  //ϵ��k��ʽ�ķ�ĸ
    float k;
    float b;    //ֱ�߷���y=kx+b

    //��x��y��ƽ��ֵ
    while(i < count-1)
    {
        sumX += i+startX;
        sumY += data[i];
        i++;
    }
    averageX = sumX/count;
    averageY = sumY/count;

    //��k
    do
    {
        sumUp += (data[i]-averageY) * (i+startX-averageX);
        sumDown += (i+startX-averageX)*(i+startX-averageX);
    } while(--i >= 0);
    if(sumDown == 0)    //��ֹ��ĸΪ0
    {
        k = 0;
    }
    else
    {
        k = sumUp/sumDown;
    }

    //��b
    b = averageY-k*averageX;

    //����������line������

}


