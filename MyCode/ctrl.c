/*
 * @file    ctrl.c
 * @author  Baohan
 * @date    2022/05/02
 */

#include "ctrl.h"

// PID parameter array: *Param[5] = {kp, ki, kd, target_value, limitation}
float spdParam[5] = {80, 0, 15, 37.3, PWM_DUTY_MAX - 1000};
float dirParam[5] = {0.6, 0, 0, 0, SERVO_DUTY_MAX};       // PD control for direction, ki should be 0

// PID struct define
PID_t leftSpdPid;
PID_t rightSpdPid;
PID_t dirPid;

uint8_t g_stopFlag = 0;
uint8_t stallFlag = 0;
uint8_t parallelFlag = 0;
uint8_t num = 0;
uint8_t count = 0;

/*!
 * @brief       Motor control init, including encoder init
 *
 * @return      None
 */
void MotorInit(void)
{
    // Servo motor init
    pwm_init(SERVO_PIN, SERVO_FREQ, SERVO_MID);
	tft180_show_string(0, 1 * 16, "motor: ");
    // Self check
    system_delay_ms(200);
    pwm_set_duty(SERVO_PIN, GetServoDuty(10));
    system_delay_ms(200);
	tft180_show_string(0, 1 * 16, "motor: =");
    pwm_set_duty(SERVO_PIN, GetServoDuty(-10));
    system_delay_ms(200);
    pwm_set_duty(SERVO_PIN, GetServoDuty(0));
    system_delay_ms(200);
	tft180_show_string(0, 1 * 16, "motor: ==");

    // Motor and encoder init
    pwm_init(MOTOR_L_PIN, MOTOR_FREQ, 0);
    pwm_init(MOTOR_R_PIN, MOTOR_FREQ, 0);

    gpio_init(MOTOR_L_DIR_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(MOTOR_R_DIR_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);

    encoder_dir_init(ENCODER_L_TIM, ENCODER_L_SPD_PIN, ENCODER_L_DIR_PIN);
    encoder_dir_init(ENCODER_R_TIM, ENCODER_R_SPD_PIN, ENCODER_R_DIR_PIN);

    system_delay_ms(50);
    tft180_show_string(0, 1 * 16, "motor: ===");

    // Pid init
    PidInit(&leftSpdPid);
    PidInit(&rightSpdPid);
    PidInit(&dirPid);
    tft180_show_string(0, 1 * 16, "motor: ====");

    // Interrupt init
	interrupt_set_priority(TIM7_IRQn, 1);    // Set priority to 1
	tft180_show_string(0, 1 * 16, "motor: ==== [done]");
}


uint32_t distance = 0;
void GetDistance()
{
    static int16_t leftDistance = 0, rightDistance = 0;
    leftDistance = (encoder_get_count(ENCODER_L_TIM));
    encoder_clear_count(ENCODER_L_TIM);
    rightDistance = (encoder_get_count(ENCODER_R_TIM));
    encoder_clear_count(ENCODER_R_TIM);

    distance += (abs(leftDistance) + abs(rightDistance)) / 2;
}


void DipHandler(void)
{
    uint8_t stall, parallel;
    stall = gpio_get_level(D14);
    parallel = gpio_get_level(D15);

    // if (stall == 0 && parallel == 1)
    // {
    //     stallFlag = 0;
    //     parallelFlag = 6;
    // }
    // if (stall == 1 && parallel == 0)
    // {
    //     stallFlag = 5;
    //     parallel = 0;
    // }
    // if (stall == 0 && parallel == 0)
    // {
    //     stallFlag = 0;
    //     parallelFlag = 0;
    // }
    // if (stall == 1 && parallel == 1)
    // {
    //     stallFlag = 5;
    //     parallelFlag = 6;
    // }

    if (stall == 1 && parallel == 1)
    {
        Menu_array[0] = 6700;
        Menu_array[1] = 6850;
        Menu_array[2] = 7300;
        Menu_array[3] = 11300;
    }
    if (stall == 0 && parallel == 1)
    {
        Menu_array[0] = 6430;
        Menu_array[1] = 6300;
        Menu_array[2] = 7100;
        Menu_array[3] = 10900;
    }
    if (stall == 0 && parallel == 0)
    {
        Menu_array[0] = 6430;
        Menu_array[1] = 6600;
        Menu_array[2] = 7100;
        Menu_array[3] = 10900;
    }
}


/*!
 * @brief       Get direction error
 * 
 * @param       None
 * @return      dirError        Direction error
 */
int16_t GetDirError(uint8_t status)
{
    int16_t dirError;
    uint8_t targetLine = 0;
    switch (status)
    {
    case 0:     // 倒车入库
        targetLine = 87;    // 160 -- 80  300 -- 75
        break;
    case 1:     // 侧方停车
        targetLine = 85;
        break;
    default:
        break;
    }
    for (uint8_t i = 15; i < MT9V03X_W - 15; i+= 10)
    {
        dirError += targetLine - bottomEdge[i];
    }

    return dirError;
}


void ReverseStallParking(void)
{
    static int32_t angle = -14;
    static uint16_t cnt = 0;
    if (stallFlag == 4)
    {
        spdParam[3] = 22;
        if (pinLevel[2] == 1)
        {
            angle = -13;
            pwm_set_duty(SERVO_PIN, GetServoDuty(angle));
        }
        if (angle == -13)
        {
            // GetDistance();
            // if (distance > 3000)
            // {
            if (cnt < 630)
            {
                cnt++;
                return;
            } else if (cnt < 690)
            {
                distance = 0;
                pwm_set_duty(SERVO_PIN, GetServoDuty(0));
                cnt++;
                return;
            } else
            {
                stallFlag = 5;
                spdParam[3] = 52.6;
                angle = 0;
            }
            // }
            
        }
        
    } else
    {
        // pwm_set_duty(TIM2_PWM_CH1_A15, angle);
        spdParam[3] = -30;
        if (stallFlag == 2)
        {
            GetDistance();
        }

        if (distance < 2000)
        {
            gpio_set_level(D12, 1);
        } else
        {
            gpio_set_level(D12, 0);
        }
        

        if (distance < Menu_array[0])
        {
            pwm_set_duty(SERVO_PIN, GetServoDuty(angle));
        } else if (distance > Menu_array[1])
        {
            angle = 0;
            distance = 0;
            pwm_set_duty(SERVO_PIN, GetServoDuty(angle));
            spdParam[3] = 0;
            pwm_set_duty(MOTOR_L_PIN, 1200);
            pwm_set_duty(MOTOR_R_PIN, 1200);
            stallFlag = 3;
            return;
        }
        
    }
}


void ParallelReverseParking(void)
{
    static int32_t angle = -13;
    if (parallelFlag == 5)
    {
        // angle = 13;
        // pwm_set_duty(SERVO_PIN, GetServoDuty(angle));   
        // spdParam[3] = 22;
        // if (pinLevel[3] == 1)
        // {
        //     angle = -12;
        //     pwm_set_duty(SERVO_PIN, GetServoDuty(angle));
        // }
        // if (angle == -12)
        // {
        //     GetDistance();
        //     if (distance > 7000)
        //     {
        //         parallelFlag = 6;
        //         pwm_set_duty(SERVO_PIN, 0);
        //         distance = 0;
        //         spdParam[3] = 0;
        //     }
        // }
        static uint16_t cnt = 0;
        spdParam[3] = 35;
        angle = 14;
        pwm_set_duty(SERVO_PIN, GetServoDuty(angle));
        if (cnt < 200)
        {
            cnt++;
            return;
        } else if (cnt < 350)
        {
            angle = -14;
            pwm_set_duty(SERVO_PIN, GetServoDuty(angle));
            cnt++;
            return;
        } else
        {
            stallFlag = 6;
            spdParam[3] = 0;
            angle = 0;
            pwm_set_duty(SERVO_PIN, GetServoDuty(angle));
        }
    } else
    {
        // pwm_set_duty(TIM2_PWM_CH1_A15, angle);
        spdParam[3] = -0.3;
        if (parallelFlag == 2 || parallelFlag == 3)
        {
            GetDistance();
        }

        if (distance < 7000)
        {
            gpio_set_level(D12, 1);
        } else
        {
            gpio_set_level(D12, 0);
        }

        if (distance < Menu_array[2])
        {
            pwm_set_duty(SERVO_PIN, GetServoDuty(angle));
        } else if (distance < Menu_array[3])
        {
            parallelFlag = 3;
            angle = 13;
            pwm_set_duty(SERVO_PIN, GetServoDuty(angle));
        } else// if (distance > 11600)
        {
            angle = 0;
            pwm_set_duty(SERVO_PIN, GetServoDuty(angle));
            spdParam[3] = 0;
            pwm_set_duty(MOTOR_L_PIN, 7500);
            pwm_set_duty(MOTOR_R_PIN, 7500);
            parallelFlag = 4;
            distance = 0;
            gpio_set_level(D12, 1);
            return;
        }
        
    }
}


void ParkingHandler(void)
{
    if (stallFlag != 0 && stallFlag < 5)
    {
        ReverseStallParking();
        return;
    } else if (parallelFlag != 0)
    {
        ParallelReverseParking();
        return;
    }
    
    if (stallFlag == 0 || (stallFlag == 5 && parallelFlag == 0))
    {
        if ((int)tPosition[0] > 65 && (int)tPosition[0] < 75) 
        {
            if (count == 0)
            {
                num++;
                count = 1;
            }
        } else
        {
            count = 0;
        }

        if (stallFlag == 0)
        {
            ServoControl(GetDirError(0));
            if (num == 4)
            {
                // pwm_set_duty(SERVO_PIN, GetServoDuty(14));
                stallFlag = 1;
                spdParam[3] = 0;
                num = 0;
            }
        } else if (stallFlag == 5 && parallelFlag == 0)
        {
            ServoControl(GetDirError(1));
            if (num == 3)
            {
                // pwm_set_duty(SERVO_PIN, GetServoDuty(14));
                parallelFlag = 1;
                // spdParam[0] = 0;
                num = 0;
            }
        }
    }
}


/*!
 * @brief       Servo PID control
 *
 * @param       dirError        Direction error
 * @return      None
 */
void ServoControl(const float dirError)
{
    static int32_t servoOut = 0;
    
    // Servo pid
    PidInit(&dirPid);
    servoOut = PidPosControl(&dirPid, dirParam, dirError);

    // Servo output
    pwm_set_duty(SERVO_PIN, (SERVO_MID + servoOut));
}


/*!
 * @brief       Speed PID control
 *
 * @param       None
 * @return      None
 */
void SpeedControl(void)
{
    static int16_t leftSpeed = 0;
    static int16_t rightSpeed = 0;
    static int32_t leftSpdOut = 0;
    static int32_t rightSpdOut = 0;


    // Speed output
    if (g_stopFlag == 1)
    {
        // Change target speed to 0
        spdParam[3] = 0;
        leftSpdOut = PidPosControl(&leftSpdPid, spdParam, leftSpeed);
        rightSpdOut = PidPosControl(&rightSpdPid, spdParam, rightSpeed);
    } else if (stallFlag == 1 || parallelFlag == 1)
    {
        // PID calculate
        pwm_set_duty(MOTOR_L_PIN, 0);
        pwm_set_duty(MOTOR_R_PIN, 0);
    } else if (stallFlag == 2 || parallelFlag == 2)
    {
        leftSpdOut = PidPosControl(&leftSpdPid, spdParam, leftSpeed);
        rightSpdOut = PidPosControl(&rightSpdPid, spdParam, rightSpeed);
        leftSpdOut -= 180;
        rightSpdOut += 180;
    } else if (parallelFlag == 3)
    {
        leftSpdOut = PidIncControl(&leftSpdPid, spdParam, leftSpeed);
        rightSpdOut = PidIncControl(&rightSpdPid, spdParam, rightSpeed);
        leftSpdOut += 180;
        rightSpdOut -= 180;
    } else
    {
        // Get encoder speed data
        leftSpeed = encoder_get_count(ENCODER_L_TIM);
        encoder_clear_count(ENCODER_L_TIM);
        rightSpeed = - encoder_get_count(ENCODER_R_TIM);
        encoder_clear_count(ENCODER_R_TIM);

        // PID calculate
        leftSpdOut = PidPosControl(&leftSpdPid, spdParam, leftSpeed);
        rightSpdOut = PidPosControl(&rightSpdPid, spdParam, rightSpeed);
        leftSpdOut += (int32_t)dirPid.out;
        rightSpdOut -= (int32_t)dirPid.out;
    }

    if (leftSpdOut >= 0)
    {
        pwm_set_duty(MOTOR_L_PIN, leftSpdOut);
        gpio_set(MOTOR_L_DIR_PIN, 0);
    } else
    {
        pwm_set_duty(MOTOR_L_PIN, -leftSpdOut);
        gpio_set(MOTOR_L_DIR_PIN, 1);
    }
    if (rightSpdOut >= 0)
    {
        pwm_set_duty(MOTOR_R_PIN, rightSpdOut);
        gpio_set(MOTOR_R_DIR_PIN, 0);
    } else
    {
        pwm_set_duty(MOTOR_R_PIN, -rightSpdOut);
        gpio_set(MOTOR_R_DIR_PIN, 1);
    }
}