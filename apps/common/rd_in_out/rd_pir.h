#ifndef RD_PIR_H
#define RD_PIR_H

#include "rd_in_out.h"

#define I2C_CLOCK					200000
#define OPT3001_CONFIG_REG_HIGH                 0xC4   // xem trong datasheet phan configure register
//#define OPT3001_CONFIG_REG_HIGH                 0xC4   // xem trong datasheet phan configure register
#define OPT3001_CONFIG_REG_LOW                  0x10   // xem trong datasheet phan configure register
//#define OPT3001_CONFIG_REG_LOW                  0x14   // xem trong datasheet phan configure register
#define  I2C_CLK_SPEED                       200000
#define  SLAVE_DEVICE_ADDR                   0x88

#define OPT3001_RESULT_REGISTER                 0x00
#define OPT3001_RESULT_REGISTER_LEN             1
#define OPT3001_CONFIG_REGISTER                 0x01
#define OPT3001_CONFIG_REGISTER_LEN             1


#define TIME_KEEP_MOTION_DF_S  27
enum
{
	Motion_None = 0,
	NoHave_Motion,
	Motion_Keeping,
	Motion_Start,
	Motion_Stop
};

typedef struct {
	u32 LuxLastSendTick_s;
	u32 LuxLastSend;
	u32 LuxCurrent;
	u8 MotionLast;
	u8 MotionCurrent;
	u32 MotionLastTick_s;
	u16 MotionKeeping_TimeOut_s;
}PIR_Stt_t;


#endif





