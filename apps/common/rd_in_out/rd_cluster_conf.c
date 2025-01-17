#include "rd_in_out.h"

static u8 start_save = 0;
static u32 tick_save = 0;
output_t rd_output[NUM_OUTPUT_MAX] ;

rd_ctx_t rd_lightcTx[NUM_OUTPUT_MAX];

const u16 rd_sw_inClusterList[] =
{
#ifdef ZCL_ON_OFF
	ZCL_CLUSTER_GEN_ON_OFF,
#endif
};

const u16 rd_sw_outClusterList[] =
{
#ifdef ZCL_OTA
    ZCL_CLUSTER_OTA,
#endif
};

#define RD_SW_IN_CLUSTER_NUM		(sizeof(rd_sw_inClusterList)/sizeof(rd_sw_inClusterList[0]))
#define RD_SW_OUT_CLUSTER_NUM		(sizeof(rd_sw_outClusterList)/sizeof(rd_sw_outClusterList[0]))

const af_simple_descriptor_t rd_sw_simpleDesc[NUM_OUTPUT_MAX] =
{
		{
			HA_PROFILE_ID,                      		/* Application profile identifier */
			HA_DEV_ONOFF_OUTPUT,
			RD_ENDPOINT_1,              		/* Endpoint */
			1,                                  		/* Application device version */
			0,											/* Reserved */
			RD_SW_IN_CLUSTER_NUM,           		/* Application input cluster count */
			RD_SW_OUT_CLUSTER_NUM,          		/* Application output cluster count */
			(u16 *)rd_sw_inClusterList,    			/* Application input cluster list */
			(u16 *)rd_sw_outClusterList,
		},

		{
			HA_PROFILE_ID,                      		/* Application profile identifier */
			HA_DEV_ONOFF_OUTPUT,
			RD_ENDPOINT_2,              		/* Endpoint */
			1,                                  		/* Application device version */
			0,											/* Reserved */
			RD_SW_IN_CLUSTER_NUM,           		/* Application input cluster count */
			RD_SW_OUT_CLUSTER_NUM,          		/* Application output cluster count */
			(u16 *)rd_sw_inClusterList,    			/* Application input cluster list */
			(u16 *)rd_sw_outClusterList,
		},

		{
			HA_PROFILE_ID,                      		/* Application profile identifier */
			HA_DEV_ONOFF_OUTPUT,
			RD_ENDPOINT_3,              		/* Endpoint */
			1,                                  		/* Application device version */
			0,											/* Reserved */
			RD_SW_IN_CLUSTER_NUM,           		/* Application input cluster count */
			RD_SW_OUT_CLUSTER_NUM,          		/* Application output cluster count */
			(u16 *)rd_sw_inClusterList,    			/* Application input cluster list */
			(u16 *)rd_sw_outClusterList,
		},

//		{
//			HA_PROFILE_ID,                      		/* Application profile identifier */
//			HA_DEV_ONOFF_OUTPUT,
//			RD_ENDPOINT_4,              		/* Endpoint */
//			1,                                  		/* Application device version */
//			0,											/* Reserved */
//			RD_SW_IN_CLUSTER_NUM,           		/* Application input cluster count */
//			RD_SW_OUT_CLUSTER_NUM,          		/* Application output cluster count */
//			(u16 *)rd_sw_inClusterList,    			/* Application input cluster list */
//			(u16 *)rd_sw_outClusterList,
//		},
//
//		{
//			HA_PROFILE_ID,                      		/* Application profile identifier */
//			HA_DEV_ONOFF_OUTPUT,
//			RD_ENDPOINT_5,              		/* Endpoint */
//			1,                                  		/* Application device version */
//			0,											/* Reserved */
//			RD_SW_IN_CLUSTER_NUM,           		/* Application input cluster count */
//			RD_SW_OUT_CLUSTER_NUM,          		/* Application output cluster count */
//			(u16 *)rd_sw_inClusterList,    			/* Application input cluster list */
//			(u16 *)rd_sw_outClusterList,
//		},
};

_CODE_ZCL_ static status_t zcl_rd_sw_clientCmdHandler(zclIncoming_t *pInMsg)
{
	rd_log_uart("zcl_rd_sw_clientCmdHandler\n");
	u8 status = ZCL_STA_SUCCESS;

	zcl_scene_cmdPayload_t cmdPayload;
	memset((u8 *)&cmdPayload, 0, sizeof(zcl_scene_cmdPayload_t));

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_ONOFF_OFF:
		case ZCL_CMD_ONOFF_ON:
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	if(status == ZCL_STA_SUCCESS){
		if(pInMsg->clusterAppCb){
			pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmdPayload);
		}
	}
	return status;
}

