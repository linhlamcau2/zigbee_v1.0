#include "rd_pir.h"
#include "rd_in_out.h"

#define TIME_SEND_LUX_S					20
#define DELTA_LUX_UPDATE 				30

u8 SendLuxFlag = 0;

PIR_Stt_t PIR_STT_Common = {
		.LuxLastSendTick_s =0,
		.LuxLastSend = 0,
		.LuxCurrent  = 0,
		.MotionLast = Motion_None,
		.MotionCurrent = Motion_None,
		.MotionLastTick_s = 0,
		.MotionKeeping_TimeOut_s = TIME_KEEP_MOTION_DF_S,
//RD_Todo
};


void RD_Lpn_led_but_Init(void)
{
	gpio_set_func(LED_B, AS_GPIO);
	gpio_set_output_en(LED_B, 1);
	gpio_set_input_en(LED_B, 0);
	gpio_write(LED_B, 0);   // off

	gpio_set_func(LED_R, AS_GPIO);
	gpio_set_output_en(LED_R, 1);
	gpio_set_input_en(LED_R, 0);
	gpio_write(LED_R, 0);   // off

	gpio_set_func(BUTTON_CONFIG, AS_GPIO);
	gpio_setup_up_down_resistor(BUTTON_CONFIG, PM_PIN_UP_DOWN_FLOAT);
	gpio_set_output_en(BUTTON_CONFIG, 0);
	gpio_set_input_en(BUTTON_CONFIG, 1);
}


void RD_I2cInit(void){
	drv_i2c_master_init(I2C_CLOCK);
	drv_i2c_gpio_set(I2C_GPIO_GROUP_C0C1);
	i2c_set_id(SLAVE_DEVICE_ADDR);
}

u16 RD_Light_Sensor_read_raw_val() {
	u16  lux_read_data;
	u8 RD_i2c_rx_buff[3] = {0};
	u8 RD_i2c_tx_buff[2] = {OPT3001_CONFIG_REG_HIGH,OPT3001_CONFIG_REG_LOW};

	drv_i2c_write_series(SLAVE_DEVICE_ADDR, OPT3001_CONFIG_REGISTER, OPT3001_CONFIG_REGISTER_LEN, (u8 *)RD_i2c_tx_buff, 2);

	WaitMs(20);   //1S

	drv_i2c_read_series(SLAVE_DEVICE_ADDR, OPT3001_RESULT_REGISTER, OPT3001_RESULT_REGISTER_LEN, (u8 *)RD_i2c_rx_buff, 3);

//	i2c_write_series(OPT3001_CONFIG_REGISTER,OPT3001_CONFIG_REGISTER_LEN,(u8 *)RD_i2c_tx_buff, 2);
//	i2c_read_series(OPT3001_RESULT_REGISTER,OPT3001_RESULT_REGISTER_LEN, (u8 *)RD_i2c_rx_buff, 2);

	lux_read_data = (RD_i2c_rx_buff[0]<<8) | RD_i2c_rx_buff[1];
	return lux_read_data;
}

u32 CalculateLux(u32 rsp_lux)
{
	u16 lux_LSB = 0;
	u8 lux_MSB = 0;
	u32 lux_Value = 0;
	u16 pow = 1;
	u8 i;
	lux_LSB = rsp_lux & 0x0FFF;
	lux_MSB = ((rsp_lux>>12) & 0x0F);
	for(i=0;i<lux_MSB;i++){
		pow=pow*2;
	}
	lux_Value = pow * lux_LSB /100;
	// RD_EDIT: calib lux
	rd_log_uart("Lux i2c: %d", lux_Value);
	if(lux_Value > 60)
	{
		u16 calib_lux =0;
		if(lux_Value <5) calib_lux =0;
		else
		{
			 calib_lux = 27 * lux_Value / 10 +25;
		}
		if(calib_lux < 0)  calib_lux =0;
		if(calib_lux > 10000) calib_lux = 9999; //RD_EDIT: Max lux value

		lux_Value = calib_lux;
	}
	return lux_Value;
}

