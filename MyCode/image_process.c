/*
 * @file    image_process.c
 * @author  Baohan
 * @date    2022/10/14
 */

#include "image_process.h"

uint8_t BinarizedImage[HEIGHT][WIDTH] = {{0}};
float tPosition[20];
int16_t tNumber;
int16_t bottomEdge[MT9V03X_W];     //底线的纵坐标
/*!
 *  @brief      Binarize the input image
 *
 *  @param      image[]       二值化后的数组
 *  @return     NULL
 */
void ImageBinarization(uint8_t imageIn[HEIGHT][WIDTH], uint8_t threshold, uint8_t imageOut[HEIGHT][WIDTH])
{
    uint16_t width = WIDTH - 1;  //最后一列不参与运算  187
    uint16_t height = HEIGHT;
    uint8_t *data = imageIn[0];  //指向像素数据的指针
    for (uint16_t i = 0; i < height; i++)
    {
        for (uint16_t j = 0; j < width; j++)
        {
            if(data[i * 188 + j]>threshold)
                imageOut[i][j]=255;
            else
                imageOut[i][j]=0;
        }
    }
}


/*!
 *  @brief      图像噪点过滤
 *
 *  @param      image[]       二值化后的数组
 *  @return     NULL
 */
void FiltrateImage(uint8_t image[HEIGHT][WIDTH])
{
    uint8_t row; //行
    uint8_t line; //列

    for (row = 1; row < HEIGHT - 1; row++)
    {
        for (line = 1; line < HEIGHT - 1; line = line + 1)
        {
            if ((image[row][line] == 0)
                    && (image[row - 1][line] + image[row + 1][line] + image[row][line + 1] + image[row][line - 1] > 510))
            {
                image[row][line] = 255;
            }
            else if ((image[row][line] == 255)
                    && (image[row - 1][line] + image[row + 1][line] + image[row][line + 1] + image[row][line - 1] < 510))
            {
                image[row][line] = 0;
            }
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  找下边界，参数为图像数组地址，宽度（列数）,高度（行数），底边数组地址
//-------------------------------------------------------------------------------------------------------------------
void FindBottomEdge(uint8 *image, uint16 col, uint16 row ,int16 *bottomEdge)
{
    uint8* data = image;
    int16 i;  //col188
    int16 j;  //row120
    uint8 color_change_count;  //变色计数
	for(i = 10; i < col - 10; i++)
	{
		bottomEdge[i] = row;
		color_change_count=0;
		for(j=row-1; j >= 0; j--)
		{
			if(data[j * col + i] == 0)
				color_change_count++;
			else
				color_change_count=0;
			if(color_change_count==2)
			{
				bottomEdge[i]=j+1;
				break;
			}	
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  找T字形，参数为图像数组地址，宽度（列数）,高度（行数），底边数组地址，T字位置数组地址
//  返回T字个数
//-------------------------------------------------------------------------------------------------------------------
int16 FindTSign(uint8 *image, uint16 col, uint16 row ,int16 *bottomEdge,float *tPosition)
{
    uint8* data = image;
    int16 i;  //col188
    int16 j;  //row120
	int16 T_num=0;             //T字形数量
	int16 bottom_width[col];   //底边宽度数组
    uint8 color_change_count;  //变色计数
	for(i=60;i<130;i++)
	{
		bottom_width[i]=0;
		color_change_count=0;
		for(j=bottomEdge[i]-1;j>=0;j--)
		{
			if(data[j*col+i]==255)
				color_change_count++;
			else
				color_change_count=0;
			if(color_change_count==2)
			{
				break;
			}
			bottom_width[i]++;
		}
	}
	int16 start=0;
	for(i=60;i<130;i++)
	{
		if(start==0)
		{
			if(bottom_width[i]>WIDTH_THRESHOLD)
			{
				color_change_count=0;
				start=i;
			}				
		}
		else
		{
			if(bottom_width[i]<=WIDTH_THRESHOLD)
			{
				tPosition[T_num]=(start+i-1)/2.0;
				T_num++;
				start=0;
				if(T_num==18)
					break;
			}			
		}
	}
	if(start!=0)
	{
		tPosition[T_num]=(start+i)/2.0;
		T_num++;
	}
	
	return T_num;
}


