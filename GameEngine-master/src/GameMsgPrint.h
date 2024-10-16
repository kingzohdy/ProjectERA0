#pragma once
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include <list>

//tolua_begin
enum EMID_MESSAGE
{
	MID_NPCSHOP_MONEYNOTENOUGH_SELLFAIL = 1,		// ���Ͻ�Ǯ����,����ʧ��!
	MID_NPCSHOP_BULLNOTENOUGH_SELLFAIL,				// ���ϲ�������,����ʧ��!
	MID_NPCSHOP_EXCHANGEITEMNOTENOUGH_SELLFAIL,		// �һ������Ӳ���,����ʧ��!
	MID_NPCSHOP_MONEYNOTENOUGH_BUYSAVEFAIL,			// ���Ͻ�Ǯ����,�ع�ʧ��!	
	MID_NPCSHOP_BULLNOTENOUGH_BUYSAVEFAIL,			// ���ϲ�������,�ع�ʧ��!
	MID_NPCSHOP_BULLNOTENOUGH_REPAIRFAIL,			// ���ϲ�������,ά��ʧ��!
	MID_NPCSHOP_NOTNEEDREPAIR,						// ����Ҫ����
	
	MID_ARMPROCE_ARMSTAR,							// װ��Ʒ������
	MID_ARMPROCE_LEVEL,								// װ���ȼ�����
	MID_ARMPROCE_FIGHT,								// װ��ս��������
	MID_ARMPROCE_IDENTIFIABILITY,					// װ�������ɹ�
	MID_ARMPROCE_GEMLEVELUP,						// ��ʯ�����ɹ�
	MID_ARMPROCE_GEMINLAY,							// ��ʯ��Ƕ�ɹ�
	MID_ARMPROCE_GEMPICK,							// ��ʯժȡ�ɹ�
	MID_ARMPROCE_GEMPICK_FAIL,						// ��ʯժȡʧ��
	MID_ARMPROCE_GEMINLAY_FAIL,						// ��ʯ��Ƕʧ��
	MID_ARMPROCE_GEMLEVELUP_FAIL,					// ��ʯ����ʧ��
	MID_ARMPROCE_IDENTIFIABILITY_FAIL,				// װ������ʧ��
	MID_ARMPROCE_ARMSTAR_FAIL,						// װ��Ʒ������ʧ��
	MID_ARMPROCE_LEVEL_FAIL,						// װ���ȼ�����ʧ��
	MID_ARMPROCE_FIGHT_FAIL,						// װ��ս��������ʧ��
	MID_ARMPROCE_NOTGEMEFFECTINLAY,					// ������Ƕ��ʯ
	MID_ARMPROCE_NOTSAMEGEMTYPE,					// ��ʯ���Ͳ�һ��
	MID_ARMPROCE_NOTSAMELEVEL,						// ��ʯ�ȼ���һ��
	MID_ARMPROCE_NOTGEM,							// ���Ǳ�ʯ���ܷ���
	MID_ARMPROCE_NOTINLAYGEM,						// ����Ƕ�౦ʯ��������
	MID_ARMPROCE_NOTEQUIP,							// ���ȷ���װ��
	MID_ARMPROCE_SAMEGEMTYPEINLAY,					// ����Ƕ�б�����ͬ���ͱ�ʯ��

