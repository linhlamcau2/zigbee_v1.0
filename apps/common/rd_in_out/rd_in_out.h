#ifndef RD_IN_OUT_H
#define RD_IN_OUT_H

#include "zb_common.h"
#include "../rd_log/rd_log.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "../sampleLight/sampleLight.h"
#include "app_ui.h"
#include "tl_common.h"

#define CTCU_1	1
#define CTCU_2	2
#define CTCU_3	3
#define CTCU_4	4

#define TYPE_CTCU	CTCU_4

#define CYCLE_MODE_PULSE		10
#define CYCLE_MODE_KEEP			1000
#define CYCLE_MODE_LEVEL		40

#define	INPUT1				GPIO_PC2
#define	INPUT2				GPIO_PD3
#define	INPUT3				GPIO_PC1
#define INPUT4 				GPIO_PD4



#define OUTPUT_1					GPIO_PC0
#define OUTPUT_2					GPIO_PB7
#define OUTPUT_3					GPIO_PB6
#define OUTPUT_4					GPIO_PB5

#if(TYPE_CTCU == CTCU_1)
	#define RD_SCAN_PINS	{INPUT1}
	#define NUM_OUTPUT_MAX 1
#endif

#if(TYPE_CTCU == CTCU_2)
	#define RD_SCAN_PINS	{INPUT1,INPUT2}
	#define NUM_OUTPUT_MAX 2
#endif

#if(TYPE_CTCU == CTCU_3)
	#define RD_SCAN_PINS	{INPUT1,INPUT2,INPUT3}
	#define NUM_OUTPUT_MAX 3
#endif

#if(TYPE_CTCU == CTCU_4)
	#define RD_SCAN_PINS	{INPUT1,INPUT2,INPUT3,INPUT4}
	#define NUM_OUTPUT_MAX 4
#endif


#define RD_OUTPUT_RELAY	{OUTPUT_1, OUTPUT_2,OUTPUT_3,OUTPUT_4}
#define num_pin  sizeof(pin_scan)/sizeof(pin_scan[0])


#define FREQ_LED 25000
#if(TYPE_CTCU == CTCU_1)
	#define NUM_LED_DATA (2*2*NUM_OUTPUT_MAX)
#else
	#define NUM_LED_DATA (2*NUM_OUTPUT_MAX)
#endif

#define NUM_BYTE_FRAME_DATA  (4 * NUM_LED_DATA)


#define RD_ENDPOINT_MILITONE  0x01
#define RD_ENDPOINT_1	0x01
#define RD_ENDPOINT_2	0x02
#define RD_ENDPOINT_3	0x03
#define RD_ENDPOINT_4	0x04


#define RD_SW_CB_CLUSTER_NUM  	1
#define ZCL_RD_SW_ATTR_NUM		2


#define LED_DI GPIO_PC4
#define LED_CI GPIO_PC3
#define RESET_TOUCH_PIN				GPIO_PA1

typedef struct
{
	u8 stt;
	u8 mode_start;
}output_t;

enum
{
	MODE_START_OFF = 0,
	MODE_START_ON,
	MODE_START_RESTORE,
};
typedef void (*evt_handler_func_t)(void * arg);

typedef struct{
	ev_timer_callback_t  evt_handler_func;
	ev_timer_event_t *timerLedEvt;
	u8 	oriSta;		//original state before blink
	u8 	sta;		//current state in blink
	u8 	times;		//blink times
}rd_ctx_t;


extern u32 pin_scan[];
extern u32 relay_out[];

void rd_write_led_out(u8 idx, u8 stt);
void rd_handle_input();
void rd_sw_init();
void rd_sw_report_conf();

void rd_save_stt();
nv_sts_t rd_output_restore(void);
void rd_process_save_stt_out();
void rd_light_blink(u8 times, u8 time_delay_100ms, u8 idx);
void rd_light_factory_rst();

void rd_save_default();

void Config_Pin_Led_Lc8823(void);
void init_led_data();
void ctrl_led(u8 id, u8 level);
void ctrl_all(u8 level);

#endif
