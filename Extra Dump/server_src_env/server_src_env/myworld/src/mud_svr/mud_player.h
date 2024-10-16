#ifndef MUD_PLAYER_H
#define MUD_PLAYER_H

int mud_player_acc_login_err(const TFRAMEHEAD* pstFrameHead, short nResult);

int mud_player_kickoff_text_notify(MudPlayer *pstPlayer, char *pszMsg);

int mud_player_kickoff(MudPlayer *pstPlayer, char *pszMsg);

int mud_player_disconnect(MudPlayer * pstPlayer);

MudPlayer *mud_player_alloc(int iUin, const char *pszAccount, MudUinCache *pstUinCache);

int mud_player_free(MudPlayer *pstPlayer,char* pszReason);

MudPlayer *mud_player_get_by_uin(int iUin);

LPSHTABLE mud_attach_uincache();

int mud_attach_player();

int UinHashCmp(const void * pv1 , const void *pv2);
unsigned int UinHashCode(const void* pvData);

MudPlayer* z_idx_player(int iIdx);

int mud_player_close_connection(TFRAMEHEAD *pstFrameHead);

int mud_active_player(int iCount);

// 安全地通过名字获取Player
MudPlayer* mud_name_player(const char* pszName);

int mud_role_login_res(MudPlayer *pstPlayer);

int mud_role_logout_res(MudPlayer *pstPlayer);

int mud_region_msg(const char *pszMsg, int iMsgLen);

int mud_zone_msg(const char *pszMsg, int iMsgLen);

/*
 * CS Handlers :
 */
int mud_acc_login_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                      TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int mud_acc_logout_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                       TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int mud_role_login_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                       TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int mud_role_logout_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                       TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int mud_chat_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int mud_player_heartbeat(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int m_player_enter_chat_room(MudPlayer *pstPlayer,unsigned char bRoom);
int m_player_offline_exp_change(MudPlayer *pstPlayer,int iMudExp,int iZoneExp);
int m_is_player_offline_exp_full(MudPlayer *pstPlayer);
int m_player_offline_exp_full(MudPlayer *pstPlayer,int iType);
int m_player_cltmsg_friend(MudPlayer *pstPlayer,CSPKG *pstCSPkgRes);
int m_is_player_chat_channel_silence(MudPlayer *pstPlayer, int iChannel);
int mud_chat_to_zone(MudPlayer *pstPlayer, const CSCHATREQ *pstChat);
int m_player_leave_chat_room_info(MudPlayer *pstPlayer,unsigned char bRoom);


#endif // MUD_PLAYER_H