u32 RD_ReadLightSensor(void){
		u32 rawLux = RD_Light_Sensor_read_raw_val();
		u32 luxReadBuff = CalculateLux(rawLux);

		return luxReadBuff;
}

void rd_set_motion_stt(u8 stt)
{
	rd_log_uart("rd_set_motion_stt %d\n", stt);
	zcl_iasZoneAttr_t *zone = zcl_iasZoneAttrGet();
	zone->zoneMotionStatus = stt;
}

void rd_set_lux_level(u32 level)
{
	PIR_STT_Common.LuxCurrent = level;
	rd_log_uart("rd_set_motion_stt %d\n", level);
	zcl_iasZoneAttr_t *zone = zcl_iasZoneAttrGet();
	zone->zoneLuxStatus = level;
}

void rd_pir_init()
{
	RD_Lpn_led_but_Init();
	pyd1598Init();
	RD_I2cInit();
}

void RD_PIR_UpdateSensorAC(void)
{
	u8 Motion_Signal = 0;
	Motion_Signal = pyd1598Read_motion();
	if(Motion_Signal == 1)
	{
		PIR_STT_Common.MotionLastTick_s  = RD_SYS_32K_TICK_S;
		rd_log_uart("motion read =1 : %d\n", RD_SYS_32K_TICK_S);
		if((PIR_STT_Common.MotionLast == Motion_Start) || (PIR_STT_Common.MotionLast == Motion_Keeping))
		{
			PIR_STT_Common.MotionLast = PIR_STT_Common.MotionCurrent = Motion_Keeping;
		}
		if((PIR_STT_Common.MotionLast == Motion_None) || (PIR_STT_Common.MotionLast == Motion_Stop))
		{
			PIR_STT_Common.MotionLast = PIR_STT_Common.MotionCurrent = Motion_Start;
			rd_set_motion_stt(1);
		}

	}

	else
	{
		if(PIR_STT_Common.MotionLastTick_s > RD_SYS_32K_TICK_S) {PIR_STT_Common.MotionLastTick_s = 0;}

		if((PIR_STT_Common.MotionLast == Motion_Start) || (PIR_STT_Common.MotionLast == Motion_Keeping))
		{
			if(RD_SYS_32K_TICK_S - PIR_STT_Common.MotionLastTick_s >= PIR_STT_Common.MotionKeeping_TimeOut_s)
			{
				PIR_STT_Common.MotionLast = PIR_STT_Common.MotionCurrent = Motion_Stop;
				rd_set_motion_stt(0);
			}
			else
			{
				PIR_STT_Common.MotionLast = PIR_STT_Common.MotionCurrent = Motion_Keeping;
			}
		}
		else
		{
			PIR_STT_Common.MotionLast = PIR_STT_Common.MotionCurrent = Motion_None;
		}
	}
}

void RD_PIR_CheckSendLux(void)
{
	if(PIR_STT_Common.LuxLastSendTick_s > RD_SYS_32K_TICK_S) {PIR_STT_Common.LuxLastSendTick_s = 0;}
	if( (RD_SYS_32K_TICK_S - PIR_STT_Common.LuxLastSendTick_s >= TIME_SEND_LUX_S))
	{
		PIR_STT_Common.LuxLastSendTick_s = RD_SYS_32K_TICK_S;
		PIR_STT_Common.LuxCurrent = RD_ReadLightSensor();
		rd_log_uart("Lux update: %d\n", PIR_STT_Common.LuxCurrent);
		int delta_Lux = PIR_STT_Common.LuxCurrent - PIR_STT_Common.LuxLastSend;
		if((abs(delta_Lux)) >= DELTA_LUX_UPDATE)
		{
			PIR_STT_Common.LuxLastSend = PIR_STT_Common.LuxCurrent;
			rd_set_lux_level(PIR_STT_Common.LuxCurrent);
		}
	}
}


