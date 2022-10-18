/*!
 * @file    shanwai.c
 * @author  Baohan
 * @date    2022/10/17
 */

#include "shanwai.h"

#define CMD_WARE    3

void VcanSendware(void *wareaddr, uint32_t waresize)//山外波形函数
{
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    uart_write_buffer(UART_1, cmdf, sizeof(cmdf));
    uart_write_buffer(UART_1, (uint8_t *)wareaddr, waresize);
    uart_write_buffer(UART_1, cmdr, sizeof(cmdr));

    // UART_PutNChar(UART, cmdf, sizeof(cmdf));               //先发送前命令 注意设置好串口号
    // UART_PutNChar(UART, (uint8_t *)wareaddr, waresize);    //发送数据
    // UART_PutNChar(UART, cmdr, sizeof(cmdr));    //发送后命令
}