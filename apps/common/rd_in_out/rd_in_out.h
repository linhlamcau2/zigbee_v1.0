#ifndef RD_IN_OUT_H
#define RD_IN_OUT_H

#include "zb_common.h"
#include "../rd_log/rd_log.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "../sampleLight/sampleLight.h"
#include "app_ui.h"
#include "tl_common.h"

#define CYCLE_MODE_PULSE		10
#define CYCLE_MODE_KEEP			1000
#define CYCLE_MODE_LEVEL		40


#define INPUT1	GPIO_PD3
#define INPUT2 	GPIO_PD4
#define INPUT3	GPIO_PA0
#define INPUT4 	GPIO_PA1
#define INPUT5	GPIO_PC2


#define OUTPUT_1					GPIO_PD2
#define OUTPUT_2					GPIO_PB1
#define OUTPUT_3					GPIO_PB5
#define OUTPUT_4					GPIO_PC1
#define OUTPUT_5					GPIO_PC3


#define RD_SCAN_PINS	{INPUT1, INPUT2,INPUT3,INPUT4,INPUT5}

#define RD_OUTPUT_LED	{OUTPUT_1, OUTPUT_2,OUTPUT_3,OUTPUT_4,OUTPUT_5}
#define num_pin  sizeof(pin_scan)/sizeof(pin_scan[0])


#define NUM_OUTPUT_MAX 5


#define RD_ENDPOINT_MILITONE  0x03
#define RD_ENDPOINT_1	0x03
#define RD_ENDPOINT_2	0x04
#define RD_ENDPOINT_3	0x05
#define RD_ENDPOINT_4	0x06
#define RD_ENDPOINT_5	0x07

#define RD_SW_CB_CLUSTER_NUM  	1
#define ZCL_RD_SW_ATTR_NUM		2


typedef struct
{
	u8 stt;
	u8 mode_start;
}output_t;

typedef void (*evt_handler_func_t)(void * arg);

typedef struct{
	ev_timer_callback_t  evt_handler_func;
	ev_timer_event_t *timerLedEvt;
	u8 	oriSta;		//original state before blink
	u8 	sta;		//current state in blink
	u8 	times;		//blink times
}rd_ctx_t;


extern u32 pin_scan[];
extern u32 led_out[];

void rd_write_led_out(u8 idx, u8 stt);
void rd_handle_input();
void rd_sw_init();
void rd_sw_report_conf();

void rd_save_stt();
nv_sts_t rd_output_restore(void);
void rd_process_save_stt_out();
void rd_light_blink(u8 times, u8 time_delay_100ms, u8 idx);
void rd_light_factory_rst();
#endif
