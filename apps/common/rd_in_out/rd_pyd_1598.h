#ifndef PYD_1598_H_
#define PYD_1598_H_

#include "rd_in_out.h"

#define PYD_REG_PIN				GPIO_PB6
#define PYD_DIRECT_LINK_PIN		GPIO_PB5

#define PYD_THRESHOLD_DF		0x0E     // threshold from 0 -> 255. higher value, lower sensitive
//#define LED_PIN					GPIO_PC4
#define PYD_1598_CONFIG_DATA	0x0fb58800  // du lieu chi bao gom 25 bit trong so cao


typedef struct{
	union{
		u32 data;
		struct{
			u32 notUse						:7;  // 7 lowest bit
			u32 countMode					:1;

			u32 reserved2					:1; // must be 0 decimal
			u32 hpfCutOff					:1;
			u32 Reserved1					:2;  // must be 2 decimal
			u32 signalSource				:2;
			u32 operationModes				:2;

			u32 windowTime					:2;
			u32 pulseCounter				:2;
			u32 blindTime					:4;

			u32 threshold					:8;  // 8 highest bit
		};
	};
}pyd_config_t;

#endif
