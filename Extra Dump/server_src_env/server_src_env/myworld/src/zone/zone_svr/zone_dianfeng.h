

#ifndef ZONE_DIANFENG_H
#define ZONE_DIANFENG_H

#include "zone_svr.h"

extern int g_EnterDianFeng;

int player_dianfeng_baoming_check(ZONESVRENV* pstEnv, Player *pstPlayer,DIANFENGGLOBAL *pstDianFengGlobal,DIANFENGINFO *pstDianFengInfo);
int player_dianfeng_baoming_data_new(ZONESVRENV* pstEnv, Player *pstPlayer,DIANFENGJOININFO *pstDianFengJoinInfo,unsigned int uiCID);
int player_dianfeng_baoming(ZONESVRENV* pstEnv, Player *pstPlayer,CSDIANFENGBAOMING *pstCSDianFengBaoMing);
int player_dianfeng_transmit_check(ZONESVRENV* pstEnv, Player *pstPlayer,DIANFENGINFO *pstDianFengInfo);
int player_dianfeng_transmit(ZONESVRENV* pstEnv, Player *pstPlayer,CSDIANFENGTRANSMIT *pstCSDianFengTransmit);
int player_support_val_chg(ZONESVRENV *pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo,tdr_ulonglong RoleID);
int player_dianfeng_guessinfos_and_support_val_chg(ZONESVRENV* pstEnv, Player *pstPlayer,GUESSINFOS *pstGuessInfos,DIANFENGGLOBAL *pstDianFengGlobal);
int player_dianfeng_guess_check(ZONESVRENV* pstEnv, Player *pstPlayer,GUESSINFOS *pstGuessInfos,DIANFENGGLOBAL *pstDianFengGlobal);
int player_dianfeng_guess(ZONESVRENV* pstEnv, Player *pstPlayer,GUESSINFOS *pstGuessInfos);
int player_moshi_dianfeng_join_award(ZONESVRENV *pstEnv,char *pName,int iNum,tdr_ulonglong iMoney);
int player_moshi_dianfeng_first_award(ZONESVRENV *pstEnv,char *pName,int iNum,tdr_ulonglong iMoney);
int player_clt_need_which_one_group_info(ZONESVRENV *pstEnv,Player *pstPlayer,CSDIANFENGGROUPINFOC *pstCSDianFengGroupInfoC);
int player_group_info(ZONESVRENV *pstEnv,Player *pstPlayer,DIANFENGJOININFO *pstDianFengJoinInfo,int iWitchOne);
int player_group_info_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,int iWhichOne,int iDFversion,DFGROUPINFO *pstDFGroup);
int player_win_loss_ui_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,DIANFENGWINLOSSUI *pstDianFengWinLoss);
int player_transmit_res_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,int iRes);
int player_baoming_res_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,int iRes);
int player_guess_res_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,int iRes,GUESSINFOS stGuessInfos,DIANFENGGLOBAL *pstDianFengGlobal);
int player_pipei_succeed_ui_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,int res);
int player_first_or_effigy_info_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,int type);
int dianfeng_set_open(ZONESVRENV* pstEnv, time_t tTime);
time_t time_to_mon(time_t t1);
int player_dianfeng_tick(ZONESVRENV* pstEnv);
int dianfeng_clean_some_data(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal,int type);
int dianfeng_one_group_first_judge(ZONESVRENV *pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo,int iWhichJob);
int dianfeng_group_first_if_all_produce(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal);
int dianfeng_close_tick(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal);
int player_dianfeng_no_ready_del_group(ZONESVRENV *pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo);
int player_dianfeng_no_reday_del(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal);
int player_dianfeng_pipei(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal);
int all_login_player_dianfeng_baoming_dlg(ZONESVRENV *pstEnv);
int all_login_player_dianfeng_baoming_msg(ZONESVRENV *pstEnv);
int all_player_dianfeng_transmit_dlg(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal);
int player_dianfeng_join_transmit_dlg(ZONESVRENV *pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo);
int player_dianfeng_login_transmit_dlg(ZONESVRENV *pstEnv,Player *pstPlayer,DIANFENGGLOBAL *pstDianFengGlobal);
int player_dianfeng_login_check(ZONESVRENV* pstEnv,Player *pstPlayer);
int dianfeng_effigy_set(ZONESVRENV *pstEnv, Player *pstPlayer, int iMap, RESPOS *pstPos,int WhichOne);
int player_dianfeng_effigy_set_data(ZONESVRENV *pstEnv, Player *pstPlayer,DFFIRSTINFO *pstDFEffigyInfo,DFFIRSTINFO *pstCSDFEffigyInfo,int iMap, RESPOS *pstPos);
int dianfeng_ret_guess_win_num(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal);
int player_dianfeng_send_mail_award(ZONESVRENV *pstEnv,int num,tdr_ulonglong ullMoney,DIANFENGGLOBAL *pstDianFengGlobal);
int player_dianfeng_group_pipei(ZONESVRENV *pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo);
int dianfeng_enter_pworld_tick(ZONESVRENV *pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo);
int player_pipei_bad(DIANFENGJOININFO *pstDianFengJoinInfo,DIANFENGPIPEI *pstDianFengPiPei,int k);
int player_dianfeng_curr_map_same_as_reday_map(ZONESVRENV *pstEnv,Player *pstMemberPlayer);
int dianfeng_get_map_pos(ZONESVRENV* pstEnv,  MapIndex *pstMapIdx,
							RESPOS *pstPos, Player *pstPlayer);
