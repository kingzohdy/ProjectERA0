
#ifndef ZONE_CLT_H
#define ZONE_CLT_H

#include "zone_svr.h"

#define MAX_SPANSVR_BROADCAST_GROUP 5

typedef struct
{
	TFRAMEHEAD stFrameHeadTemp;
	int iConnID;
}SpanSvrBroadcastInfo;

typedef struct
{
	int iNum;
	SpanSvrBroadcastInfo astGroupInfo[MAX_SPANSVR_BROADCAST_GROUP];
}SpanSvrBroadcastGroup;


#define Z_CSHEAD_INIT(pstHead, unCmd) {(pstHead)->Cmd = unCmd;(pstHead)->Magic = PROTOCOL_MAGIC;(pstHead)->Ver=TDR_METALIB_PROTO_CS_VERSION;}

int z_clt_msg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, char *pszMsg, int iMsgLen, TFRAMEHEAD *pstFrameHead);


//int z_cltmsg_send(ZONESVRENV *pstEnv, Player *pstPlayer,  CSPKG *pstCSPkgRes);

int  z_cltmsg_send_trace(ZONESVRENV *pstEnv, Player *pstPlayer, CSPKG *pstCSPkgRes, const char *pszFile, int iLine);
#define z_cltmsg_send(Env, Player, CSPkg) \
	z_cltmsg_send_trace(Env, Player, CSPkg, __FILE__, __LINE__)

int z_cltmsg_send_stop(ZONESVRENV *pstEnv, Player *pstPlayer,  CSPKG *pstCSPkgRes);
int z_cltmsg_broadcast(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, CSPKG *pstCSPkgRes);
int z_cltmsg_broadcast_except(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, CSPKG *pstCSPkgRes, Player *pstPlayerExcept);
int z_cltmsg_limit_broadcast(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, Player **apstPlayer, int iNum, CSPKG *pstCSPkgRes);
int z_send_sysmsg(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, int iSysType, int iErrno, ...);
int z_send_sys_str(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, int iSysType, char *pszStr);
int z_send_sys_str_mud(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, char *pszName, int iSysType, int iErrno, ...);
int z_sys_strf( ZONESVRENV *pstEnv, Player *pstPlayer, int iSysType, const char *pszStr, ...);
int z_ssmsg_send(ZONESVRENV *pstEnv, SSPKG *pstSSPkg, int iDstID);

int z_team_sys_strf(ZONESVRENV *pstEnv, ZoneTeam *pstTeam, int iSysType, int iErrno, ...);
int z_sys_strf_broadcast(ZONESVRENV *pstEnv, int iSysType, int iErrno, ...);

int z_send_to_region_msg(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, int iCmd,char *szData,int iLen);
int z_cltmsg_broadcast_condition(ZONESVRENV *pstEnv, CSPKG *pstCSPkgRes, int (*condition)(ZONESVRENV *,Player *,void *));

/**
 * iConnIdx [in]: 连接索引
 * iNewID [in]: 新ID,set routing成功后连接索引将和这个ID绑定
 * iZoneSvrBusID [in]: 源始zone_svr的busid
 * iZoneConnBusID [in]: 这个bus将会被替换成本进程的bus
 * pvExtraData [in]: 额外数据,可为NULL
 * iExtraDataLen [in]: 额外数据长度,可为0
 * 执行此函数后,将会回调z_set_routing_res
 */
int z_set_routing_req(ZONESVRENV* pstEnv, int iConnIdx, int iNewID,
                      int iZoneSvrBusID, int iZoneConnBusID,int iBattleSvrBusID,
                      const void *pvExtraData, int iExtraDataLen);

int z_set_limits_req(ZONESVRENV* pstEnv, int iZoneConnBusID, int fEnabled,
                     int iParam);

int z_set_routing_res(ZONESVRENV* pstEnv, const TFRAMEHEAD *pstRes);

int battle_proxy_request(ZONESVRENV* pstEnv, Player *pstPlayer, CSPKG *pstPkg);

int battle_proxy_respond(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                         SSBATTLEPROXY *pstRes);

int battle_mirror_sync_request(ZONESVRENV* pstEnv, int iBusID, int iCmd,
                               const void *pstRawData, int iDataLen);

int battle_mirror_sync_respond(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
                               int iMsgSrc, SSBATTLEMIRRORSYNC *pstRes);

int normal_battle_camp_broadcast(ZONESVRENV *pstEnv, Player *pstPlayer, CSPKG *pstPkg);

int z_pkg_to_mud(ZONESVRENV *pstEnv,  CSPKG *pstPkg,char *pszName,uint iClanID,int iTransType);
int z_cmd_to_mud(ZONESVRENV *pstEnv,char *pszReceiver,char *pszName,int iCmd, SSCMDDATA* pstData);

int normal_battle_camp_broadcast_except(ZONESVRENV *pstEnv, Player *pstPlayer, CSPKG *pstPkg, Player *pstExcept);
int z_info_get_to_mud(ZONESVRENV *pstEnv,SSINFOGETREQ* pstReq, int iType);

int zone_proxy_respond(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                         SSBATTLEPROXY *pstRes);
int zone_proxy_request(ZONESVRENV* pstEnv, Player *pstPlayer, CSPKG *pstPkg);
int z_get_player_connectid(ZONESVRENV *pstEnv,Player *pstPlayer);

int z_sys_strf_map_broadcast(ZONESVRENV *pstEnv, ZONEMAPINST * pstMapInst, int iSysType, int iErrno, ...);

int zone_mirror_sync_respond(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
                               int iMsgSrc, SSBATTLEMIRRORSYNC *pstRes);

int z_send_sys_str_msg(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, int iSysType, int iErrno, ...);

int z_sys_strf_span_broadcast(ZONESVRENV *pstEnv, int iBusID, int iSysType, char *pszStr, ...);

int zone_proxy_sparta_request(ZONESVRENV* pstEnv, Player *pstPlayer, CSPKG *pstPkg);

int z_team_sys_strf2(ZONESVRENV *pstEnv, ZoneTeam *pstTeam, int iSysType, char *pszStr, ...);
int z_pkg_all_world(ZONESVRENV *pstEnv,CSPKG *pstPkg);

int z_sys_strf_span_broadcast_clan(ZONESVRENV *pstEnv, int iBusID, int iClanID,int iSysType, char *pszStr, ...);
int z_pkg_span_clan(ZONESVRENV *pstEnv,CSPKG *pstPkg, int iBusID, int iClanID);
#endif /* ZONE_CLT_H */
