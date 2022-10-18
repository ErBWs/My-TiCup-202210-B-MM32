/*
 * @file    gps.h
 * @author  Baohan
 * @date    2022/04/01
 */

#include "gps.h"

/*
 * @brief       Show the GPS info on the screen
 * @param       void
 * @return      void
 */
void ShowAllGpsInfo(void)
{
    // GPS info
    tft180_show_string(0, 0, "gps_state:"); // Whether the GPS is working
    tft180_show_string(0, 1, "longitude:"); // 经
    tft180_show_string(0, 2, "latitude :"); // 纬
    tft180_show_string(0, 3, "speed    :");
    tft180_show_string(0, 4, "direction:");
    tft180_show_string(0, 5, "satellite:"); // The number of working satellite
    if (gps_tau1201_flag)
    {
        gps_data_parse();

        tft180_show_uint(10, 0, gps_tau1201.state, 1);
        tft180_show_float(10, 1, gps_tau1201.longitude, 3, 5);
        tft180_show_float(10, 2, gps_tau1201.latitude, 3, 5);
        tft180_show_float(10, 3, gps_tau1201.speed, 3, 4);
        tft180_show_float(10, 4, gps_tau1201.direction, 3, 4);
        tft180_show_uint(10, 5, gps_tau1201.satellite_used, 4);
        gps_tau1201_flag = 0;
    }
}


/*
 * @brief       Get GPS data
 * @param       void
 * @return      void
 */
void LocateGps(void)
{
    tft180_show_string(0, 7, "gps:");
    if (gps_tau1201_flag)
    {
        gps_data_parse();

        tft180_show_uint(4, 7, gps_tau1201.state, 1); // Whether the GPS is working
        gps_tau1201_flag = 0;
    }
}


/*
 * @brief       Convert double data(8 Byte) into unsigned int data(4 Byte)
 * @param       val     The object to be converted
 * @param       *arr    Target uint32 array
 * @note        Use a "double" pointer to point to this array, 
 *              or use memcpy function
 *              can convert int data back to double
 */
void DoubleToInt(double val, uint32_t *arr)
{
    memcpy(arr, &val, 2 * sizeof(uint32_t));
}


/*
 * @brief       Save points to flash
 * @param       state       0:Save points to flash buffer array
 *                          1:Write buffer array to flash
 * @return      void
 * @note        When finished saving all points, use SavePoints(1) to write data to flash
 */
void SavePoints(uint8_t state)
{
    static uint16_t bufIndex = 0;      // Flash buffer array index(0-255)
    static uint8_t secIndex = 127;     // Flash section index(127-)
    static uint8_t pageIndex = 0;      // Flash page index(0-3)

    uint32_t arr[2];

    switch (state)
    {
        case 0:
        {
            if (bufIndex < 256)
            {
                // Save longitude info in buffer array
                DoubleToInt(gps_tau1201.longitude, arr);
                flash_union_buffer[bufIndex].uint32_type = arr[0];
                flash_union_buffer[++bufIndex].uint32_type = arr[1];

                // Save latitude info in buffer array
                DoubleToInt(gps_tau1201.latitude, arr);
                flash_union_buffer[++bufIndex].uint32_type = arr[0];
                flash_union_buffer[++bufIndex].uint32_type = arr[1];

                bufIndex++;
            } else if (pageIndex < 4)
            {
                // Change page automatically
                flash_write_page_from_buffer(secIndex, pageIndex);
                flash_buffer_clear();
                pageIndex++;
                bufIndex = 0;
                SavePoints(0);
            } else
            {
                // Change section automatically
                flash_write_page_from_buffer(secIndex, pageIndex);
                flash_buffer_clear();
                pageIndex = 0;
                bufIndex = 0;
                secIndex--;
                SavePoints(0);
            }
            break;
        }
            
        case 1:
        {
            if (!bufIndex)
            {
                break;
            } else
            {
                /* Erase certain flash page if there are data in it
                 * This part is annotated because this check is done in flash write function */
                // if (flash_check(secIndex, pageIndex))
                //     flash_erase_page(secIndex, pageIndex);
                flash_write_page_from_buffer(secIndex, pageIndex);
                flash_buffer_clear();
                break;
            }
        }

        default:
            break;
    }
}