int player_moshi_dianfeng_money_pool_award(ZONESVRENV *pstEnv,char *pName,tdr_ulonglong iMoney);

int	dianfeng_champion_record(ZONESVRENV *pstEnv,char *pName,int iWhichJob);
int dianfeng_group_champion_record(ZONESVRENV *pstEnv,char *pName,DFFIRSTINFO *pstDFChampionInfo);

int player_login_effigy_send_to_clt(ZONESVRENV* pstEnv,Player *pstPlayer,DIANFENGGLOBAL *pstDianFengGlobal);
int dianfeng_effigy_login_set(ZONESVRENV *pstEnv,DFFIRSTINFO *pstDFEffigyInfo,int iWhichJob);
int dianfeng_player_pworld_die_dispose(ZONESVRENV* pstEnv,Player *pstPlayer);
int dianfeng_red_pk_info(ZONESVRENV* pstEnv,PIPEIINFOS *pstPiPei,int iRound,int *iPingNum,int *iRedWinNum,int *iRedLossNum);
int dianfeng_if_pk_win(ZONESVRENV* pstEnv,PIPEIINFOS *pstPiPei,int iRound);
int dianfeng_pk_end_dispose(ZONESVRENV* pstEnv,PIPEIINFOS *pstPiPei,Player *pstRedPlayer,Player *pstBulePlayer,int iRound,int k,ZONEPWORLDINST *pstPworldInst,ZONEPWORLD *pstPworld,int whichJob);
int z_dianfeng_change_map(ZONESVRENV* pstEnv, Player *pstPlayer,int iMapIndex, RESPOS stResPos,int Type);
int player_career_pipei_del_baoming_add(ZONESVRENV* pstEnv,char *pName,int k,int whichJob);
int z_dianfeng_pworld_down_tm_end(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPworldInst,ZONEPWORLD *pstPworld);
int dianfeng_get_joininfo_by_career(ZONESVRENV* pstEnv,DIANFENGJOININFO **ppstDianFengJoin,DIANFENGGLOBAL *pstDianFengGlobal,int career);
int dianfeng_pworld_tick(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst, int iSec);
int player_login_baoming_pipei_queue_check(ZONESVRENV* pstEnv, Player *pstPlayer,DIANFENGGLOBAL *pstDianFengGlobal);
int dianfeng_get_info_by_wid(ZONESVRENV* pstEnv,int *index,DIANFENGJOININFO *pstDianFengJoin,ZONEPWORLDINST *pstPworldInst);
int dianfeng_by_wid_get_pipei(ZONESVRENV* pstEnv,PIPEIINFOS **ppstPiPei,DIANFENGGLOBAL *pstDianFengGlobal,ZONEPWORLDINST *pstPworldInst,int *iCarceer,int *iWhere);
int dianfeng_get_ready_map_by_career(ZONESVRENV* pstEnv,int career,int *iMapIndex);
int player_dianfeng_if_chamption(ZONESVRENV* pstEnv,DIANFENGGLOBAL *pstDianFengGlobal,Player *pstPlayer);
int dianfeng_update_name(ZONESVRENV* pstEnv, const char* pszOldName, const char* pszNewName);
int dianfeng_chg_baoming_and_pipei_name(ZONESVRENV* pstEnv, const char* pszOldName, const char* pszNewName,DIANFENGJOININFO *pstDianFengJoinInfo);
int dianfeng_chg_champion_and_effigy_name(ZONESVRENV* pstEnv, const char* pszOldName, const char* pszNewName,DFFIRST *pstDFFirst);
int dianfeng_give_award(ZONESVRENV* pstEnv,int iAwardType,Player *pstPlayer);
int player_dianfeng_if_guess(ZONESVRENV* pstEnv,Player *pstPlayer, CSDIANFENGIFGUESS *pstCSDianFengIfGuess);
int dianfeng_add_ready_buff_and_effect(ZONESVRENV* pstEnv,Player *pstRedPlayer,Player *pstBulePlayer,int iRound);
int dianfeng_bofang_shengli_effect(ZONESVRENV* pstEnv,Player *pstWinPlayer);
int dianfeng_bofang_loss_effect(ZONESVRENV* pstEnv,Player *pstLossPlayer);
int dianfeng_get_dir(ZONESVRENV* pstEnv,Player *pstPlayer,int *iDir);
int dianfeng_gm_clean(ZONESVRENV* pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo);
int dianfeng_chamption_clear(ZONESVRENV* pstEnv,DFFIRSTINFO *pstDFChampionInfo);
int dianfeng_effigy_clear(ZONESVRENV* pstEnv,DFFIRSTINFO *pstDFEffigyInfo);
int player_dianfeng_guess_mingdan_mail(ZONESVRENV *pstEnv,CSDIANFENGGUESSMINGDAN *pstCSDianFengGuessMingDan);
int dianfeng_guess_mingdan_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,CSDIANFENGGUESSMINGDAN *pstGuessMingDan);
int dianfeng_guess_mingdan(ZONESVRENV* pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo,tdr_ulonglong RoleID);
int dianfeng_get_guess_mingdan(ZONESVRENV* pstEnv,Player *pstPlayer,DIANFENGGLOBAL *pstDianFengGlobal,int iType);
int dianfeng_clear(ZONESVRENV* pstEnv,int iflag);
int player_dianfeng_if_in_baoming_pipei(ZONESVRENV* pstEnv,char *pName,DIANFENGJOININFO *pstDianFengJoinInfo);
int dianfeng_yanchi_dispose_tm(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPworldInst);
int dianfeng_yanchi_group_tick(ZONESVRENV* pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo,int iCarceer);
int dianfeng_yanchi_tick(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal);
int dianfeng_revive_sth(ZONESVRENV *pstEnv,Player *pstPlayer);
int dianfeng_is_first_msg(ZONESVRENV *pstEnv,int iWhichJob,DIANFENGGLOBAL *pstDianFengGlobal);
int dianfeng_chamption_clan_info_update(ZONESVRENV *pstEnv,Player *pstPlayer,unsigned int ClanId);
int dianfeng_guang_bo(ZONESVRENV *pstEnv,Player *pstPlayer);
int all_player_dianfeng_baoming_and_transmit_dlg_close(ZONESVRENV *pstEnv);
int  if_player_has_dingshen_status(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_leave_dianfeng_repose(ZONESVRENV* pstEnv, Player *pstPlayer);
int dianfeng_check_tick(ZONESVRENV* pstEnv,DIANFENGGLOBAL *pstDianFengGlobal);
int dianfeng_group_check_tick(ZONESVRENV* pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo,int iMap);
int dianfeng_restore_status(ZONESVRENV* pstEnv,ZONEPWORLD *pstPworld,Player *pstPlayer);
int dianfeng_clear_champion_by_career(ZONESVRENV* pstEnv,Player *pstPlayer,int Career);




#endif

























