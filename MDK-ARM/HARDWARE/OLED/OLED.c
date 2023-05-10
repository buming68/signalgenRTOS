 //              ----------------------------------------------------------------
//              GND    µçÔ´µØ
//              VCC  ½Ó5V»ò3.3vµçÔ´
//              D0   ½ÓPB6£¨SCL£©
//              D1   ½ÓPB7£¨SDA£©
//              RES  ½ÓPB8
//              DC   ½ÓPB9              
//              ----------------------------------------------------------------

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
//OLEDµÄÏÔ´æ
//´æ·Å¸ñÊ½ÈçÏÂ.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
char Whole_map[8][128];

//ÏòSSD1106Ð´ÈëÒ»¸ö×Ö½Ú¡£
//dat:ÒªÐ´ÈëµÄÊý¾Ý/ÃüÁî
//cmd:Êý¾Ý/ÃüÁî±êÖ¾ 0,±íÊ¾ÃüÁî;1,±íÊ¾Êý¾Ý;
#if OLED_MODE==1
//?SSD1106???????
//dat:??????/??
//cmd:??/???? 0,????;1,????;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	DATAOUT(dat);	    
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		   
//	OLED_CS_Clr();
	OLED_WR_Clr();	 
	OLED_WR_Set();
//	OLED_CS_Set();	  
	OLED_DC_Set();	 
} 	    	    
#else
//?SSD1306???????
//dat:??????/??
//cmd:??/???? 0,????;1,????;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
//	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
			{
		   OLED_SDIN_Set();
			}
else
		   OLED_SDIN_Clr();
				OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
//	OLED_CS_Set();
	OLED_DC_Set();   	  
} 
#endif
	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//??OLED??    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC??
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//??OLED??     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC??
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//????,???,????????!??????!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //?????(0~7)
		OLED_WR_Byte (0x00,OLED_CMD);      //??????—????
		OLED_WR_Byte (0x10,OLED_CMD);      //??????—????   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //????
}


//???????????,??????
//x:0~127
//y:0~63
//mode:0,????;1,????				 
//size:???? 16/12
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//???????			
		if(x>Max_Column-1){x=0;y=y+2;}
	if((c + ' ') < 128)	
	{
		if(SIZE ==16)
		{
			OLED_Set_Pos(x,y);
			for(i=0;i<8;i++)
				OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
				OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
		else 
		{	
			OLED_Set_Pos(x,y+1);
			for(i=0;i<6;i++)
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);	
		}
	}
	else
	{
		c = c -128;
		if(SIZE ==16)
		{
			OLED_Set_Pos(x,y);
			for(i=0;i<8;i++)
				OLED_WR_Byte(~F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
				OLED_WR_Byte(~F8X16[c*16+i+8],OLED_DATA);
		}
		else 
		{	
			OLED_Set_Pos(x,y+1);
			for(i=0;i<6;i++)
			OLED_WR_Byte(~F6x8[c][i],OLED_DATA);	
		}
	}
}
//mode = 0??????
void OLED_ShowmodeChar(u8 x,u8 y,u8 chr,u8 show_mode)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//???????			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			{
				if(show_mode == 1)
					OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
				else
					OLED_WR_Byte(~F8X16[c*16+i],OLED_DATA);	
			}
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			{
				if(show_mode == 1)
					OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
				else
					OLED_WR_Byte(~F8X16[c*16+i+8],OLED_DATA);	
			}
			}
			else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n??
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//??2???
//x,y :????	 
//len :?????
//size:????
//mode:??	0,????;1,????
//num:??(0~4294967295);	 		  
void OLED_ShowmodeNum(u8 x,u8 y,u32 num,u8 len,u8 size2,u8 show_mode)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowmodeChar(x+(size2/2)*t,y,' ',show_mode);
				continue;
			}else enshow=1; 
		 	 
		}
		OLED_ShowmodeChar(x+(size2/2)*t,y,temp+'0',show_mode);
	}
}
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0'); 
	}
} 
//????????
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//????
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********????:????BMP??128×64?????(x,y),x???0~127,y?????0~7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


//???SSD1306					    
void OLED_Init(void)
{
  u8 i, j;
  GPIO_InitTypeDef  GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //??PB????

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	 //PB6~9????  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //??50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	                 //???GPIOB6~9
	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//????
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0???? 0xa1??
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0???? 0xc8??
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 
	OLED_Clear();
	OLED_Set_Pos(0,0);                                                                              
	for(i = 0; i < 8; i++)
		for(j = 0; j < 128; j++)
			Whole_map[i][j] = 0;
} 

void OLED_DrawPoint(u8 x,u8 y)
{
  u8 x_index;
  u8 y_index, col_move;
  x_index = x%128;
  y_index = (y / 8)%8;
  col_move = y % 8;
  Whole_map[y_index][x_index] |= 0x01 << col_move;
 }
