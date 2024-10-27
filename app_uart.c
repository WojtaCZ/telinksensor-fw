#include "tl_common.h"
#include "drivers.h"

volatile unsigned char uart_rx_flag=0;
volatile unsigned char uart_dmairq_tx_cnt=0;
volatile unsigned char uart_dmairq_rx_cnt=0;

#define UART_DATA_LEN    124      //data max ?    (UART_DATA_LEN+4) must 16 byte aligned
typedef struct{
	unsigned int dma_len;        // dma len must be 4 byte
	unsigned char data[UART_DATA_LEN];
}uart_data_t;

uart_data_t trans_buff = {0,   {0,} };

void app_uart_init(void)
{
	// Init UART GPIO
	uart_gpio_set(UART_TX_PD7, UART_RX_PA0);

	// Reset all uart registers
	uart_reset();

	// Set 115200 baud rate
	uart_init(12, 15, PARITY_NONE, STOP_BIT_ONE);

	// Enable TX DMA
	uart_dma_enable(0, 1); 
	irq_set_mask(FLD_IRQ_DMA_EN);
	dma_chn_irq_enable(FLD_DMA_CHN_UART_TX, 1);

	uart_irq_enable(0, 0);  

	irq_enable();

}

void app_uart_irq_proc(void)
{

	unsigned char uart_dma_irqsrc;
	//1. UART irq
	uart_dma_irqsrc = dma_chn_irq_status_get();///in function,interrupt flag have already been cleared,so need not to clear DMA interrupt flag here


	if(uart_dma_irqsrc & FLD_DMA_CHN_UART_TX){
		dma_chn_irq_status_clr(FLD_DMA_CHN_UART_TX);

	}

}


void puts(char *s) { 
		while(uart_tx_is_busy());
	trans_buff.dma_len = 0;

	while(*s)
	{
		trans_buff.data[trans_buff.dma_len] = *s++;
		trans_buff.dma_len += 1;

		if(trans_buff.dma_len == UART_DATA_LEN)
		{
			while(uart_tx_is_busy());
			uart_dma_send((unsigned char*)&trans_buff);
			while(uart_tx_is_busy());
			trans_buff.dma_len = 0;
		}
	}

	if(trans_buff.dma_len)
	{
		while(uart_tx_is_busy());
		uart_dma_send((unsigned char*)&trans_buff);
	}
}