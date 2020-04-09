#ifndef __SYS_H__
#define __SYS_H__

#ifdef MAIN_CONFIG
	#define EXT
#else 
	#define EXT extern
#endif

#include <rthw.h>
#include <rtthread.h>
	
#include "stm32f10x.h"
	
typedef enum LEVEL{LOW = 0, HIGH} GPIO_LEVEL;

#define GET_HIGH_BYTE(x)	((x) >> 8)
#define GET_LOW_BYTE(x)		(x) & 0xff


//driver header
#include "st7735s.h"

void st7735_draw_point(rt_uint16_t x, rt_uint16_t y, rt_uint16_t color);

#endif


