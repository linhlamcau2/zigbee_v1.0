#include"rd_in_out.h"

#define RD_SCAN_PINS		{BUTTON_CONFIG}
#define RD_OUTPUT_LED 		{LED_B,LED_R}


#define NUM_OUTPUT_MAX 		2
u32 pin_scan[] = RD_SCAN_PINS;
u32 led_out[] = RD_OUTPUT_LED;
static u16 count = 0;

rd_ctx_t rd_lightcTx[NUM_OUTPUT_MAX];
u8 rd_output[NUM_OUTPUT_MAX] = {0};


void rd_light_factory_rst()
{
	for(u8 i=0; i< 5; i++)
	{
		for(u8 j=0; j< NUM_OUTPUT_MAX; j++)
		{
			drv_gpio_write(led_out[j], 1);
		}
		sleep_ms(300);
		drv_wd_clear();
		for(u8 j=0; j< NUM_OUTPUT_MAX; j++)
		{
			drv_gpio_write(led_out[j], 0);
		}
		sleep_ms(300);
		drv_wd_clear();
	}
}

u8 rd_read_input(u8 idx)
{
	u16 stt = !gpio_read(pin_scan[idx]);
//	if(stt > 0)
//	{
//		rd_log_uart("but %d active\n",idx);
//	}
//	u16 stt = gpio_read(button_pin[id]);
//	u8 st = (stt && 0xff) && (button_pin[id] & 0xff);
	return (stt >0);
}

void rd_scan_button()
{
	u8 stt = rd_read_input(0);
	if(stt)
	{
		count ++;
		if(count == CYCLE_MODE_PULSE)
		{
			rd_log_uart("but press\n");
		}
		else if(count == CYCLE_MODE_KEEP)
		{
			rd_log_uart("but hold\n");
			rd_light_factory_rst();
			zb_factoryReset();
		}
		else if(count > CYCLE_MODE_KEEP)
		{
			count = CYCLE_MODE_KEEP + 1;
		}
	}
	else
	{
		count = 0;
	}
}

s32 rd_light_blink_TimerEvtCb(void *arg)
{
	u8 idx = *(u8 *)arg;
	if(rd_lightcTx[idx].sta == rd_lightcTx[idx].oriSta)
	{
		if(rd_lightcTx[idx].times)
		{
			rd_lightcTx[idx].times -- ;
			if(rd_lightcTx[idx].times <= 0)
			{
				drv_gpio_write(led_out[idx], rd_lightcTx[idx].oriSta);
				rd_lightcTx[idx].timerLedEvt = NULL;
				return -1;
			}
		}
	}

	rd_lightcTx[idx].sta = !rd_lightcTx[idx].sta;
	drv_gpio_write(led_out[idx], rd_lightcTx[idx].sta);

	return 0;
}

u8 list_led[] = {0,1};
void rd_light_blink(u8 times, u8 time_delay_100ms, u8 idx)
{
	rd_lightcTx[idx].oriSta = rd_output[idx];
	rd_lightcTx[idx].times = times;
	if(!rd_lightcTx[idx].timerLedEvt)
	{
		rd_lightcTx[idx].sta = ! rd_lightcTx[idx].oriSta;
		drv_gpio_write(led_out[idx], rd_lightcTx[idx].sta);
		rd_lightcTx[idx].timerLedEvt = TL_ZB_TIMER_SCHEDULE(rd_light_blink_TimerEvtCb, (void *)&list_led[idx], 100 * time_delay_100ms);
	}
}


