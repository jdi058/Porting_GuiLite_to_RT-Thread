#include "config.h"

static int uart1_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
#if 0
	NVIC_InitTypeDef NVIC_InitStructure;
#endif
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
#if 0
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure);
#if 0
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif
	USART_Cmd(USART1, ENABLE);

	return 0;
}

INIT_BOARD_EXPORT(uart1_init);

#if 0
void USART1_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		res =USART_ReceiveData(USART1);
		USART_SendData(USART1, res);
	}
}
#endif

void rt_hw_console_output(const char *str)
{
  rt_enter_critical();
	
	while(*str!='\0')
	{
    if (*str == '\n')
    {
       USART_SendData(USART1, '\r');
	     while(USART_GetFlagStatus(USART1, USART_FLAG_TC)== RESET);
    }

		USART_SendData(USART1, *(str++));
	  while(USART_GetFlagStatus(USART1, USART_FLAG_TC)== RESET);
	}

  rt_exit_critical(); 
}

char rt_hw_console_getchar(void)
{
	int ch = -1;
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
	{
		ch = (int)USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
	else
	{
		if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
		{
			 USART_ClearFlag(USART1, USART_FLAG_ORE);
		}
		rt_thread_mdelay(10);
	}
	return ch;
}

