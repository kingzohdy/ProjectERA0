#pragma once
#include "OgreSingleton.h"
#include "GameMsgPrint.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "ServerTimer.h"
#include "ClientInterface.h"
#include <time.h>
#include <map>
#include <vector>

#define MAX_GAMEWIZARD_STEP 80	

//tolua_begin

enum
{
	GAMEWIZARD_CLOSEALL				= 1,		// ����������ʾ		
	GAMEWIZARD_FIRST_UI				= 2,		// ��һ��UI
	GAMEWIZARD_FIRST_TALK			= 4,		// ��һ�ζԻ�
	GAMEWIZARD_FIRST_GETMISSION		= 8,		// ��һ��������ȡ
	GAMEWIZARD_FIRST_LEVELUP		= 16,		// ��һ������
	GAMEWIZARD_FIRST_GETMONEY		= 32,		// ��һ�λ�ý�Ǯ
	GAMEWIZARD_FIRST_GOCOPY			= 64,		// ��һ���¸���
	GAMEWIZARD_FIRST_KILLMONSTER	= 128,		// ��һ��ɱ��

	GAMEWIZARD_FIRST_USEDSKILL		= 1001,		// ��һ��ʹ�ü���
	GAMEWIZARD_FIRST_SAW_SHOULEI	= 1002,		// ��һ�ο�������
	GAMEWIZARD_FIRST_GIFT			= 1004,		// ��һ���������
	GAMEWIZARD_FIRST_SAWPOWDER		= 1008,		// ��һ�ο���ըҩͰ
	GAMEWIZARD_FIRST_GETWEAPON		= 1016,		// ��һ�νӴ�����
	GAMEWIZARD_FIRST_SAWXPITEM		= 1032,		// ��һ�ο���XP��
	GAMEWIZARD_FIRST_COLLECT		= 1064,		// ��һ�βɼ�
	GAMEWIZARD_FIRST_PRESENT		= 1128,		// ��һ����Ʒ��

	GAMEWIZARD_FIRST_STUDYSKILL		= 2002,		// ��һ��ѧϰ����
	GAMEWIZARD_FIRST_GETBOOTY		= 2004,		// ��һ��ʰȡ����
	GAMEWIZARD_FIRST_LOOT			= 2008,		// ��һ���ռ�
	GAMEWIZARD_FIRST_SAWSWORD		= 2064,		// ��һ�ο������浶
	GAMEWIZARD_FIRST_FLASHHELP		= 2128,		// ��һ��FLASHָ��

	GAMEWIZARD_FIRST_GETREAGENT		= 3001,		// ��һ�λ��ҩ��
	GAMEWIZARD_FIRST_FLY			= 3002,		// ��һ�η���
	GAMEWIZARD_FIRST_GETMONSETER	= 3004,		// ��һ��ˢ��
	GAMEWIZARD_FIRST_GETFOOD		= 3008,		// ��һ�νӴ�ʳ��
	GAMEWIZARD_FIRST_GETPET			= 3016,		// ��һֻ����
	GAMEWIZARD_FIRST_POWERFULL		= 3032,		// ��һ��XP����
	GAMEWIZARD_FIRST_SPECIALMISSION	= 3064,		// ��һ��������Ϊ
	GAMEWIZARD_FIRST_LIMITMISSION	= 3128,		// ��һ����ʱ����

	GAMEWIZARD_FIRST_SAW_JIJIASHOU	= 4001,		// ��һ�ο�����ʱ��е
	GAMEWIZARD_FIRST_MACHINEPOWER	= 4002,		// ��һ�ο�����ʱ��е��ҩ
	GAMEWIZARD_FIRST_MOVE			= 4004,		// ��һ���ƶ�	
	GAMEWIZARD_FIRST_ROTATE			= 4008,		// ��һ����ת�ӽ�
	GAMEWIZARD_FIRST_COMMMISSION	= 4016,		// ��һ���ύ����
	GAMEWIZARD_FIRST_GETSKILLBOOK	= 4032,		// ��һ�λ�ȡ������
	GAMEWIZARD_SETSHORTCUT			= 4064,		// ��һ�ν���Ϸ���ÿ�ݼ�
	GAMEWIZARD_FIRST_OPENCONTAINER	= 4128,		// ��һ�δ򿪱���

	GAMEWIZARD_FIRST_OPENFRIEND		= 5001,		// ��һ�δ򿪺��ѽ���
	GAMEWIZARD_FIRST_OPENEQUIP		= 5002,		// ��һ�δ��������
	GAMEWIZARD_FIRST_OPENPET		= 5004,		// ��һ�δ򿪳������
	GAMEWIZARD_FIRST_OPENRIDE		= 5008,		// ��һ�δ��������
	GAMEWIZARD_FIRST_OPENTASK		= 5016,		// ��һ�δ��������
	GAMEWIZARD_FIRST_OPENSTASH		= 5032,		// ��һ�δ򿪲ֿ����
	GAMEWIZARD_FIRST_OPENMAIL		= 5064,		// ��һ�δ��ʼ�����
	GAMEWIZARD_FIRST_OPENSHOP		= 5128,		// ��һ�δ��̵����

	GAMEWIZARD_FIRST_OPENTRUST		= 6001,		// ��һ�δ�ί�н���
	GAMEWIZARD_FIRST_OPENTRADE		= 6002,		// ��һ�δ򿪽��׽���
	GAMEWIZARD_FIRST_OPENTLEARN		= 6004,		// ��һ�δ򿪼���ѧϰ����
	GAMEWIZARD_FIRST_GETPETACTOR	= 6008,		// ��һ�λ�ó���
	GAMEWIZARD_FIRST_USEPETEGG		= 6016,		// ��һ��ʹ�ó��ﵰ

	GAMEWIZARD_FIRST_SEELEADER		= 6032,		// ��һ�μ����峤
	GAMEWIZARD_FIRST_SEETEACHER		= 6064,		// ��һ�μ�������ѧϰԱ
	GAMEWIZARD_FIRST_SEETEACHER2	= 8004,		// ��һ�μ�������ѧϰԱ
	GAMEWIZARD_FIRST_SEEMEDICASALER	= 6128,		// ��һ�μ���ҩƷ����

	GAMEWIZARD_FIRST_SEECLANMANAGER	= 7001,		// ��һ�μ����������Ա
	GAMEWIZARD_FIRST_SEEACTIMANAGER	= 7002,		// ��һ�μ��������Ա
	GAMEWIZARD_FIRST_SEERIDESALER	= 7004,		// ��һ�μ�����������
	GAMEWIZARD_FIRST_SEEPETSALER	= 7008,		// ��һ�μ�����������
	GAMEWIZARD_FIRST_SEEWEAPONSALER	= 7016,		// ��һ�μ�����������
	GAMEWIZARD_FIRST_SEEDEFANDSALER	= 7032,		// ��һ�μ�����������
	GAMEWIZARD_FIRST_SEEARMPROCER	= 7064,		// ��һ�μ���װ���ӹ�Ա
	GAMEWIZARD_FIRST_SEETRANSPORTER	= 7128,		// ��һ�μ�������Ա