_CODE_ZCL_ static status_t zcl_rd_sw_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_rd_sw_clientCmdHandler(pInMsg);
	}else{
//		return zcl_gp_serverCmdHandler(pInMsg);
	}
	return 0;
}

_CODE_ZCL_ status_t zcl_rd_sw_register(u8 endpoint, u16 manuCode, u8 arrtNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ON_OFF, manuCode, arrtNum, attrTbl, zcl_rd_sw_cmdHandler, cb);
}

void rd_sw_onoff(u8 cmd, u8 idx)
{
	u8 stt = rd_output[idx].stt;
	u8 stt_set = (cmd == ZCL_CMD_ONOFF_OFF) ? 0: 1;
	if(stt != stt_set)
	{
		rd_write_led_out(idx,stt_set);
		rd_save_stt();
	}
}

status_t zcl_rd_sw_Cb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	rd_log_uart("zcl_rd_sw_Cb %d\n",pAddrInfo->dstEp);
	switch(pAddrInfo->dstEp)
	{
		case RD_ENDPOINT_1:
		case RD_ENDPOINT_2:
		case RD_ENDPOINT_3:
//		case RD_ENDPOINT_4:
//		case RD_ENDPOINT_5:
		{
			if(cmdId == ZCL_CMD_ONOFF_ON || cmdId == ZCL_CMD_ONOFF_OFF)
			{
				rd_sw_onoff(cmdId, pAddrInfo->dstEp - RD_ENDPOINT_MILITONE);
			}
			break;
		}
		default:
			break;
	}
	return 0;
}

const zclAttrInfo_t rd_sw_attrTbl[] =
{
		{ZCL_ATTRID_ONOFF,           ZCL_DATA_TYPE_BOOLEAN,ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&rd_output[0].stt },
		{ZCL_ATTRID_START_UP_ONOFF,  ZCL_DATA_TYPE_ENUM8,ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&rd_output[0].mode_start},

		{ZCL_ATTRID_ONOFF,           ZCL_DATA_TYPE_BOOLEAN,ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&rd_output[1].stt },
		{ZCL_ATTRID_START_UP_ONOFF,  ZCL_DATA_TYPE_ENUM8,ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&rd_output[1].mode_start},

		{ZCL_ATTRID_ONOFF,           ZCL_DATA_TYPE_BOOLEAN,ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&rd_output[2].stt },
		{ZCL_ATTRID_START_UP_ONOFF,  ZCL_DATA_TYPE_ENUM8,ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&rd_output[2].mode_start},

//		{ZCL_ATTRID_ONOFF,           ZCL_DATA_TYPE_BOOLEAN,ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&rd_output[3].stt },
//		{ZCL_ATTRID_START_UP_ONOFF,  ZCL_DATA_TYPE_ENUM8,ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&rd_output[3].mode_start},
//
//		{ZCL_ATTRID_ONOFF,           ZCL_DATA_TYPE_BOOLEAN,ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&rd_output[4].stt },
//		{ZCL_ATTRID_START_UP_ONOFF,  ZCL_DATA_TYPE_ENUM8,ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&rd_output[4].mode_start}
};

const zcl_specClusterInfo_t g_rd_sw_ClusterList[NUM_OUTPUT_MAX] =
{
	#ifdef ZCL_ON_OFF
		{ZCL_CLUSTER_GEN_ON_OFF,MANUFACTURER_CODE_NONE,ZCL_RD_SW_ATTR_NUM,	&rd_sw_attrTbl[0],	zcl_rd_sw_register,	zcl_rd_sw_Cb},
		{ZCL_CLUSTER_GEN_ON_OFF,MANUFACTURER_CODE_NONE,ZCL_RD_SW_ATTR_NUM,	&rd_sw_attrTbl[2],	zcl_rd_sw_register,	zcl_rd_sw_Cb},
		{ZCL_CLUSTER_GEN_ON_OFF,MANUFACTURER_CODE_NONE,ZCL_RD_SW_ATTR_NUM,	&rd_sw_attrTbl[4],	zcl_rd_sw_register,	zcl_rd_sw_Cb},
//		{ZCL_CLUSTER_GEN_ON_OFF,MANUFACTURER_CODE_NONE,ZCL_RD_SW_ATTR_NUM,	&rd_sw_attrTbl[6],	zcl_rd_sw_register,	zcl_rd_sw_Cb},
//		{ZCL_CLUSTER_GEN_ON_OFF,MANUFACTURER_CODE_NONE,ZCL_RD_SW_ATTR_NUM,	&rd_sw_attrTbl[8],	zcl_rd_sw_register,	zcl_rd_sw_Cb},
	#endif
};

