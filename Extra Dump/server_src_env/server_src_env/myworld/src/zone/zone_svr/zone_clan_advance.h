#ifndef ZONE_CLAN_ADVANCE_H
#define ZONE_CLAN_ADVANCE_H

#include "zone_machine.h"

extern const ClanMachinePropertyEntry *kEmptyMachineProperty;
extern const CLANGENERALRESULT *kEmptyGeneralResult;

// ��ȡ����Ƽ�������Ϣ
const CLANTECHDEF* clan_tech_def(ZONESVRENV *pstEnv, int iCategory, int iLevel);

// ��ȡ�����Ʒ������Ϣ,Ŀǰ����Ʒ,��е������
CLANPRODUCTDEF* clan_product_def(ZONESVRENV *pstEnv, int iID);

// ����߼���Ϣ�Ŀ���
int player_clan_advance(ZONESVRENV *pstEnv, Player *pstPlayer);

// �з��Ƽ�
int player_clan_research(ZONESVRENV *pstEnv, Player *pstPlayer,
                         const CSCLANRESEARCH *pstReq);

// ȡ���з��Ƽ�
int player_clan_cancel_research(ZONESVRENV *pstEnv, Player *pstPlayer,
                                const CSCLANCANCELRESEARCH *pstReq);

// ������Ʒ/��е
int player_clan_make(ZONESVRENV *pstEnv, Player *pstPlayer,
                     const CSCLANMAKE *pstReq);

// ȡ��������Ʒ/��е
int player_clan_cancel_making(ZONESVRENV *pstEnv, Player *pstPlayer,
                              const CSCLANCANCELMAKING *pstReq);

// �Ӳֿ�����ȡ��е
int player_clan_grab_machine(ZONESVRENV *pstEnv, Player *pstPlayer,
                             const CSCLANGRABMACHINE *pstReq);

// �ٻػ�е�Żزֿ���
int player_clan_callback_machine(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 const CSCLANCALLBACKMACHINE *pstReq);

// �Ի�е���в���
int player_clan_machine_supply(ZONESVRENV *pstEnv, Player *pstPlayer,
                               const CSCLANMACHINESUPPLY *pstReq);

// ����ֿ�������л�
int player_clan_storage_grid_lock_switch(ZONESVRENV *pstEnv, Player *pstPlayer,
                                     const CSCLANSTORAGEGRIDLOCKSWITCH *pstReq);

// ����ֿ�ݻ���Ʒ
int player_clan_storage_grid_destroy(ZONESVRENV *pstEnv, Player *pstPlayer,
		                             const CSCLANSTORAGEGRIDDESTROY *pstReq);

// ����߼����ݳ�ʼ��
void clan_advance_init(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);


// ����߼���������
// ��������ڹ��ᱻɾ����ʱ�����һ��
void clan_advance_cleanup(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);


// ����߼�tick
int clan_advance_tick(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);


// ���乫��ֿ���λ
int clan_extend_storage(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iWhich,
		                int iGridNum);

// ��Ӷ���������ֿ���
int clan_storage_add_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                             int iWhich, int iDefID, int iNum);

// ���ⲿ��Ӷ������ֿ���, ����: ��ɫ�ύ�ĵ���...
int clan_storage_add_entries_external(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iID, int iNum);

// ��չ���ֿ�
int clan_storage_clear(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iWhich);


// �ӹ���ֿ���ɾ������
int clan_storage_del_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                             int iWhich, int iDefID, int iNum);

// ����Ƿ��ܴӹ���ֿ���ɾ��ָ���Ķ���
int clan_storage_check_del_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                   int iWhich, int iID, int iNum);

// ����е���ջع���ֿ�ʱ���ô˺���
int clan_machine_on_callback(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                             Monster *pstMon);

// ����е���ݻ�(ս��...)ʱ���ô˺���
int clan_machine_on_destroy(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                            Monster *pstMon);

