#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
//OLED模式设置
//0:4线串行模式
//1:并行8080模式
//#define Show_By_Buffer 1
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------使用4线串行接口OLED端口定义----------------  					   

extern char Whole_map[8][128];
#define OLED_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_6)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void clear_wholemap(void);
void plot_wholemap(void);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_Showtemp(u8 x, u8 y);
void OLED_Showdamp(u8 x, u8 y);
void OLED_ShowSettemp(u8 x, u8 y);
void OLED_Showpredam(u8 x, u8 y);
void OLED_Shownowdam(u8 x, u8 y);
void OLED_Show_expect_damp(u8 x, u8 y);
void OLED_ShowmodeNum(u8 x,u8 y,u32 num,u8 len,u8 size2,u8 show_mode);
void OLED_ShowmodeChar(u8 x,u8 y,u8 chr,u8 show_mode);
void LCD_Show_CEStr(u16 x0,u16 y0, u8 *pp);
void LCD_Show_ModeCEStr(u16 x0,u16 y0, u8 *pp, u8 show_mode);
void draw_cell(u8 x, u8 y, u8 remain_len, u8 all_len);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

#endif 


