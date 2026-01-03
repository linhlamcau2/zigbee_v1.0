#include "esp32_interface.h"
#include "zb_common.h"
#include "../../proj/drivers/drv_putchar.h"
#include "../log_sys/log_sys.h"
#include "../user_utils/task.h"
#include "../user_utils/static_queue.h"

// big endian 
#define BE_U16(b0, b1) \
    ((u16)(((uint16_t)(b0) << 8) | (u16)(b1)))
// little endian
#define LE_U16(b0, b1) \
    ((u16)(((uint16_t)(b1) << 8) | (u16)(b0)))
/* Get Most Significant Byte (MSB) from uint16_t */
#define U16_MSB(x)   ((u8)((x) >> 8))
/* Get Least Significant Byte (LSB) from uint16_t */
#define U16_LSB(x)   ((u8)((x) & 0xFF))
/*
 * Split uint16_t into bytes in Big Endian order
 * Byte0 = MSB, Byte1 = LSB
 */
#define U16_BE_BYTE0(x)   U16_MSB(x)
#define U16_BE_BYTE1(x)   U16_LSB(x)
/*
 * Split uint16_t into bytes in Little Endian order
 * Byte0 = LSB, Byte1 = MSB
 */
#define U16_LE_BYTE0(x)   U16_LSB(x)
#define U16_LE_BYTE1(x)   U16_MSB(x)

#define RX_BUFF_LEN 64
#define TX_BUFF_LEN 64

typedef struct{
	u32 dataLen;
	u8 dataPayload[1];
}uart_rxData_t;

typedef struct 
{
	u8 data[64];
	u16 len;
}msg_t;

#define MAX_QUEUE_MSG_RX	20
SQUEUE_DEFINE(queue_msg_rx, msg_t, MAX_QUEUE_MSG_RX)

__attribute__((aligned(4))) u8 tx_buff[TX_BUFF_LEN] = {0};
__attribute__((aligned(4))) u8 rx_buff[RX_BUFF_LEN] = {0};
volatile u16 rx_buff_count = 0;

void uart_send_data(const u8 *dataSend, u16 dataLen)
{
	for(int i=0; i< dataLen; i++)
	{
		uart_ndma_send_byte(*(dataSend+i));
	}
}

u8 cal_check_sum(u8 *par, u8 len)
{
	u16 sum = 0;
	for(u8 i=0; i<len; i++)
	{
		sum += *(par+i);
	}
	return (u8)(sum & 0xff);
}

int parse_data_to_buff(u8 *buff,u8 *data, u8 len, u16 opcode)
{
    *buff = 0xaa;
    *(buff+1) = 0xf5;
    *(buff+2) = len+3;
    *(buff+3) = U16_BE_BYTE0(opcode);
    *(buff+4) = U16_BE_BYTE1(opcode);
    if(data && len)    memcpy(&buff[5],data,len);
    *(buff+5+len) = cal_check_sum(&buff[2],len+3);
    return len+6;
}

int forward_data_via_uart(u8 *data, u8 len)
{
    if(len > 60)
        return -1;
    u8 buff_temp[64];
    buff_temp[0] = 0xaa;
    buff_temp[1] = 0xf5;
    buff_temp[2] = len + 1;
    memcpy(&buff_temp[3], data, len);
    buff_temp[len + 3] = cal_check_sum(&buff_temp[2], len + 1);
    uart_send_data(buff_temp, len + 4);
    return 0;
}

int tx_req_meshrouter_info(void)
{
    int len = parse_data_to_buff(tx_buff, NULL, 0, OP_GET_MESHROUTER);
    uart_send_data(tx_buff, len);
    return 0;
}

int tx_req_coor_param(void)
{
    int len = parse_data_to_buff(tx_buff, NULL, 0, OP_GET_COOR_PAR);
    uart_send_data(tx_buff, len);
    return 0;
}

int tx_req_kickout(void)
{
    int len = parse_data_to_buff(tx_buff, NULL, 0, OP_KICKOUT);
    uart_send_data(tx_buff, len);
    return 0;
}

int tx_req_restart(void)
{
    int len = parse_data_to_buff(tx_buff, NULL, 0, OP_REQ_RESTART);
    uart_send_data(tx_buff, len);
    return 0;
}

int tx_get_netkey_csp(void)
{
    int len = parse_data_to_buff(tx_buff, NULL, 0, OP_GET_NETKEY_CSP);
    uart_send_data(tx_buff, len);
    return 0;
}

int tx_add_group(u16 id_gr)
{
    u8 data[2];
    data[0] = U16_BE_BYTE0(id_gr);
    data[1] = U16_BE_BYTE1(id_gr);

    int len = parse_data_to_buff(tx_buff, data, 2, OP_ADD_TO_GR);
    uart_send_data(tx_buff, len);
    return 0;
}

int tx_del_from_group(u16 id_gr)
{
    u8 data[2];
    data[0] = U16_BE_BYTE0(id_gr);
    data[1] = U16_BE_BYTE1(id_gr);

    int len = parse_data_to_buff(tx_buff, data, 2, OP_DEL_FR_GR);
    uart_send_data(tx_buff, len);
    return 0;
}

int tx_del_group(u16 id_gr)
{
    u8 data[2];
    data[0] = U16_BE_BYTE0(id_gr);
    data[1] = U16_BE_BYTE1(id_gr);

    int len = parse_data_to_buff(tx_buff, data, 2, OP_DEL_GR);
    uart_send_data(tx_buff, len);
    return 0;
}

