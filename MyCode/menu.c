/*!
 * @file    menu.c
 * @author  Baohan
 * @date    2022/06/18
 */


#include "menu.h"


uint8_t runFlag = 0;
EasyKey_t keyUp, keyDown, keyLeft, keyRight;
//keyCenter;


int Menu_array[7] = {6430, 6600, 7100, 11000, 0, 0, 0};
// int Menu_array[7] = {6550, 6850, 7300, 11150, 0, 0, 0};

/*!
 * @brief       Init of menu and dip switch
 *
 * @return      None
 */
void MenuInit(void)
{
    //DIP switch init
    gpio_init(D14, GPI, GPIO_LOW, GPI_PULL_UP);
    gpio_init(D15, GPI, GPIO_LOW, GPI_PULL_UP);
    tft180_show_string(0, 2 * 16, "menu: ==");
    system_delay_ms(100);

    //5_dir switch init
    EasyKeyInit(&keyUp, D0, SCANNER_PERIOD);
    EasyKeyInit(&keyDown, D1, SCANNER_PERIOD);
    EasyKeyInit(&keyLeft, D2, SCANNER_PERIOD);
    EasyKeyInit(&keyRight, D3, SCANNER_PERIOD);
    flash_read_page_to_buffer(FLASH_SECTION_127, FLASH_PAGE_3);//flash读取到缓冲区
    // EasyKeyInit(&keyCenter, A8, SCANNER_PERIOD);
    tft180_show_string(0, 2 * 16, "menu: ====");
    system_delay_ms(100);

    // Timer init 
	pit_ms_init(TIM8_PIT, 10);
	interrupt_set_priority(TIM8_UP_IRQn, 2);
    //pit_interrupt_ms(CCU6_1, PIT_CH0, 10);       // Use 10ms interrupt: cc61_pit_ch0
    tft180_show_string(0, 2 * 16, "menu: ===== [done]");
}


/*!
 * @brief       Main page of menu
 *
 * @return      None
 */
void ShowMenuMainPage(void)
{
    tft180_show_string(0, 0, "Car");
    // tft180_show_string(0, 1 * 16, "+");
    tft180_show_string(0, 1 * 16, "  1.start");
    tft180_show_string(0, 2 * 16, "  2.RSP");
    tft180_show_string(0, 3 * 16, "  3.PRP");
    tft180_show_string(0, 4 * 16, "  4.TAT");
    tft180_show_string(0, 5 * 16, "  5.show image");
}
/*
*@brief page of motor params
*
*@return None 
*/
/*!
 * @brief       Adjust float data
 *
 * @param       val     Target value
 * @return      None
 */
void ConfigFloat(float *val)
{
    
    static uint8_t add = 0;
    static float add_num = 1;
    while (keyUp.state!= release)
    {
        if (keyUp.state == hold)
        {
            add += 1;
            add = add%3;
            if(add == 0)
            {
                add_num = 1;
            }
            else if(add == 1)
            {
                add_num = 0.1f;
            }
            else if(add == 2)
            {
                add_num = 0.01f;
            }
            tft180_show_uint(85,0* 16,add,3);
            break;
        }
        else if (keyDown.state == hold)
        {
            break;
        }
        /* code */
    }
    while (keyLeft.state != release)
    {   
        if (keyLeft.state == press)
        {
            
            *val -= add_num;;
            break;
        }
        
    }

    while (keyRight.state != release)
    {
        if (keyRight.state == press)
        {
            
            *val += add_num;
            break;
        } 
       
    }
}
/*!
 * @brief       Adjust int data
 *
 * @param       val     Target value
 * @return      None
 */
void ConfigInt(int *val)
{
    static uint8_t add = 0;
    static uint32_t add_num = 1;
    while (keyUp.state!= release)
    {
        if (keyUp.state == hold)
        {
            add += 1;
            add = add%3;
            if(add == 0)
            {
                add_num = 1;
            }
            else if(add == 1)
            {
                add_num = 10;
            }
            else if(add == 2)
            {
                add_num = 100;
            }
            tft180_show_uint(85,0* 16,add,3);
            break;
        }
        else if (keyDown.state == hold)
        {
            break;
        }
    }
    while (keyLeft.state != release)
    {
        if (keyLeft.state == press)
        {
            
            *val -= add_num;
            break;
        }
        
    }

    while (keyRight.state != release)
    {
        if (keyRight.state == press)
        {
            
            *val += add_num;
            break;
        } 
        
    }
}


/*!
 * @brief       Page of motor params
 *
 * @return      None
 */
