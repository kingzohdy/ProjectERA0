#ifndef MUD_CLT_H
#define MUD_CLT_H

#define Z_CSHEAD_INIT(pstHead, unCmd) \
do{ \
	(pstHead)->Cmd = unCmd; \
 	(pstHead)->Magic = PROTOCOL_MAGIC; \
 	(pstHead)->Ver = TDR_METALIB_PROTO_CS_VERSION; \
}while(0)

#ifndef WIN32
#define max(a, b)	((a) > (b) ? (a) : (b))
#define min(a, b)	((a) < (b) ? (a) : (b))
#endif

#define Z_SSHEAD_INIT(pstHead, unCmd) {(pstHead)->Cmd = unCmd;(pstHead)->Magic = PROTOCOL_MAGIC;}

int mud_cltmsg_send(MudPlayer *pstPlayer, const CSPKG *pstCSPkgRes);

int mud_cltmsg_send_stop(MudPlayer *pstPlayer,  CSPKG *pstCSPkgRes);

int mud_dispatch_clt_msg(MUD_SVRENV *pstEnv, char *pszMsg, int iMsgLen,
                         TFRAMEHEAD *pstFrameHead);

int mud_send_role_list(MudPlayer *pstPlayer, const MudUinCache *pstRoleList);

int mud_cltmsg_broadcast(CSPKG *pstCSPkgRes);

int mud_cltmsg_broadcast_except(CSPKG *pstCSPkgRes, MudPlayer *pstPlayerExcept);

int mud_cltmsg_limit_broadcast(MudPlayer **apstPlayer, int iNum, CSPKG *pstCSPkgRes);

int mud_room_broadcast(ChatRoomInfo *pstChatRomm, CSPKG *pstCSPkgRes);

int mud_room_broadcast_except(ChatRoomInfo *pstChatRomm, CSPKG *pstCSPkgRes, MudPlayer *pstPlayerExcept);

// 系统消息,类似printf
int mud_sys_strf(MudPlayer *pstPlayer, int iType, const char *pszFormat, ...);

int mud_ssmsg_send(SSPKG *pstSSPkg, int iDstID);

// 广播至指定公会
int mud_cltmsg_broadcast_clan(const CSPKG *pstPkg, const CLANID *pstClanID);

int mud_enter_chat_room_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int mud_out_chat_room_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int mud_chat_room_list_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int mud_chat_channel_set_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int mud_offline_exp_take_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int watch_player_clt(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int m_pkg_to_zone(CSPKG *pstPkg,char *pszName,int iTransType,unsigned int uiClanID);
int m_relation_clt(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int m_send_tip_by_name(char *pszMsg, char* pszReceiver,int iOnlineType);
int m_cltmsg_send_by_name(const CSPKG *pstCSPkgRes,char* pszName, int iOnlineType);
int m_cmd_to_zone(char *pszReceiver,char *pszName,int iCmd, SSCMDDATA* pstData);
int m_send_emotion(MudPlayer* pstPlayer);
int m_info_get_to_zone(SSINFOGETREQ* pstReq, int iType);
int m_relation_addfriend_res(MudPlayer* pstPlayer,CSADDFRIENDRES* pstRes, ROLEBUDDYMEMBER *pstBuddyMember);
int m_get_buddy_member(MudPlayer* pstPlayer,ROLEBUDDYMEMBER* pstBuddyMember,BUDDYINFO *pstBuddyInfo);
int m_friend_add(MudPlayer *pstPlayer, ROLEBUDDYMEMBER *pstOtherMember, int iOnlineType);

#endif 

