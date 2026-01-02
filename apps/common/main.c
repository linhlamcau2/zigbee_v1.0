/********************************************************************************************************
 * @file    main.c
 *
 * @brief   This is the source file for main
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

#include "zb_common.h"

#include "rd_log/rd_log.h"
#include "../proj/os/ev_poll.h"
#include "../proj/drivers/drv_uptime.h"
#include "string.h"
#include "log_sys/log_sys.h"
#include "user_utils/task.h"

//#include "../sampleLight/sampleLight.h"

/*
 * main:
 * */

 u8 rd_par_test[60] = {0};
extern void rd_nema_report(u8* p_data);
u8 data_test[50] ={1,2,3,4,5,6,7,8,9,10,
					11,12,13,14,15,16,17,18,19,20,
					21,22,23,24,25,26,27,28,29,30,
					31,32,33,34,35,36,37,38,39,40,
					41,42,43,44,45,46,47,48,49,50};
int rd_send_report_test(void *param)
{
	rd_par_test[2] = 0x39;
	rd_nema_report(rd_par_test);
	LOGI("Test");
	// LOGD_HEX(data_test,50);
	return 1;
}
void rd_log_mac()
{
	// rd_log_uart("mac: ");
	// for(int i=0; i<8; i++)
	// {
	// 	rd_log_uart("%d ",*((u8 *)&g_zbMacPib.extAddress + i));
	// }
	// rd_log_uart("\n");
}
int main(void){
	startup_state_e state = drv_platform_init();

	drv_uptime_init();
	u8 isRetention = (state == SYSTEM_DEEP_RETENTION) ? 1 : 0;

	os_init(isRetention);

#if 0
	extern void moduleTest_start(void);
	moduleTest_start();
#else

	// rd_init_uart();
	// extern void rd_gpio_init();
	// rd_gpio_init();   //RD_EDIT: GPIO_INIT
	extern void user_init(bool isRetention);
	user_init(isRetention);

	drv_enable_irq();
	log_init();
//	ev_on_poll(EV_POLL_HCI,rd_test_task);
#if (MODULE_WATCHDOG_ENABLE)
	drv_wd_setInterval(600);
    drv_wd_start();
#endif

#if VOLTAGE_DETECT_ENABLE
    u32 tick = clock_time();
#endif
    // drv_uart_tx_start((u8 *)"hi\n",3);
    // rd_log_uart("start prg\n");
//    extern void rd_print_light(void);
//    rd_print_light();
//    extern void rd_print_reporting(void);
   rd_print_reporting();
    // rd_log_mac();
	TASK_INIT();
	TASK_ADD(rd_send_report_test, NULL, 10000, 1000); // every 5s ,delay 2s
	while(1){
#if VOLTAGE_DETECT_ENABLE
		if(clock_time_exceed(tick, 200 * 1000)){
			voltage_detect(0);
			tick = clock_time();
		}
#endif

#if defined(MCU_CORE_B92)
		drv_vbusWatchdogClose();
#endif

    	ev_main();

#if (MODULE_WATCHDOG_ENABLE)
		drv_wd_clear();
#endif

		tl_zbTaskProcedure();
		TASK_RUN();
//		rd_send_report_test();
//		rd_blink_led();
//		tl_printf("test: %d %d\n",1,2);
//		static u32 last_tick = 0;
//		if(clock_time() - last_tick > 3 * 1000 *1000)
//		{
//			rd_log_uart("tesst: %d %d %d\n", 1,2,4);
//			last_tick = clock_time();
//		}
//		drv_uart_tx_start((u8 *)"hi\n",3);
//		rd_ev_log("hello\n");
//		sprintf()
#if	(MODULE_WATCHDOG_ENABLE)
		drv_wd_clear();
#endif
	}

#endif

	return 0;
}