	GAMEWIZARD_FIRST_HAVEWEAPON		= 8001,		// ��һ���쵽����
	GAMEWIZARD_FIRST_HAVERIDE		= 8002,		// ��һ��װ��������
	GAMEWIZARD_FIRST_SAWTASKLINK	= 8004,		// ��һ�ο�����������
	GAMEWIZARD_FIRST_GETHP			= 8008,		// ��һ�λ�ú�ҩ
	GAMEWIZARD_FIRST_GETMP			= 8016,		// ��һ�λ����ҩ
	GAMEWIZARD_FIRST_GETXIAN_ITEM	= 8032,		// ��һ�λ�þ���
	GAMEWIZARD_FIRST_HAVE_JING_LING	= 8064,		// ��һ��ʹ�þ���
	GAMEWIZARD_FIRST_OPENSKILL		= 8128,		// ��һ�δ��̵����

	GAMEWIZARD_FIRST_EQUIPWEAPON	= 9001,		// TODO: ��һ��װ������
	GAMEWIZARD_FIRST_EQUIPCLOSE		= 9002,		// ��һ��װ���·�
	GAMEWIZARD_FIRST_HAVECLOSE		= 9004,		// ��һ���쵽�·�
	GAMEWIZARD_FIRST_SHOPPING		= 9008,		// ��һ�ι���
	GAMEWIZARD_FIRST_USEHP			= 9016,		// ��һ��ʹ�ú�ҩ
	GAMEWIZARD_FIRST_USEMP			= 9032,		// ��һ��ʹ����ҩ
	GAMEWIZARD_FIRST_LEARNSKILL		= 9064,		// ��һ��ѧϰ����
	GAMEWIZARD_FIRST_ENTERWORLD		= 9128,		// ��һ�ν�����Ϸ

	GAMEWIZARD_FIRST_LOGINTIMECITY	= 10001,	// ��һ�ν�ʱ��ǣ�������ָ����ɾ��
	GAMEWIZARD_FIRST_SETGAMEOPT		= 10002,	// ��һ�ν���Ϸ������Ϸ
	GAMEWIZARD_FIRST_GETGENIUSOPT	= 10004,	// ��һ�λ���츳
	GAMEWIZARD_FIRST_GETQUEST_FROMUI	= 10008,	// ��һ�δ���������������
	GAMEWIZARD_FIRST_FINISHQUEST_FROMUI	= 10016,	// ��һ�δ�������������
	GAMEWIZARD_FIRST_FIGHT				= 10032,	// ��һ��ս��
	GAMEWIZARD_FIRST_USE_SWORD			= 10064,	// ��һ��ʹ�û��浶
	GAMEWIZARD_FIRST_USEWEAPONITEM		= 10128,	// ��һ��ʹ������

	GAMEWIZARD_FIRST_EQUIP_TO_EQUIP_FRAME= 11001,	// ��һ��װ������������ʾ���������
	GAMEWIZARD_FIRST_PICKSWORD			= 11002,	// ��һ��ʰȡ���浶
	GAMEWIZARD_FIRST_PICKMACHINE		= 11004,	// ��һ��ʰȡ��ʱ��е
	GAMEWIZARD_FIRST_USE_TMPMACHINE		= 11008,	// ��һ��ʹ����ʱ��е
	GAMEWIZARD_FIRST_SAWPAOTAI			= 11016,	// ��һ�ο�����̨
	GAMEWIZARD_FIRST_PICKPAOTAI			= 11032,	// ��һ��ʰȡ��̨
	GAMEWIZARD_FIRST_USE_TPAOTAI		= 11064,	// ��һ��ʹ����̨
	GAMEWIZARD_FIRST_PICKSHOULEI		= 11128,	// ��һ��ʰȡ����
	GAMEWIZARD_FIRST_USE_TSHOULEI		= 12001,	// ��һ��ʹ����ʱ��е
	GAMEWIZARD_FIRST_SAW_ZHUCHENG_TRAN	= 12002,	// ��һ�ο������͵�ʱ��ǵĴ�ʹ
	GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN	= 12004,	// ��һ���봫�͵�ʱ��ǵĴ�ʹ�Ի�
	GAMEWIZARD_FIRST_TRAN_TO_SKY_COPY	= 12008,	// ��һ���봫�͵���յ�����
	GAMEWIZARD_FIRST_FLY_QUEST			= 12016,	// ��һ�η��е������ȡ
	GAMEWIZARD_FIRST_CLICK_PET_ZHONG_CHENG_BTN_WHEN_LOW= 12032,	// �ڵ�һ�γ����ҳ϶�̫�͵�ʱ����ѱ����ť
	GAMEWIZARD_FIRST_PET_FIGHT			= 12064,	// ��һ�γ������
	GAMEWIZARD_FIRST_PET_HP_LOW			= 12128,	// ��һ�γ���Ѫ��̫��
	GAMEWIZARD_FIRST_BLINK_PET_BTN_WHEN_LOW= 13001,	// �ڵ�һ�γ����ҳ϶�̫�͵���˸��������ﰴť
	GAMEWIZARD_FIRST_SELF_LEVEL_UP		= 13002,	// ��һ���Լ��ֶ�����
	GAMEWIZARD_FIRST_TALK_TO_SKY_COPY_NPC= 13004,	// �ڽӵ����͵���յ������������NPC�ĵ�һ�ζԻ�
	GAMEWIZARD_FIRST_TALK_TO_FLY_QUEST_NPC= 13008,	// �ڽӵ����е��������NPC�ĵ�һ�ζԻ�
	GAMEWIZARD_FIRST_CAMP_TRUMP	= 13016,	// ��һ�γ�������
	GAMEWIZARD_FIRST_GETRIDE_ITEM		= 13032,		// ��һ�λ�������Ʒ
	GAMEWIZARD_FIRST_BLINK_CON_STRENGTHEN_RIDE_ITEM_BTN= 13064,		// ��һ���ñ�����ǿ����˵ĵ�����˸
	GAMEWIZARD_FIRST_BLINK_STRENGTHEN_BTN	= 13128,		// ��һ��ǿ�����	
	GAMEWIZARD_FIRST_PUT_STRENGTHEN_RIDE_TO_FRAME= 14001,	// ��һ�ν����������Ʒ�ŵ��������
	GAMEWIZARD_FIRST_SEE_COPY_TRAN_DOOR		= 14002,	// ��һ�ο�������������	
	GAMEWIZARD_FIRST_TALK_COPY_TRAN_DOOR	= 14004,	// ��һ���븱�������ŶԻ�	
	GAMEWIZARD_FIRST_GET_CHENG_HAO			= 14008,	// ��һ�λ�óƺ�	

