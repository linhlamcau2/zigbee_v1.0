/********************************************************************************************************
 * @file    app_ui.c
 *
 * @brief   This is the source file for app_ui
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

#if (__PROJECT_TL_CONTACT_SENSOR__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleSensor.h"
#include "app_ui.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
#define CYCLE_ACTIVE_BUTTON		10
#define CYCLE_HOLD_BUTTON		1000
#define CYCLE_DETECT_LEVEL		40
static u32 button_pin[] = KB_SCAN_PINS;
#define num_but_pin  sizeof(button_pin)/sizeof(button_pin[0])
static u16 but_count[num_but_pin] = {0};
static u8 stt_pin[num_but_pin] = {0};

u32 last_tick_wakeup = 0;

enum
{
	BUT_IDLE = 0,
	BUT_PRESS,
	BUT_KEEPING_ONCE,
	BUT_KEEPING_LONG
};

typedef u8 (*detect_t)(u8 id);
typedef void (*handle_state_t) (u8 id);
typedef struct
{
	u8 mode;
	detect_t detector;
	handle_state_t handler;
}input_ctr_t;

void buttonShortPressed(u8 btNum);
void buttonKeepPressed(u8 btNum);
u8 rd_detect_button(u8 id);
u8 rd_detect_sensor(u8 id);
void rd_handle_mode_sensor(u8 id);
void rd_handle_mode_button(u8 id);

enum
{
	MODE_BUTTON=0,
	MODE_SENSOR
};

enum
{
	STATE_OFF= 0,
	STATE_PRESS = STATE_OFF,
	STATE_ON = 1,
	STATE_KEEPING = STATE_ON,
};
static input_ctr_t in_handle_arr[] ={
		{MODE_BUTTON,rd_detect_button,rd_handle_mode_button},
		{MODE_SENSOR,rd_detect_sensor,rd_handle_mode_sensor},
		{MODE_SENSOR,rd_detect_sensor,rd_handle_mode_sensor},
};


void rd_gpio_init()
{
	foreach_arr(i, button_pin)
	{
		gpio_setup_up_down_resistor(button_pin[i], PM_PIN_UP_DOWN_FLOAT);
		gpio_set_func(button_pin[i], AS_GPIO);
		gpio_set_input_en(button_pin[i], 1);
	}
}

u8 rd_read_input(u8 id)
{
	u16 stt = !gpio_read(button_pin[id]);
//	u16 stt = gpio_read(button_pin[id]);
//	u8 st = (stt && 0xff) && (button_pin[id] & 0xff);
	return (stt >0);
}

u8 rd_detect_button(u8 id)
{
	if(rd_read_input(id))
	{
		but_count[id] ++;
		if(but_count[id] == CYCLE_ACTIVE_BUTTON)
		{
			rd_log_uart("but%d press\n", id);
			stt_pin[id] = STATE_PRESS;
			return 1;
		}
		else if (but_count[id] == CYCLE_HOLD_BUTTON)
		{
			rd_log_uart("but%d hold\n", id);
			stt_pin[id] = STATE_KEEPING;
			return 1;
		}
		else if (but_count[id] > CYCLE_HOLD_BUTTON)
		{
			but_count[id] = CYCLE_HOLD_BUTTON + 1;
			return 0;
		}
	}
	else
	{
		but_count[id] = 0;
		return 0;
	}
	return 0;
}

u8 rd_detect_sensor(u8 id)
{
	u8 stt = rd_read_input(id);
	if(stt_pin[id] != stt)
	{
		but_count[id]++;
		if (but_count[id] == CYCLE_DETECT_LEVEL)
		{
			rd_log_uart("in %d change logic %d\n",id,stt);
			stt_pin[id] = stt;
			return 1;
		}
		if (but_count[id] > CYCLE_DETECT_LEVEL )
		{
			but_count[id] = CYCLE_DETECT_LEVEL + 1;
		}
	}
	else
	{
		but_count[id] = 0;
	}
	return 0;
}

void rd_handle_mode_button(u8 id)
{
	if(id == PIN_BUTTON1)
	{
		if(stt_pin[id] == STATE_PRESS)
		{

		}
		else if(stt_pin[id] == STATE_KEEPING)
		{
			zb_factoryReset();
		}
	}
}

void rd_handle_mode_sensor(u8 id)
{
	if(id == PIN_DOOR_SENSOR)
	{
		if(zb_isDeviceJoinedNwk())
		{
			zcl_iasZoneAttr_t *zone = zcl_iasZoneAttrGet();
			zone->zoneStatus = stt_pin[PIN_DOOR_SENSOR];
		}
		else
		{
			zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
		}
	}
	else if(id == PIN_ANTI_REMOVAL)
	{
		if(zb_isDeviceJoinedNwk())
		{
			zcl_iasZoneAttr_t *zone = zcl_iasZoneAttrGet();
			zone->hangonStatus = !stt_pin[PIN_ANTI_REMOVAL];
		}
	}
}


void rd_handle_button()
{
	u8 check =0;
	for(u8 i =0; i< sizeof(in_handle_arr) /sizeof(in_handle_arr[0]); i++)
	{
		if(in_handle_arr[i].detector)
		{
			if(in_handle_arr[i].detector(i))
			{
				if(in_handle_arr[i].handler)
				{
					in_handle_arr[i].handler(i);
				}
				check++;
			}
		}
	}
	if(check == 0)
	{
		g_sensorAppCtx.keyPressed = 0;
		last_tick_wakeup = clock_time();
	}
	else
	{
		g_sensorAppCtx.keyPressed = 1;
	}
}

void led_on(u32 pin)
{
	drv_gpio_write(pin, LED_ON);
}

void led_off(u32 pin)
{
	drv_gpio_write(pin, LED_OFF);
}

void light_on(void)
{
	led_on(LED_POWER);
}

void light_off(void)
{
	led_off(LED_POWER);
}

void light_init(void)
{
	led_off(LED_POWER);
}


s32 zclLightTimerCb(void *arg)
{
	u32 interval = 0;

	if(g_sensorAppCtx.sta == g_sensorAppCtx.oriSta){
		g_sensorAppCtx.times--;
		if(g_sensorAppCtx.times <= 0){
			g_sensorAppCtx.timerLedEvt = NULL;
			return -1;
		}
	}

	g_sensorAppCtx.sta = !g_sensorAppCtx.sta;
	if(g_sensorAppCtx.sta){
		light_on();
		interval = g_sensorAppCtx.ledOnTime;
	}else{
		light_off();
		interval = g_sensorAppCtx.ledOffTime;
	}

	return interval;
}

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
	u32 interval = 0;
	g_sensorAppCtx.times = times;

	if(!g_sensorAppCtx.timerLedEvt){
		if(g_sensorAppCtx.oriSta){
			light_off();
			g_sensorAppCtx.sta = 0;
			interval = ledOffTime;
		}else{
			light_on();
			g_sensorAppCtx.sta = 1;
			interval = ledOnTime;
		}
		g_sensorAppCtx.ledOnTime = ledOnTime;
		g_sensorAppCtx.ledOffTime = ledOffTime;

		g_sensorAppCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval);
	}
}

void light_blink_stop(void)
{
	if(g_sensorAppCtx.timerLedEvt){
		TL_ZB_TIMER_CANCEL(&g_sensorAppCtx.timerLedEvt);

		g_sensorAppCtx.times = 0;
		if(g_sensorAppCtx.oriSta){
			light_on();
		}else{
			light_off();
		}
	}
}

/*******************************************************************
 * @brief	Button click detect:
 * 			SW1. keep press button1 5s === factory reset
 * 			SW1. short press button1   === send level step with OnOff command (Up)
 * 			SW2. short press button2   === send level step with OnOff command (Down)
 *
 */