void clear_wholemap(void)
{
  u8 i, j;
  for(i = 0; i < 8; i++)
    for(j = 0; j < 128; j++)
       Whole_map[i][j] = 0;
}
void plot_wholemap(void)
{
  u8 i, j;
//  OLED_Set_Pos(0,0);
  for(i = 0; i < 8; i++)
  {
    OLED_Set_Pos(0,i);
    for(j = 0; j < 128; j++)
      OLED_WR_Byte(Whole_map[i][j],OLED_DATA);
  }
}
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //?????? 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //?????? 
	else if(delta_x==0)incx=0;//??? 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//??? 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //????????? 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//???? 
	{  
                OLED_DrawPoint(uRow,uCol);
//		LCD_DrawPoint(uRow,uCol);//?? 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}
void OLED_Showtemp(u8 x, u8 y)
{
	OLED_ShowCHinese(x,y,0);
	OLED_ShowCHinese(x+16,y,1);
	OLED_ShowCHinese(x+32,y,4);
	OLED_ShowCHinese(x+48,y,5);
}
void OLED_Showpredam(u8 x, u8 y)
{
	OLED_ShowCHinese(x,y,8);
	OLED_ShowCHinese(x+16,y,6);
}
void OLED_Shownowdam(u8 x, u8 y)
{
	OLED_ShowCHinese(x,y,0);
	OLED_ShowCHinese(x+16,y,1);
}
void OLED_ShowSettemp(u8 x, u8 y)
{
	OLED_ShowCHinese(x,y,6);
	OLED_ShowCHinese(x+16,y,7);
	OLED_ShowCHinese(x+32,y,2);
	OLED_ShowCHinese(x+48,y,3);
}
void OLED_Showdamp(u8 x, u8 y)
{
	OLED_ShowCHinese(x,y,0);
	OLED_ShowCHinese(x+16,y,1);
	OLED_ShowCHinese(x+32,y,2);
	OLED_ShowCHinese(x+48,y,3);
}
void OLED_Show_expect_damp(u8 x, u8 y)
{
	OLED_ShowCHinese(x,y,6);
	OLED_ShowCHinese(x+16,y,7);
	OLED_ShowCHinese(x+32,y,2);
	OLED_ShowCHinese(x+48,y,3);
}
void LCD_Show_CEStr(u16 x0,u16 y0, u8 *pp)
{
	volatile u8 temp,i, j;
	volatile u16 y=y0;
	volatile u16 x=x0;
	volatile u8 *p = pp;
	
	volatile u16 index = 0;
	while(*p != 0)
	{
		if((*p) < 128)
		{
			y = y0;
			x = x0;
			OLED_ShowChar(x,y, *p);
			x0 += 8;
			p++;
		}
		else
		{
			for(index = 0; index< NUM_GB16; index++)
			{
				if((*p) == codeGB_16[index].Index[0])
					if((*(p+1)) == codeGB_16[index].Index[1])
							break;
			}
//			OLED_ShowNum(0,0,index,3,16);
//			delay_ms(200);
			if(index == NUM_GB16) index = 0;
			for(j = 0; j < 2; j++)
			{
				x = x0;
				y = y0 + j;
				OLED_Set_Pos(x0,y);
				for(i = 0; i < 16; i++)
				{
					temp = codeGB_16[index].Msk[j*16+i];
					OLED_WR_Byte(temp,OLED_DATA);
				}
			}
			x0 += 16;
			p+=2;
		}
	}
}
void LCD_Show_ModeCEStr(u16 x0,u16 y0, u8 *pp, u8 show_mode)
{
	volatile u8 temp,i, j;
	volatile u16 y=y0;
	volatile u16 x=x0;
	volatile u8 *p = pp;
	
	volatile u16 index = 0;
	while(*p != 0)
	{
		if((*p) < 128)
		{
			y = y0;
			x = x0;
			OLED_ShowmodeChar(x,y, *p, show_mode);
			x0 += 8;
			p++;
		}
		else
		{
			for(index = 0; index< NUM_GB16; index++)
			{
				if((*p) == codeGB_16[index].Index[0])
					if((*(p+1)) == codeGB_16[index].Index[1])
							break;
			}
			if(index == NUM_GB16) index = 0;
			for(j = 0; j < 2; j++)
			{
				x = x0;
				y = y0 + j;
				OLED_Set_Pos(x0,y);
				for(i = 0; i < 16; i++)
				{
					temp = codeGB_16[index].Msk[j*16+i];
					if(show_mode)
						OLED_WR_Byte(temp,OLED_DATA);
					else
						OLED_WR_Byte(~temp,OLED_DATA);
				}
			}
			x0 += 16;
			p+=2;
		}
	}
}
void draw_cell(u8 x, u8 y, u8 remain_len, u8 all_len)
{
	u8 i, x_start;

	OLED_Set_Pos(x,y);
	for(i = 0; i < 4; i++)
		OLED_WR_Byte(0XFC,OLED_DATA);
	OLED_WR_Byte(0XFF,OLED_DATA);
	for(i = 0; i < all_len; i++)
		OLED_WR_Byte(0X03,OLED_DATA);
	OLED_WR_Byte(0XFF,OLED_DATA);
	x_start = x + 5 + all_len - remain_len;
	OLED_Set_Pos(x_start,y);
	for(i = 0; i < remain_len+1; i++)
		OLED_WR_Byte(0XFF,OLED_DATA);
		
	OLED_Set_Pos(x,y+1);
	for(i = 0; i < 4; i++)
		OLED_WR_Byte(0X3F,OLED_DATA);
	OLED_WR_Byte(0XFF,OLED_DATA);
	for(i = 0; i < all_len; i++)
		OLED_WR_Byte(0XC0,OLED_DATA);
	OLED_WR_Byte(0XFF,OLED_DATA);
	x_start = x + 5 + all_len - remain_len;
	OLED_Set_Pos(x_start,y+1);
	for(i = 0; i < remain_len+1; i++)
		OLED_WR_Byte(0XFF,OLED_DATA);		
}
