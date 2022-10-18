/*!
 * @file    shanwai.c
 * @author  Baohan
 * @date    2022/10/17
 */

#include "shanwai.h"

#define CMD_WARE    3

void VcanSendware(void *wareaddr, uint32_t waresize)//ɽ�Ⲩ�κ���
{
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����

    uart_write_buffer(UART_1, cmdf, sizeof(cmdf));
    uart_write_buffer(UART_1, (uint8_t *)wareaddr, waresize);
    uart_write_buffer(UART_1, cmdr, sizeof(cmdr));

    // UART_PutNChar(UART, cmdf, sizeof(cmdf));               //�ȷ���ǰ���� ע�����úô��ں�
    // UART_PutNChar(UART, (uint8_t *)wareaddr, waresize);    //��������
    // UART_PutNChar(UART, cmdr, sizeof(cmdr));    //���ͺ�����
}