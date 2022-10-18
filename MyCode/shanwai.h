/*!
 * @file    shanwai.h
 * @author  Baohan
 * @date    2022/10/17
 *
  */

#ifndef _SHANWAI_H
#define _SHANWAI_H

#include <stdint.h>
#include "zf_driver_uart.h"

void VcanSendware(void *wareaddr, uint32_t waresize);

#endif
