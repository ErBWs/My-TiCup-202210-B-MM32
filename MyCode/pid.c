/*!
 * @file    pid.c
 * @author  Baohan
 * @date    2021/12/21
 * @see     https://www.cnblogs.com/foxclever/p/8902029.html
 */

#include "pid.h"

/*!
 * @brief       Pid init
 *
 * @param       pid        PID struct
 * @return      None.
 */
void PidInit(PID_t *pid)
{
    pid->lastErr = 0;
    pid->preLastErr = 0;
    pid->outMax = 3000;
    pid->errMax = 800;
    pid->errMin = 300;
    pid->out = 0;
}


/*!
 * @brief       Change the coefficient of integral
 *
 * @param       error       Error
 * @param       pid         PID struct
 * @return      Coefficient of integral
 *
 * @note        Modify this part to implement different function
 */
float ConfigIntegral(float error, PID_t *pid)
{
    float coeffi;

    // Get linear integration coefficient
    if (fabsf(error) <= pid->errMin)
        coeffi = 1;
    else if (fabsf(error) >= pid->errMax)
        coeffi = 0;
    else
        coeffi = (pid->errMax - fabsf(error)) / (pid->errMax - pid->errMin);

    return coeffi;
}


/*!
 * @brief       Incremental pid control
 *
 * @param       pid             PID struct
 * @param       param           Parameter array of pid:
 *                              param[0]: kp,
 *                              param[1]: ki,
 *                              param[2]: kd,
 *                              param[3]: target value,
 *                              param[4]: output limitation defined by device
 * @param       nowData         Current data
 * @return      PID output
 */
int32_t PidIncControl(PID_t *pid, const float *param, float nowData)
{
    static float coeffi = 0;     // Coefficient of integration
    static float delta = 0;
    static float pErr = 0, iErr = 0, dErr = 0;

    // Get coefficient of Integration;
    float thisErr = param[3] - nowData;
    coeffi = ConfigIntegral(thisErr, pid);

    // Trapezoidal integration
    float preI = (thisErr + pid->lastErr) / 2;
    pErr = thisErr - pid->lastErr;
    dErr = thisErr - 2 * (pid->lastErr) + pid->preLastErr;

    // Integration limitation
    if (pid->out > pid->outMax)
    {
        if (preI <= 0)
            iErr = preI;
    } else if (pid->out < (-pid->outMax))
    {
        if (preI >= 0)
            iErr = preI;
    } else
    {
        iErr = preI;
    }

    // Calculate
    delta = param[0] * pErr
            + coeffi * param[1] * iErr
            + param[2] * dErr;

    // Save errors for next calculation
    pid->preLastErr = pid->lastErr;
    pid->lastErr = thisErr;
    pid->out += delta;

    pid->out = Limitation(pid->out, -param[4], param[4]);
    return (int32_t) pid->out;
}


/*!
 * @brief       Position pid control
 *
 * @param       pid             PID struct
 * @param       param           Parameter array of pid:
 *                              param[0]: kp,
 *                              param[1]: ki,
 *                              param[2]: kd,
 *                              param[3]: target value,
 *                              param[4]: output limitation defined by device
 * @param       nowData         Current data
 * @return      PID output
 */
int32_t PidPosControl(PID_t *pid, const float *param, float nowData)
{
    static float coeffi = 0;     // Coefficient of integration
    static float delta = 0;
    static float pErr = 0, iErr = 0, dErr = 0;

    // Get coefficient of Integration;
    float thisErr = param[3] - nowData;
    coeffi = ConfigIntegral(thisErr, pid);

    // Trapezoidal integration
    float preI = (thisErr + pid->lastErr) / 2;
    pErr = thisErr;
    dErr = thisErr - pid->lastErr;

    // Integration limitation
    if (pid->out > pid->outMax)
    {
        if (preI <= 0)
            iErr += preI;
    } else if (pid->out < (-pid->outMax))
    {
        if (preI >= 0)
            iErr += preI;
    } else
    {
        iErr += preI;
    }

    // Calculate
    pid->out = param[0] * pErr
               + coeffi * param[1] * iErr
               + param[2] * dErr;

    // Save error for next calculate
    pid->lastErr = thisErr;

    pid->out = Limitation(pid->out, -param[4], param[4]);
    return (int32_t) pid->out;
}