#ifndef USER_TFT180_H_
#define USER_TFT180_H_

#include "zf_common_headfile.h"


void lcd_drawpoints_big_around(uint16 x,uint16 y,uint16 color);
void lcd_drawpoints_big_down(int16 x,int16 y,uint16 color);
void lcd_draw_bottomline(int16 *line,uint16 color);
void lcd_draw_Ts(float *tPosition,int16 *bottomEdge,int16 tNumber);
void lcd_drawpoints_big_r(uint16 x,uint16 y,uint16 color);
void lcd_drawpoints_big_l(uint16 x,uint16 y,uint16 color);
void lcd_drawpoints_big_lr(uint16 x,uint16 y,uint16 color);
void lcd_drawline_lie_big_lr(float *line,uint16 zonghangshu,int16 end,uint16 color);
void lcd_drawline_lie_big_lr2(int16 *line,uint16 zonghangshu,int16 end,uint16 color);
void lcd_drawline_hang(int16 hang,int16 tuxianggaodu,uint16 color);


#endif /* MY_TFT18_H_ */
