#include "log_sys.h"
#include "proj/common/printf.h"

#if(LOG_ENABLE)
#if(LOG_MODE_CONFIG == USE_SOFT_UART)
	sUartTypeDef sUart1;

	static void sUartIoInit(GPIO_PinTypeDef pin, unsigned char mode){
		if(mode == sUartTxMode){
			gpio_set_func(pin, AS_GPIO);
			gpio_set_output_en(pin,1);
			gpio_set_input_en(pin,0);
		}
	}

	static void sUartInit(sUartTypeDef *uart){
		uart->txPin = SUART_TX_PIN_LOG;
		uart->rxPin = SUART_RX_PIN_LOG;
		sUartIoInit(uart->txPin,sUartTxMode);
		gpio_write(uart->txPin,1);
	}

	static void softUartSendByte(sUartTypeDef *uart,unsigned char data){     //default baud: 9600bps
		GPIO_PinTypeDef pin = uart->txPin;
		unsigned char r = irq_disable();
		gpio_write(pin,0);
		sleep_us(UART_TIME_BIT_START);
		for(unsigned char j=0;j<8;j++){
			if(data & (1<<j)){
				gpio_write(pin,1);
			}
			else {
				gpio_write(pin,0);
			}
			sleep_us(UART_BIT_TIME);
		}
		gpio_write(pin,1);
		sleep_us(UART_BIT_TIME);
		irq_restore(r);
	}

	static void softUartSend(sUartTypeDef *uart,unsigned char *data){
		while(*data != '\0'){
			softUartSendByte(uart,*(data++));
		}
	}

#elif(LOG_MODE_CONFIG == USE_HARD_UART)
	static unsigned char uart_CSend(unsigned char* data){
	#if (HCI_ACCESS == HCI_USE_UART)
		while(*data != '\0')
		{
			uart_ndma_send_byte(*(data++));
		}
	#endif
		return 0;
	}
#endif

void log_output(const char *level, const char *fmt, ...) {
    char buff[160];
    char *out = buff;
    u32 time_ms = clock_time_ms();
	out += sprintf(out, "[%s][%d ms] ", level, time_ms);
    va_list args;
    va_start(args, fmt);
    print(&out, fmt, args);
    va_end(args);
    *out = '\n';
	*(out + 1) = '\0';
#if(LOG_MODE_CONFIG == USE_SOFT_UART)
    softUartSend(&sUart1, (unsigned char *)buff);
#elif(LOG_MODE_CONFIG == USE_HARD_UART)
	uart_CSend((unsigned char *)buff);
#endif
}

void format_hex_string(char *out, const unsigned char *data, unsigned int len)
{
    for (unsigned int i = 0; i < len; i++) {
        unsigned char byte = data[i];
        char high = (byte >> 4) & 0x0F;
        char low  = byte & 0x0F;
        *out++ = (high < 10) ? ('0' + high) : ('A' + high - 10);
        *out++ = (low  < 10) ? ('0' + low)  : ('A' + low  - 10);
        *out++ = ' ';
    }
    *out = '\0';
}

extern u8 tbl_mac[6];
void log_init(){
#if(LOG_MODE_CONFIG == USE_SOFT_UART)
	sUartInit(&sUart1);
#elif(LOG_MODE_CONFIG == USE_HARD_UART)
	uart_gpio_set(HUART_TX_PIN_LOG,HUART_RX_PIN_LOG);	
	uart_init_baudrate(BAUD_RATE, CLOCK_SYS_CLOCK_HZ, PARITY_NONE, STOP_BIT_ONE);	
	uart_dma_enable(0,0);
#endif
	LOGI("Start Program...............");
	LOGI("MAC: %x %x %x %x %x %x",tbl_mac[0],tbl_mac[1],tbl_mac[2],tbl_mac[3],tbl_mac[4],tbl_mac[5]);
}

#endif