void rd_sw_init()
{
	for(u8 i =0; i< NUM_OUTPUT_MAX; i++)
	{
		af_endpointRegister(RD_ENDPOINT_1 + i, (af_simple_descriptor_t *)&rd_sw_simpleDesc[i], zcl_rx_handler, NULL);
		zcl_register(RD_ENDPOINT_1 + i, RD_SW_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)&g_rd_sw_ClusterList[i]);
		u8 reportableChange = 0x00;
		bdb_defaultReportingCfg(RD_ENDPOINT_1+i, HA_PROFILE_ID, ZCL_CLUSTER_GEN_ON_OFF, ZCL_ATTRID_ONOFF,
									0x0000, 0x003c, (u8 *)&reportableChange);
	}
}

void rd_save_stt()
{
	if(!start_save && zb_isDeviceJoinedNwk())
	{
		start_save = 1;
		tick_save = clock_time();
	}
}

nv_sts_t rd_nvs_save()
{
	nv_sts_t st = NV_SUCC;
	output_t rd_output_cur[NUM_OUTPUT_MAX] ;
	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_RD_OUTPUT, sizeof(rd_output_cur), (u8*)rd_output_cur);
	if(st == NV_SUCC)
	{
		if(memcmp((void *)rd_output_cur, (void*)rd_output,sizeof(rd_output)) != 0)
		{
			st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_RD_OUTPUT, sizeof(rd_output), (u8*)rd_output);
		}
	}
	else if(st == NV_ITEM_NOT_FOUND)
	{
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_RD_OUTPUT, sizeof(rd_output), (u8*)rd_output);
	}
	if(st == NV_SUCC)
	{
		rd_log_uart("rd save success\n");
	}
	return st;
}

void rd_save_default()
{
	memset((void *)rd_output, 0 ,sizeof(rd_output));
	rd_nvs_save();
}

void rd_init_output()
{
	for(u8 i=0; i< NUM_OUTPUT_MAX; i++)
	{
		if(rd_output[i].mode_start == MODE_START_OFF || rd_output[i].mode_start == MODE_START_ON)
		{
			rd_output[i].stt = (rd_output[i].mode_start == MODE_START_OFF) ? 0: 1;
		}
//		drv_gpio_write(led_out[i], !rd_output[i].stt);
		init_led_data();
	}

}

nv_sts_t rd_output_restore(void)
{
	nv_sts_t st = NV_SUCC;
	output_t rd_output_cur[NUM_OUTPUT_MAX] ;
	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_RD_OUTPUT, sizeof(rd_output_cur), (u8*)rd_output_cur);
	if(st == NV_SUCC)
	{
		memcpy((void *)rd_output, (void*)rd_output_cur,sizeof(rd_output));
//		rd_log_uart("rd init on/off: %d, start: %d\n",rd_output[0].stt,rd_output[0].mode_start);
	}
	else
	{
		rd_save_default();
	}
	rd_init_output();

	return st;
}

void rd_process_save_stt_out()
{
	if(start_save && clock_time_exceed(tick_save,3 * 1000 *1000) && zb_isDeviceJoinedNwk())
	{
		rd_nvs_save();
		start_save = 0;
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
//				drv_gpio_write(led_out[idx], rd_lightcTx[idx].oriSta);
				ctrl_led(idx,rd_lightcTx[idx].oriSta);
				rd_lightcTx[idx].timerLedEvt = NULL;
				return -1;
			}
		}
	}

	rd_lightcTx[idx].sta = !rd_lightcTx[idx].sta;
//	drv_gpio_write(led_out[idx], rd_lightcTx[idx].sta);
	ctrl_led(idx,rd_lightcTx[idx].sta);

	return 0;
}

//u8 list_led[] = {0,1,2,3,4};
u8 list_led[] = {0,1,2};
void rd_light_blink(u8 times, u8 time_delay_100ms, u8 idx)
{
	rd_lightcTx[idx].oriSta = rd_output[idx].stt;
	rd_lightcTx[idx].times = times;
	if(!rd_lightcTx[idx].timerLedEvt)
	{
		rd_lightcTx[idx].sta = ! rd_lightcTx[idx].oriSta;
//		drv_gpio_write(led_out[idx], rd_lightcTx[idx].sta);
		ctrl_led(idx,rd_lightcTx[idx].sta);
		rd_lightcTx[idx].timerLedEvt = TL_ZB_TIMER_SCHEDULE(rd_light_blink_TimerEvtCb, (void *)&list_led[idx], 100 * time_delay_100ms);
	}

}
