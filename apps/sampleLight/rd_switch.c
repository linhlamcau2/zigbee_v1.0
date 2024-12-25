#include "tl_common.h"
#include "zcl_include.h"
#include "sampleLight.h"

#include "../common/rd_log/rd_log.h"


const u16 rd_sw_inClusterList[] =
{
	ZCL_CLUSTER_GEN_BASIC,
	ZCL_CLUSTER_GEN_IDENTIFY,
#ifdef ZCL_GROUP
	ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_SCENE
	ZCL_CLUSTER_GEN_SCENES,
#endif
#ifdef ZCL_ON_OFF
	ZCL_CLUSTER_GEN_ON_OFF,
#endif
#ifdef ZCL_ZLL_COMMISSIONING
	ZCL_CLUSTER_TOUCHLINK_COMMISSIONING,
};

const u16 rd_sw_outClusterList[] =
{
#ifdef ZCL_OTA
    ZCL_CLUSTER_OTA,
#endif
};

#define RD_SW_IN_CLUSTER_NUM		(sizeof(rd_sw_inClusterList)/sizeof(rd_sw_inClusterList[0]))
#define RD_SW_OUT_CLUSTER_NUM		(sizeof(rd_sw_outClusterList)/sizeof(rd_sw_outClusterList[0]))

const af_simple_descriptor_t rd_sw_simpleDesc =
{
		HA_PROFILE_ID,                      		/* Application profile identifier */
		HA_DEV_ONOFF_OUTPUT,
		RD_SWITCH_ENDPOINT,              		/* Endpoint */
		1,                                  		/* Application device version */
		0,											/* Reserved */
		RD_SW_IN_CLUSTER_NUM,           		/* Application input cluster count */
		RD_SW_OUT_CLUSTER_NUM,          		/* Application output cluster count */
		(u16 *)rd_sw_inClusterList,    			/* Application input cluster list */
		(u16 *)rd_sw_outClusterList,
};

const zcl_specClusterInfo_t g_rd_sw_ClusterList[] =
{
		{ZCL_CLUSTER_GEN_BASIC,				 MANUFACTURER_CODE_NONE, 	0, 	NULL,  		NULL,			 NULL},
		{ZCL_CLUSTER_GEN_IDENTIFY,			 MANUFACTURER_CODE_NONE, 	0,	NULL,		NULL,		 NULL},
	#ifdef ZCL_GROUP
		{ZCL_CLUSTER_GEN_GROUPS,			 MANUFACTURER_CODE_NONE, 	0, 	NULL,  		NULL,			 NULL},
	#endif
	#ifdef ZCL_SCENE
		{ZCL_CLUSTER_GEN_SCENES,			 MANUFACTURER_CODE_NONE, 	0,		NULL,		NULL,			 NULL},
	#endif
	#ifdef ZCL_ON_OFF
		{ZCL_CLUSTER_GEN_ON_OFF,			 MANUFACTURER_CODE_NONE, 	0,		NULL,			zcl_rd_sw_register,			 NULL},
	#endif
};

u8 RD_SW_CB_CLUSTER_NUM = (sizeof(g_rd_sw_ClusterList)/sizeof(g_rd_sw_ClusterList[0]));

_CODE_ZCL_ static status_t zcl_rd_sw_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_gp_clientCmdHandler(pInMsg);
	}else{
//		return zcl_gp_serverCmdHandler(pInMsg);
	}
}

_CODE_ZCL_ status_t zcl_rd_sw_register(u8 endpoint, u16 manuCode, u8 arrtNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ON_OFF, manuCode, arrtNum, attrTbl, zcl_rd_sw_cmdHandler, cb);
}
