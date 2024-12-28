#include "rd_log.h"
#include "../../proj/drivers/drv_putchar.h"


#define RD_ENABLE_UART		1

typedef struct{
	u32 dataLen;
	u8 dataPayload[1];
}uart_rxData_t;



typedef char *va_list;

#define _INTSIZEOF(n)   	( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define va_start(ap,v)  	( ap = (va_list)&v + _INTSIZEOF(v) )
#define va_arg(ap,t)    	( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap)      	( ap = (va_list)0 )


void rd_put_char(unsigned char byte)
{
	drv_uart_tx_start((u8 *)&byte,1);
}

void int_to_str(int num) {
    char str[10];
    int i = 0;
    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    str[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
    for(int t = 0; t<i; t++)
    {
        rd_put_char(str[t]);
    }
}

int rd_log_uart(const char *format, ...){

#if(RD_ENABLE_UART)
	char span;
	long m;
	va_list arg_ptr;
	va_start(arg_ptr, format);
	while((span = *(format++)))
    {
		if(span != '%')
        {
			rd_put_char(span);
		}
        else
        {
			span = *(format++);
			if(span == 'd')
            {
				m = va_arg(arg_ptr,int);//get value of char
				if(m<0)
                {
					rd_put_char('-');
					m = -m;
				}
				int_to_str(m);
			}
            else if(span == 0)
            {
				break;
			}
            else
            {
				rd_put_char(span);
			}
		}
	}
	va_end(arg_ptr);
#endif
	return 0;
}


int rd_log_data_with_len(u8 *data, u8 len)
{
	rd_log_uart("data: ");
	for(u8 i=0; i<len; i++)
	{
		rd_log_uart("%d ",data[i]);
	}
	rd_log_uart("\n");
	return 0;
}

__attribute__((aligned(4))) u8 moduleTest_uartTxBuf[4] = {0};
__attribute__((aligned(4))) u8 moduleTest_uartRxBuf[32] = {0};
volatile u8  T_uartPktSentSeqNo = 0;
volatile u32 T_uartPktRecvSeqNo = 0;
volatile u32 T_uartPktRecvLen = 0;
volatile u32 T_uartPktSentExcept = 0;

void module_test_uartRcvHandler(void){
	/*
	 * the format of the uart rx data: length(4 Bytes) + payload
	 *
	 * */
	uart_rxData_t *rxData = (uart_rxData_t *)moduleTest_uartRxBuf;
	T_uartPktRecvSeqNo = rxData->dataPayload[0];

	if(T_uartPktRecvSeqNo == 0){
		T_uartPktRecvLen = rxData->dataLen;
	}

	if(T_uartPktRecvSeqNo == 0xBB){
		drv_adc_enable(1);
	}else{
		drv_adc_enable(0);
	}
}

void rd_init_uart()
{
	UART_PIN_INIT();

	drv_uart_init(115200, moduleTest_uartRxBuf, sizeof(moduleTest_uartRxBuf)/sizeof(u8), module_test_uartRcvHandler);

}

void rd_test_task()
{
	static u32 last_tick = 0;
	if(clock_time() - last_tick > 4 * 1000 * 1000)
	{
		drv_uart_tx_start((u8 *)"hello\n",6);
		last_tick = clock_time();
	}
}


