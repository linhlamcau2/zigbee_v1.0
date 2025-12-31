/********************************************************************************************************
 * @file    drv_uptime.c
 *
 * @brief   This is the source file for uptime counter using timer interrupt
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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

#include "../tl_common.h"
#include "drv_uptime.h"
#include "../platform/chip_8258/timer.h"
#include "../platform/chip_8258/register.h"


// Uptime counter in milliseconds (updated by timer interrupt)
static volatile unsigned long g_uptime_ms = 0;

// Flag to indicate if uptime counter is initialized
static u8 g_uptime_initialized = 0;

/**
 * @brief     Uptime timer callback function.
 *            This function should be called from TIMER0 interrupt handler.
 *            It increments uptime counter and sets next interrupt.
 * @param[in] none.
 * @return    none.
 */
_attribute_ram_code_ void drv_uptime_timer_callback(void)
{
	if(g_uptime_initialized){
		// Increment uptime counter
		g_uptime_ms++;
		
		// Set next interrupt capture point (1ms later)
		// 1ms = 16000 ticks (at 16MHz, 16 ticks per microsecond)
		// Read current timer tick value from register
		unsigned long current_tick = reg_tmr_tick(TIMER0);
		timer_set_cap_tick(TIMER0, current_tick + 3*CLOCK_16M_SYS_TIMER_CLK_1MS);
	}
}

/**
 * @brief     Initialize uptime counter using TIMER0 interrupt.
 *            Timer will generate interrupt every 1ms to update uptime counter.
 *            NOTE: This will use TIMER0 exclusively for uptime counting.
 * @param[in] none.
 * @return    none.
 */
void drv_uptime_init(void)
{
	u32 r = drv_disable_irq();
	
	// Reset uptime counter
	g_uptime_ms = 0;
	
	// Configure TIMER0 for system clock mode
	timer_set_mode(TIMER0, TIMER_MODE_SYSCLK);
	
	// Set initial tick to 0
	timer_set_init_tick(TIMER0, 0);
	
	// Set capture tick for 1ms interrupt (16000 ticks = 1ms at 16MHz)
	timer_set_cap_tick(TIMER0, 3*CLOCK_16M_SYS_TIMER_CLK_1MS);
	
	// Enable timer interrupt
	timer_irq_enable(TIMER0);
	
	// Start timer
	timer_start(TIMER0);
	
	g_uptime_initialized = 1;
	
	drv_restore_irq(r);
}

/**
 * @brief     Get uptime in milliseconds (counted by timer interrupt).
 * @param[in] none.
 * @return    uptime in milliseconds.
 */
unsigned long drv_uptime_get_ms(void)
{
	return g_uptime_ms;
}

/**
 * @brief     Get uptime in seconds (counted by timer interrupt).
 * @param[in] none.
 * @return    uptime in seconds.
 */
unsigned long drv_uptime_get_s(void)
{
	return g_uptime_ms / 1000;
}

/**
 * @brief     Reset uptime counter to zero.
 * @param[in] none.
 * @return    none.
 */
void drv_uptime_reset(void)
{
	u32 r = drv_disable_irq();
	g_uptime_ms = 0;
	drv_restore_irq(r);
}