	GAMEWIZARD_FIRST_SWORD_PWORLD_ANILM		= 14016,	// ��һ�ο������浶��������
	GAMEWIZARD_FIRST_PAOTAI_PWORLD_ANILM	= 14032,	// ��һ�ο�����̨��������
	GAMEWIZARD_FIRST_SHOULEI_PWORLD_ANILM	= 14064,	// ��һ�ο������׸�������
	GAMEWIZARD_FIRST_JIJIA_PWORLD_ANILM		= 14128,	// ��һ�ο��������޸�������
	GAMEWIZARD_FIRST_CURE_NPC				= 15001,	// ��һ�ξ��η���
	GAMEWIZARD_FIRST_CURE_NPC_AFTER_TALK	= 15002,	// ��������Ի����һ�ξ��η���
	GAMEWIZARD_FIRST_OPEN_BUY_CONFIRM		= 15004,	// ��һ�ε��ȷ�Ϲ������
	GAMEWIZARD_SECOND_SELF_LEVEL_UP			= 15008,	// �ڶ����Լ��ֶ�����
	GAMEWIZARD_FIRST_BLINK_MAIN_BTN_GET_CHENG_HAO= 15016,	// ��һ�λ�óƺ������ǰ�ť����
	GAMEWIZARD_FIRST_BLINK_CHANRACTER_FRAME_LABLE_BTN_GET_CHENG_HAO	= 15032,	// ��һ�λ�óƺ����������ĳƺű�ǩ��˸
	GAMEWIZARD_FIRST_BLINK_CHENG_HAO_BTN_IN_CHENG_HAO_FRAME_GET_CHENG_HAO= 15064,// ��һ�λ�óƺ��óƺ�����ʹ�óƺŰ�ť��˸
	GAMEWIZARD_SECOND_SAWSWORD				= 15128,	// �ڶ��ο������浶
	GAMEWIZARD_SECOND_USE_SWORD				= 16001,	// �ڶ���ʹ�û��浶
	GAMEWIZARD_FIRST_BLINK_LIAN_HUA_LABEL	= 16002,	// ��һ����������ǩҳ��˸
	GAMEWIZARD_FIRST_REPAIR					= 16004,	// ���� 1081 ȫ������
	GAMEWIZARD_FIRST_BLINK_LIAN_HUA_CONFIRM_ZHU_RU_BTN	= 16008,	// ��һ����ȷ��ע�뾭�鰴ť��˸
	GAMEWIZARD_FIRST_BLINK_LIAN_HUA_FRAME_LIAN_HUA_BTN	= 16016,	// ��һ����������ť��˸
	GAMEWIZARD_FIRST_EQUIP_NOT_ENOUGH_DUR	= 16032,	// ��һ����ʾװ���;öȲ���
	GAMEWIZARD_FIRST_BLINK_REPAIR_ALL_BTN	= 16064,	// ��һ����˸��ȫ������ť
	GAMEWIZARD_FIRST_BLINK_CONFIRM_REPAIR_BTN= 16128,	// ��һ��ȷ�����������ť
	GAMEWIZARD_SECOND_PICKSWORD				= 17001,	// �ڶ���ʰȡ���浶
	GAMEWIZARD_FIRST_BLINK_SHORTCUT_RIDE_BTN= 17002,		// ��һ����˸�������˰�ť

	GAMEWIZARD_FIRST_BLINK_NPCTALK_LEARN_PET_SKILL_OPTION	= 17004,	// ��һ����˸NPC�Ի��������ĳ��＼��ѧϰѡ��
	GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN= 17008,	// ��һ����˸�����������ĳ��＼��ѧϰ��ͼ��
	GAMEWIZARD_FIRST_BLINK_PETSKILLFRAME_SKILL_GRID_BTN		= 17016,	// ��һ����˸���＼��ѧϰ�������ļ��ܸ���
	GAMEWIZARD_FIRST_BLINK_PETSKILLFRAME_LEARN_SKILL_BTN	= 17032,	// ��һ����˸���＼��ѧϰ��������ѧϰ��ť

	GAMEWIZARD_FIRST_BLINK_NPCTALK_STRENGTHEN_PET_OPTION	= 17064,	// ��һ����˸NPC�Ի��������ĳ��＼��ǿ��ѡ��
	GAMEWIZARD_FIRST_BLINK_CONTAINER_STRENGTHEN_ITEM_BTN	= 17128,	// ��һ����˸�����������ĳ���ǿ��ʯͼ��
	GAMEWIZARD_FIRST_BLINK_PESTRENGTHENFRAME_STRENGTHEN_BTN	= 18001,	// ��һ����˸����ǿ����������ǿ����ť
	
	GAMEWIZARD_FIRST_OPEN_CONTAINERFRAME_FIRST_GET_ADD_PET_EXP_ITEM	= 18002,// ��һ�λ�ó��ﾭ�����ʱ��򿪱���
	GAMEWIZARD_FIRST_BLINK_CONTAINER_ADD_PET_EXP_ITEM_BTN	= 18004,	// ��һ����˸�����������ĳ��ﾭ���ͼ��

	GAMEWIZARD_FIRST_BLINK_NPCTALK_LEVEL_UP_BAO_SHI_OPTION	= 18008,	// ��һ����˸NPC�Ի��������ı�ʯ����ѡ��
	GAMEWIZARD_FIRST_BLINK_LEVELUPBAOSHIFRAME_ITEM_BTN		= 18016,	// ��һ�α�ʯ������������װ����ť
	GAMEWIZARD_FIRST_BLINK_LEVELUPBAOSHIFRAME_STUFF_BTN		= 18032,	// ��һ�α�ʯ�����������Ĳ��ϰ�ť
	GAMEWIZARD_FIRST_BLINK_LEVELUPBAOSHIFRAME_QIANGHUA_BTN	= 18064,	// ��һ�α�ʯ������������ǿ����ť

	GAMEWIZARD_FIRST_BLINK_NPCTALK_XIANGQIAN_BAO_SHI_OPTION	= 18128,	// ��һ����˸NPC�Ի�����������Ƕ��ʯѡ��
	GAMEWIZARD_FIRST_BLINK_XIANGQIANBAOSHIFRAME_ITEM_BTN	= 19001,	// ��һ����˸NPC��һ����Ƕ��ʯ�����װ����ť
	GAMEWIZARD_FIRST_BLINK_XIANGQIANBAOSHIFRAME_BAO_SHI_STUFF_BTN= 19002,	// ��һ����˸NPC��һ����Ƕ��ʯ����ı�ʯ���ϰ�ť
	GAMEWIZARD_FIRST_BLINK_XIANGQIANBAOSHIFRAME_INSURE_STUFF_BTN= 19004,	// ��һ����˸NPC��һ����Ƕ��ʯ�������Ƕ����ť
	GAMEWIZARD_FIRST_BLINK_XIANGQIANBAOSHIFRAME_XIANGQIAN_BTN	= 19008,	// ��һ����˸NPC��һ����Ƕ��ʯ�������Ƕ��ť