	MID_ARMPROCE_NOTGEMPICKITEM,					// û��ժȡ��ʯ����Ҫ��Ʒ
	MID_ARMPROCE_NOUPDATEGEM,						// ����Ҫ�����ı�ʯ
	MID_ARMPROCE_NOGEMUPDATEITEM,					// û��������ʯ����Ҫ��Ʒ
	MID_ARMPROCE_NOTEQUIPITEM,						// �������Ʒ����װ��
	MID_ARMPROCE_NOTHOLDEQUIP,						// �����װ��û�д���
	MID_ARMPROCET_NOTARMSAFEITEM,					// ������û��Ʒ���������շ�
	MID_ARMPROCET_FIGHTSAFEITEM,					// ������û��ս�����������շ�
	MID_ARMPROCET_GEMSAFEITEM,						// ������û����Ƕ��
	MID_ARMPROCET_FIGHTLIMIT,						// װ��ս�����ȼ����ɸߵ���װ��Ʒ��
	MID_ARMPROCET_NOTGEMPICK,						// ȱ�ٶ�Ӧ�ı�ʯժȡ��
	MID_ARMPROCET_NOTSTARSTUFF,						// ����Ĳ�����Ʒ�ʵĲ���
	MID_ARMPROCET_NOTFIGHTSTUFF,					// ����Ĳ�����ս�����Ĳ���
	MID_ARMPROCET_NOTEQUIPLEVELSTUFF,				// ����Ĳ�����װ���ȼ��Ĳ���
	MID_ARMPROCET_NOTHOLEINLAY,						// ��װ�����޿���Ƕ�Ŀ�
	MID_ARMPROCET_NOTHOLEEQUIP,						// ��װ�������п�װ��
	MID_ARMPROCET_HOLEHASGEM,						// �ÿ�����Ƕ��ʯ
	MID_ARMPROCET_HOLECOLORDIFWITHGEM,				// �ÿ���ɫ����ʯ��ɫ��ƥ��
	MID_ARMPROCET_INLAY_LEVELLIMIT,					// ��ʯ�ȼ���װ���ȼ�����
	MID_ARMPROCET_HOLEHASSAMEGEM,					// ��װ������Ƕ�����ֱ�ʯ
	MID_ARMPROCET_NOTINLAYGEM,						// ��װ��δ��Ƕ��ʯ
	MID_ARMPROCET_NOTEQUIPASKLEVELITEM,				// ����Ʒ����ƥ��ı�ʯ������
	MID_ARMPROCET_NOTEQUIPASKPICKITEM,				// ����Ʒ����ƥ��ı�ʯժȡ��
	MID_ARMPROCET_NOTCHOICEPICKGEM,					// ��ѡ����Ҫժȡ�ı�ʯ
	MID_ARMPROCET_CANNOTINDENTITY,					// װ�����ɼ���
	MID_ARMPROCET_NOTINDENTITYEQUIP,				// װ��δ����
	MID_ARMPROCET_NOTENOUGHBULL,					// ��������
	MID_ARMPROCET_NOTNEXTITEMLEVEL,					// װ����������

	MID_EXCHANGE_ACTIONFORBID,						//����״̬�в���ִ�д˲���
	
	
	MID_EQUIP_NOTENOUGHEXP,							// ��ǰ����ֵ����, ����ʧ��
	MID_TRUST_NOTENOUGHLV,							// �ȼ�δ����ʮ��

	MID_TRUST_NOTENOUGHLVORITEM,					// �ȼ������ǰί��������Ʒ����
	MID_TRUST_HASTODAYLIMITFINISH,					// �Ѵﱾ��ί�����ƴ���
	MID_TRUST_MAX_ONETIME,							// �Ѵﵽһ������ί��������Ŀ������

	MID_NPCSHOP_TRADENUMCANNOTNULL,					// ��Ʒ����������Ϊ��
	MID_NPCSHOP_TRADEPRICECANNOTNULL,				// ��Ʒ���ۼ۲���Ϊ��
	MID_NPCSHOP_TRADEBUYNUMCANNULL,					// �������������Ϊ��
	MID_NPCSHOP_NOTCHOICEITEM,						// ��ѡ����Ҫ�������Ʒ��

	MID_NPCSHOP_NONEEDREPAIR,						// ����ά��

	MID_STALL_CANSTALLSTATE,						// ��̯״̬�����ϼ���Ʒ
	
	MID_MAIL_NOTENOUGHLV,							// �ȼ�δ��ʮ��
	MID_MAIL_HASINMAILSENDITEM,						// ����Ʒ���ڴ����ʼ���Ʒ�б���

	MID_FIGHT_IN,									// ����ս��״̬
	MID_FIGHT_OUT,									// �뿪ս��״̬

	MID_SHORTCUT_MPNOTENOUGH,						// MP����	
	MID_SHORTCUT_ENERGENOTENOUGH,					// ��������
	MID_SHORTCUT_NOTITEM,							// ��Ʒ������
	MID_SHORTCUT_CANNOTUSESPELL_BUF,				// ��ǰ״̬����ʹ�ü���
	MID_SHORTCUT_CANNOTUSESPELL_MOVE,				// ��ǰ״̬�²���ʹ��λ�Ƽ���
	MID_SHORTCUT_CANNOTUSESPELL_INMORPH,			// ����״̬����ʹ�ü���
	MID_SHORTCUT_CANNOTUSESPELL_XPLIMIT,			// ��ǰ״̬����ʹ��XP����
	MID_SHORTCUT_CANNOTUSESPELL_INSTALL,			// ��̯״̬����ʹ�ü���

	MID_PATH_FINDNONPC,								// Ŀ��NPC���ڵ�ǰ��ͼ
	MID_XP_COOLDOWNTIME,							// ���ܴ�����ȴ״̬
	MID_XP_NOENOUGHMP,								// �����MP����
	MID_XP_NOENOUGHXP,								// ��ҵ�XP����
	MID_XP_NOENOUGHCOLLECT,							// ��ҵĻ���������
	MID_XP_NOINMAP,									// ֻ���ڸ�����ʹ��
	MID_XP_NOCAMPTRUMP,								// ��ǰû�г�������

	MID_MAIL_SENDER_NAME_NULL,						// �ʼ�����������Ϊ��
	MID_MAIL_TITLE_NULL,							// �ʼ�����Ϊ��
	MID_MAIL_NO_MONEY,								// �����ʼ��������ʲ���

	MID_TRADE_SELF_FIGHT,							// ����������ս��״̬�����ܽ��н���
	MID_TRADE_SELF_STALL,							// ���������ڰ�̯״̬�����ܽ��н���
	MID_TRADE_SELF_FLY,								// ���������ڷ���״̬�����ܽ��н���
	MID_TRADE_SELF_DEAD,							// ��������������״̬�����ܽ��н���
	MID_TRADE_SELF_MORPH,							// ���������ڱ���״̬�����ܽ��н���
	MID_TRADE_SELF_TRUMP_PRE,						// ���������ٻ����飬���ܽ��н���
	MID_TRADE_CANNOT_REPEAT_CONFIRM_TRADE_,			// �����ظ�ȷ�Ͻ���
	MID_TRADE_CANNOT_DIST_TOO_LONG,					// �����Զ�޷����뽻��
	MID_TRADE_CANNOT_NOT_CAN_TRADE_OBJECT,			// Ŀ������޷�����

	MID_TRADE_ACTOR_FIGHT,							// �����׵��Ƿ�������ս��״̬�����ܽ��н���
	MID_TRADE_ACTOR_STALL,							// �����׵��Ƿ������ڰ�̯״̬�����ܽ��н���
	MID_TRADE_ACTOR_FLY,							// �����׵��Ƿ������ڷ���״̬�����ܽ��н���
	MID_TRADE_ACTOR_DEAD,							// �����׵��Ƿ�����������״̬�����ܽ��н���
	MID_TRADE_DISTANCE_LONG,						// ˫������̫Զ�����ܽ��н���
	MID_TRADE_INVITE_TIME_OUT,						// �Է���ʱ��û��Ӧ��������

	MID_TRADE_RELATION_XIONGSHOU,					// �Է����Լ��ĳ��˲��ܽ��н���
	MID_TRADE_RELATION_SUDI,						// �Է����Լ����޵��б��ܽ��н���
	MID_TRADE_RELATION_BLACK,						// �Է��ں������в��ܽ��н���

	MID_STALL_DISTANCE_LONG,						// ˫������̫Զ�����ܲ鿴�Է���̯λ��Ϣ
	MID_STALL_SELF_EMPTY,							// û����Ʒ�ϼܣ����ܽ��а�̯
	MID_STALL_SELF_FIGHT,							// ����������ս��״̬�����ܽ��а�̯
	MID_STALL_SELF_PK,								// ����������PK״̬�����ܽ��а�̯
	MID_STALL_SELF_RIDE,							// ���Ǵ������״̬�����ܰ�̯
	MID_STALL_SELF_MORPH,							// ���������ڱ���״̬�����ܲ鿴�Է�̯λ��Ϣ
	MID_STALL_SEE_FIGHT,							// ����������ս��״̬�����ܲ鿴�Է�̯λ��Ϣ
	MID_MULTIMAP_AUTOFIND_FAIL,						// ���ͼ�Զ�Ѱ·ʧ��
	MID_USESKILL_INCOLLECT_FAIL,					// ���ڲɼ�������ʹ�ü���
	MID_USEITEM_INCOLLECT_FAIL,						// ���ڲɼ�������ʹ����Ʒ
	MID_STALL_SELF_TRUMP_PRE,						// �����ٻ����飬���ܰ�̯
	MID_STALL_SELF_NEAR_NPC,						// ����NPC̫�������ܰ�̯
	MID_STALL_SELF_NEAR_OTHER_STALL,				// ��������̯λ̫�������ܰ�̯
	MID_STALL_SELF_DEAD,							// ��������������״̬�����ܽ��а�̯
	MID_STALL_NOTENOUGHLV,							// �ȼ�δ����ʮ��
	MID_STALL_SEE_PK,								// �д�״̬�����ܲ鿴�Է�̯λ��Ϣ
	MID_STALL_CANNOT_EMPTY,							// ���ܿ���̯λ���޷��¼�
	MID_STALL_SEE_DEAD,								// ����״̬���޷����̯λ
	MID_STALL_TOOMUCH_MONEY,						// �����ܶ�ӵ�ǰЯ�������ó�������Я����������

	MID_FOLLOW_START_FAIL,							// ���ܸ���
	MID_ITEM_USE_FAIL_INPRE,						// ����ʹ��������Ʒ
	MID_ITEM_USE_FAIL_NOTINAREA,					// ֻ����ָ������ʹ��
	MID_ITEM_USE_FAIL_IN_STALL,						// ���ڰ�̯״̬�޷�ʹ����Ʒ

	MID_FRIEND_REPEAT_REQ,							// �ظ������ĳ��Ϊ����
	MID_FRIEND_IS_ENEMY,							// ���������ĳ��˲��ܼ�Ϊ����
	MID_FRIEND_IS_SU_DI,							// ����������׷ɱ�����в��ܼ�Ϊ����
	MID_FRIEND_IS_BLACK,							// ���������ĺ������У����ܼ�Ϊ����

	MID_FITTINGMGR_NOFITSTUFF,						// �ò��ϲ���װ�����
	MID_FITTINGMGR_NOTFITGROUP,						// �����ܲ�����Ƕ������
	MID_FITTINGMGR_CAREEALIMIT,						// ְҵ����,������Ƕ�����
	MID_FITTINGMGR_INLAYSPECIALLIMIT,				// ����Ƕ��ֻ��װ���������
	MID_FITTINGMGR_INLAYNORMALLIMIT,				// ����Ƕ��ֻ��װ���޻������
	MID_FITTINGMGR_SAMEIDLIMIT,						// ������Ƕ��ͬid�����
	MID_FITTINGMGR_SAMETYPELIMIT,					// ������Ƕ��ͬ���͵����
	MID_FITTINGMGR_NEEDBULLLIMIT,					// װ���������Ҫ�Ĳ�������

	MID_TRUMP_PER_FAIL_INFIGHT,						// ����״̬�²��ܳ�������
	MID_TRUMP_PER_FAIL_INDEAD,						// ս��״̬�²��ܳ�������
	MID_TRUMP_ISPRE,								// ���ڳ������飬���Ժ�
	MID_TRUMP_BACK_FAIL,							// ��ǰ״̬�����ٻؾ���

	MID_SPELL_UNUSED_NOAMM,							// ȱ���ӵ�
	MID_SPELL_UNUSED_RIGHTARM,						// ȱ����������
	MID_SPELL_UNUSED_LEFTARM,						// ȱ����������
	MID_SPELL_UNUSED_SHOULDER,						// ȱ�ټ������
	MID_SPELL_UNUSED_BACK,							// ȱ�ٱ�������
	MID_SPELL_UNUSED_NONEEQUIP,						// ȱ������

	MID_SPELL_UNUSED_CAMP,							// ʩ��������Ӫ������
	MID_SPELL_UNUSED_OBJ,							// ʩ�����󲻷���
	MID_SPELL_UNUSED_DEAD,							// Ŀ������
	MID_SPELL_UNUSED_MASK_TARGET,					// Ŀ���м���ħ���赲
	MID_SPELL_UNUSED_MASK_POINT,					// �ص��м���ħ���赲
	MID_SPELL_UNUSED_INCOLLECT,						// �ڲɼ�״̬��
	
	MID_SPELL_NOTARGET,								// û��Ŀ��
	MID_AUTOFIND_SELF_LIMIT,						// ����״̬���ƣ����ܽ����Զ�Ѱ·