void RSP(void)
{
    tft180_full(RGB565_BLACK);
    static uint8_t index = 1;
    static uint8_t index_1 = 0;
    
    tft180_show_string(0, 0 * 16, "ReverseStall P");
    tft180_show_string(0, 1 * 16, "1.D1");
    tft180_show_string(0, 2 * 16, "2.D2");
    tft180_show_string(0, index * 16, "-");

    tft180_show_float(85, 1 * 16, Menu_array[0], 5, 2);
    tft180_show_float(85, 2 * 16, Menu_array[1], 5, 2);

    while (1)
    {
        if (keyUp.state == press && index > 1)
            index--;
        else if (keyDown.state == press && index < 4)
            index++;
        if (index_1 == index)
        {
            tft180_show_string(0, index * 16, "-");
        } 
        else
        {
            tft180_show_string(0, 0 * 16, "ReverseStall P");
            tft180_show_string(0, 1 * 16, "1.D1");
            tft180_show_string(0, 2 * 16, "2.D2");
            tft180_show_string(0, index * 16, "-");

            tft180_show_float(85, 1 * 16, Menu_array[0], 5, 2);
            tft180_show_float(85, 2 * 16, Menu_array[1], 5, 2);
            index_1 = index;
        }
        
        switch (index)
        {
            case 1:
                ConfigInt(&Menu_array[0]);
                break;
            case 2:
                ConfigInt(&Menu_array[1]);
                break;
            default:
                break;
        }
    
        tft180_show_float(85, 1 * 16, Menu_array[0], 5, 2);
        tft180_show_float(85, 2 * 16, Menu_array[1], 5, 2);

        // if (keyCenter.state == hold)
        if (keyDown.state == hold)
        {
            tft180_full(RGB565_BLACK);
            break;
        }
    }
}
/*!
 * @brief       Page of servo params
 *
 * @return      None
 */
void PRP(void)
{
    tft180_full(RGB565_BLACK);
    static uint8_t index = 1;
    static uint8_t index_1 = 0;
    tft180_show_string(0, 0 * 16, "ParallelReverse P");
    tft180_show_string(0, index * 16, "-");
    tft180_show_string(0, 1 * 16, "1.D1");
    tft180_show_string(0, 2 * 16, "2.D2");
    tft180_show_string(0, 3 * 16, "3.D3");

    tft180_show_float(85, 1, Menu_array[2], 5, 2);
    tft180_show_float(85, 2, Menu_array[3], 5, 2);
    tft180_show_float(85, 3, Menu_array[4], 5, 2);

    while (1)
    {
        if (keyUp.state == press && index > 1)
            index--;
        else if (keyDown.state == press && index < 3)
            index++;
        if (index_1 == index)
        {
            tft180_show_string(0, index * 16, "-");
        }
        else
        {
            tft180_show_string(0, 0 * 16, "ParallelReverse P");
            tft180_show_string(0, index * 16, "-");
            tft180_show_string(0, 1 * 16, "1.D1");
            tft180_show_string(0, 2 * 16, "2.D2");
            tft180_show_string(0, 3 * 16, "3.D3");

            tft180_show_float(85, 1, Menu_array[2], 5, 2);
            tft180_show_float(85, 2, Menu_array[3], 5, 2);
            tft180_show_float(85, 3, Menu_array[4], 5, 2);
            index_1 = index;
        }
        switch (index)
        {
            case 1:
                ConfigInt(&Menu_array[0]);
                break;
            case 2:
                ConfigInt(&Menu_array[1]);
                break;
            case 3:
                ConfigInt(&Menu_array[2]);
                break;
            default:
                break;
        }
        tft180_show_float(85, 1 * 16, Menu_array[0], 5, 2);
        tft180_show_float(85, 2 * 16, Menu_array[1], 5, 2);
        tft180_show_float(85, 3 * 16, Menu_array[2], 5, 2);

        // if (keyCenter.state == hold)
        if (keyDown.state == hold)
        {
            tft180_full(RGB565_BLACK);
            break;
        }
            
    }
}
void TAT(void)
{
    tft180_full(RGB565_BLACK);
    static uint8_t index = 1;
    static uint8_t index_1 = 0;
    tft180_show_string(0, 0 * 16, "T&Thresholding");
    tft180_show_string(0, index * 16, "-");
    tft180_show_string(0, 1 * 16, "1.T");
    tft180_show_string(0, 2 * 16, "2.THR");

    tft180_show_float(85, 1, Menu_array[5], 5, 2);
    tft180_show_float(85, 2, Menu_array[6], 5, 2);

    while (1)
    {
        if (keyUp.state == press && index > 1)
            index--;
        else if (keyDown.state == press && index < 2)
            index++;
        if (index_1 == index)
        {
            tft180_show_string(0, index * 16, "-");
        }
        else
        {
            tft180_show_string(0, 0 * 16, "T&Thresholding");
            tft180_show_string(0, index * 16, "-");
            tft180_show_string(0, 1 * 16, "1.T");
            tft180_show_string(0, 2 * 16, "2.THR");

            tft180_show_float(85, 1, Menu_array[5], 5, 2);
            tft180_show_float(85, 2, Menu_array[6], 5, 2);
            index_1 = index;
        }
        switch (index)
        {
            case 1:
                ConfigInt(&Menu_array[5]);
                break;
            case 2:
                ConfigInt(&Menu_array[6]);
                break;
            default:
                break;
        }
        tft180_show_float(85, 1 * 16, Menu_array[5], 5, 2);
        tft180_show_float(85, 2 * 16, Menu_array[6], 5, 2);
        // if (keyCenter.state == hold)
        if (keyDown.state == hold)
        {
            tft180_full(RGB565_BLACK);
            break;
        }            
    }
}
/*
*备份，复原
*
*/
/*void BackUP_Recover(uint8_t BR)
{
    if(BR == 0)
    {
        for (size_t i = 0; i < count; i++)
        {
            flash_data_union.int16_type[i] = Menu_array[i];
        }
        flash_write_page_from_buffer(FLASH_SECTION_127, FLASH_PAGE_3);
    }
    else(BR == 1)
    {
        for (size_t i = 0; i < count; i++)
        {
            Menu_array[i] = flash_data_union.int16_type[i];
        }
    }
}*/
/*!
 * @brief       Show Status of dip switch
 *
 * @return      None
 */