	GAMEWIZARD_FIRST_BLINK_CONTAINERFRAME_BOOTY_ITEM_BTN	= 19016,	// ��һ����˸��������ʰȡ��������ȡ������Ʒ

	GAMEWIZARD_FIRST_BLINK_EXIST_WAIT_BTN			= 19032,/// ��������һ�α���
	GAMEWIZARD_FIRST_BLINK_EXIST_INVITE_BTN			= 19064,/// ��������һ���յ�����
	GAMEWIZARD_FIRST_BLINK_EXIST_SCORE_BTN			= 19128,/// ��������һ�ν���
	GAMEWIZARD_FIRST_BLINK_BCT_PROCESS_BTN			= 20001,/// �ٲ�����һ�ν���
	GAMEWIZARD_FIRST_BLINK_BCT_FINISH_BTN			= 20002,/// �ٲ�����һ�ι���

	GAMEWIZARD_FIRST_BLINK_NPCTALK_ENTER_FUBEN_OPTION	= 20004,	// ��һ����˸NPC�Ի��������Ľ��븱��ѡ��

	GAMEWIZARD_FIRST_BLINK_NPCTALK_STUDY_SKILL_OPTION	= 20008,	// ��һ����˸NPC�Ի���������ѧϰ����ѡ��
	GAMEWIZARD_FIRST_BLINK_NPCLEARNSKILL_STUDY_BTN		= 20016,	// ��һ����˸����ѧϰ����ѧϰ��ť


	GAMEWIZARD_FIRST_POP_SWORD_XINSHOU_HELP_BTN			= 20032,		// ������һ�����浶��������ʾ
	GAMEWIZARD_SECOND_POP_SWORD_XINSHOU_HELP_BTN		= 20064,		// �����ڶ������浶��������ʾ
	GAMEWIZARD_FIRST_POP_JIJIA_XINSHOU_HELP_BTN			= 20128,		// ������һ�������޵�������ʾ
	GAMEWIZARD_FIRST_POP_SHOULEI_XINSHOU_HELP_BTN		= 21001,		// ������һ�����׵�������ʾ
	GAMEWIZARD_FIRST_POP_PAOTAI_XINSHOU_HELP_BTN		= 21002,		// ������һ����̨��������ʾ

	GAMEWIZARD_FIRST_POP_MOUSE_HABIT_XINSHOU_FRAME		= 21004,		// ������һ���޸����ϰ�ߵ�������ʾ


	GAMEWIZARD_FIRST_SEE_STUDY_SKILL_NPC				= 21008,		// ��һ�ο�������ѧϰNPC
	GAMEWIZARD_FIRST_BEGIN_LIAN_HUA						= 21016,		// ��һ����������

	GAMEWIZARD_FIRST_JIU_ZHI_DENG_LONG_CAO				= 21032,		// ��һ�ξ�����ɫ������
	GAMEWIZARD_FIRST_ADD_TIAN_FU						= 21064,		// ��һ�μ��츳

	GAMEWIZARD_FIRST_BLINK_PET_EGG_CONTAINER_BTN		= 21128,		// ��һ����˸���ﵰ�ı�����ť
	GAMEWIZARD_FIRST_GET_PET_BLINK_PET_SHORTCUT_BTN		= 22001,		// ��һ�λ�ó�����˸�����ݼ�

	GAMEWIZARD_FIRST_LEVEL_UP_TRUMP						= 22002,		// ��һ����������ɹ�

	GAMEWIZARD_FIRST_BLINK_BCT_FAIL_BTN					= 22004,				/// �ٲ�����һ��ʧ��
	GAMEWIZARD_FIRST_FIRST_SET_PET_USE_ITEM				= 22008,				/// ��һ�����ó���ʹ��ҩƷ����

	GAMEWIZARD_FIRST_CLOSE_SELF_LEVEL_UP				= 22016,	// ��һ��S�Լ��ֶ��ر�������ʾ
	GAMEWIZARD_SECOND_CLOSE_SELF_LEVEL_UP				= 22032,	// �ڶ����Լ��ֶ��ر��ֶ�������ʾ
	GAMEWIZARD_FIRST_COMMIT_MAIL						= 22064,	// ��һ���ύ�ʼ�
	GAMEWIZARD_FIRST_CLOSE_PET_ZHONG_CHENG_BTN_TIPS		= 22128,	// ��һ���ֶ��ر�ѱ����������ָ��tips

	GAMEWIZARD_FIRST_BLINK_AUTO_WEI_YANG_BTN			= 23001,	// ��һ����˸�Զ�ι����ť
	GAMEWIZARD_FIRST_USE_NEW_SKILL						= 23002,	// ��һ��ʹ���»�õļ���
	GAMEWIZARD_FIRST_SET_CHAT_PING_BI					= 23004,	// ��һ������Ƶ������
	
	GAMEWIZARD_FIST_TRUMP_QUEST_XP_FULL					= 23008,	//��һ�ξ�������XP��
	GAMEWIZARD_FIST_TRUMP_QUEST_FAIRY_MORPH				= 23016,	//��һ�ξ������������
	GAMEWIZARD_FIRST_SET_TUAN_DUICHAT_PING_BI			= 23032,	// ��һ�������Ŷ�Ƶ������
	GAMEWIZARD_FIRST_SET_NEW_CHAT_PING_BI				= 23064,	// ��һ�������µĲ߻�Ƶ������

	GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_MAP                = 24001,    // ��һ�δ򿪹����ͼ
	GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_TECH               = 24002,    // ��һ�δ򿪹���Ƽ�
	GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_PRODUCT            = 24004,    // ��һ�δ򿪹�������
	GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_LEARN_TECH         = 24008,     // ��һ��ѧϰ�Ƽ�

	GAMEWIZARD_FIRST_OPEN_SKILL_RUNE					= 24016,    // ��һ�δ򿪼��ܷ���
	GAMEWIZARD_FIRST_BLINK_JIJIASHOU_CONTAINER_BTN		= 24032,	// ��һ�λ�û�����
	GAMEWIZARD_FIRST_USE_PET_HETI						= 24064,	// ��һ��ʹ�ó������
	GAMEWIZARD_FIRST_GET_BROKEN_JIJIASHOU_STEP1			= 24128,	// ��һ����𻵵Ļ�����
	GAMEWIZARD_FIRST_GET_BROKEN_JIJIASHOU_STEP2			= 25001,	// ��һ����𻵵Ļ�����
	GAMEWIZARD_FIRST_TRUMP_LEVEL_UP						= 25002,	// ��һ����������
	GAME_WIZARD_FIRST_TRUMP_STAR_UP						= 25004,	// ��һ��ǿ������
	GAME_WIZARD_FIRST_TRUMP_KEYIN						= 25008,	// ��һ�ξ����ӡ
	GAMEWIZARD_FIRST_BLINK_GET_SPAR_CONTAINER_BTN		= 25016,	// ��һ�λ�þ�ʯ