void buttonKeepPressed(u8 btNum){
	if(btNum == VK_SW1){
		g_sensorAppCtx.state = APP_FACTORY_NEW_DOING;
//		zb_factoryReset();
	}else if(btNum == VK_SW2){

	}
}

extern int rd_log_uart(const char *format, ...);
extern u16 rd_get_short_addr();
extern u8 rd_get_dstEp();

void buttonShortPressed(u8 btNum){
	if(btNum == VK_SW1){
		if(zb_isDeviceJoinedNwk()){
			rd_log_uart("but1 zb_isDeviceJoinedNwk\n");
			zcl_iasZoneAttr_t *zone = zcl_iasZoneAttrGet();
			zone->zoneStatus = !zone->zoneStatus;
//			epInfo_t dstEpInfo;
//			memset((u8 *)&dstEpInfo, 0, sizeof(epInfo_t));
//
//			dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
//			dstEpInfo.dstEp = rd_get_dstEp();
//			dstEpInfo.dstAddr.shortAddr = rd_get_short_addr();
//			dstEpInfo.profileId = HA_PROFILE_ID;
//
//			rd_log_uart("LOG- dstEp: %d,shortAddr: %d\n",dstEpInfo.dstEp,dstEpInfo.dstAddr.shortAddr);
//			zoneStatusChangeNoti_t statusChangeNotification;
//
//			statusChangeNotification.zoneStatus = ZONE_STATUS_TEST;
//			statusChangeNotification.extStatus = 0;
//			statusChangeNotification.zoneId = ZCL_ZONE_ID_INVALID;
//			statusChangeNotification.delay = 0;
//
//			zcl_iasZone_statusChangeNotificationCmd(SAMPLE_SENSOR_ENDPOINT, &dstEpInfo, TRUE, &statusChangeNotification);
		}else{
			zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
		}
	}else if(btNum == VK_SW2){
		if(zb_isDeviceJoinedNwk()){
			rd_log_uart("but1 zb_isDeviceJoinedNwk\n");
			zcl_iasZoneAttr_t *zone = zcl_iasZoneAttrGet();
			zone->hangonStatus = !zone->hangonStatus;
		}
	}
}


