/*
 * @file    mathematics.h
 * @author  Baohan
 * @date    2022/04/09
 */

#ifndef _mathematics_h
#define _mathematics_h

#include <math.h>
#include <stdint.h>

extern uint16_t line[120];

float GetVariance(uint16_t data[], uint16_t count, uint16_t startX, uint16_t target_line[]);
void OrdinaryLeastSquares(uint16_t data[], uint16_t count, uint16_t startX);

#endif /* CODE_MATHEMATICS_H_ */