void DipState(void)
{
    uint8_t dip1 = !gpio_get_level(D14);
    uint8_t dip2 = !gpio_get_level(D15);

    // DIP1
    tft180_show_string(0, 4, "- dip1");
    if (dip1) tft180_show_string(136, 4, " on");
    else tft180_show_string(136, 4, "off");

    // DIP2
    tft180_show_string(0, 5, "- dip2:");
    if (dip2) tft180_show_string(136, 5, " on");
    else tft180_show_string(136, 5, "off");
}
/*!
 * @brief       Show images, including camera images and processed images
 *
 * @return      None
 */
void ShowImage(void)
{
    tft180_full(RGB565_BLACK);
    while (1)
    {
        // tft180_displayimage032_zoom((const uint8_t *)&BinarizedImage, 188, 120, 160, 128);
		tft180_displayimage032_zoom((const uint8_t *)&BinarizedImage, 188, 120, 160, 120);
        lcd_draw_bottomline(bottomEdge,RGB565_RED);
        lcd_draw_Ts(tPosition,bottomEdge,tNumber);

        // if (keyCenter.state == hold)
        if (keyUp.state == hold)
        {
            tft180_full(RGB565_BLACK);
            break;
        }
    }
}
/*!
 * @brief       Main function of menu
 *
 * @return      None
 */
void MenuOperator(void)
{
    tft180_full(RGB565_BLACK);
    static uint8_t index = 1;
    static uint8_t index_1 = 0;
    ShowMenuMainPage();

    while (runFlag == 0)
    {
        if (keyUp.state == press && index > 1)
            index--;
        else if (keyDown.state == press && index < 6)
            index++;
        if (index_1 == index)
        {
            tft180_show_string(0, index * 16, "+");
        }
        else
        {
            ShowMenuMainPage();
            tft180_show_string(0, index * 16, "+");
            index_1 = index;
        }
        
        //ShowMenuMainPage();
        //tft180_show_string(0, index * 16, "+");

        // if (keyRight.state == press)
        if (keyUp.state == hold)
        {
            switch (index)
            {
                case 1:
                    runFlag = 1;
                    pit_enable(TIM7_PIT);
                    tft180_full(RGB565_BLACK);
                    break;
                case 2:
                    RSP();
                    break;
                case 3:
                    PRP();
                    break;
                case 4:
                    TAT();
                    break;
                case 5:
                    ShowImage();
                    break;
                /*case 6:
                    BackUP_Recover(0);
                    break;
                case 7:
                    BackUP_Recover(1);
                    break;*/
                default:
                    break;
            }
            index = 1;
        }
    }
    // interrupt_global_enable();
}


/*!
 * @brief       This function is used for testing other function
 *
 * @param       None
 * @return      None
 *
 * @note        This function should be called below EasyKeyHandler() in timer interrupt
 */
void TestBench(void)
{
    switch (keyUp.state)
    {
    case press:
        tft180_show_string(0, 0, "press      ");
        break;
    case multiClick:
        tft180_show_string(0, 0, "multi-click");
        break;
    case hold:
        tft180_show_string(0, 0, "hold       ");
        break;
    default:
        break;
    }
}