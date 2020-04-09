#include "st7735s.h"
#include "pic.h"

extern void spi_sim_write(rt_uint8_t data);

typedef enum pin_name {
	REST,
	DC,
	BLK
}PIN_NAME;

struct st7735_gpio {
	PIN_NAME pin_name;
	rt_uint32_t clock;
	GPIO_TypeDef* GPIOx;
	rt_uint32_t pin;
};

struct st7735_gpio st7735_gpio_def[] = {
	{REST, RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_2},
	{DC, 	 RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_3},
	{BLK,  RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_5}
};

#define ST7735_REST(level) \
			(level == HIGH) ? GPIO_SetBits(st7735_gpio_def[REST].GPIOx, st7735_gpio_def[REST].pin) \
			:GPIO_ResetBits(st7735_gpio_def[REST].GPIOx, st7735_gpio_def[REST].pin)
#define ST7735_DC(level) \
			(level == HIGH) ? GPIO_SetBits(st7735_gpio_def[DC].GPIOx, st7735_gpio_def[DC].pin) \
			:GPIO_ResetBits(st7735_gpio_def[DC].GPIOx, st7735_gpio_def[DC].pin)
#define ST7735_BLK(level) \
			(level == HIGH) ? GPIO_SetBits(st7735_gpio_def[BLK].GPIOx, st7735_gpio_def[BLK].pin) \
			:GPIO_ResetBits(st7735_gpio_def[BLK].GPIOx, st7735_gpio_def[BLK].pin)

static int st7735_gpio_init(void)
{
	rt_uint32_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	for(i = 0; i < sizeof(st7735_gpio_def)/sizeof(st7735_gpio_def[0]); i++)
	{
		RCC_APB2PeriphClockCmd(st7735_gpio_def[i].clock, ENABLE);
		GPIO_InitStructure.GPIO_Pin = st7735_gpio_def[i].pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(st7735_gpio_def[i].GPIOx, &GPIO_InitStructure);
		GPIO_SetBits(st7735_gpio_def[i].GPIOx, st7735_gpio_def[i].pin);
	}
	return RT_EOK;
}

void st7735_write_data(rt_uint8_t data)
{
	spi_sim_write(data);
}

void st7735_write_command(rt_uint8_t command)
{
	ST7735_DC(LOW);
	spi_sim_write(command);
	ST7735_DC(HIGH);
}

void st7735_set_address(rt_uint16_t x_start, rt_uint16_t y_start, 
											rt_uint16_t x_end, rt_uint16_t y_end)
{
	if((DISPLAY_DIRECTION == TOP_TO_BOTTOM) || (DISPLAY_DIRECTION == BOTTOM_TO_TOP))
	{
		st7735_write_command(ST7735_CMD_CASET);
		st7735_write_data(GET_HIGH_BYTE(x_start + 2));
		st7735_write_data(GET_LOW_BYTE(x_start + 2));
		st7735_write_data(GET_HIGH_BYTE(x_end + 2));
		st7735_write_data(GET_LOW_BYTE(x_end + 2));
		st7735_write_command(ST7735_CMD_RASET);
		st7735_write_data(GET_HIGH_BYTE(y_start + 1));
		st7735_write_data(GET_LOW_BYTE(y_start + 1));
		st7735_write_data(GET_HIGH_BYTE(y_end + 1));
		st7735_write_data(GET_LOW_BYTE(y_end + 1));
		st7735_write_command(ST7735_CMD_RAMWR);
	}
	else if((DISPLAY_DIRECTION == LEFT_TO_RIGHT) || (DISPLAY_DIRECTION == RIGHT_TO_LEFT))
	{
		st7735_write_command(ST7735_CMD_CASET);
		st7735_write_data(GET_HIGH_BYTE(x_start + 1));
		st7735_write_data(GET_LOW_BYTE(x_start + 1));
		st7735_write_data(GET_HIGH_BYTE(x_end + 1));
		st7735_write_data(GET_LOW_BYTE(x_end + 1));
		st7735_write_command(ST7735_CMD_RASET);
		st7735_write_data(GET_HIGH_BYTE(y_start + 2));
		st7735_write_data(GET_LOW_BYTE(y_start + 2));
		st7735_write_data(GET_HIGH_BYTE(y_end + 2));
		st7735_write_data(GET_LOW_BYTE(y_end + 2));
		st7735_write_command(ST7735_CMD_RAMWR);
	}
}

void st7735_draw_point(rt_uint16_t x, rt_uint16_t y, rt_uint16_t color)
{
	st7735_set_address(x, y, x, y);
	st7735_write_data(GET_HIGH_BYTE(color));
	st7735_write_data(GET_LOW_BYTE(color));
}


void st7735_fill(rt_uint16_t x_start, rt_uint16_t y_start,
							rt_uint16_t x_end, rt_uint16_t y_end,
							rt_uint16_t color)
{          
	rt_uint16_t i,j; 
	st7735_set_address(x_start, y_start, x_end - 1, y_end - 1);//ÉèÖÃÏÔÊ¾·¶Î§
	for(i = y_start; i < y_end; i++)
	{													   	 	
		for(j = x_start; j < x_end; j++)
		{
			st7735_write_data(GET_HIGH_BYTE(color));
			st7735_write_data(GET_LOW_BYTE(color));
		}
	} 					  	    
}

