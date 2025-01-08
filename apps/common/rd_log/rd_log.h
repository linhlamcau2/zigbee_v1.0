#ifndef RD_LOG_H
#define RD_LOG_H

#include "zb_common.h"

#define RD_UART_TX_PIN		GPIO_PD7
#define RD_UART_RX_PIN		GPIO_PA0

//#define RD_UART_TX_PIN		GPIO_PB6
//#define RD_UART_RX_PIN		GPIO_PB5

#define UART_PIN_INIT()		do{	\
									drv_uart_pin_set(RD_UART_TX_PIN, RD_UART_RX_PIN);	\
								}while(0)


int rd_log_uart(const char *format, ...);
void rd_init_uart();

void rd_test_task();
#endif