int tx_set_tx_power(u16 power)
{
    u8 data[2];
    data[0] = U16_BE_BYTE0(power);
    data[1] = U16_BE_BYTE1(power);

    int len = parse_data_to_buff(tx_buff, data, 2, OP_SET_TX_POWER);
    uart_send_data(tx_buff, len);
    return 0;
}

int tx_set_rated_power(u16 power)
{
    u8 data[2];
    data[0] = U16_BE_BYTE0(power);
    data[1] = U16_BE_BYTE1(power);

    int len = parse_data_to_buff(tx_buff, data, 2, OP_SET_RATE_POW);
    uart_send_data(tx_buff, len);
    return 0;
}

///dim
int tx_ctrl_dim_all(u16 dim)
{
    u8 data[2];
    data[0] = U16_BE_BYTE0(dim);
    data[1] = U16_BE_BYTE1(dim);

    int len = parse_data_to_buff(tx_buff, data, 2, OP_CTR_DIM_ALL);
    uart_send_data(tx_buff, len);
    return 0;
}

int tx_ctrl_dim_single(u16 dim)
{
    u8 data[2];
    data[0] = U16_BE_BYTE0(dim);
    data[1] = U16_BE_BYTE1(dim);

    int len = parse_data_to_buff(tx_buff, data, 2, OP_CTR_DIM);
    uart_send_data(tx_buff, len);
    return 0;
}

int tx_ctrl_dim_group(u16 id_gr, u16 dim)
{
    u8 data[4];
    data[0] = U16_BE_BYTE0(id_gr);
    data[1] = U16_BE_BYTE1(id_gr);
    data[2] = U16_BE_BYTE0(dim);
    data[3] = U16_BE_BYTE1(dim);

    int len = parse_data_to_buff(tx_buff, data, 4, OP_CTR_DIM_GROUP);
    uart_send_data(tx_buff, len);
    return 0;
}

//scene control 
int tx_set_scene(u16 scene_id)
{
    u8 data[2];
    data[0] = U16_BE_BYTE0(scene_id);
    data[1] = U16_BE_BYTE1(scene_id);

    int len = parse_data_to_buff(tx_buff, data, 2, OP_SET_SCENE);
    uart_send_data(tx_buff, len);
    return 0;
}

int tx_del_scene(u16 scene_id)
{
    u8 data[2];
    data[0] = U16_BE_BYTE0(scene_id);
    data[1] = U16_BE_BYTE1(scene_id);

    int len = parse_data_to_buff(tx_buff, data, 2, OP_DEL_SCENE);
    uart_send_data(tx_buff, len);
    return 0;
}

int tx_recall_scene(u16 scene_id)
{
    u8 data[2];
    data[0] = U16_BE_BYTE0(scene_id);
    data[1] = U16_BE_BYTE1(scene_id);

    int len = parse_data_to_buff(tx_buff, data, 2, OP_RECALL_SCENE);
    uart_send_data(tx_buff, len);
    return 0;
}


int send_msg_rx_to_queue(u8 *data, u16 len)
{
	msg_t msg;
	msg.len = len;
	memcpy(msg.data,data,len);
    if (SQUEUE_ENQUEUE(&queue_msg_rx, &msg) == 0)
	{
		
	}
	else
	{
		
	}
	return 0;
}

void uart_rx_callback(u8 c)
{
	
	if(rx_buff_count < RX_BUFF_LEN)
			rx_buff[rx_buff_count ++] = c;
	if((rx_buff_count == 1 && rx_buff[0] != 0x55) || (rx_buff_count == 2 && rx_buff[1] != 0xaa ))
	{
		rx_buff_count = 0;
		return;
	}
	if(rx_buff_count > 2)
	{
		u8 len = rx_buff[2];
		if((u8)rx_buff_count == (len+3))
		{
			//Todo: send to queue
            send_msg_rx_to_queue(rx_buff, rx_buff_count);
            rx_buff_count = 0;
		}
	}	
}

void handle_rx_uart(void){
	uart_rxData_t *rxData = (uart_rxData_t *)rx_buff;
	u32 user_uartPktRecvSeqNo = rxData->dataPayload[0];
	if(1){
        int len = rxData->dataLen;
        // LOGD_HEX(rxData->dataPayload, len);
        for(int i=0; i< len; i++)
        {
            uart_rx_callback(rxData->dataPayload[i]);
        }
	}
}

extern void rd_nema_report(u8* p_data, u8 len);

int handle_msg_rx(int *param)
{
    msg_t *msg;
    if (SQUEUE_DEQUEUE(&queue_msg_rx, msg) == 0)
    {
        LOGI("handle_msg_rx");
        u8 length = msg->len;
        u8 *par = msg->data;
        u8 *data = par + 2;
        u8 len = *data;
        u8 crc_temp = cal_check_sum(data, len);
        u8 crc = *(par + length-1);
        if(1)
        // if (crc == crc_temp)
        {
            LOGI("oke: %d",len);
            par[2] = len-1;
            rd_nema_report(par + 2, len);
            //Todo: send via zigbee
        }
    }
    return 1;
}

void esp32_interface_init()
{
	CFG_UART_PIN_INIT();

	drv_uart_init(115200, rx_buff, sizeof(rx_buff)/sizeof(u8), handle_rx_uart);
    SQUEUE_INIT(queue_msg_rx);
    TASK_ADD(handle_msg_rx, NULL, 300, 1000);

}