void keyScan_keyPressedCB(kb_data_t *kbEvt){
	//u8 toNormal = 0;
	u8 keyCode = kbEvt->keycode[0];
	//static u8 lastKeyCode = 0xff;

	buttonShortPressed(keyCode);

	if(keyCode == VK_SW1){
		g_sensorAppCtx.keyPressedTime = clock_time();
		g_sensorAppCtx.state = APP_FACTORY_NEW_SET_CHECK;
	}
}


void keyScan_keyReleasedCB(u8 keyCode){
	g_sensorAppCtx.state = APP_STATE_NORMAL;
}

void app_key_handler(void){
//	static u8 valid_keyCode = 0xff;
//	if(g_sensorAppCtx.state == APP_FACTORY_NEW_SET_CHECK){
//		if(clock_time_exceed(g_sensorAppCtx.keyPressedTime, 5*1000*1000)){
//			buttonKeepPressed(VK_SW1);
//		}
//	}
//	if(kb_scan_key(0 , 1)){
//		if(kb_event.cnt){
//			g_sensorAppCtx.keyPressed = 1;
//			keyScan_keyPressedCB(&kb_event);
//			if(kb_event.cnt == 1){
//				valid_keyCode = kb_event.keycode[0];
//			}
//		}else{
//			keyScan_keyReleasedCB(valid_keyCode);
//			valid_keyCode = 0xff;
//			g_sensorAppCtx.keyPressed = 0;
//		}
//	}
	static u32 last_tick = 0;
	if(last_tick > clock_time())
	{
		last_tick = 0;
	}
	else if(clock_time() - last_tick >5 * 1000 * 16)
	{
		rd_handle_button();
		last_tick = clock_time();
	}
}

#endif  /* __PROJECT_TL_CONTACT_SENSOR__ */
