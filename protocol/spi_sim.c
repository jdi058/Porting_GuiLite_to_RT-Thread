#include "config.h"

typedef enum spi_pin_name {
	SCLK,
	MOSI,
	CS
}SPI_PIN_NAME;

struct spi_gpio {
	SPI_PIN_NAME pin_name;
	rt_uint32_t clock;
	GPIO_TypeDef* GPIOx;
	rt_uint32_t pin;
};

struct spi_gpio spi_gpio_def[] = {
	{SCLK, RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_0},
	{MOSI, RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_1},
	{CS,   RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_4}
};

#define SPI_SCLK(level) \
			(level == HIGH) ? GPIO_SetBits(spi_gpio_def[SCLK].GPIOx, spi_gpio_def[SCLK].pin) \
			:GPIO_ResetBits(spi_gpio_def[SCLK].GPIOx, spi_gpio_def[SCLK].pin)
#define SPI_MOSI(level) \
			(level == HIGH) ? GPIO_SetBits(spi_gpio_def[MOSI].GPIOx, spi_gpio_def[MOSI].pin) \
			:GPIO_ResetBits(spi_gpio_def[MOSI].GPIOx, spi_gpio_def[MOSI].pin)
#define SPI_CS(level) \
			(level == HIGH) ? GPIO_SetBits(spi_gpio_def[CS].GPIOx, spi_gpio_def[CS].pin) \
			:GPIO_ResetBits(spi_gpio_def[CS].GPIOx, spi_gpio_def[CS].pin)

static int spi_sim_init(void)
{
	rt_uint32_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	for(i = 0; i < sizeof(spi_gpio_def)/sizeof(spi_gpio_def[0]); i++)
	{
		RCC_APB2PeriphClockCmd(spi_gpio_def[i].clock, ENABLE);
		GPIO_InitStructure.GPIO_Pin = spi_gpio_def[i].pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(spi_gpio_def[i].GPIOx, &GPIO_InitStructure);
		GPIO_SetBits(spi_gpio_def[i].GPIOx, spi_gpio_def[i].pin);
	}
	
	return RT_EOK;
}
INIT_DEVICE_EXPORT(spi_sim_init);
//MSH_CMD_EXPORT(spi_sim_init, "spi sim test");

void spi_sim_write(rt_uint8_t data)
{
	rt_uint8_t i;
	SPI_CS(LOW);
	for(i = 0; i < 8; i++) {
		SPI_SCLK(LOW);
		if(data & 0x80) {
			SPI_MOSI(HIGH);
		} else {
			SPI_MOSI(LOW);
		}
		SPI_SCLK(HIGH);
		data <<= 1;
	}
	SPI_CS(HIGH);
}

