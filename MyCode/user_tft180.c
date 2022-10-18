

#include "user_tft180.h"

//显示这格和附近24格（含缩放处理）
void lcd_drawpoints_big_around(uint16 x,uint16 y,uint16 color)
{
    int x_i,y_j;
    x_i=x;
    y_j=y;
    int i,j;
    for(i=x_i-2;i<=x_i+2;i++)
    {
        for(j=y_j-2;j<=y_j+2;j++)
        {
            if((i>=2)&&(i<=186)&&(j>=2)&&(j<=118))
            tft180_draw_point(((uint16)(i/187.0*159)),(uint16)j,color);
        }
    }
}

//显示这格和下边的格
void lcd_drawpoints_big_down(int16 x,int16 y,uint16 color)
{
    int x_i,y_j;
    x_i=x;
    y_j=y;
    int i,j;
    for(i=x_i;i<=x_i;i++)
    {
        for(j=y_j;j<=y_j+1;j++)
        {
            if((i>=0)&&(i<=159)&&(j>=0)&&(j<=119))
            tft180_draw_point((uint16)i,(uint16)j,color);
        }
    }
}

//显示这数组对应列并向下加粗（含缩放）
//参数：数组坐标地址，图像行数，结束行，颜色
//int16数组专用
void lcd_draw_bottomline(int16 *line,uint16 color)
{
    int16 i;
    for(i=0;i<188;i++)
    {
        lcd_drawpoints_big_down((uint16)(i/187.0*159),line[i],color);
    }
}

//显示Td的位置
//参数：T位置数组地址，底边坐标数组，T数量
void lcd_draw_Ts(float *tPosition,int16 *bottomEdge,int16 tNumber)
{
    int16 i;
    for(i=0;i<tNumber;i++)
    {
        lcd_drawpoints_big_around((int)tPosition[i],bottomEdge[(int)tPosition[i]],RGB565_BLUE);
    }
}




//显示这格和右边的格
void lcd_drawpoints_big_r(uint16 x,uint16 y,uint16 color)
{
    int x_i,y_j;
    x_i=x;
    y_j=y;
    int i,j;
    for(i=x_i;i<=x_i+1;i++)
    {
        for(j=y_j;j<=y_j;j++)
        {
            if((i>=0)&&(i<=159)&&(j>=0)&&(j<=119))
            tft180_draw_point((unsigned short)i,(unsigned short)j,color);
        }
    }
}

//显示这格和左边的格
void lcd_drawpoints_big_l(uint16 x,uint16 y,uint16 color)
{
    int x_i,y_j;
    x_i=x;
    y_j=y;
    int i,j;
    for(i=x_i-1;i<=x_i;i++)
    {
        for(j=y_j;j<=y_j;j++)
        {
            if((i>=0)&&(i<=159)&&(j>=0)&&(j<=119))
            tft180_draw_point((unsigned short)i,(unsigned short)j,color);
        }
    }
}

//显示这格和左右两边的格
void lcd_drawpoints_big_lr(uint16 x,uint16 y,uint16 color)
{
    int x_i,y_j;
    x_i=x;
    y_j=y;
    int i,j;
    for(i=x_i-1;i<=x_i+1;i++)
    {
        for(j=y_j;j<=y_j;j++)
        {
            if((i>=0)&&(i<=159)&&(j>=0)&&(j<=119))
            tft180_draw_point((unsigned short)i,(unsigned short)j,color);
        }
    }
}

//显示这数组对应列并向左右两边加粗（含缩放）
//参数：数组坐标地址，图像行数，结束行，颜色
//float数组专用
void lcd_drawline_lie_big_lr(float *line,uint16 zonghangshu,int16 end,uint16 color)
{
    int16 i;
    for(i=0;i<end;i++)
    {
        lcd_drawpoints_big_lr((unsigned short)(line[i]/188.0*160),(unsigned short)(zonghangshu-1-i),color);
    }
}

//显示这数组对应列并向左右两边加粗（含缩放）
//参数：数组坐标地址，图像行数，结束行，颜色
//int16数组专用
void lcd_drawline_lie_big_lr2(int16 *line,uint16 zonghangshu,int16 end,uint16 color)
{
    int16 i;
    for(i=0;i<end;i++)
    {
        lcd_drawpoints_big_lr((unsigned short)(line[i]/188.0*160),(unsigned short)(zonghangshu-1-i),color);
    }
}

//显示这行
//参数：要输出的行数（从下往上），图像高度，颜色
//int16数组专用
void lcd_drawline_hang(int16 hang,int16 tuxianggaodu,uint16 color)
{
    int16 i;
    hang=tuxianggaodu-hang;
    if((hang>=0)&&(hang<=tuxianggaodu))
    {
        for(i=0;i<=159;i++)
        {
            tft180_draw_point(i,(unsigned short)hang,color);
        }
    }
}