void st7735_show_picture(rt_uint16_t x, rt_uint16_t y, 
												rt_uint16_t length,rt_uint16_t width, 
												const u8 pic[])
{
	rt_uint16_t i,j,k=0;
	st7735_set_address(x, y, x + length - 1, y + width - 1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			st7735_write_data(pic[k*2]);
			st7735_write_data(pic[k*2+1]);
			k++;
		}
	}			
}

static int st7735_init(void)
{
	st7735_gpio_init();
	
	ST7735_REST(LOW);
	rt_thread_mdelay(100);
	ST7735_REST(HIGH);
	rt_thread_mdelay(100);
	
	ST7735_BLK(HIGH);
	rt_thread_mdelay(120);
	
	st7735_write_command(ST7735_CMD_SLPOUT);
	rt_thread_mdelay(120);
	
	st7735_write_command(ST7735_CMD_FRMCTR1); 
	st7735_write_data(0x05); 
	st7735_write_data(0x3C); 
	st7735_write_data(0x3C); 
	st7735_write_command(ST7735_CMD_FRMCTR2); 
	st7735_write_data(0x05);
	st7735_write_data(0x3C); 
	st7735_write_data(0x3C); 
	st7735_write_command(ST7735_CMD_FRMCTR3); 
	st7735_write_data(0x05); 
	st7735_write_data(0x3C); 
	st7735_write_data(0x3C); 
	st7735_write_data(0x05); 
	st7735_write_data(0x3C); 
	st7735_write_data(0x3C); 

	st7735_write_command(ST7735_CMD_INVCTR);
	st7735_write_data(0x03); 

	st7735_write_command(ST7735_CMD_PWCTR1); 
	st7735_write_data(0x28); 
	st7735_write_data(0x08); 
	st7735_write_data(0x04); 
	st7735_write_command(ST7735_CMD_PWCTR2); 
	st7735_write_data(0XC0); 
	st7735_write_command(ST7735_CMD_PWCTR3); 
	st7735_write_data(0x0D); 
	st7735_write_data(0x00); 
	st7735_write_command(ST7735_CMD_PWCTR4); 
	st7735_write_data(0x8D); 
	st7735_write_data(0x2A); 
	st7735_write_command(ST7735_CMD_PWCTR5); 
	st7735_write_data(0x8D); 
	st7735_write_data(0xEE); 
 
	st7735_write_command(ST7735_CMD_PWCTR5);
	st7735_write_data(0x1A);
	st7735_write_command(ST7735_CMD_MADCTL);
	if(DISPLAY_DIRECTION == TOP_TO_BOTTOM)
		st7735_write_data(0x00);
	else if(DISPLAY_DIRECTION == BOTTOM_TO_TOP)
		st7735_write_data(0xC0);
	else if(DISPLAY_DIRECTION == LEFT_TO_RIGHT)
		st7735_write_data(0x70);
	else if(DISPLAY_DIRECTION == RIGHT_TO_LEFT)
		st7735_write_data(0xA0); 
	
	st7735_write_command(ST7735_CMD_GMCTRP1); 
	st7735_write_data(0x04); 
	st7735_write_data(0x22); 
	st7735_write_data(0x07); 
	st7735_write_data(0x0A); 
	st7735_write_data(0x2E); 
	st7735_write_data(0x30); 
	st7735_write_data(0x25); 
	st7735_write_data(0x2A); 
	st7735_write_data(0x28); 
	st7735_write_data(0x26); 
	st7735_write_data(0x2E); 
	st7735_write_data(0x3A); 
	st7735_write_data(0x00); 
	st7735_write_data(0x01); 
	st7735_write_data(0x03); 
	st7735_write_data(0x13); 
	st7735_write_command(ST7735_CMD_GMCTRN1); 
	st7735_write_data(0x04); 
	st7735_write_data(0x16); 
	st7735_write_data(0x06); 
	st7735_write_data(0x0D); 
	st7735_write_data(0x2D); 
	st7735_write_data(0x26); 
	st7735_write_data(0x23); 
	st7735_write_data(0x27); 
	st7735_write_data(0x27); 
	st7735_write_data(0x25); 
	st7735_write_data(0x2D); 
	st7735_write_data(0x3B); 
	st7735_write_data(0x00); 
	st7735_write_data(0x01); 
	st7735_write_data(0x04); 
	st7735_write_data(0x13); 

	st7735_write_command(ST7735_CMD_COLMOD); 
	st7735_write_data(0x05); 
	st7735_write_command(ST7735_CMD_DISPON);
	
	//st7735_fill(0, 0, LCD_W, LCD_H, RED);
	//st7735_show_picture(rt_uint16_t x, rt_uint16_t y, rt_uint16_t length,rt_uint16_t width, const u8 pic[])
	//st7735_show_picture(0, 0, 128, 160, gImage_123123);
	
	return RT_EOK;
}
INIT_DEVICE_EXPORT(st7735_init);
//MSH_CMD_EXPORT(lcd_init, "lcd init");
