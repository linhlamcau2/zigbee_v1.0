#ifndef RD_IN_OUT_H
#define RD_IN_OUT_H

#include "zb_common.h"
#include "../rd_log/rd_log.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "../sampleContactSensor/sampleSensor.h"
#include "app_ui.h"
#include "tl_common.h"


#define RD_SYS_32K_TICK_S				(pm_get_32k_tick()/32767)
#define BUTTON_CONFIG		GPIO_PB4

#define LED_B				GPIO_PC2
#define LED_R				GPIO_PB7
#define NUM_OUTPUT_MAX		2
#define num_pin				1

#define CYCLE_MODE_PULSE		10
#define CYCLE_MODE_KEEP			1000
#define CYCLE_MODE_LEVEL		40


typedef struct{
	ev_timer_callback_t  evt_handler_func;
	ev_timer_event_t *timerLedEvt;
	u8 	oriSta;		//original state before blink
	u8 	sta;		//current state in blink
	u8 	times;		//blink times
}rd_ctx_t;

typedef struct
{
	u8 motion;
	u32 lux;
}cb10_t;

void rd_scan_button();
void rd_light_blink(u8 times, u8 time_delay_100ms, u8 idx);

void rd_set_motion_stt(u8 stt);
void rd_set_lux_level(u32 level);


u8 pyd1598Read_motion(void);
void pyd1598Init();

void rd_pir_init();
void RD_PIR_UpdateSensorAC(void);
void RD_PIR_CheckSendLux(void);

#endif