	GAMEWIZARD_FIRST_CHANGE_PET_MODE					= 25032,	// ��һ�α������ģʽ
	GAMEWIZARD_FIRST_SPARFRAME_SHOW						= 25064,	// ��һ�δ���Դ����
	GAMEWIZARD_FIRST_IN_TEAM							= 25128,	// ��һ�ν������
	GAMEWIZARD_FIRST_IN_AUTO_ASSIST						= 26001,	// ��һ�δ򿪹һ����
	GAMEWIZARD_FIRST_GET_PK_TIPS						= 26002,	// ��һ����ʾPK��ʾ
	GAMEWIZARD_FIRST_GET_RUNE_CONTAINER_BTN				= 26004,	// ��һ�λ�÷���
	GAMEWIZARD_FIRST_OPEN_RUNE_RESET					= 26008,	// ��һ�δ򿪷����������
	GAMEWIZARD_FIRST_ACCEPT_TASK_TO_WORLD_MAP_SEND		= 26016,	// ���������һ�γ���������������
	GAMEWIZARD_FIRST_EQUIP_FENGYIN						= 26032,	// ��һ��װ������ӡ
	GAMEWIZARD_FIRST_SPAR_EQURATE						= 26064,	// ��һ�ξ�ʯ����
	GAMEWIZARD_FIRST_GET_CLAN_PWORLD					= 26128,	// ��һ����ʾ��������
	GAMEWIZARD_FIRST_GET_CLAN__BANNER_GLORY				= 27001,	// ��һ����ʾ����ս����ҫ
	GAMEWIZARD_FIRST_SET_AUTO_QUEST_DEFAULT				= 27002,	// ��һ�������Զ�����Ĭ��ֵ
	GAMEWIZARD_FIRST_SHOW_LOCK_ATTR_WARN				= 27004,	// ��һ����ʾ����������ʾ
	GAMEWIZARD_FIRST_SHOW_LOCK_ATTR_SAFE				= 27008,	// ��һ����ʾ���Ա�����ʾ
	GAMEWIZARD_FIRST_IN_VIP_ASSIST						= 27016,	// ��һ�δ򿪸߼�����
	GAMEWIZARD_FIRST_SET_CLAN_GROUP_PINGBI				= 27032,	// ��һ�ε�¼��Ϸǿ������ս�Ų�����
	GAMEWIZARD_FIRST_QUEST_IF_YOU_CAN					= 27064,	// ��һ��������������
	GAMEWIZARD_FIRST_QUEST_SAVE_OTHERS					= 27128,	// ��һ�����񣺾���
	GAMEWIZARD_FIRST_CLAN_DART_DELIVERY					= 28001,	// ��һ�ι�������
	// END 10001	
};

// ��������
enum
{
	WIZARD_SPRING_GETQUEST,			// �����ȡ����
	WIZARD_SPRING_QUESTCOMMIT,		// �����ύ����
	WIZARD_SPRING_LEVELUP,			// ��������
	WIZARD_SPRING_OBJENTERVIEW,		// ���������Ұ����
	WIZARD_SPRING_STUDY,			// ѧϰ����
	WIZARD_SPRING_OPENUI,			// ��UI
};

#pragma pack(1)
struct tagOneDayGiftCond_ToLua
{
    unsigned char LVMin;                           	/*   ��С�ȼ� */
    unsigned char LVMax;                           	/*   ���ȼ� */
    unsigned char PloyNum;                         	/*   ��ɻ���� */
    unsigned char OnlineCon;                       	/*   ������������ */
    int OnlineTM;                        	/*  Ver.243 ÿ������ʱ��|��λ:�� */
	int ContOnlineTM;                    	/*  Ver.1145 ÿ����������ʱ��|��λ:�� */
	int UserType;                        	/*  Ver.1145 �û�����|ȡֵΪGIFT_USER_TYPE */
	int ClanLevel;                       	/*  Ver.1185 ����ȼ� ��Ʒ����ֻ��ʾ��Ӧ�ȼ��� */
	int ClanTouch;                       	/*  Ver.1185 ���������������� */
	int ClanCityLevel;                   	/*  Ver.1337 ������еȼ� */
	int TotalContri;                     	/*  Ver.1367 ���׶����� */
};
    
struct tagGiftItem_ToLua
{
    int ItemID;                         
    int ItemNum;                        
};

struct tagGiftItems_ToLua
{
    int Career;                         
    tagGiftItem_ToLua GiftItems[ONEDAY_GIFT_ITEM_MAX];
};

struct tagOneDayGift_ToLua
{
    int GiftID;                         
    char Name[LVGIFT_NAME_LEN];           	/*  Ver.243 ����� */
    char TypeName[LVGIFT_NAME_LEN];       	/*  Ver.243 �������� */
    int GiftType;                       
    tagOneDayGiftCond_ToLua GiftCond;                       
    tagGiftItems_ToLua GiftItems[CAREER_MAX];          
    char GiftDesc[LVGIFT_DESC_LEN];           	/*  Ver.243 ������� */
	int MoneyType;                       	/*  Ver.487  */
	int Money;                           	/*  Ver.487  */
	int Exp;                             	/*  Ver.1185 ����ȼ������Ȼ�������ؾ��齱�� */
	int ClanContri;                      	/*  Ver.1185 ���ṱ�׶Ƚ��� */
};

struct tagLVGift_ToLua
{
    int LV;                             
    char Name[LVGIFT_NAME_LEN];           	/*  Ver.243 ����� */
    tagGiftItems_ToLua GiftItems[CAREER_MAX];          
    char GiftDesc[LVGIFT_DESC_LEN];           	/*  Ver.243 ������� */
	int MoneyType;                       	/*  Ver.487  */
	int Money;                           	/*  Ver.487  */
};

struct tagGiftInfo_ToLua
{
    int GiftID;                         
    int GiftTM;                       
};

struct tagGiftLvList_ToLua  
{
    unsigned char Lv;                    /*   ��ȡ�������ĵȼ� */
};