	MID_TRUST_ALREADY_ORDER,						// ��ί���Ѿ��ҵ��������ظ��ҵ�
	MID_FIND_PATH_FAIL,								// Ѱ·ʧ�ܣ�Ŀ��㲻�ɴ�
	MID_MARKET_NOT_ENOUGH_GOLD,						//��ȯ����
	MID_MARKET_NOT_ENOUGH_BIND_GOLD,				//�󶨵�ȯ����
	MID_MARKET_NOT_FRIEND,							//�������ͺ���
	MID_MARKET_SEND_TRADE_SUCESS,					//�������׳ɹ�
	MID_MARKET_TRADE_SUCESS,						//���׳ɹ�
	MID_MARKET_DESTROY_SUCESS,						//���������ɹ�
	MID_MARKET_PACKAGE_PRESENT_SUCESS,				//���ͳɹ�
	MID_MARKET_BUY_TIME_OUT,						//������Ʒ��ʧЧ

	MID_PET_PRE_FAIL_TRUMP,							// �����ٻ����飬�����ٻ�����
	MID_PET_PRE_FAIL_USEITEM,						// ����ʹ����Ʒ�������ٻ�����
	MID_PET_PRE_FAIL_SKILLPRE,						// ���ڽ��м��������������ٻ�����
	MID_PET_PRE_FAIL_RIDEPRE,						// �����ٻ���ˣ������ٻ�����
	MID_PET_PRE_FAIL_COLLECT,						// ��ǰ״̬�������ٻ�����
	MID_PET_PRE_FAIL_FIGHT,							// ս��״̬�������ٻ�����
	MID_PET_PRE_FAIL_STALL,							// ��̯״̬�������ٻ�����
	MID_PET_PRE_FAIL_IN_OTHER_RIDE,					// ����������������ٻ�����
	MID_PET_PRE_FAIL_IN_RIDE_MACHINE,				// ������е�������ٻ�����
	MID_PET_FAIL_IN_FREEDOM_ANIL,					// �ó����Ѿ��������������ɽ��иò�����
	MID_PET_FAIL_FREEDOM_IN_FIGHT,					// �ó������ڳ��������ɷ���
	MID_PET_PRE_FAIL_SOME_STATE,					// ��ǰ״̬�������ٻ�����

	MID_TRUMP_PRE_FAIL_PET,							// �����ٻ���������ٻ�����
	MID_TRUMP_PRE_FAIL_USEITEM,						// ����ʹ����Ʒ�������ٻ�����
	MID_TRUMP_PRE_FAIL_SKILLPRE,					// ���ڽ��м��������������ٻ�����
	MID_TRUMP_PRE_FAIL_COLLECT,						// ��ǰ״̬�������ٻ�����
	MID_TRUMP_PRE_FAIL_RIDEPRE,						// �����ٻ���ˣ������ٻ�����

	MID_RIDE_PRE_FAIL_TRUMP,						// �����ٻ����飬����ʹ�����
	MID_RIDE_PRE_FAIL_USEITEM,						// ����ʹ����Ʒ������ʹ�����
	MID_RIDE_PRE_FAIL_SKILLPRE,						// ���ڽ��м�������������ʹ�����
	MID_RIDE_PRE_FAIL_COLLECT,						// ��ǰ״̬������ʹ�����
	MID_RIDE_PRE_FAIL_PET,							// �����ٻ��������ʹ�����
	MID_RIDE_PRE_FAIL_NO_RIDE,						// û�и����
	MID_RIDE_PRE_FAIL_SOME_STATE,					// ��ǰ״̬������ʹ�����

	MID_INFIGHT,									// ��ս��״̬������ʹ��
	MID_NOINFIGHT,									// �ڷ�ս��״̬������ʹ��

	MID_NEED_TARGET,								// ����ѡ��һ��Ŀ��
	MID_NEED_MONSTER_TARGET,						// ����ѡ��һ������Ŀ��
	MID_NEED_NPC_TARGET,							// ����ѡ��һ��NPCĿ��	
	MID_NEED_NPC_PLAYER,							// ����ѡ��һ�����	

	MID_APPLY_TEAM_PLAYER_IS_ENEMY,					// �Է�����ĳ��ˣ������������
	MID_APPLY_TEAM_PLAYER_IS_TMPENEMY,				// �Է������׷ɱ�����������������
	MID_INVITE_TEAM_PLAYER_IS_TMPENEMY,				// �Է������׷ɱ�����������������
	MID_APPLY_TEAM_SELF_DEAD,						// �����Ѿ������޷����
	MID_APPLY_TEAM_OTHER_DEAD,						// �Է��Ѿ������޷����
	MID_APPLY_TEAM_OTHER_IN_BLACK,					// �Է�����ĺ������У��޷����
	MID_APPLY_TEAM_OTHER_IN_ZHUISHA,				// ������׷ɱ�����е�������
	MID_APPLY_TEAM_OTHER_IN_XIONGSHOU,				// ���������������е�������
	MID_APPLY_TEAM_OTHER_IN_TEAM,					// ���Ѿ�ӵ�ж���

	MID_USE_ITEM_FAILE_HPENOUGH,					// ��ǰ����ֵ����,����Ҫʹ��
    MID_USE_ITEM_FAILE_MPENOUGH,					// ��ǰħ��ֵ����,����Ҫʹ��
    MID_USE_ITEM_FAILE_LVENOUGH,					// �ȼ�����,�޷�ʹ��

	MID_AUTO_TEAM_FAIL_AGREE_READY,					// �Ѿ�����׼��״̬�������ٴν���ͬ�����
	MID_MARKET_NO_SEARCH,							// �̳�û�в��ҵ�����

	MID_AUTO_TEAM_FAIL_TRAN_BY_EXCHANGE,			// ����״̬���ܴ���
	MID_AUTO_TEAM_FAIL_TRAN_BY_STALL,				// ��̯״̬���ܴ���
	MID_AUTO_TEAM_FAIL_TRAN_BY_DEAD,				// ����״̬���ܴ���
	MID_AUTO_TEAM_FAIL_TIME_OUT,					// ��ʱ���ܴ���

	MID_SECONDARYSKILL_WRONG_STATE,					//���ڲ���ʹ������ܵ�״̬����
	MID_USEITEMPRE_WRONG_STATE,						//���ڲ���ʹ����Ʒ������״̬����

	MID_USEITEM_IN_FLY_STATE,						//�ڷ��е�ʱ�� ����ʹ�õ���
	MID_AUTOFIND_IN_FLY_STATE,						//�ڷ���״̬�����Զ�Ѱ·
	MID_SHORTCUT_IN_FLY_STATE,						//����״̬�� ����ʹ�ü���

	MID_PWORLD_RESTOREITEM,							//�����в���ʹ�ûָ��Ե���
	MID_MAP_NOSKILL,								//��ͼ�в���ʹ�ü���
	MID_SAFE_ENV_NOSKILL,							//��ȫ������ʹ�ü���
	MID_SELF_BUSY,									//������������
	MID_SELF_DEAD,									//���Ѿ�����
	MID_SELF_SPELLPRE,								//������������
	MID_CANNT_USE_INPK,								//�д�״̬�²���ʹ�ô˼���
	MID_NOT_ENOUGH_ENERGY,							//��������
	MID_TARGET_TO_FAR,								//Ŀ��̫Զ

	MID_TRUMP_PER_FAIL_MOVE,						//�ƶ��У������ٻ�����
	MID_MOVE_FAIL_STALL,							// ��̯״̬�޷��ƶ�

	MID_PICK_ITEM_TOO_FAR,							// ����̫Զ���޷�ʰȡ
	MID_SECONDARYSKILL_FAILED_INRIDE,				//���״̬�У��޷����пƼ�����
	MID_SECONDARYSKILL_FAILED_INFIGHT,				//ս��״̬�У��޷����пƼ�����
	MID_SECONDARYSKILL_FAILED_INCASTING,			//�����У��޷����пƼ�����
	MID_SECONDARYSKILL_FAILED_CANNTCONTROL,			//�ܿ���״̬�У��޷����пƼ�����
	MID_SECONDARYSKILL_FAILED_COLDDOWN,				// �������ȴ��

	MID_CANNOT_CATCH_PET,							// ���ǿ��Բ�׽�ĳ���
	MID_CATCH_PET_DEAD,								// ��׽�ĳ���������
	MID_FAIL_WEI_XING,								// ֹͣβ��
	MID_NPCSHOP_BATTLE_GLORY_SELLFAIL,				// ��������,����ʧ��!
	MID_NPCSHOP_AOSHAN_BRAND_SELLFAIL,				// �������Ӳ���,����ʧ��!
	MID_NPCSHOP_LIFT_BRAND_SELLFAIL,				// ��ɳ���Ӳ���,����ʧ��!
	MID_TRUMP_PER_FAIL_BADSTATUS,					// ��״̬�£��޷��ٻ�����
	MID_RESOLVE_WRONG_STATE,						// ���ڲ���ʹ�÷ֽ��״̬����
	MID_RESOLVE_FAILED_INCASTING,					//�����У��޷��ֽ�
	MID_RESOLVE_CONTAINER_LOCK,						//�����������޷��ֽ�
	MID_RESOLVE_FAILED_CANNTCONTROL,				//�ܿ���״̬�У��޷����пƼ�����
	MID_RIDE_PRE_FAILED_CLAN_LIMIT_CAPTION,			//����ʧ�ܣ�������ֻ��ʱ���ռ�칫��᳤����ʹ��Ȩ��
	MID_RIDE_PRE_FAILED_CLAN_LIMIT_MEMBER,			//����ʧ�ܣ�������ֻ��ʱ���ռ�칫���Ա����ʹ��Ȩ��
	MID_USE_ITEM_FAILED_CLAN_LIMIT_CAPTION,			//ʹ��ʧ�ܣ�����Ʒֻ��ʱ���ռ�칫��᳤����ʹ��Ȩ��
	MID_USE_ITEM_FAILED_CLAN_LIMIT_MEMBER,			//ʹ��ʧ�ܣ�����Ʒֻ��ʱ���ռ�칫��᳤����ʹ��Ȩ��
	MID_USE_BACK_CLAN_ITEM_FAILED_INFIGHT,			//ս��״̬����ʹ�ù�����д��;�
	MID_USE_PET_EGG_PET_FULL,						//����������
	MID_PET_MAP_NO_PET_FIGHT,						//��ǰ��ͼ���ﲻ�ɳ�ս
	MID_PET_MAP_NO_FAIRY_FIGHT,						//��ǰ��ͼ�޷��ٻ�����

	MID_MUST_MACHINE_COLLECT,						//����ɼ����ɼ�
	MID_MACHINE_UNMATCH_COLLECT,					//�ɼ�����ƥ��
	MID_MACHINE_UNMATCH_COLLECT_MORMAL,				//�ɼ�����ƥ�䵫�ɼ�������ɼ�����Ϊ0
	MID_USE_MACHINE_ITEM_IN_MORPH,					//�������޷�ʹ�ù����е
	MID_USE_MORPH_ITEM_IN_MACHINE,					//�ڻ�е��ʹ�ñ���ҩˮ

	MID_ENTER_CLANISLAND_IN_FIGHT,					//ս��״̬���ܴ���
	MID_ENTER_CLANISLAND_IN_MACHINE,				//��е״̬���ܴ���
	MID_SUMMON_MUST_BE_CAPTAIN,						//���;���ʹ�ñ����Ƕӳ�
	MID_SUMMON_MUST_BE_PRESIDENT,					//���;���ʹ�ñ����ǻ᳤
	MID_NOTENOUGH_COST_ITEM,						//������Ҫ���ĵĵ�����������
	MID_CHATLINK_MAX_NUM,							//���õ����������Ѵﵽ���
	MID_CHATLINK_SAME_EXIST,						//�Ѵ�����ͬ���͵�����

	MID_NO_ATTACK_WORLD_CITY_CONTROL,				//������Ч��������Դ���������ڣ���ǰ�����޷�������
	MID_NO_ITEM_USE_IN_THIS_MAP,					//�˵�ͼ����ʹ�ô���Ʒ

	MID_NPCSHOP_SPARTA_GLORY_SELLFAIL,				// ������ҫ����,����ʧ��!

	MID_NO_CHANGE_STALL_MODEL,						//��û���ı����̯λ�����Ρ�
};
//tolua_end

struct RectEffectData
{
	unsigned int	limit;
};

class IGameMsgPrint
{
public:
	virtual void updateActorAttr( GameActor* pActor ) {}
	virtual void updateActorPetStatus( GameActor* pActor, bool bPetFight = false ) {}
	virtual void ShowSysMsg( const char* szText ){};
	virtual void SendUIEvent( const char *pevent ) {};
	virtual void showSelEffect( bool bdel = false ) {};

	virtual void showSelectEffect( const char* szAllowPath, const char* szUnAllowPath, int dis, int radio, bool spin ) {};
	virtual void showAdjustEffect( const char* szAllowPath, const char* szUnAllowPath, void *pExtendData ) {};
	virtual void showCircularEffect( const char *szPath, unsigned int nLimit, void *pExtendData){};
	virtual void showRectEffect(const char* szAllowPath, unsigned int width, void *pExtendData) {};
	virtual void showAngleEffect(const char *szPath, int nCumulateTime, unsigned int nDistance){};
	virtual void hideSelectEffect() {};

