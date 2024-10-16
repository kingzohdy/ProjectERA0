#ifndef MUD_DB_H
#define MUD_DB_H

int mud_dispatch_db_msg(MUD_SVRENV *pstEnv, const char *pszMsg, int iMsgLen,int iMsgSrc);

int db_select_role_req(MudPlayer *pstPlayer, tdr_ulonglong ullRoleID);

int db_select_role_list_req(MudPlayer *pstPlayer, int iUin);

int db_select_shadow_req(MudPlayer *pstPlayer, tdr_ulonglong ullRoleID);

int db_update_shadow_req(MudPlayer *pstPlayer, const MudShadowData *pstShadowData);

int db_buddy_select_req(MudPlayer *pstPlayer);
int db_buddy_update_req(MudPlayer* pstPlayer,ROLEDATABUDDY* pstRoleBuddy);


	
int mud_torm_exchg_ver();

#endif // MUD_DB_H