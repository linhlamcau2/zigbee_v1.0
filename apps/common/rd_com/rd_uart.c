#include "rd_uart.h"
#include "tl_common.h"
#include "../../zbhci/zbhci.h"
#include "../../proj/drivers/drv_putchar.h"

u8 rd_tx_buff[RD_TX_LEN_MAX];
u8 len_tx;
u8 rd_rx_buff[RD_RX_LEN_MAX] = {0};
u8 rd_buff[RD_RX_LEN_MAX] = {0};


void rd_send_uart(const u8 *dataSend, u16 dataLend)
{
	for(int i=0; i< dataLend; i++)
	{
		uart_ndma_send_byte(*(dataSend+i));
	}
}

u8 rd_cal_check_sum(u8 *par, u8 len)
{
	u16 sum = 0;
	for(u8 i=0; i<len; i++)
	{
		sum += *(par+i);
	}
	return (u8)(sum & 0xff);
}


