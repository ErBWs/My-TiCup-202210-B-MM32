/*!
 * @file    pid.h
 * @author  Baohan
 * @date    2021/12/21
 * @see     https://www.cnblogs.com/foxclever/p/8902029.html
 */

#ifndef _pid_h
#define _pid_h

#include <stdint.h>
#include <math.h>

/*!
 * @brief   Limit the value of 'x'
 *
 * @param   x       Value need to be limited
 * @param   low     The low limit
 * @param   high    The high limit
 * @return  Limited 'x'
 */
#define Limitation(x, low, high) ((x) < (low) ? (low) : ((x) > (high) ? (high) : (x)))

typedef struct
{
    float lastErr;      // Last time error
    float preLastErr;   // Last time error of last time
    float outMax;       // Max of PID output
    float errMax;       // Error larger than this will separate the integral
    float errMin;       // Error larger than this and smaller than errMax will decrease the coefficient of integral
    float out;          // Output
}PID_t;

void PidInit(PID_t *pid);
float ConfigIntegral(float error, PID_t *pid);

int32_t PidIncControl(PID_t *pid, const float *param, float nowData);
int32_t PidPosControl(PID_t *pid, const float *param, float nowData);

#endif