// ����е��ˢ��ʱ���ô˺���
int clan_machine_on_update(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                           Monster *pstMon);

// ��ȡ�����е���ݵ�����ֵ
const ClanMachinePropertyEntry* 
clan_machine_cached_effect(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iID);

// ��ȡ����Ƽ����ĵĿƼ�����.
// iTechID�ĺ���:
// > 0: ָ��ĳһ��Ƽ������ĵĿƼ�����.
// = 0: ���пƼ������ĵĿƼ�����.
// < 0: ĳһ��Ƽ������ĵĿƼ�����.
int clan_used_tech_point(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                         int iTechID);

// ��ɫ���������Լ���˵Ļ�е
int player_clan_commit_machine(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 Npc *pstNpc);

// ��ɫ���ſ��Ͻ���ʯ
int player_clan_commit_ore(ZONESVRENV *pstEnv, Player *pstPlayer, Npc *pstNpc, ROLEGRID *pastRoleGrid, int iNum);


// ȷ����ɫ���ڹ����е��
int player_clan_machine_ensure_down(ZONESVRENV *pstEnv, Player *pstPlayer);


// �ӹ����̵����򹫻���Ʒ, �������������������, ����֮
int player_clan_shop_buy(ZONESVRENV *pstEnv, Player *pstPlayer, Npc *pstNpc,
                         const SELLGOODS *pstSellGoods, int iNum);


// ����Id���ҹ�����Ϣ��
extern ZoneClan* z_id_clan(ZONESVRENV *pstEnv, unsigned int nID);


// �з�����
int player_clan_research_speedup(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 const CSCLANRESEARCHSPEEDUP *pstReq);

// �������
int player_clan_making_speedup(ZONESVRENV *pstEnv, Player *pstPlayer,
                               const CSCLANMAKINGSPEEDUP *pstReq);

// ��ɫ���Լ�����������е���Ʒ�ύ������ֿ�.
int player_clan_commit_item(ZONESVRENV *pstEnv, Player *pstPlayer, int iGridIdx);

// ���ⲿ��Ӷ�������Դ������, ������Դ���Ʒ...
int clan_resource_add_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iID, int iNum,int* piRealNum);

// ������Դ���ĸ������л�
int player_clan_resource_lock_switch(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      const CSCLANRESOURCELOCKSWITCH *pstReq);
// ������Դ������Ʒ�һ�
int player_clan_resource_grab_item(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      const CSCLANRESOURCEGRABITEM* pstReq);

// ��ȡ��Դ��Ϣ
CLANRESOURCEINFO* clan_resource_get_info(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iID);

// ������Դ���ĵ���Դ
int clan_resource_dec_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iID, int iNum);

// ��Դ��Ϣ�仯֪ͨ
int clan_resource_notify(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      CLANRESOURCEINFO* pstNotify);

// ������Դ���ĵ���Դ
int clan_resource_dec_entries_in(ZONESVRENV *pstEnv, CLANRESOURCE *pstResource,
                                      int iID, int iNum,CLANRESOURCEINFO* pstNotify);

// �ύ������Դ
int player_clan_commit_item_byid(ZONESVRENV *pstEnv, Player *pstPlayer, int iItemID,int* piNum);

// ����.�ύ������Դ
int clan_commit_item_byid(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iItemID,int iNum);

int player_open_res_info_ui(ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID, int iUIType);
int clan_getres_info_msg(ZONESVRENV* pstEnv, ZoneClan *pstClan, Player *pstPlayer, int iPage);
int player_clan_getres_info(ZONESVRENV *pstEnv, Player *pstPlayer, CSCLANVIEWGETRES *pstReq);

// �з�/�������֪ͨ
int clan_running_progress_notify(ZONESVRENV *pstEnv,
                                        ZoneClan *pstZoneClan,
                                        const CLANRUNNINGPROGRESS *pstProgress,
                                        int iNotify, int iCode);
#endif //  ZONE_CLAN_ADVANCE_H
