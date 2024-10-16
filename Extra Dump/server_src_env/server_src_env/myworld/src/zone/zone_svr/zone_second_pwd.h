/*
**  @file $RCSfile: zone_second_pwd.h,v $
**  general description of this module
**  $Id: zone_second_pwd.h,v 1.8 2014/03/12 06:02:54 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2014/03/12 06:02:54 $
**  @version $Revision: 1.8 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#ifndef ZONE_SECOND_PWD_H
#define ZONE_SECOND_PWD_H

#include "zone_svr.h"

int player_secondpwd_set(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer,SECONDPWDCLT *pstSecondPwdClt);
int player_secondpwd_input(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer,SECONDPWDCLT *pstSecondPwdClt);
int player_secondpwd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int player_second_pwd_info_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag, int Result);
int player_send_to_clt_popup_dialog(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer,tdr_ulonglong iResId);


#endif
