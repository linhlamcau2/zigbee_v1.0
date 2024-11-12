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

#include "rd_in_out/rd_in_out.h"
#include "rd_log/rd_log.h"
#include "../proj/os/ev_poll.h"
#include "string.h"


/*
 * main:
 * */
int main(void){
	startup_state_e state = drv_platform_init();

	u8 isRetention = (state == SYSTEM_DEEP_RETENTION) ? 1 : 0;

	os_init(isRetention);

#if 0
	extern void moduleTest_start(void);
	moduleTest_start();
#else

	extern void user_init(bool isRetention);
	user_init(isRetention);

	drv_enable_irq();
	//////////-------rd_init---------//////////
//	rd_init_gpio();
//	rd_init_uart();
//	ev_on_poll(EV_POLL_HCI,rd_test_task);
#if (MODULE_WATCHDOG_ENABLE)
	drv_wd_setInterval(600);
    drv_wd_start();
#endif

#if VOLTAGE_DETECT_ENABLE
    u32 tick = clock_time();
#endif
    rd_log_uart("start prg\n");
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


