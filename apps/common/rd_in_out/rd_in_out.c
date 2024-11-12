#include"rd_in_out.h"
#include "zb_common.h"


static void init_gpio(u32 pin)
{
	drv_gpio_func_set(pin);
	drv_gpio_output_en(pin, 1); 		//enable output
	drv_gpio_input_en(pin, 0);		//disable input
	drv_gpio_write(pin, 1);
}

void rd_init_gpio()
{
	init_gpio(OUTPUT_1);
	init_gpio(OUTPUT_2);
}

void rd_blink_led()
{
	drv_gpio_write(OUTPUT_1,1);
	drv_gpio_write(OUTPUT_2,0);
//	WaitMs(1000);
//	drv_wd_clear();
//	drv_gpio_write(OUTPUT_1,0);
//	drv_gpio_write(OUTPUT_2,0);
//	WaitMs(1000);
//	drv_wd_clear();
}
