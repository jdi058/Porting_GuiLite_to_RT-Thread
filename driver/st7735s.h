#ifndef __ST7735S_H_
#define __ST7735S_H_
#include "config.h"

#define TOP_TO_BOTTOM 0x00
#define BOTTOM_TO_TOP 0x01
#define LEFT_TO_RIGHT 0x02
#define RIGHT_TO_LEFT 0x03

#define DISPLAY_DIRECTION		BOTTOM_TO_TOP

#if DISPLAY_DIRECTION == TOP_TO_BOTTOM || DISPLAY_DIRECTION == BOTTOM_TO_TOP
#define LCD_W 128
#define LCD_H 160
#else
#define LCD_W 160
#define LCD_H 128
#endif

#define ST7735_CMD_NOP						(0x00)
#define ST7735_CMD_SWRESET				(0x01)
#define ST7735_CMD_RDDID					(0x04)
#define ST7735_CMD_RDDST					(0x09)
#define ST7735_CMD_RDDPM					(0x0A)
#define ST7735_CMD_RDDMADCTL			(0x0B)
#define ST7735_CMD_RDDCOLMOD			(0x0C)
#define ST7735_CMD_RDDIM					(0x0D)
#define ST7735_CMD_RDDSM					(0x0E)
#define ST7735_CMD_RDDSDR					(0x0F)
#define ST7735_CMD_SLPIN					(0x10)	
#define ST7735_CMD_SLPOUT					(0x11)
#define ST7735_CMD_PTLON					(0x12)
#define ST7735_CMD_NORON					(0x13)
#define ST7735_CMD_INVOFF					(0x20)
#define ST7735_CMD_INVON					(0x21)
#define ST7735_CMD_GAMSET					(0x26)
#define ST7735_CMD_DISPOFF				(0x28)
#define ST7735_CMD_DISPON					(0x29)
#define ST7735_CMD_CASET					(0x2A)
#define ST7735_CMD_RASET					(0x2B)
#define ST7735_CMD_RAMWR					(0x2C)
#define ST7735_CMD_RGBSET					(0x2D)
#define ST7735_CMD_RAMRD					(0x2E)
#define ST7735_CMD_PTLAR					(0x30)
#define ST7735_CMD_SCRLAR					(0x33)
#define ST7735_CMD_TEOFF					(0x34)
#define ST7735_CMD_TEON						(0x35)
#define ST7735_CMD_MADCTL					(0x36)
#define ST7735_CMD_VSCSAD					(0x37)
#define ST7735_CMD_IDMOFF					(0x38)
#define ST7735_CMD_IDMON					(0x39)
#define ST7735_CMD_COLMOD					(0x3A)
#define ST7735_CMD_RDID1					(0xDA)
#define ST7735_CMD_RDID2					(0xDB)
#define ST7735_CMD_RDID3					(0xDC)
#define ST7735_CMD_FRMCTR1				(0xB1)
#define ST7735_CMD_FRMCTR2				(0xB2)
#define ST7735_CMD_FRMCTR3				(0xB3)
#define ST7735_CMD_INVCTR					(0xB4)
#define ST7735_CMD_PWCTR1					(0xC0)
#define ST7735_CMD_PWCTR2					(0xC1)
#define ST7735_CMD_PWCTR3					(0xC2)
#define ST7735_CMD_PWCTR4					(0xC3)
#define ST7735_CMD_PWCTR5					(0xC4)
#define ST7735_CMD_VMCTR1					(0xC5)
#define ST7735_CMD_VMOFCTR				(0xC7)
#define ST7735_CMD_WRID2					(0xD1)
#define ST7735_CMD_WRID3					(0xD2)
#define ST7735_CMD_NVFCTR1				(0xD9)
#define ST7735_CMD_NVFCTR2				(0xDE)
#define ST7735_CMD_NVFCTR3				(0xDF)
#define ST7735_CMD_GMCTRP1				(0xE0)
#define ST7735_CMD_GMCTRN1				(0xE1)
#define ST7735_CMD_GCV						(0xFC)

#define WHITE         	 					0xFFFF
#define BLACK         	 					0x0000	  
#define BLUE           	 					0x001F  
#define BRED             					0XF81F
#define GRED 			       					0XFFE0
#define GBLUE			       					0X07FF
#define RED           	 					0xF800
#define MAGENTA       	 					0xF81F
#define GREEN         	 					0x07E0
#define CYAN          	 					0x7FFF
#define YELLOW        	 					0xFFE0
#define BROWN 			     					0XBC40 
#define BRRED 			     					0XFC07 
#define GRAY  			     					0X8430 
#define DARK_BLUE      	 					0X01CF	
#define LIGHT_BLUE      					0X7D7C 
#define GRAY_BLUE       	 				0X5458
#define LIGHT_GREEN     	 				0X841F
#define LIGHT_GRAY 			     			0XC618
#define LIGHT_GRAY_BLUE       		0XA651
#define LIGHT_BROWN_BLUE      		0X2B12

#endif