struct tagRoleGiftInfo_ToLua
{
    tagGiftLvList_ToLua GiftLvList[MAX_LV_GIFT];         	/*   ��ȡ���ĵȼ��Ľ��� */
    char OneDayNum; 
    tagGiftInfo_ToLua GiftInfoOneDay[MAX_ONEDAY_GIFT]; 	/*   �Ѿ���ȡ����ÿ�ս��� */
    int OneDayOnlineTM;                  	/*  Ver.243 ��������ʱ��|�� */
    int OnlineTMUpTM;                    	/*  Ver.243 ��������ʱ����µ�ʱ�� */
};
struct tagCoverItem_ToLua
{
	int ItemID;                         
	int Num;                            
	int Rate;                           
};
struct tagContCoverGift_ToLua
{
	int Day;                             					/*   ����ǩ������ */
    char Name[RES_NAME_LEN];              					/*   ���� */
	tagCoverItem_ToLua Items[MAX_CONT_COVER_GIFT_ITEM];		/*   �������� */
};
struct tagNewRoleGift_ToLua
{
	int ID;                             
	char Name[RES_NAME_LEN];              	/*   ���� */
	int Sec;                             	/*   ������֮����ȡ */
	tagGiftItem_ToLua Items[MAX_NEW_ROLE_GIFT_ITEM];   	/*   �������� */
};
enum
{
    CLIENT_GIFT_TYPE_LV = GIFT_TYPE_LV, 	        /* �ȼ����� */
    CLIENT_GIFT_TYPE_ONEDAY = GIFT_TYPE_ONEDAY, 	/* ÿ�ս��� */
	CLIENT_GIFT_TYPE_CONTI = GIFT_TYPE_CONTI,		/* ����ÿ�����߶�ý��� */
	CLIENT_GIFT_TYPE_ACCGIFT =GIFT_TYPE_ACCGIFT,	/* ���⽱��  �ʾ��� */
	CLIENT_GIFT_TYPE_PROPGIFT = GIFT_TYPE_PROPGIFT,	/* ��ֵ���� */
	CLIENT_GIFT_TYPE_CONTI_GROUP = GIFT_TYPE_CONTI_GROUP, 	/* ���������齱�� */
	CLIENT_GIFT_TYPE_CDKEY = GIFT_TYPE_CDKEY,				/* CDKey ��� */
	CLIENT_GIFT_TYPE_CONT_COVER = GIFT_TYPE_CONT_COVER, 	/* ����ǩ����� */
};

enum 
{
	CDKEY_FAIL_NO_TOLUA = 0, 	/* �ɹ� */
	CDKEY_FAIL_VAILD_TOLUA = 1, 	/* cdkey��Ч */
	CDKEY_FAIL_USE_TOLUA = 2, 	/* cdkey�Ѿ�ʹ�ù��� */
	CDKEY_FAIL_OVERDUE_TOLUA = 3, 	/* cdkey���� */
	CDKEY_FAIL_DUMP_TOLUA = 4, 	/* �ظ���ȡ��ͬ���͵���� */
	CDKEY_FAIL_SYS_TOLUA = 5, 	/* ϵͳ���� */
	CDKEY_FAIL_MANY_TOLUA = 6, 	/* �����ɫͬʱ��ʹ��ͬһ��cdkey */
	CDKEY_FAIL_LEVEL_LOW_TOLUA = 7, 	/* �ȼ����� */
	CDKEY_FAIL_LEVEL_HIGH_TOLUA = 8, 	/* �ȼ����� */
};


#define MAX_ACC_GIFT_LIST_TOLUA MAX_ACC_GIFT_LIST
#define NEW_PROPSDEF_RECORD_MAX_TOLUA NEW_PROPSDEF_RECORD_MAX
#define PROPS_GIFT_ITEM_NUM_TOLUA PROPS_GIFT_ITEM_NUM

struct tagContOnlineInfo_ToLua
{
	int GiftID;                         
	unsigned char ContDay;                        
	int UpDayTm;                        
	int OnlineTm;                       
	int UpOnlineTm;                     
	unsigned char NextFlag;                       
	unsigned char IsGift;                         
};

struct tagMiscContOnlineInfo_ToLua
{
	int Num;                            
	tagContOnlineInfo_ToLua ContOnlineInfo[MAX_CONTGIFT];
};

struct tagContOnlineGiftDef_ToLua
{
	int GiftID;                         
	char Name[LVGIFT_NAME_LEN];           	/*   ����� */
	unsigned char Day;                             	/*   �������߼��� */
	unsigned char Hour;                            	/*   ÿ�����߼�Сʱ */
	unsigned char IsNext;                          	/*   ������ ��һ���ܷ����� 0-��  1-�� */
	tagGiftItems_ToLua GiftItems[CAREER_MAX];          
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   ������� */
	int MoneyType;                       	/*  Ver.487  */
	int Money;                           	/*  Ver.487  */
};

enum {
	ACCGIFT_CAN_GET_TOLUA = 0,
	ACCGIFT_CANNT_GET_TOLUA = 1,
};

struct tagAccGift_ToLua
{
	int ID;  
	unsigned int Uin;                            
	unsigned long long RoleID;                          	/*  Ver.1221  */
	int Type;                            	/*   �������4-�������(�ƺ�,��è��) 5-�ʾ���� */
	int Flag;                            	/*   ���ֶβ�����д, �ɷ�����д�� */
	char Name[LVGIFT_NAME_LEN];           	/*   ����� */
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   ������� */
	tagGiftItem_ToLua GiftItems[MAX_ACC_GIFT_LIST];   
	tdr_datetime_t SetGiftTime;                     	/*  Ver.1221 ����ʱ�� */
	tdr_datetime_t GetGiftTime;                     	/*  Ver.1221 ��ȡʱ�� */
};

struct tagAccGiftInfo_ToLua
{
	int Count;                          
	tagAccGift_ToLua AccGifts[MAX_ACC_GIFT];
};

/* ������������� */
struct tagPropsDef_ToLua
{
	unsigned int GroupID;                         	/*   ��ID|ͬһ�����ڽ׶�Ϊһ��|��id��ҪΨһ(������ɻ���ʧЧ��ID��ͬ) */
	tdr_ulonglong CashUp;                          	/*   ��ֵ����,�����ֵ20000��ȯ������������ */
	char BeginDate[DB_TIME_LEN];          	/*   ��ֵ��ʱ��ζ���|��ʼ���� */
	char EndDate[DB_TIME_LEN];            	/*   ��ֵ��ʱ��ζ���|�������� 20110620��20110621��ʾ2����Ч */
	char Name[32];                        	/*   ����|ͬһ����,ֻ��Ҫ����С��CashUp������д���� */
	int IconID;                          	/*  Ver.1930 ���ͼ�� */
	char Title[64];                       	/*   ����|ͬһ����,ֻ��Ҫ����С��CashUp������д���� */
	char DesHead[512];                    	/*   ����ͷ��|ͬһ����,ֻ��Ҫ����С��CashUp������д���� */
	char DesTail[512];                    	/*   ����β��|ͬһ����,ֻ��Ҫ����С��CashUp������д���� */
	tagGiftItem_ToLua PropsGiftItem[PROPS_GIFT_ITEM_NUM];
};

/* ��ֵ�������ȡ��Ϣ */
struct tagPropsGiftGetInfo_ToLua
{
	tdr_ulonglong CashUp;                          	/*   PropsDef���涨���CashUp */
	int GetTime;                         	/*   ��ȡʱ�� */
};

