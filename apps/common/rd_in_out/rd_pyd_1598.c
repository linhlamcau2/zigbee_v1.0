#include "rd_pyd_1598.h"
#include "rd_in_out.h"

pyd_config_t pydConfig;
static u8 pydInitFlag = 0;

void pyd1598IOInit(){
	gpio_set_func(PYD_REG_PIN, AS_GPIO);
	gpio_set_output_en(PYD_REG_PIN, 1);

	gpio_set_func(PYD_DIRECT_LINK_PIN, AS_GPIO);
	gpio_set_output_en(PYD_DIRECT_LINK_PIN, 1);

	gpio_write(PYD_REG_PIN,1);
	gpio_write(PYD_DIRECT_LINK_PIN,1);
}


void pyd1598RegWrite(unsigned int data){
	gpio_write(PYD_DIRECT_LINK_PIN,0);
	gpio_write(PYD_REG_PIN,0);
	sleep_ms(1);//RD_EDIT: NOTE todo
	for(char i=0;i<25;i++){
		gpio_write(PYD_REG_PIN,0);
		sleep_us(1);
		gpio_write(PYD_REG_PIN,1);
		sleep_us(1);
		if((data >> (31 - i)) & 1){
			gpio_write(PYD_REG_PIN,1);

		}
		else {
			gpio_write(PYD_REG_PIN,0);
		}
		sleep_us(100);
	}
	gpio_write(PYD_REG_PIN,0);
	gpio_write(PYD_DIRECT_LINK_PIN,0);
	sleep_us(660);

	gpio_set_func(PYD_DIRECT_LINK_PIN, AS_GPIO);
	gpio_set_output_en(PYD_DIRECT_LINK_PIN, 0);
	gpio_set_input_en(PYD_DIRECT_LINK_PIN, 1);
	gpio_setup_up_down_resistor(PYD_DIRECT_LINK_PIN,PM_PIN_PULLDOWN_100K);
}

void pyd1598RegInit(u8 ThresholdSet){
	pydConfig.threshold = ThresholdSet;   	// threshold from 0 -> 255. higher value, lower sensitive
	pydConfig.blindTime = 15;     	// time want to bind  = 0.5 + pydConfig.blindTime * 0.5
    pydConfig.pulseCounter = 2;   	// thong so nay can xem lai
    pydConfig.windowTime = 1;     	//
    pydConfig.operationModes = 2;	//
    pydConfig.signalSource = 0;		//
    pydConfig.Reserved1 = 2;		// must be 2
    pydConfig.hpfCutOff = 0;		//
    pydConfig.reserved2 = 0;		// must be 0
    pydConfig.countMode = 0;		// thong so nay can xem lai

	pyd1598RegWrite(pydConfig.data);

	rd_log_uart("Set threshold: %d\n", pydConfig.threshold);
}


void pyd1598Init(){
	if(!pydInitFlag){
		pydInitFlag = 1;
//		clock_init(SYS_CLK_32M_Crystal);

		pyd1598IOInit();
		sleep_ms(100);
		pyd1598RegInit(PYD_THRESHOLD_DF);

		sleep_ms(50);
//		clock_init(SYS_CLK_16M_Crystal);
	}
}

u8 pyd1598Read_motion(void){
	if(gpio_read(PYD_DIRECT_LINK_PIN)){
		gpio_set_func(PYD_DIRECT_LINK_PIN, AS_GPIO);
		gpio_set_output_en(PYD_DIRECT_LINK_PIN, 1);
		gpio_set_input_en(PYD_DIRECT_LINK_PIN, 0);
		gpio_write(PYD_DIRECT_LINK_PIN,0);
		sleep_ms(12);
		gpio_set_func(PYD_DIRECT_LINK_PIN, AS_GPIO);
		gpio_set_output_en(PYD_DIRECT_LINK_PIN, 0);
		gpio_set_input_en(PYD_DIRECT_LINK_PIN, 1);
		gpio_setup_up_down_resistor(PYD_DIRECT_LINK_PIN,PM_PIN_UP_DOWN_FLOAT);
		return 1;
	}
	return 0;
}






