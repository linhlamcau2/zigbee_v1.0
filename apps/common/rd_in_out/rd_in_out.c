#include"rd_in_out.h"


u32 pin_scan[] = RD_SCAN_PINS;
u32 led_out[] = RD_OUTPUT_LED;

void rd_handle_mode_pulse(u8 idx);
void rd_handle_mode_level(u8 idx);
void rd_handle_mode_keep(u8 idx);

extern output_t rd_output[NUM_OUTPUT_MAX];
enum
{
	MODE_NONE = 0,
	MODE_PULSE = BIT(0),
	MODE_LEVEL = BIT(1),
	MODE_KEEP  = BIT(2),
	MODE_BUTTON = MODE_PULSE | MODE_KEEP,
	MODE_MAX = MODE_KEEP
};

typedef void (*handle_state_t) (u8 id);

typedef struct
{
	u8 mode;
	handle_state_t handler;
}input_ctr_t;

typedef struct
{
	u32 pin;
	u8 mode;
	u8 stt;
	u16 cycle_max;
	u16 count;
}pin_inf_t;

static pin_inf_t pin_inf[num_pin];

static input_ctr_t in_handle_arr[] ={
		{MODE_NONE,NULL},
		{MODE_PULSE,rd_handle_mode_pulse},
		{MODE_LEVEL,rd_handle_mode_level},
		{MODE_KEEP,rd_handle_mode_keep}
};

u8 is_mode_pulse(u8 idx)
{
	return pin_inf[idx].mode & MODE_PULSE;
}

u8 is_mode_level(u8 idx)
{
	return pin_inf[idx].mode & MODE_LEVEL;
}

u8 is_mode_keep(u8 idx)
{
	return pin_inf[idx].mode & MODE_KEEP;
}

void rd_register_mode_pin_input(u8 idx, u8 mode)
{
	if(idx < num_pin)
	{
		pin_inf[idx].pin = pin_scan[idx];
		pin_inf[idx].mode = mode;
		pin_inf[idx].stt = 0;
		if(mode & MODE_KEEP)
		{
			pin_inf[idx].cycle_max = CYCLE_MODE_KEEP;
		}
		else if(mode & MODE_LEVEL)
		{
			pin_inf[idx].cycle_max = CYCLE_MODE_LEVEL;
		}
		else if(mode & MODE_PULSE)
		{
			pin_inf[idx].cycle_max = CYCLE_MODE_PULSE;
		}
	}
}

u8 rd_detect_mode_input(u8 idx,u8 stt)
{
	if(is_mode_pulse(idx) || is_mode_keep(idx))
	{
		if(stt)
		{
			pin_inf[idx].count ++;
			if(pin_inf[idx].count == CYCLE_MODE_PULSE)
			{
				return MODE_PULSE;
			}
			else if(pin_inf[idx].count == CYCLE_MODE_KEEP)
			{
				return MODE_KEEP;
			}
			else if(pin_inf[idx].count > pin_inf[idx].cycle_max)
			{
				pin_inf[idx].count = pin_inf[idx].cycle_max + 1;
			}
		}
		else
			pin_inf[idx].count = 0;
	}
	else if(is_mode_level(idx))
	{
		if(stt != pin_inf[idx].stt)
		{
			pin_inf[idx].count ++;
			if(pin_inf[idx].count == CYCLE_MODE_LEVEL)
			{
				pin_inf[idx].stt = stt;
				return MODE_LEVEL;
			}
		}
		else
		{
			pin_inf[idx].count = 0;
		}
	}
	return MODE_NONE;
}

void rd_gpio_init()
{
	memset((void *)pin_inf, 0, sizeof(pin_inf));
	memset((void *)rd_output, 0, sizeof(rd_output));
	foreach_arr(i, pin_scan)
	{
		gpio_setup_up_down_resistor(pin_scan[i], PM_PIN_PULLUP_10K);
		gpio_set_func(pin_scan[i], AS_GPIO);
		gpio_set_input_en(pin_scan[i], 1);
		rd_register_mode_pin_input(i,MODE_BUTTON);
	}

	foreach_arr(i, led_out)
	{
		gpio_setup_up_down_resistor(led_out[i], PM_PIN_PULLUP_10K);
		gpio_set_func(led_out[i], AS_GPIO);
		gpio_set_output_en(led_out[i], 1);
		gpio_write(led_out[i],0);
	}
}

void rd_write_led_out(u8 idx, u8 stt)
{
	drv_gpio_write(led_out[idx], stt);
	rd_output[idx].stt = stt;
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

void rd_handle_mode_pulse(u8 idx)
{
	rd_log_uart("but %d pulse\n",idx);
	switch(idx)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		{
			if(zb_isDeviceJoinedNwk())
			{
				u8 stt = rd_output[idx].stt;
				rd_write_led_out(idx, !stt);
				rd_save_stt();
			}
			break;
		}
		default:
			break;
	}
}

void rd_handle_mode_level(u8 idx)
{
	rd_log_uart("but %d level %d \n",idx,pin_inf[idx].stt);
	switch(idx)
	{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		default:
			break;
	}
}

void rd_handle_mode_keep(u8 idx)
{
	rd_log_uart("but %d keep\n",idx);
	switch(idx)
	{
		case 0:
		{
			gLightCtx.state = APP_FACTORY_NEW_DOING;
			zb_factoryReset();
			break;
		}
		case 1:
			break;
		case 2:
			break;
		default:
			break;
	}
}


void rd_handle_input()
{
	for(u8 idx= 0; idx < num_pin; idx++)
	{
		u8 stt = rd_read_input(idx);
		u8 mode_detect = rd_detect_mode_input(idx,stt);
		for(u8 i =0; i < sizeof(in_handle_arr)/sizeof(in_handle_arr[0]); i++)
		{
			if(mode_detect ==in_handle_arr[i].mode)
			{
				if(in_handle_arr[i].handler)
				{
					in_handle_arr[i].handler(idx);
				}
			}
		}
	}
}