/* ������Ʒ */
struct tagPropsGift_ToLua
{
	unsigned int Uin;                            
	unsigned int GroupID;                         	/*   PropsDef���ж������ID */
	tdr_ulonglong CashNow;                         	/*   ��ǰ��ֵ���� */
	tdr_datetime_t LastCashTime;                    	/*   ��ֵ���������µ�ʱ�� */
	int GiftGetNum;                      	/*   ����ȡ������� */
	tagPropsGiftGetInfo_ToLua GiftGetInfo[PROPS_LIMIT_NUM];    	/*   ����ȡ�����Ϣ */
};

struct tagPropGiftInfo_ToLua
{
	int DefCount;                        	/*   ���Def���� */
	tagPropsDef_ToLua PropsDef[NEW_PROPSDEF_RECORD_MAX];
	int Count;                           	/*   ������� */
	tagPropsGift_ToLua PropsGift[PROPS_LIMIT_MONTH];  
};

struct tagContiGroup_ToLua
{
	int ID;                             
	int Day;                            
};

struct tagGiftOne_ToLua
{
	int Day;                             	/*   ����������������  */
	int ItemID1;                        
	int ItemNum1;                       
	int ItemID2;                        
	int ItemNum2;                       
	int ItemID3;                        
	int ItemNum3;                       
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   ������� */
};

struct tagOnlineGiftGroup_ToLua
{
	int ID;                              	/*   ���� */
	int Time;                            	/*   ��λ|�� */
	char Name[LVGIFT_NAME_LEN];           	/*   ����� */
	int OneNum;                         
	tagGiftOne_ToLua GiftOnes[MAX_GIFT_GROUP];       
};

struct tagMediaGiftItems_ToLua
{
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   ���� */
	tagGiftItem_ToLua GiftItems[MEDIAGIFT_OPTIONS_ITEM_MAX];
};

struct tagPreViewGift_ToLua
{
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   ���� */
	int MediaGiftID;                    
};


struct tagMediaGiftDef_ToLua
{
	int MediaGiftID;                    
	char GiftDesc[LVGIFT_NAME_LEN];       	/*   ����� */
	tagMediaGiftItems_ToLua SelectAllGift;  /*   �ػ�ȡ���� */
	tagMediaGiftItems_ToLua SelectOneGift;  /*   Nѡ1��ѡ���Ի�ȡ���� */
	tagPreViewGift_ToLua PreViewGift;		/*   ��ý�����Ԥ�� */
};

struct tagLtrGiftItem_ToLua
{
	int ItemID;                         
	int ItemNum;                        
	int Rate;                            	/*   �õ�������ߵĸ��ʣ�������ʣ� */
};

struct tagLtrGiftItems_ToLua
{
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   ���� */
	tagLtrGiftItem_ToLua GiftItems[MEDIAGIFT_OPTIONS_ITEM_MAX];
};

struct tagNextLtrGift_ToLua
{
	char Desc[LVGIFT_DESC_LEN];           	/*   ���� */
	int LtrID;                          
};

struct tagLtrGiftDef_ToLua
{
	int LtrID;                          
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   ������� */
	unsigned char MaxTime;                         	/*   һ��ѭ���������� */
	unsigned char RangeTime;                       	/*   ��ǰ��齱��Χ�������ǰN�γ齱��Χ�� */
	tagLtrGiftItems_ToLua LtrGiftItems;                    	/*   �齱�����õ��� */
	tagNextLtrGift_ToLua NextLtrGift;                     	/*   ��һ�γ齱 */
	unsigned char EverLoop;                        	/*   �Ƿ�����ѭ����1 ����ѭ����0 ֻ��ѭ��1�� */
};

struct tagBuChangDef_ToLua
{
	int Day;                             	/*   ��ʧ����*/
	int Bull;                            	/*   ��������*/
	tagGiftItem_ToLua GiftItem[MAX_BUCHANG_ITEM];      	/*   ��������*/
	int Modulus;                         	/*  Ver.2090 ���鲹��ϵ��|�ٷֱ�*/
};
struct tagArmBuChangDef_TuLua
{
	short Lvl;                             	/*   �ȼ���|�ȼ�/10,��0��ʼ */
	short Career;                          	/*   ְҵ */
	int ItemID[MAX_ARM_BUCHANG_NUM];    
};

struct tagPosition_ToLua
{
	tagPosition_ToLua( int nx, int ny )
	{
		x = nx;
		y = ny;
	}
	int x;
	int y;
};


tagMediaGiftDef_ToLua* GetMediaGiftDef( int nGiftId );

tagLtrGiftDef_ToLua* GetLtrGiftDef( int nGiftId );

tagBuChangDef_ToLua* GetBuChangDef( int nDay );

tagArmBuChangDef_TuLua* GetArmBuChangDef( int nLvl, int nCareer );

#define MEDIAGIFT_OPTIONS_ITEM_MAX_TOLUA MEDIAGIFT_OPTIONS_ITEM_MAX
#define CLAN_GIFT_TOUCH_EXP_LEVEL_TOLUA CLAN_GIFT_TOUCH_EXP_LEVEL
#define MAX_BUCHANG_ITEM_TOLUA MAX_BUCHANG_ITEM

#pragma pack()
//tolua_end

class GameWizard//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameWizard>, public IGameWizard
{//tolua_export
public:
	GameWizard(void);
	~GameWizard(void);
public:
	//tolua_begin 
	// ���������ȡ��������
	void sendAwardRequest( int nAwardID );
	// �������������ȡ�������
	void sendGiftRequest( int nLv, int nGiftId, int nType );	
	//tolua_end

	// ������������ص���Ϣ
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

public:	
	// ���������Ұ
	virtual void objEnterView(GameActor *actor);
	// �����뿪��Ұ
	virtual void objLeaveView(GameActor *actor);
	// ʹ�õ���
	virtual void useContainerItem( int itemId, int listType, int GridIdx );
	// ��������NPC
	void updateQuestNpc( int nQuestState, int nQuestId, int nRecycleID );
	// ������ɴ���
	void springByFinishQuest( int nQuestId );
	// �����ȡ����
	void springByGetQuest( int nQuestId, int nRecycleID );
	// �����ύ����
	void springByQuestCommit( int nQuestId );
	// ����������
	void springByQuestDrop( int nQuestId );
	// ��������
	void springByLevelUp( int nLv );
	// ���������Ұ����
	void springByObjectEnter( int objType, int objResId, int actorId, int dis );
	// ���볡������
	void springByEnterMap( int mapId );
	// ѧϰ���ܴ���
	void springByStudySkill( int skillId );
	
	// ��ȡ��������������Ϣ
	void setSetp( const char* szStep );
	// ��ȡ������Ϣ
	char* getSetp();
	//tolua_begin
	// ���������Ϣ
	void zeroSetp();
	// ������
	void doStep( int nStep );

