#ifndef ESP32_INTERFACE_H_
#define ESP32_INTERFACE_H_

#include "zb_common.h"
#include "../../proj/drivers/drv_putchar.h"

#define CFG_UART_TX_PIN		GPIO_PD7
#define CFG_UART_RX_PIN		GPIO_PA0

#define CFG_UART_PIN_INIT()		do{	\
									drv_uart_pin_set(CFG_UART_TX_PIN, CFG_UART_RX_PIN);	\
								}while(0)


enum
{
    /* -------- Mesh / Router Info -------- */
    OP_GET_MESHROUTER        = 0x0002,  /* Ask MeshRouter information */
    OP_GET_COOR_PAR          = 0x0003,  /* Ask coordination parameters */

    /* -------- Mesh Configuration -------- */
    OP_SET_TX_POWER          = 0x0007,  /* Set TX power for meshrouter */
    OP_SET_NET_KEY           = 0x0009,  /* Set NET_KEY */

    OP_KICKOUT               = 0x000A,  /* Remove lamp from network */

    /* -------- Group Management -------- */
    OP_ADD_TO_GR             = 0x000C,  /* Add lamp to group */
    OP_DEL_FR_GR             = 0x000D,  /* Remove lamp from group */
    OP_DEL_GR                = 0x000E,  /* Delete group */

    /* -------- Device Control -------- */
    OP_REQ_RESTART           = 0x000F,  /* Request device reboot */

    OP_SET_RATE_POW          = 0x0010,  /* Set rated power for lamp */
    OP_SET_THRES_WARN        = 0x0011,  /* Set warning threshold */
    OP_SET_SCENE             = 0x0012,  /* Set scene */
    OP_DEL_SCENE             = 0x0013,  /* Delete scene */
    OP_RECALL_SCENE          = 0x0014,  /* Recall / activate scene */

    /* -------- DIM Control -------- */
    OP_CTR_DIM_ALL           = 0x0015,  /* Control DIM for all lamps */
    OP_CTR_DIM               = 0x0016,  /* Control DIM for single lamp */
    OP_CTR_DIM_GROUP         = 0x0017,  /* Control DIM for group */

    OP_SET_DIM_START_UP      = 0x0018,  /* Set DIM at power-up */
    OP_SET_TIME_RSP          = 0x0019,  /* Set response time */

    /* -------- Scene Control -------- */
    OP_RECALL_SCENE_GR       = 0x001A,  /* Recall scene by group */
    OP_RECALL_SCENE_ALL      = 0x001B,  /* Recall scene for all */

    OP_SET_THRES_WARN_ALL    = 0x001C,  /* Set warning threshold for all */
    OP_SET_THRES_WARN_GR     = 0x001D,  /* Set warning threshold for group */

    OP_SET_RATE_POW_GR       = 0x001E,  /* Set rated power for group */
    OP_SET_RATE_POW_ALL      = 0x001F,  /* Set rated power for all */

    OP_SET_DIM_GR            = 0x0020,  /* Set rated DIM for group */
    OP_SET_DIM_ALL           = 0x0021,  /* Set rated DIM for all */

    OP_DEL_SCENE_GR          = 0x0022,  /* Delete scene for group */
    OP_DEL_SCENE_ALL         = 0x0023,  /* Delete scene for all */

    /* -------- Query / Notification -------- */
    OP_GET_NETKEY_CSP        = 0x1000,  /* Query NET_KEY and CSP */

    OP_NOTI_STT_NET          = 0x1001,  /* Network status notification */
    OP_NOTI_COM_SUC          = 0x1002,  /* Communication success notification */

};

/* Request MeshRouter information */
int tx_req_meshrouter_info(void);

/* Request coordination parameters */
int tx_req_coor_param(void);

/* -------- Network / Configuration -------- */

/* Set TX power for meshrouter */
int tx_set_tx_power(u16 power);

/* Set NET_KEY */
int tx_set_net_key(const u8 *net_key, u8 len);

/* Remove lamp from mesh network */
int tx_req_kickout(void);

/* Request device reboot */
int tx_req_restart(void);

/* Query NET_KEY and CSP */
int tx_get_netkey_csp(void);

/* -------- Group Management -------- */

/* Add lamp to group */
int tx_add_group(u16 id_gr);

/* Remove lamp from group */
int tx_del_from_group(u16 id_gr);

/* Delete group */
int tx_del_group(u16 id_gr);

/* -------- Power / DIM Configuration -------- */

/* Set rated power for lamp */
int tx_set_rated_power(u16 power);

/* Set DIM value for all lamps */
int tx_ctrl_dim_all(u16 dim);

/* Control DIM for a single lamp */
int tx_ctrl_dim_single(u16 dim);

/* Control DIM for a group */
int tx_ctrl_dim_group(u16 id_gr, u16 dim);

/* Set DIM value at power-up */
int tx_set_dim_startup(u16 dim);

/* Set response time */
int tx_set_time_rsp(u16 time_ms);

/* -------- Scene Control -------- */

/* Set scene */
int tx_set_scene(u16 scene_id);

/* Delete scene */
int tx_del_scene(u16 scene_id);

/* Recall / activate scene */
int tx_recall_scene(u16 scene_id);

/* Recall scene by group */
int tx_recall_scene_group(u16 scene_id);

/* Recall scene for all lamps */
int tx_recall_scene_all(u16 scene_id);

/* Delete scene for group */
int tx_del_scene_group(u16 scene_id);

/* Delete scene for all lamps */
int tx_del_scene_all(u16 scene_id);



void esp32_interface_init();
int forward_data_via_uart(u8 *data, u8 len);

#endif /* ESP32_INTERFACE_H_ */