	virtual void npcEnterView( int id ) {};
	virtual void npcLeaveView( int id ) {};

	virtual void setItemLink( Item& itemInfo ) {};
	virtual void setPostionLink( int mapId, int x, int y ) {};
	virtual void setMissionLink( int missionId, int nRecycleID ) {};

	virtual void showMidTip( EMID_MESSAGE type ) {};

	virtual void newSkill(int id, int lv) {};
	virtual void forgetSkill(int id, int lv) {};
	virtual void playerLvChange( int lv ) {};
	virtual void endCollect() {};
};

class GameMsgPrint : public Ogre::Singleton<GameMsgPrint>
{
public:
	GameMsgPrint()
	{

	}
	~GameMsgPrint()
	{

	}
	
	void registGameMsgHandler( IGameMsgPrint* p ) 
	{
		m_hander.push_back(p);
	}
	
	void unRegistGameMsgHandler( IGameMsgPrint* p ) 
	{
		for( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); )
		{
			if( *iter == p )
			{
				iter = m_hander.erase(iter);
				continue;
			}
			++iter;
		}
	}
	
	void updateActorAttr( GameActor* pActor )
	{
		for( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(), end = m_hander.end(); iter != end; ++iter )
		{
			(*iter)->updateActorAttr( pActor );
		}
	}

	virtual void updateActorPetStatus( GameActor* pActor, bool bPetFight = false )
	{
		for( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(), end = m_hander.end(); iter != end; ++iter )
		{
			(*iter)->updateActorPetStatus( pActor, bPetFight );
		}
	}

	void ShowSysMsg( const char* szText )
	{
		for( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->ShowSysMsg( szText );
		}
	}

	void SendUIEvent( const char *pevent )
	{
		for( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->SendUIEvent( pevent );
		}
	}

	void showSelEffect( bool bdel = false )
	{
		for( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->showSelEffect( bdel );
		}
	}

	void showSelectEffect( const char* szAllowPath, const char* szUnAllowPath, int dis, int radio, bool spin )
	{
		for( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->showSelectEffect( szAllowPath, szUnAllowPath, dis, radio, spin );
		}
	}

	void showAdjustEffect( const char* szAllowPath, const char* szUnAllowPath, void *pExtendData )
	{
		for( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->showAdjustEffect( szAllowPath, szUnAllowPath, pExtendData );
		}
	}

	void showCircularEffect(const char *szPath, unsigned int nLimit, void *pExtendData)
	{
		for( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->showCircularEffect( szPath, nLimit, pExtendData );
		}
	}

	void showRectEffect(const char *szPath, unsigned int width, void *pExtendData)
	{
		for( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->showRectEffect( szPath, width, pExtendData );
		}
	}
	
	void showAngleEffect( const char *szPath, int nCumulateTime, unsigned int nDistance )
	{
		for( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->showAngleEffect( szPath, nCumulateTime, nDistance );
		}
	}

	void hideSelectEffect()
	{
		for( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->hideSelectEffect();
		}
	}


	void npcEnterView( int id )
	{
		for ( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->npcEnterView( id);
		}
	}

	void npcLeaveView( int id ) 
	{
		for ( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->npcLeaveView( id);
		}		
	};

	void setItemLink( Item& itemInfo ) 
	{
		for ( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->setItemLink( itemInfo );
		}	
	}

	void setPostionLink( int mapId, int x, int y )
	{
		for ( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->setPostionLink( mapId, x, y );
		}	
	}

	void setMissionLink( int missionId, int nRecycleID ) 
	{
		for ( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->setMissionLink( missionId, nRecycleID );
		}	
	}
	
	void showMidTip( EMID_MESSAGE type ) 
	{
		for ( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->showMidTip( type );
		}
	}

	void newSkill(int id, int lv)
	{
		for ( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->newSkill(id, lv);
		}
	}

	void forgetSkill(int id, int lv)
	{
		for ( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->forgetSkill(id, lv);
		}
	}

	void playerLvChange( int lv ) 
	{
		for ( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->playerLvChange( lv );
		}
	}

	void endCollect()
	{
		for ( std::list<IGameMsgPrint*>::iterator iter = m_hander.begin(); iter != m_hander.end(); ++iter )
		{
			(*iter)->endCollect();
		}
	}

private:
	std::list<IGameMsgPrint*> m_hander;
};