	void undoStep( int nStep );
	// �����Ƿ��Ѿ������
	bool isDo( int nStep );
	// �ر�NPC�Ի�����
	void hideNpcTalkByResId( int nResId );
	//��ʾNPC�Ի�����
	void showNpcTalkByResId( int id );
	// ɾ��NPC�Ի�����
	void delNpcTalkByActorId( int id );
	// ����NPC�Ի�����
	void setNpcTalkByActorId( int nId, const char* szTalk, const char* szDrawBack, bool bShowOnce, int nOffsetX, int nOffsetY, 
						const char* szTexPath, bool bShowTex, int nTexOffsetX, int nTexOffsetY, int nDoStep = 0, 
						int nDuration = 10000 );


	// ɾ������ĶԻ�����
	void delActorTalkByActorId( int nActorId );
	// ɾ������ĶԻ�����
	void delActorTalkByActorId( GameActor* pActor );


	// ���ù���ĶԻ�����
	void setActorTalkByActorId( int nActorId, const char* szTalk, const char* szDrawBack, bool bShowOnce, int nOffsetX, int nOffsetY, 
							const char* szTexPath, bool bShowTex, int nTexOffsetX, int nTexOffsetY, int nDoStep = 0, 
							int nDuration = 10000 );

	// ����ĳЩ���͹���Ի�����
	void setGroupActorsByResId( int nResId, const char* szTalk, const char* szDrawBack, bool bShowOnce, int nOffsetX, int nOffsetY, 
		const char* szTexPath, bool bShowTex, int nTexOffsetX, int nTexOffsetY, int nDoStep = 0, 
		int nDuration = 10000 );

	// ɾ��ĳЩ���͹���Ի�����
	void delGroupActorsByResId( int nResId );
	// ɾ�����е������
	void delAllDropItemPop( );

	// ���÷����Ի�����
	void setFlyMonsterTalk( int nActorId, const char* szTalk, const char* szDrawBack, const char* szTexPath );

	const tagContOnlineGiftDef_ToLua* getContOnlineGiftDef( int nID );

	tagPosition_ToLua getPlayTopWindowPos( int actorId );
	//tolua_end

	// ���ù���ĶԻ�����
	void setActorTalkByActorId( GameActor* pActor, const char* szTalk, const char* szDrawBack, bool bShowOnce, 
		int nOffsetX, int nOffsetY, const char* szTexPath, bool bShowTex, int nTexOffsetX, 
		int nTexOffsetY, int nDoStep = 0, int nDuration = 10000 );

public:
    //tolua_begin 
    // ���ÿ�ս�������
    int getOneDayGiftNum()
	{ 
		return (int)m_vectOneDayGift.size(); 
	}

    // ��õȼ���������
    int getLVGiftNum(){ return (int)m_vectLVGift.size(); }
    // ���ÿ�ս�����Ϣ
    tagOneDayGift_ToLua* getOneDayGiftDef( int nIndex );
    // ��õȼ�������Ϣ
    tagLVGift_ToLua* getLVGiftDef( int nIndex );
    // ����������ȡ������Ϣ
    tagRoleGiftInfo_ToLua* getRoleGiftInfo();
    // ���ʣ��ĵȴ�ʱ��
    int getLeavingTime( int nTotalTime );
    // �ܷ���ȡ����
    bool canGetAwardGift();
    // �Ƿ���ʱ��
    bool isToday( int nTime );
    // ������ҵ�½ʱ��
    void setRoleLoginTm( int nLoginTm )
	{ 
		m_nRoleLoginTm = nLoginTm; 
	}
	tagMiscContOnlineInfo_ToLua* getContOnlineInfo();
	tagAccGiftInfo_ToLua* getAccGiftInfo();

	//��ʼ�����
	void InitGiftData();
	//��ֵ���
	tagPropGiftInfo_ToLua* getPropGiftInfo();
	// �����������
	int getContiNum();
	tagContiGroup_ToLua* getContiData( int nIdx );
	const tagOnlineGiftGroup_ToLua* getOnlineGiftGroupDef( int nID );
	tagGiftOne_ToLua* getGiftInfo( int nID,int nDay );
	// CDKey ���
	void sendCDKeyGiftRequest( const char* szCDKey );

	bool CanShowPreCreateGift();

	bool CanGetPreCreateGift();

	bool IsVipNetBarUser();

	int GetVipNetBarGiftNum();

	tagOneDayGift_ToLua* GetVipNetBarGift(int nIdx);

	int getLtrGiftCount(int itemId);

	int GetClanOneDayGift();

	tagOneDayGift_ToLua* GetClanOneDayGift( int nIdx );

	bool IsAlreadGetClanDayGift( int nIdx );

	void UpdateDirEntity( int nId );

	void HideDirEntity();

	// ˢ����Ӫ���
	void RequestReloadAccgift();

	const char* uLongLongToString(tdr_ulonglong uid)
	{
		tdr_longlong id;
		memcpy(&id,&uid,sizeof(id));
		_i64toa_s(id,m_szID,sizeof(m_szID),10);
		return m_szID;
	}
	int compareValue( tdr_ulonglong Val1, tdr_ulonglong Val2 );

	void requestPropsGift( int id, tdr_ulonglong cash );
	
	// ����ǩ������
	const tagContCoverGift_ToLua* getContCoverGiftDef( int nDays );
	void  contGiftCheckIN( int nDays );
	int   getContCoverMaxDays();
	const tagNewRoleGift_ToLua* getNewRoleGiftDef( int nID );
	void  getNewRoleGift( int nID );
	// MT���
	void  sendAwardMTGift();
    //tolua_end
	void LoadOneDayGift();

	bool isCanGetClanGift();

	bool IsAlreadyGetOneDayGift( int giftId );

private:
	char m_szStep[MAX_GAMEWIZARD_STEP];
	int                             m_nRoleLoginTm;
	tagRoleGiftInfo                 m_RoleGiftInfo;
	tagMiscContOnlineInfo			m_CountOnlineInfo;
    std::vector<tagOneDayGift*>     m_vectOneDayGift;
    std::vector<tagLVGift*>         m_vectLVGift;
	tagAccGiftInfo					m_AccGiftInfo;		//�������
	tagPropGiftInfo					m_PropGiftInfo;		//��ֵ���
	std::vector<tagContiGroup>		m_vContiGroup;		//�����������
	char							m_PreCreateGift;
	bool							m_bShowPreCreate;
	int								m_nLastRequestTime;

	std::vector<tagOneDayGift*>		m_vVipBarGift;		//��Ȩ�����������߽���
	std::vector<tagOneDayGift*>		m_vectClanGift;		//�����Ա����

	int								m_UserType;

	ROLELTRINFO						m_RoleLtrInfo;

	Ogre::Entity*					m_pDirEntity;
	char							m_szID[24];
};//tolua_export
