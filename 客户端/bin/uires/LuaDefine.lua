--����ȫ�ֱ�������������װ�ľֲ�����
DEFAULT_UI_WIDTH = 1024;
DEFAULT_UI_HEIGHT = 768;
g_monsterPinZhi = { [0] = "΢", [1] = "��", [2] = "��", [3] = "��", [4] = "Ӳ", [5] = "ϡ", [6] = "��", [7] ="BOSS", [8]= "��" };
-----------------------------------------------------------��ʯϵͳ��ʾ-���-------------------------------------------------------------
--��Դ
DEFAULT_NENGYUAN_TASKID = 1085;
--��ʯ
DEFAULT_JINGSHI_TASKID = 1086;
--����
DEFAULT_XISHOU_TASKID = 1087;
-----------------------------------------------------------��������������ʾ-���----------------------------------------------------------
DEFAULT_WORLD_SEND_GUID_TASKID = 3131;
--------------------------------------------------------------�����鱨����ս���----------------------------------------------------------
CLAN_FLAG_SPY_AVAILABLE_ONE = 1
CLNA_FLAG_TASK_AVAILABLE_TWO = 3
CLNA_FLAG_TASK_AVAILABLE_THREE = 7
CLAN_FLAG_TASK_SPY = 2
CLAN_FLAG_TASK_SPY_ANTI = 4
CLAN_SPY_LEVEL_LIMIT_MIN = 20
--------------------------------------------------------------�ͽ��GroupID--------------------------------------------------------------
TASK_SJ_GROUPID = 133
---------------------------------------------------------------�ֽ����---------------------------------------------------------------
--�򿪷ֽ����ĵȼ�����
CONTAINER_RESOLVE_LEVEL_LIMIT_MIN = 20;
--------------------------------------------------------------------------------------------------------------------------------------

---------------------------------------------------------------��ʯ���----------------------------------------------------------------
GEM_START_MAX = 4;
GEM_LEVEL_MAX = 7;
GEM_CHARGE_LEVEL_MIN = 3;
GEM_CHARGE_LEVEL_MAX = 15;
GEM_RESOLVE_LEVEL_MIN = 4;
--------------------------------------------------------------------------------------------------------------------------------------

-----------------------------------------------------------------ս������ͼ--------------------------------------------------------------
MAX_STRONG_NUM = 6
----------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------����ұ������ͼMapID------------------------------------------------
CLAN_EVERYDAY_PWORLD_MAP_ID_LUA = 60100
--------------------------------------------------------------------------------------------------------------------------------

-------------------------------------------------------------����һع�����������------------------------------------------------
RETURN_GIFT_MAX_DAY = 30
--------------------------------------------------------------------------------------------------------------------------------

-- ��ʯЧ��
GEM_EffECT_STAR							= 0;	-- ��Ʒ��(��)��ʯ
GEM_EffECT_FIGHT						= 1;	-- ��ս�����ȼ���ʯ
GEM_EffECT_LEVEL						= 2;	-- ��װ���ȼ���ʯ
GEM_EffECT_INLAY						= 3;	-- ��Ƕ��ʯ

-- ��¼ʧ��ԭ��
AUTH_REASON_PASSWD_ERROR				= 1; 	-- /* ���벻�Ϸ� */
AUTH_REASON_NO_ACCOUNT					= 2; 	-- /* û������˺� */
AUTH_REASON_FREEZE						= 3; 	-- /* �˺ű����� ���˺Ų����� */
AUTH_IFM_ACCOUNT_ERROR					= 4; 	-- /* ����ʱ,�û������Ϸ� */
AUTH_IFM_PASSWD_ERROR					= 5; 	-- /* ����ʱ,���벻�Ϸ� */
AUTH_REASON_FREEZE_TRYMAX				= 6; 	--/* ���볢�Դ������ඳ���˺� */
AUTH_REASON_FREEZE_CHECKTOOL			= 7; 	--/* ��鹤�߶����˺� */
AUTH_IFM_CERTID_ERROR					= 11; 	--/* ��������֤����ȷ */
AUTH_IFM_AAS_ERROR						= 12; 	-- /* �û����ڷ������û�,�Ѿ������һ�������� */
AUTH_IFM_NOUSE_SERVICE					= 14; 	--/* �޷�ʹ�÷��� */
AUTH_IFM_ACCOUNT_EXSIT					= 15; 	-- /* �˻��Ѿ����� */
AUTH_IFM_ACCOUNT_ACTIVATION				= 16; 	-- /* �˻��Ѿ�������Ϸ */
AUTH_IFM_ACCOUNT_REGISTER_ERROR			= 17; 	-- /* �û�ע��ʧ�� */
AUTH_IFM_ACCOUNT_VALID_ERROR			= 18; 	-- /* ��֤����ʧ�� */
AUTH_IFM_GET_ACCOUNT_INFO_ERR			= 20; 	-- /* ��ȡ�˻���ϸ��Ϣʧ�� */
AUTH_IFM_GAME_REGION_ERROR				= 21; 	-- /* ��Ϸ������Ų���ȷ */
AUTH_IFM_GAME_ACTIVE_ERROR				= 22; 	-- /* ������Ϸ����ʧ�� */
AUTH_IFM_USE_IDCARD_ERROR				= 30; 	-- /* �û����֤�Ѿ����� */
AUTH_IFM_USE_IDCARD_REGIST_TIMES_ERROR	= 31; 	-- /* ���û�ע�����֤����50�� */
AUTH_IFM_AASUSE_IDCARD_EXIST_ERROR		= 32; 	-- /* �������û�ע�����֤�Ѵ��� */
AUTH_IFM_USE_IDCARD_FORMAT_ERROR		= 39; 	-- /* ��������֤��ʽ */
AUTH_IFM_NEED_REGIST_CODE_ERROR			= 40; 	-- /* �ڲ��ڼ䣬��Ҫ������,��Ҫ����������ĶԻ��� */
AUTH_IFM_REGIST_CODE_ERROR				= 41; 	-- /* �����벻��ȷ���޷�������Ϸ */
AUTH_IFM_ACCOUNT_REGISTER_SUCCES		= 42; 	-- /* �˺�ע��ɹ� */
AUTH_IFM_SYSTEM_BUSY					= 9999; -- /* ϵͳ��æ  */

-- ί���Ƿ���Է���
CAN_DISTRIBUTE		= 1;
CAN_NOT_DISTRIBUTE	= 2;

-- �Ա���ඨ��
GENDER_NO		= 0;
GENDER_MAN		= 1;
GENDER_WOMAN	= 2;

-- ��ֵ
VK_BACK		= 8;
VK_DELETE	= 46;
VK_ESC		= 27;
VK_SPACE	= 32;
VK_UP		= 38;  
VK_DOWN		= 40;
-- ��Ʒ����
ITEM_TYPE_TASK	= 3;
ITEM_NONE_BIND	= 0;

-- ��Ϣ��ʾ ����
CHATTIPS_TYPE			= 3;	-- ��Ϣ���� ��������ʾ

SYS_TIP				= 0;	--/* ��ʾ��Ϣ */
SYS_SVR				= 1; 	--/* ϵͳ��Ϣ */
SYS_COMBAT			= 2; 	--/* ս����Ϣ */
SYS_MIDDLE			= 3;	--/* �������й�����ʾ��Ϣ */
SYS_MIDDLE2			= 4;	--/* ������ʾ��Ϣ */
SYS_BULLETIN		= 5;	--/* ������Ϣ */
SYS_CHAT			= 6; 	--/* ����Ƶ�� */
SYS_POP_WINDOW		= 7;	--/* ����������ʾ��Ϣ */ 
SYS_BUGLE			= 8;	--/* С���� */
SYS_MSG_STUFF		= 9; 	--/* ���� */
SYS_MSG_TASK		= 10; 	--/* ������Ʒ */
SYS_MSG_ITEM		= 11; 	--/* ���� */
SYS_MSG_ARM			= 12; 	--/* װ�� */
SYS_MSG_CHAT_TIP	= 13;	--/* �������ʾ��Ϣ */
SYS_CLAN			= 14; 	--/* ����Ƶ�� */
SYS_TEAM			= 15;	--/* ���Ƶ�� */
SYS_ACTIVE			= 16;	--/* �Ƶ�� */
SYS_CURR			= 17;	--/* ʼ�յ�ǰƵ��ҳ��ʾ */
SYS_SHOW 			= 18; 	--/* ��ҫƵ�� */

--ȫ�ֱ���
g_nCAREER_NON     = 0;
g_nCAREER_WARRIOR = 1;  --սʿ
g_nCAREER_MAGICOR = 2;  --��ʦ
g_nCAREER_GUNER   = 3;	--ǹ��
g_nCAREER_CHURCH  = 4;	--��ʦ
g_nCAREER_WARRIOR_SINGEL = 17
g_nCAREER_WARRIOR_DOUBLE = 33
g_nCAREER_MAGICOR_SINGEL = 18
g_nCAREER_MAGICOR_DOUBLE = 34
g_nCAREER_GUNER_SINGEL = 19
g_nCAREER_GUNER_DOUBLE = 35
g_nCAREER_CHURCH_SINGEL = 20
g_nCAREER_CHURCH_DOUBLE = 36

MONEY_TYPE_MONEY_NAME			= "����";   -- ��Ϸ����ͨ����
MONEY_TYPE_BULL_NAME			= "����";   -- ��Ϸ�а󶨻���
MONEY_TYPE_GODCOIN_NAME			= "���";   -- ��Ϸ����ͨ��ֵ��
MONEY_TYPE_BIND_GODCOIN_NAME	= "��ȯ";   -- ��Ϸ�а󶨳�ֵ��
MONEY_TYPE_GODCOIN				= "���";	-- ���
MONEY_TYPE_CONTRI_NAME			= "���׶�";

t_raceList =
{
	[g_nCAREER_WARRIOR]		= "սʿ",
	[g_nCAREER_MAGICOR]		= "��ʦ",
	[g_nCAREER_GUNER]		= "ǹ��",
	[g_nCAREER_CHURCH]		= "��˾",

	[g_nCAREER_WARRIOR_SINGEL]	= "�ػ���",
	[g_nCAREER_WARRIOR_DOUBLE]	= "������",
	[g_nCAREER_MAGICOR_SINGEL]	= "������",
	[g_nCAREER_MAGICOR_DOUBLE]	= "����ʦ",
	[g_nCAREER_GUNER_SINGEL]	= "��ɱ��",
	[g_nCAREER_GUNER_DOUBLE]	= "������",
	[g_nCAREER_CHURCH_SINGEL]	= "ʥ����",
	[g_nCAREER_CHURCH_DOUBLE]	= "Ԧ��ʦ",
};

FONTSYTLE_NORMAL	= 0;	--��ͨ
FONTSYTLE_SHADOW	= 1;	--��Ӱ
FONTSYTLE_BORDER	= 2;	--���
FONTSYTLE_EMBOLDEN	= 3;	--�Ӵ�
TEAM_POSITION_MEMBER	= 0; -- ��ʾ����
TEAM_POSITION_CAPTAIN	= 1; -- ��ʾ�ӳ�

CLIENT_TASK_STAT_CANNOTACCEPT = 4;

function GetMoneyDesc( nType )
	if nType == ATTR_ID_MONEY then
		return MONEY_TYPE_MONEY_NAME;
	elseif nType == ATTR_ID_BULL then
		return MONEY_TYPE_BULL_NAME;
	elseif nType == ATTR_ID_BINDGODCOIN then
		return MONEY_TYPE_BIND_GODCOIN_NAME;
	elseif nType == ATTR_ID_GODCOIN then
		return MONEY_TYPE_GODCOIN;
	else
		return MONEY_TYPE_MONEY_NAME;
	end
end

ATTR_ID_MONEY		= 20;	-- ����
ATTR_ID_BULL		= 21;	-- "����"
ATTR_ID_GODCOIN		= 60;	-- ���
ATTR_ID_BINDGODCOIN	= 61;	-- ��ȯ	
ATTR_ID_STASHMONEY	= 32;	-- �ֿ���ȯ	
ATTR_ID_STASHBULL	= 65;	-- �ֿ�����
ATTR_ID_CONTRI		= 66;	-- ���ṱ�׶�
ATTR_ID_SPARTA_GLORY	= 84;	-- ����������

LIST_TYPE_XIAN	= 7; 		--/* ������λ */
LIST_TYPE_SHI	= 8;		--/* ������λ */
PET_SKILLGRID_MAX	= 10; -- ���ܸ�
PET_ARMGRID_MAX		= 6;  -- װ����
FAIRY_MAX_NUM		= 10; -- ���������λ 
GSACT_IN_MORPH	= 15;
GSACT_STALL		= 12;

-- ����״̬
FAIRY_STAT_CAMP		= 1;  --/* ���� */ 	
FAIRY_STAT_BACK		= 2;  --/* �ٻ� */
FAIRY_STAT_EVOLVE	= 4;  --/* ���� */
FAIRY_STAT_DROP		= 8;  --/* ���� */

FAIRY_MAX_SKILL		= 4;  --/* ���鼼������ */	

-- װ����ɫö��
GEM_COLOR_RED = 0; 
GEM_COLOR_BLUE = 1;
GEM_COLOR_GREEN = 2;
GEM_COLOR_COLOR = 3;

-- ��ϵ��TIPS����
CONTACT_TYPE_MAIL			= 1;
CONTACT_TYPE_MARKET			= 2;
CONTACT_TYPE_SENDITEM_MAIL	= 3;

t_GemColorDefind = 
{
	{ "��ɫ",GEM_COLOR_RED }, 
	{ "��ɫ",GEM_COLOR_BLUE }, 
	{ "��ɫ",GEM_COLOR_GREEN }, 
	{ "���",GEM_COLOR_COLOR }, 
};

USE_TARGET_PET = 1; 	--/* Ŀ���ǳ��� */			   

-- Ĭ����Ʒͼ��·��
DEFAULT_ITEM_PATH		= "uires\\TuPianLei\\WuPin\\0000000.tga";
DEFAULT_SKILL_PATH		= "uires\\TuPianLei\\JiNeng\\JiNeng\\0000.tga";
DEFAULT_MONSTER_PATH	= "uires\\TuPianLei\\TouXiang\\GuaiWuTouXiang\\000000.tga";
DEFAULT_BUFF_PATH		= "uires\\TuPianLei\\JiNeng\\Buff\\0000.tga";

function GetGemColorTable()
	return t_GemColorDefind;
end

function GetGemColorName( nColor )
	for i = 1, table.getn( t_GemColorDefind ) do
		if t_GemColorDefind[i][2] == nColor  then
			return t_GemColorDefind[i][1]; 
		end
	end
	return "";
end

--@Used: �������Ŀ�ʼλ�á�����������
startx					= 584;
RIDE_TYPE				= 1;
COLLECT_TYPE			= 2;

PET_HOUSECARL_TYPE		= 3;
PET_FAIRY_TYPE			= 4;
ITEM_USE_TYPE			= 5;
TRUMP_TYPE_PRE			= 6;

CHANNEL_TYPE			= 7;
SECONDARYSKILL_RESOLVE_TYPE	= 8;	--����ֽ⼼��
SECONDARYSKILL_OTHER_TYPE = 9;		--���������
PREPARE_TYPE			= 10;
RESOLVE_TYPE			= 11;

-- С���ȵ�ID
CHAT_ITEM_ID					= 5050000;
--@Desc:itembutton type
ARMPROCE_TYPE_SRC				= 11;
ARMPROCE_TYPE_STUFF_STAR		= 12;
ARMPROCE_TYPE_STUFF_GEM			= 13;
ARMPROCE_TYPE_INLAY				= 14;
ARMPROCE_TYPE_INLAY_GEM			= 15;
ARMPROCE_TYPE_PICK				= 16;
ARMPROCE_TYPE_PICK_GEM			= 17;
ARMPROCE_TYPE_IDENTIFIABILITY	= 18;
ARMPROCE_TYPE_PICK_INSURE		= 19;
ARMPROCE_TYPE_STUFF_FIGHT		= 20;
ARMPROCE_TYPE_STUFF_LEVELUP		= 21;
ARMPROCE_TYPE_GEMLEVEL			= 22;
ARMPROCE_TYPE_GEMPICK			= 23;

--@Desc:װ���ӹ�
ARM_ACT_STAR			= 0;	--/* װ��Ʒ������ */
ARM_ACT_LEVEL			= 1;	--/* װ���ȼ����� */
ARM_ACT_FIGHT			= 2;	--/* װ��ս�������� */
ARM_ACT_IDENTIFIABILITY = 3; 	--/* װ������ */
GEM_ACT_LEVEL_UP		= 4;	--/* ��ʯ���� */
GEM_ACT_INLAY			= 5;	--/* ��ʯ��Ƕ */
GEM_ACT_PICK			= 6;	--/* ��ʯժȡ */
SAVEITEMTYPE			= 1000;	--/* ���շ����� */
MAX_STUFF_ARMPROCE		= 10;
ARMPROCE_SRC			= 11;	-- װ������
ARMPROCE_SAFEITEM		= 12;	-- ���շ�����
ARM_STAR_LIMIT			= 12;


MAXEQUIPPOS				= 22;	-- ���װ��λ

GEM_EFFECT_STAR  = 0; --/* ��Ʒ��(��)��ʯ */
GEM_EFFECT_FIGHT = 1; --/* ��ս�����ȼ���ʯ */
GEM_EFFECT_LEVEL = 2; --/* ��װ���ȼ���ʯ */
GEM_EFFECT_INLAY = 3; --/* ��Ƕ��ʯ */
GEM_INLAY = 10;

UI_TYPE_UPGRADE_CLAN_CITY			= 18; 	-- /* �������� */
UI_TYPE_UPGRADE_CLAN_CITY_BUILDING	= 19; 	-- /* �������н��� */
UI_TYPE_UPGRADE_CLAN				= 20; 	-- /* �������� */

UI_TYPE_UPGRADE_TECH_CENTER			= 21; 	--/* �����Ƽ����� */
UI_TYPE_UPGRADE_COLLECTION_CENTER	= 22; 	--/* �����ɼ����� */
UI_TYPE_UPGRADE_FACTORY				= 23; 	--/* �������� */
UI_TYPE_UPGRADE_FARM				= 24; 	--/* �������� */
UI_TYPE_UPGRADE_WALL				= 48; 	--/* �����Ƿ� */


CLAN_BUILDING_CIVIL			= 0; 	-- /* ������ */
CLAN_BUILDING_FINANCE		= 1; 	-- /* ����� */
CLAN_BUILDING_ARM			= 2; 	-- /* ������ */
CLAN_BUILDING_TECHNOLOGY	= 3; 	-- /* �Ƽ��� */
CLAN_CITY_MAP_ID		= 60000;
BEACH_MAP_ID			= 4403;
BRAVEISLAND_MAP_ID		= 4402;
CARNIVAL_MAP_ID			= 20100;

MOVE_DURATION				= 0.5;    -- /* ���ڵĳ����ƶ�ʱ�� */

--@desc:��Ӧ��ǩ��嵱ǰ��x,yλ��
--@used:�л���ǩʱ����λ��ͬ��
EquipRealX	= 0;
EquipRealY	= 0;
SkillRealX	= 0;
SkillRealY	= 0;
PetRealX	= 0;
PetRealY	= 0;
--@desc:������������趨
MAX_SKILL_FITTINGS	= 3;
NONEFITTING			= -1;
FITTINGSLOT			= NONEFITTING;
FITTINGID			= NONEFITTING;
--@desc:�������ͣ�����
SKILL_TYPE			= 0;
DIATHESIS_TYPE		= 1;
TRUMP_SKILL_TYPE = 1002;
TMP_TANK_SKILL_TYPE = 1003;
RUNE_SKILL_TYPE		= 1004;
--@desc:���������趨
SHITYPE					= 1;  -- ����
XIANTYPE				= 0;  -- ����
--@desc:��ӦEquipFrame.lua��table:t_RoleAttrInfo
--@used:��ȡ��ɫ�����¸�������

NONE			= 0;
ONEPAGE			= 1;
MAIL_FLAG_SYSTEM	= 1;
MAIL_FLAG_READ		= 2;

MAXMAILITEM		= 8;

MAX_SKILL_ICON	= 10
MAXSTALLITEM    = 30;
MAXSTALLPET		= 10;

STALL_TYPE_VIEW	= 2;

MAIL_HEAD_FIRST_PAGE	= 0; 	--/* ��һҳ */
MAIL_HEAD_NEXT_PAGE		= 1; 	--/* ��ҳ */
MAIL_HEAD_CURRENT_PAGE	= 2; 	--/* ��ǰҳ */
MAIL_HEAD_PREV_PAGE		= 3; 	--/* ��ҳ */

t_ItemButtonCheckIndex = {};	--/* ��¼ItemTemplate���е���Ʒ{{����1,��λ1},{����2,��λ3}}

--@desc:����ָ�������������;���
MAX_PET_XIAN	= 10;
MAX_PET_SHI	= 5;

--@Desc:ѡ�е�����ID
nCurSelectQuestId	= 0;
nCurSelectRecycleId = 0;

ATTR_ID_LILIANG			= 4; 	--/* ���� */
ATTR_ID_ZHILI			= 8; 	--/* ���� */		
ATTR_ID_TILI			= 12; 	--/* ���� */		
ATTR_ID_JINGSHEN		= 13; 	--/* ���� */		
ATTR_ID_FANGYU			= 10; 	--/* ���� */		
ATTR_ID_SHANBI			= 9; 	--/* ���� */		
ATTR_ID_MINGZHONG		= 11; 	--/* ���� */

ATTR_ID_PHSIC_ATK		= 35; 	--/* �������� */
ATTR_ID_ICE_ATK			= 36; 	--/* ��Ԫ�ع����� */
ATTR_ID_BASE_ATTR_HEAVY = 73;	-- /* ������ */
ATTR_ID_BASE_ATTR_DEFHEAVY = 74;	-- /* �����ֿ� */
ATTR_ID_HEAVY_VAL = 75;	-- /* �����˺���ֵ����(���ӵİٷֱ�) */

ATTR_ID_PHSIC_DEF		= 41; 	--/* ��������� */
ATTR_ID_ICE_DEF			= 42; 	--/* ��Ԫ�ط����� */
ATTR_ID_FIRE_DEF		= 43; 	--/* ��Ԫ�ط����� */
ATTR_ID_THUNDER_DEF		= 44; 	--/* ��Ԫ�ط����� */
ATTR_ID_LIGHT_DEF		= 45; 	--/* ��Ԫ�ط����� */
ATTR_ID_NIGHT_DEF		= 46; 	--/* ��Ԫ�ط����� */

ATTR_ID_DEF_PHSIC_HURT		= 47; 	--/* ����ֿ��� */
ATTR_ID_DEF_ICE_HURT		= 48; 	--/* ��Ԫ�صֿ��� */
ATTR_ID_DEF_FIRE_HURT		= 49; 	--/* ��Ԫ�صֿ��� */
ATTR_ID_DEF_THUNDER_HURT	= 50; 	--/* ��Ԫ�صֿ��� */
ATTR_ID_DEF_LIGHT_HURT		= 51; 	--/* ��Ԫ�صֿ��� */
ATTR_ID_DEF_NIGHT_HURT		= 52; 	--/* ��Ԫ�صֿ��� */

ATTR_ID_HEAVY			= 53; 	--/* ������ */
ATTR_ID_HIT				= 54; 	--/* ���� */
ATTR_ID_FLEE			= 55;	--/* ���� */
ATTR_ID_HPADD			= 56; 	--/* �����ظ� */
ATTR_ID_MPADD			= 57; 	--/* ħ���ظ� */
ATTR_ID_HP				= 0; 	--/* HP */
ATTR_ID_HPMAX			= 1;
ATTR_ID_MP 				= 2; 	--/* MP */
ATTR_ID_FIGHT			= 33; 	--/* ս���� */
ATTR_ID_SKILLPOINT		= 19; 	--/* ���ܵ� */
ATTR_ID_DAMAGE			= 37;	--/* �ƻ��� */
ATTR_ID_NOFIGHT_HPADD	= 38;	-- 	/* ��ս��״̬HP�ָ� */
ATTR_ID_NOFIGHT_MPADD	= 39;	-- 	/* ��ս��״̬MP�ָ� */
ATTR_ID_BADSTATUS_DEF	= 40;	--	/* ����״̬���� */
ATTR_ID_MPMAX			= 3;
ATTR_ID_VIM				= 5;
ATTR_ID_VIMMAX			= 17;
ATTR_ID_EXP				= 7;
ATTR_ID_MINJIE			= 9;
ATTR_ID_WATER_DEF		= 22;
ATTR_ID_WATER_ATK		= 23;
ATTR_ID_FIRE_ATK		= 24;
ATTR_ID_THUNDER_ATK		= 25;
ATTR_ID_LIGHT_ATK		= 26;
ATTR_ID_NIGHT_ATK		= 27;
ATTR_ID_HIT_RATE		= 28;
ATTR_ID_FLEE_RATE		= 29;
ATTR_ID_MSPD			= 30;
ATTR_ID_ICE_HURT		= 68;	-- /* ħ���˺� */
ATTR_ID_HEAL_HP_ADD		= 69;	-- /* ���Ƽ����� */
ATTR_ID_HEAL_HP_MUL		= 70;	-- /* ���Ƴ����� */
ATTR_ID_ATKEE_HEAL_HP_ADD = 71;	-- /* �����Ƽ����� */
ATTR_ID_ATKEE_HEAL_HP_MUL = 72; -- /* �����Ƴ����� */
ATTR_ID_PHSIC_HURT			= 31; 

ATTR_ID_BATTLE_GLORY	= 76;	-- /* ս������ */
ATTR_ID_AOSHAN_BRAND	= 77; 	-- /* �������� */
ATTR_ID_LIFT_BRAND		= 78; 	-- /* ��ɳ���� */

ATTR_ID_SCHOOL_VAL		= 87;	-- /* ʦͽ���� */
ATTR_ID_ZHENGYI			= 89;   -- /* ����� */

ATTR_ID_REFLECTION_HP = 79;	-- /* ���˵ȼ� */
ATTR_ID_IGNORE_DEF = 80; 	-- /* �ֿ��ȼ� */
ATTR_ID_DEF_HP = 81; 		-- /* �Ʒ��ȼ� */
ATTR_ID_PHSIC_MAX_ATK = 82; --	/* �������޹����� */
ATTR_ID_MAGIC_MAX_ATK = 83; --	/* ħ�����޹����� */
ATTR_ID_DODGE = 85; 		--/* ������ */

COLOR_WHITE			= 0;	--��ɫ
COLOR_GREEN			= 1;	--��ɫ
COLOR_BULE			= 2;	--��ɫ
COLOR_PURPLE		= 3;	--��ɫ
COLOR_RED			= 4;	--��ɫ
COLOR_BLACK			= 5;	--��ɫ
COLOR_GOLD			= 7;	--��ɫ
COLOR_YELLOW		= 8;	--��ɫ

COLOR_WEAK_GREEN	= 9;	--����
COLOR_WEAK_YELLOW	= 10;	--ǳ��
COLOR_WEAK_PURPLE	= 11;	--ǳɫ
COLOR_WEAK_BLUE		= 12;	--����

COLOR_ROSE_RED		= 13;	--õ���
COLOR_SEA_GREEN		= 14;	--����
COLOR_FRESH_GREEN	= 15;	--����
COLOR_POWDER_RED	= 16;	--�ۺ�
COLOR_VIRIDESCENCE	= 17;	--����
COLOR_WEAK_WRITE	= 18;   --����

COLOR_ITEMSTART		= 19;
COLOR_USELIFE		= 20;
COLOR_ITEMATTR		= 21;
COLOR_ITEMDESC		= 22;
COLOR_USEWAY		= 23;
COLOR_SELLPRICE		= 24;
COLOR_MONEY			= 25;
COLOR_BATTLE		= 26;
COLOR_SUITGET		= 27;
COLOR_SUITNOGET		= 28;
COLOR_ITEMKEYWORLD	= 29;
COLOR_OGRANGE		= 30;
COLOR_HP			= 31;
COLOR_MP			= 32;
COLOR_ROLEATTRDESC	= 33;
COLOR_ITEMGEMINLAY	= 34;
COLOR_DARK_GOLD		= 35;
COLOR_GRAY			= 36;

ITEM_NO_LEVELLIMIT = 16

local t_ColorList = {};
t_ColorList[COLOR_RED]			= { 202, 54,  33};
t_ColorList[COLOR_BLACK]		= { 0,	 0,   0};
t_ColorList[COLOR_WHITE]		= { 254, 249, 234};
t_ColorList[COLOR_GREEN]		= { 66,  168, 61};
t_ColorList[COLOR_BULE]			= { 61,  109, 168};
t_ColorList[COLOR_GOLD]			= { 244, 188, 36};
t_ColorList[COLOR_PURPLE]		= { 151, 45, 249 };
t_ColorList[COLOR_YELLOW]		= { 232, 226, 72};
t_ColorList[COLOR_WEAK_GREEN]	= { 39,  194, 61};
t_ColorList[COLOR_WEAK_YELLOW]	= { 160, 160, 90};
t_ColorList[COLOR_WEAK_PURPLE]	= { 252, 153, 228};
t_ColorList[COLOR_WEAK_BLUE]	= { 131, 207, 239};
t_ColorList[COLOR_ROSE_RED]		= { 232, 139, 154};
t_ColorList[COLOR_SEA_GREEN]	= { 69,  222, 195};
t_ColorList[COLOR_FRESH_GREEN]	= { 45,  232, 133};
t_ColorList[COLOR_POWDER_RED]	= { 255, 197, 207};
t_ColorList[COLOR_VIRIDESCENCE] = { 139, 232, 161};
t_ColorList[COLOR_WEAK_WRITE]	= { 255, 254, 189};

t_ColorList[COLOR_ITEMSTART]	= { 221,196,159 };
t_ColorList[COLOR_USELIFE]		= { 192,76,235 };
t_ColorList[COLOR_ITEMATTR]		= { 51,141,253 };
t_ColorList[COLOR_ITEMKEYWORLD] = { 63,193,23 };   
t_ColorList[COLOR_ITEMGEMINLAY] = { 144,142,138 };


t_ColorList[COLOR_ITEMDESC]		= { 221,196,159 };
t_ColorList[COLOR_USEWAY]		= { 121,194,255 };
t_ColorList[COLOR_SELLPRICE]	= { 221,196,159 };
t_ColorList[COLOR_MONEY]		= { 209,185,102 };
t_ColorList[COLOR_BATTLE]		= { 192,76,253 };
t_ColorList[COLOR_SUITGET]		= { 64,158,238 };
t_ColorList[COLOR_SUITNOGET]	= { 105,106,106 };
t_ColorList[COLOR_OGRANGE]		= { 245,115,0 };
t_ColorList[COLOR_HP]			= { 255, 20,2 };
t_ColorList[COLOR_MP]			= { 48,170,255 };
t_ColorList[COLOR_ROLEATTRDESC]	= { 195,155,86 };
t_ColorList[COLOR_DARK_GOLD]	= { 235,220,130 };
t_ColorList[COLOR_GRAY]			= { 110,110,110 };

function GetColor()
	return t_ColorList;
end

--��װ�ľֲ�����
local t_MidTipsList = {};
t_MidTipsList[MID_NPCSHOP_MONEYNOTENOUGH_SELLFAIL]			= "����".. MONEY_TYPE_MONEY_NAME.."����,����ʧ��!";
t_MidTipsList[MID_NPCSHOP_BULLNOTENOUGH_SELLFAIL]			= "����"..MONEY_TYPE_BULL_NAME.."����,����ʧ��!";
t_MidTipsList[MID_NPCSHOP_EXCHANGEITEMNOTENOUGH_SELLFAIL]	= "�һ������Ӳ���,����ʧ��!";
t_MidTipsList[MID_NPCSHOP_MONEYNOTENOUGH_BUYSAVEFAIL]		= "����"..MONEY_TYPE_MONEY_NAME.."����,�ع�ʧ��!";

t_MidTipsList[MID_NPCSHOP_BULLNOTENOUGH_BUYSAVEFAIL]	= "����"..MONEY_TYPE_BULL_NAME.."����,�ع�ʧ��!";
t_MidTipsList[MID_NPCSHOP_BULLNOTENOUGH_REPAIRFAIL]		= "����"..MONEY_TYPE_BULL_NAME.."����,ά��ʧ��!";
t_MidTipsList[MID_NPCSHOP_NOTNEEDREPAIR]				= "����Ҫά��!";
t_MidTipsList[MID_ARMPROCE_ARMSTAR]						= "װ��Ʒ������";
t_MidTipsList[MID_ARMPROCE_LEVEL]						= "װ���ȼ�����";
t_MidTipsList[MID_ARMPROCE_FIGHT]						= "װ��ս��������";
t_MidTipsList[MID_ARMPROCE_IDENTIFIABILITY]				= "װ�������ɹ�";
t_MidTipsList[MID_ARMPROCE_GEMLEVELUP]					= "��ʯ�����ɹ�";
t_MidTipsList[MID_ARMPROCE_GEMINLAY]					= "��ʯ��Ƕ�ɹ�";
t_MidTipsList[MID_ARMPROCE_GEMPICK]						= "��ʯժȡ�ɹ�";


t_MidTipsList[MID_ARMPROCE_NOTGEMEFFECTINLAY]			= "���Ƕ�Ӧ���͵ı�ʯ";
t_MidTipsList[MID_ARMPROCE_NOTSAMEGEMTYPE]				= "��ʯ���Ͳ�һ��";
t_MidTipsList[MID_ARMPROCE_NOTSAMELEVEL]				= "��ʯ�ȼ���һ��";
t_MidTipsList[MID_ARMPROCE_NOTGEM]						= "���Ǳ�ʯ���ܷ���";

t_MidTipsList[MID_ARMPROCE_NOTEQUIP]					= " û�з���װ��";

t_MidTipsList[MID_ARMPROCE_NOTGEMPICKITEM]				= "û��ժȡ��ʯ����Ҫ��Ʒ";
t_MidTipsList[MID_ARMPROCE_NOUPDATEGEM]					= "����Ҫ�����ı�ʯ";
t_MidTipsList[MID_ARMPROCE_NOGEMUPDATEITEM]				= "û��������ʯ����Ҫ��Ʒ";
t_MidTipsList[MID_ARMPROCE_NOTEQUIPITEM]				= "�������Ʒ����װ��";
t_MidTipsList[MID_ARMPROCE_NOTHOLDEQUIP]				= "�����װ��û�д���";


t_MidTipsList[MID_ARMPROCE_GEMPICK_FAIL]				= "��ʯժȡʧ�� ";
t_MidTipsList[MID_ARMPROCE_GEMINLAY_FAIL]				= "��ʯ��Ƕʧ�� ";
t_MidTipsList[MID_ARMPROCE_GEMLEVELUP_FAIL]				= "��ʯ����ʧ�� ";
t_MidTipsList[MID_ARMPROCE_IDENTIFIABILITY_FAIL]		= "װ������ʧ�� ";
t_MidTipsList[MID_ARMPROCE_ARMSTAR_FAIL]				= "װ��Ʒ������ʧ�� ";
t_MidTipsList[MID_ARMPROCE_LEVEL_FAIL]					= "װ���ȼ�����ʧ�� ";
t_MidTipsList[MID_ARMPROCE_FIGHT_FAIL]					= "װ��ս��������ʧ�� ";
t_MidTipsList[MID_ARMPROCET_NOTENOUGHBULL]				= ""..MONEY_TYPE_BULL_NAME.."����";

t_MidTipsList[MID_ARMPROCET_NOTARMSAFEITEM]				= "������û��Ʒ���������շ�";
t_MidTipsList[MID_ARMPROCET_FIGHTSAFEITEM]				= "������û��ս�����������շ�";
t_MidTipsList[MID_ARMPROCET_GEMSAFEITEM]				= "������û����Ƕ��";
t_MidTipsList[MID_ARMPROCET_FIGHTLIMIT]					= "װ��ս�����ȼ����ɸߵ���װ��Ʒ��";
t_MidTipsList[MID_ARMPROCET_NOTGEMPICK]					= "ȱ�ٶ�Ӧ�ı�ʯժȡ��";
t_MidTipsList[MID_ARMPROCET_NOTSTARSTUFF]				= "����Ĳ�����Ʒ�ʵĲ���";
t_MidTipsList[MID_ARMPROCET_NOTFIGHTSTUFF]				= "����Ĳ�����ս�����Ĳ���";
t_MidTipsList[MID_ARMPROCET_NOTEQUIPLEVELSTUFF]			= "����Ĳ�����װ���ȼ��Ĳ���";
t_MidTipsList[MID_ARMPROCET_NOTHOLEINLAY]				= "��װ�����޿���Ƕ�Ŀ�";
t_MidTipsList[MID_ARMPROCET_NOTHOLEEQUIP]				= "��װ�������п�װ��";
t_MidTipsList[MID_ARMPROCET_HOLEHASGEM]					= "������Ƕ";
t_MidTipsList[MID_ARMPROCET_NOTINLAYGEM]				= "��װ��δ��Ƕ��ʯ";
t_MidTipsList[MID_ARMPROCET_NOTEQUIPASKLEVELITEM]		= "����Ʒ����ƥ��ı�ʯ������";
t_MidTipsList[MID_ARMPROCET_NOTEQUIPASKPICKITEM]		= "����Ʒ����ƥ��ı�ʯժȡ��";
t_MidTipsList[MID_ARMPROCET_NOTCHOICEPICKGEM]			= "��ѡ����Ҫժȡ�ı�ʯ";
t_MidTipsList[MID_ARMPROCET_CANNOTINDENTITY]			= "װ�����ɼ���";
t_MidTipsList[MID_ARMPROCET_NOTINDENTITYEQUIP]			= "װ��δ����";
t_MidTipsList[MID_ARMPROCET_NOTNEXTITEMLEVEL]			= "װ����������";

t_MidTipsList[MID_EQUIP_NOTENOUGHEXP]					= "��ǰ����ֵ���� ����ʧ��";
t_MidTipsList[MID_TRUST_NOTENOUGHLV]					= "�ȼ�δ����ʮ��";

t_MidTipsList[MID_STALL_NOTENOUGHLV]					= "�ȼ�δ����ʮ��";
t_MidTipsList[MID_STALL_CANSTALLSTATE]					= "��̯״̬�����ϼ���Ʒ";

t_MidTipsList[MID_MAIL_NOTENOUGHLV]						= "�ȼ�δ��ʮ��";
t_MidTipsList[MID_MAIL_HASINMAILSENDITEM]				= "����Ʒ���ڴ����ʼ���Ʒ�б���";
t_MidTipsList[MID_TRUST_NOTENOUGHLVORITEM]				= "�ȼ������ǰί��������Ʒ����";
t_MidTipsList[MID_TRUST_HASTODAYLIMITFINISH]			= "�Ѵﱾ��ί�����ƴ���";

t_MidTipsList[MID_NPCSHOP_TRADENUMCANNOTNULL]			= "��Ʒ����������Ϊ��";
t_MidTipsList[MID_NPCSHOP_TRADEPRICECANNOTNULL]			= "��Ʒ���ۼ۲���Ϊ��";
t_MidTipsList[MID_NPCSHOP_TRADEBUYNUMCANNULL]			= "�������������Ϊ��";
t_MidTipsList[MID_NPCSHOP_NOTCHOICEITEM]				= "��ѡ����Ҫ�������Ʒ";

t_MidTipsList[MID_NPCSHOP_NONEEDREPAIR]					= "����ά��";
t_MidTipsList[MID_FIGHT_IN]								= "����ս��״̬";
t_MidTipsList[MID_FIGHT_OUT]							= "�뿪ս��״̬";
t_MidTipsList[MID_SHORTCUT_MPNOTENOUGH]					= "��ǰħ��ֵ����";
t_MidTipsList[MID_SHORTCUT_ENERGENOTENOUGH]				= "��ǰ��������";
t_MidTipsList[MID_SHORTCUT_NOTITEM]						= "����Ʒ������";	
t_MidTipsList[MID_SHORTCUT_CANNOTUSESPELL_BUF]			= "��ǰ״̬����ʹ�ü���";
t_MidTipsList[MID_SHORTCUT_CANNOTUSESPELL_MOVE]			= "��ǰ״̬�²���ʹ��λ�Ƽ���";	 
t_MidTipsList[MID_SHORTCUT_CANNOTUSESPELL_INMORPH]		= "����״̬����ʹ�ü���";
t_MidTipsList[MID_SHORTCUT_CANNOTUSESPELL_INSTALL]		= "��̯״̬���޷�ʩ�ż���";
t_MidTipsList[MID_PATH_FINDNONPC]						= "Ŀ��NPC���ڵ�ǰ��ͼ";
t_MidTipsList[MID_XP_COOLDOWNTIME]						= "���ܴ�����ȴ״̬";
t_MidTipsList[MID_XP_NOENOUGHMP]						= "�����MP����";
t_MidTipsList[MID_XP_NOENOUGHXP]						= "����������ܲ���";
t_MidTipsList[MID_XP_NOENOUGHCOLLECT]					= "��������ս�ⲻ��";
t_MidTipsList[MID_XP_NOINMAP]							= "ֻ���ڸ�����ʹ��";
t_MidTipsList[MID_MAIL_SENDER_NAME_NULL]				= "�ռ�����������Ϊ��,�ʼ�����ʧ��";
t_MidTipsList[MID_MAIL_TITLE_NULL]						= "�ʼ����ⲻ��Ϊ��,�ʼ�����ʧ��";
t_MidTipsList[MID_MAIL_NO_MONEY]						= "���ʲ���,�ʼ�����ʧ��";
t_MidTipsList[MID_TRADE_SELF_FIGHT]						= "����ս��״̬�����ܽ��н���";
t_MidTipsList[MID_TRADE_SELF_STALL]						= "��̯״̬���޷�����";
t_MidTipsList[MID_TRADE_SELF_FLY]						= "��ǰ״̬���ܽ��н��ף����Ժ�����";
t_MidTipsList[MID_TRADE_SELF_MORPH]						= "���ڱ���״̬�����ܽ��н���";
t_MidTipsList[MID_TRADE_SELF_TRUMP_PRE]					= "�����ٻ����飬���ܽ��н���";
t_MidTipsList[MID_TRADE_CANNOT_REPEAT_CONFIRM_TRADE_]	= "�����ظ�ȷ�Ͻ���";
t_MidTipsList[MID_TRADE_CANNOT_DIST_TOO_LONG]			= "�����Զ�޷����뽻��";
t_MidTipsList[MID_TRADE_CANNOT_NOT_CAN_TRADE_OBJECT]	= "Ŀ������޷�����";


t_MidTipsList[MID_TRADE_SELF_DEAD]						= "���Ѿ����������ܽ��н���";
t_MidTipsList[MID_TRADE_ACTOR_FIGHT]					= "�Է�����ս��״̬�����ܽ��н���";
t_MidTipsList[MID_TRADE_ACTOR_STALL]					= "�Է����ڰ�̯״̬�����ܽ��н���";
t_MidTipsList[MID_TRADE_ACTOR_FLY]						= "�Է����ڷ���״̬�����ܽ��н���";
t_MidTipsList[MID_TRADE_ACTOR_DEAD]						= "�Է��Ѿ����������ܽ��н���";
t_MidTipsList[MID_TRADE_RELATION_XIONGSHOU]				= "�Է����Լ��������������ܽ��н���";
t_MidTipsList[MID_TRADE_RELATION_SUDI]					= "�Է����Լ����޵��������ܽ��н���";
t_MidTipsList[MID_STALL_DISTANCE_LONG]					= "˫������̫Զ�����ܲ鿴�Է���̯λ��Ϣ";
t_MidTipsList[MID_STALL_SELF_FIGHT]						= "ս��״̬���޷���̯";
t_MidTipsList[MID_STALL_SELF_EMPTY]						= "û����Ʒ�ϼܣ����ܰ�̯";
t_MidTipsList[MID_STALL_SELF_PK]						= "�д�״̬���޷���̯";
t_MidTipsList[MID_MULTIMAP_AUTOFIND_FAIL]				= "�����Զ�Ѱ·����ѡ���λ��";
t_MidTipsList[MID_TRADE_DISTANCE_LONG]					= "����̫Զȡ������";
t_MidTipsList[MID_TRADE_INVITE_TIME_OUT]				= "�Է�û����Ӧ��������";
t_MidTipsList[MID_TRADE_RELATION_BLACK]					= "�Է��ں������в��ܽ��н���";
t_MidTipsList[MID_STALL_DISTANCE_LONG]					= "˫������̫Զ�����ܲ鿴�Է���̯λ��Ϣ";
t_MidTipsList[MID_FOLLOW_START_FAIL]					= "��ǰ״̬��������и������";
t_MidTipsList[MID_TRUST_MAX_ONETIME]					= "�Ѵﵽһ������ί��������Ŀ������";
t_MidTipsList[MID_STALL_SEE_FIGHT]						= "ս��״̬���޷����̯λ";
t_MidTipsList[MID_STALL_SEE_PK]							= "�д�״̬���޷����̯λ";
t_MidTipsList[MID_STALL_CANNOT_EMPTY]					= "���ܿ���̯λ���޷��¼�";
t_MidTipsList[MID_STALL_SEE_DEAD]						= "����״̬���޷����̯λ";
t_MidTipsList[MID_STALL_TOOMUCH_MONEY]					= "�����ܶ�ӵ�ǰЯ�������ó�������Я����������";


t_MidTipsList[MID_STALL_SELF_RIDE]						= "���״̬�����ܰ�̯";
t_MidTipsList[MID_STALL_SELF_MORPH]						= "����״̬�����ܰ�̯";
t_MidTipsList[MID_STALL_SELF_TRUMP_PRE]					= "�����ٻ����飬���ܰ�̯";
t_MidTipsList[MID_STALL_SELF_NEAR_NPC]					= "�Ͻ���NPC��Χ��̯����ǰ���տ�����";
t_MidTipsList[MID_STALL_SELF_NEAR_OTHER_STALL]			= "��λ������̯λ����ǰ���տ�����";
t_MidTipsList[MID_STALL_SELF_DEAD]						= "����״̬���޷���̯";

t_MidTipsList[MID_ITEM_USE_FAIL_INPRE]					= "����ʹ��������Ʒ";
t_MidTipsList[MID_FRIEND_REPEAT_REQ]					= "���Ѿ����͹����룬�ȴ��Է��ظ�";
t_MidTipsList[MID_FRIEND_REPEAT_REQ]					= "������Ѿ�����ĺ����б�"; 
t_MidTipsList[MID_FRIEND_IS_ENEMY]						= "Ŀ���������������޷���Ӻ��ѣ����Ƴ��������";
t_MidTipsList[MID_FRIEND_IS_SU_DI]						= "�Է�������޵������У��޷���Ӻ��ѣ��밴��F�������޵������鿴";

t_MidTipsList[MID_FRIEND_IS_BLACK]						= "�Է�����ĺ������У��޷���Ӻ��ѣ��밴��F�����򿪺������鿴";

t_MidTipsList[MID_XP_NOCAMPTRUMP]						= "��Ҫ��Ӧ���鴦�ڳ���״̬��";		   
t_MidTipsList[MID_SHORTCUT_CANNOTUSESPELL_XPLIMIT]		= "��ǰ״̬����ʹ��XP����"; 
t_MidTipsList[MID_ARMPROCET_HOLECOLORDIFWITHGEM]		= "�ÿ���ɫ����ʯ��ɫ��ƥ��";  
t_MidTipsList[MID_ARMPROCET_HOLEHASSAMEGEM]				= "��װ������Ƕ�����ֱ�ʯ";
--t_MidTipsList[MID_FRIEND_REPEAT_REQ]					= "���Ѿ����͹����룬�ȴ��Է��ظ�";
t_MidTipsList[MID_FITTINGMGR_NOFITSTUFF]				= "�ò��ϲ���װ�����";
t_MidTipsList[MID_FITTINGMGR_NOTFITGROUP]				= "�����ܲ�����Ƕ������";
t_MidTipsList[MID_FITTINGMGR_CAREEALIMIT]				= "ְҵ����,������Ƕ�����";
t_MidTipsList[MID_FITTINGMGR_INLAYSPECIALLIMIT]			= "����Ƕ��ֻ��װ���������";
t_MidTipsList[MID_FITTINGMGR_INLAYNORMALLIMIT]			= "����Ƕ��ֻ��װ���޻������";
t_MidTipsList[MID_FITTINGMGR_SAMEIDLIMIT]				= "������Ƕ��ͬ�����";
t_MidTipsList[MID_FITTINGMGR_SAMETYPELIMIT]				= "������Ƕ��ͬ���͵����";
t_MidTipsList[MID_FITTINGMGR_NEEDBULLLIMIT]				= "װ���������Ҫ��"..MONEY_TYPE_BULL_NAME.."����"; 
t_MidTipsList[MID_USESKILL_INCOLLECT_FAIL]				= "���ڲɼ�������ʹ�ü���";
t_MidTipsList[MID_USEITEM_INCOLLECT_FAIL]				= "���ڲɼ�������ʹ�ø���Ʒ";
t_MidTipsList[MID_TRUMP_PER_FAIL_INFIGHT]				= "ս��״̬�²��ܳ�������";
t_MidTipsList[MID_TRUMP_PER_FAIL_INDEAD]				= "����״̬�²��ܳ�������";
t_MidTipsList[MID_ARMPROCET_INLAY_LEVELLIMIT]			= "��ʯ�ȼ���װ���ȼ�����";	
t_MidTipsList[MID_ARMPROCE_NOTINLAYGEM]					= "����Ƕ�౦ʯ��������";
t_MidTipsList[MID_EXCHANGE_ACTIONFORBID]		= "����״̬��,����ִ�д˲���";	

t_MidTipsList[MID_SPELL_UNUSED_NOAMM]					= "ȱ���ӵ�";
t_MidTipsList[MID_SPELL_UNUSED_RIGHTARM]				= "ȱ����������";
t_MidTipsList[MID_SPELL_UNUSED_LEFTARM]					= "ȱ����������";
t_MidTipsList[MID_SPELL_UNUSED_SHOULDER]				= "ȱ�ټ������";
t_MidTipsList[MID_SPELL_UNUSED_BACK]					= "ȱ�ٱ�������";
t_MidTipsList[MID_SPELL_UNUSED_NONEEQUIP]				= "ȱ�ټ��ܶ�Ӧ������";
                                               		
t_MidTipsList[MID_SPELL_UNUSED_CAMP]					= "���ܶ�Ŀ��ʹ��";
t_MidTipsList[MID_SPELL_UNUSED_OBJ]						= "���ܶ�Ŀ��ʹ��";
t_MidTipsList[MID_SPELL_UNUSED_DEAD]					= "Ŀ��������";
t_MidTipsList[MID_SPELL_UNUSED_MASK_TARGET]				= "Ŀ�겻����Ұ��";
t_MidTipsList[MID_SPELL_UNUSED_MASK_POINT]				= "��ѡ����ȷ������";
t_MidTipsList[MID_SPELL_UNUSED_INCOLLECT]				= "���ڲɼ�";
t_MidTipsList[MID_ITEM_USE_FAIL_NOTINAREA]				= "����ָ������ʹ�ø���Ʒ��";
t_MidTipsList[MID_ARMPROCE_SAMEGEMTYPEINLAY]			= "����Ƕ�б�����ͬ���ͱ�ʯ��";
t_MidTipsList[MID_ITEM_USE_FAIL_IN_STALL]				= "��̯״̬���޷�ʹ����Ʒ";

t_MidTipsList[MID_AUTOFIND_SELF_LIMIT]					= "�������ƣ����ܽ����Զ�Ѱ·";		 
t_MidTipsList[MID_SPELL_NOTARGET]						= "����ѡ��һ��Ŀ��";	

t_MidTipsList[MID_TRUST_ALREADY_ORDER]					= "��ί���Ѿ��ҵ��������ظ��ҵ�";
t_MidTipsList[MID_FIND_PATH_FAIL]						= "Ŀ���ڸ����л򲻿ɵ�������Ѱ·ʧ��";		
t_MidTipsList[MID_TRUMP_ISPRE]							= "���ڳ������飬���Ժ�";
t_MidTipsList[MID_TRUMP_BACK_FAIL]						= "����ʹ�ü��ܣ��޷��ٻؾ���";

t_MidTipsList[MID_MARKET_NOT_ENOUGH_GOLD]				= "û���㹻��"..MONEY_TYPE_GODCOIN_NAME.."����";
t_MidTipsList[MID_MARKET_NOT_ENOUGH_BIND_GOLD]			= "û���㹻��"..MONEY_TYPE_BIND_GODCOIN_NAME.."����";
t_MidTipsList[MID_MARKET_NOT_FRIEND]					= "�Է�Ϊ���Ѳ������ͣ�";
t_MidTipsList[MID_MARKET_SEND_TRADE_SUCESS]				= "���׷����ɹ���";
t_MidTipsList[MID_MARKET_TRADE_SUCESS]					= "���׳ɹ���";
t_MidTipsList[MID_MARKET_DESTROY_SUCESS]				= "���������ɹ���";
t_MidTipsList[MID_MARKET_PACKAGE_PRESENT_SUCESS]		= "���ͳɹ���";
t_MidTipsList[MID_MARKET_BUY_TIME_OUT]					= "����Ʒ��ʧЧ��";

t_MidTipsList[MID_PET_PRE_FAIL_TRUMP]					= "�����ٻ����飬�����ٻ����"
t_MidTipsList[MID_PET_PRE_FAIL_USEITEM]					= "����ʹ����Ʒ�������ٻ����"
t_MidTipsList[MID_PET_PRE_FAIL_SKILLPRE]				= "���ڽ��м��������������ٻ����"
t_MidTipsList[MID_PET_PRE_FAIL_RIDEPRE]					= "�����ٻ���ˣ������ٻ����"
t_MidTipsList[MID_PET_PRE_FAIL_COLLECT]					= "���ڲɼ��������ٻ����"
t_MidTipsList[MID_PET_PRE_FAIL_FIGHT]					= "ս��״̬�������ٻ����"
t_MidTipsList[MID_PET_PRE_FAIL_STALL]					= "��̯״̬�������ٻ����"
t_MidTipsList[MID_PET_PRE_FAIL_IN_OTHER_RIDE]			= "����������������ٻ����"
t_MidTipsList[MID_PET_PRE_FAIL_IN_RIDE_MACHINE]			= "������е�������ٻ����"
t_MidTipsList[MID_PET_FAIL_IN_FREEDOM_ANIL]				= "�ó����Ѿ��������������ɽ��иò�����"
t_MidTipsList[MID_PET_FAIL_FREEDOM_IN_FIGHT]			= "�ó��������ٻ��У����ɷ�����"
t_MidTipsList[MID_PET_PRE_FAIL_SOME_STATE]				= "��ǰ״̬�������ٻ����"

t_MidTipsList[MID_TRUMP_PRE_FAIL_PET]					= "�����ٻ���������ٻ����飡"
t_MidTipsList[MID_TRUMP_PRE_FAIL_USEITEM]				= "����ʹ����Ʒ�������ٻ����飡"
t_MidTipsList[MID_TRUMP_PRE_FAIL_SKILLPRE]				= "���ڽ��м��������������ٻ����飡"
t_MidTipsList[MID_TRUMP_PRE_FAIL_COLLECT]				= "���ڲɼ��������ٻ����飡"
t_MidTipsList[MID_TRUMP_PRE_FAIL_RIDEPRE]				= "�����ٻ���ˣ������ٻ����飡"

t_MidTipsList[MID_RIDE_PRE_FAIL_TRUMP]					= "�����ٻ����飬����ʹ����ˣ�"
t_MidTipsList[MID_RIDE_PRE_FAIL_USEITEM]				= "����ʹ����Ʒ������ʹ����ˣ�"
t_MidTipsList[MID_RIDE_PRE_FAIL_SKILLPRE]				= "���ڽ��м�������������ʹ����ˣ�"
t_MidTipsList[MID_RIDE_PRE_FAIL_COLLECT]				= "���ڲɼ�������ʹ����ˣ�"
t_MidTipsList[MID_RIDE_PRE_FAIL_PET]					= "�����ٻ��������ʹ����ˣ�"
t_MidTipsList[MID_RIDE_PRE_FAIL_NO_RIDE]				= "û�и����"
t_MidTipsList[MID_RIDE_PRE_FAIL_SOME_STATE]				= "��ǰ״̬������ʹ����ˣ�"

t_MidTipsList[MID_INFIGHT]								= "����������ս��"
t_MidTipsList[MID_NOINFIGHT]							= "�����Ƚ���ս��"
t_MidTipsList[MID_NEED_TARGET]							= "����ѡ��һ��Ŀ��"
t_MidTipsList[MID_NEED_MONSTER_TARGET]					= "���ܶԸ�Ŀ��ʹ�������Ʒ"
t_MidTipsList[MID_NEED_NPC_TARGET]						= "����ѡ��һ��NPCĿ��"
t_MidTipsList[MID_NEED_NPC_PLAYER]						= "��ѡ��һ��������������"	

t_MidTipsList[MID_APPLY_TEAM_PLAYER_IS_ENEMY]			= "�Է��������������������"
t_MidTipsList[MID_APPLY_TEAM_PLAYER_IS_TMPENEMY]		= "Ŀ��������޵������У��޷���ӣ��밴��F�������޵������鿴"
t_MidTipsList[MID_INVITE_TEAM_PLAYER_IS_TMPENEMY]		= "Ŀ��������޵������У��޷���ӣ��밴��F�������޵������鿴"

t_MidTipsList[MID_APPLY_TEAM_SELF_DEAD]					= "��������״̬���������"	
t_MidTipsList[MID_APPLY_TEAM_OTHER_DEAD]				= "�Է���������״̬���������"	
t_MidTipsList[MID_APPLY_TEAM_OTHER_IN_BLACK]			= "�Է�����ĺ������У��������"
t_MidTipsList[MID_APPLY_TEAM_OTHER_IN_ZHUISHA]			= "�������޵������е�������"
t_MidTipsList[MID_APPLY_TEAM_OTHER_IN_XIONGSHOU]		= "���������������е�������"
t_MidTipsList[MID_APPLY_TEAM_OTHER_IN_TEAM]				= "���Ѿ�ӵ�ж���"

t_MidTipsList[MID_USE_ITEM_FAILE_HPENOUGH]				= "��ǰ����ֵ����"
t_MidTipsList[MID_USE_ITEM_FAILE_MPENOUGH]				= "��ǰħ��ֵ����"
t_MidTipsList[MID_USE_ITEM_FAILE_LVENOUGH]				= "��ǰ�ȼ�����ʹ�ø���Ʒ��"

t_MidTipsList[MID_AUTO_TEAM_FAIL_AGREE_READY]			= "�Ѿ�ͬ���ˣ������ٴ�����ͬ�����"
t_MidTipsList[MID_MARKET_NO_SEARCH]						= "���˵��Ų�����"

t_MidTipsList[MID_AUTO_TEAM_FAIL_TRAN_BY_EXCHANGE]		= "����״̬���ܴ���"
t_MidTipsList[MID_AUTO_TEAM_FAIL_TRAN_BY_STALL]			= "��̯״̬���ܴ���"
t_MidTipsList[MID_AUTO_TEAM_FAIL_TRAN_BY_DEAD]			= "����״̬���ܴ���"
t_MidTipsList[MID_AUTO_TEAM_FAIL_TIME_OUT]				= "��Ѿ�����"

t_MidTipsList[MID_SECONDARYSKILL_WRONG_STATE]			= "��ǰ״̬�²���ʹ�������"
t_MidTipsList[MID_USEITEMPRE_WRONG_STATE]				= "��ǰ״̬�²���ʹ����Ʒ"

t_MidTipsList[MID_USEITEM_IN_FLY_STATE]					= "��ǰ״̬����ʹ����Ʒ�����Ժ�����"
t_MidTipsList[MID_AUTOFIND_IN_FLY_STATE]				= "��ǰ״̬�����Զ�Ѱ·�����Ժ�����"
t_MidTipsList[MID_SHORTCUT_IN_FLY_STATE]				= "��ǰ״̬����ʹ�ü��ܣ����Ժ�����"
t_MidTipsList[MID_PWORLD_RESTOREITEM]					= "�ø�������ʹ�ûָ�����Ʒ"
t_MidTipsList[MID_MAP_NOSKILL]							= "��ǰ��ͼ����ʹ�øü���"
t_MidTipsList[MID_SAFE_ENV_NOSKILL]						= "��ȫ���ڲ���ʹ�øü���"
t_MidTipsList[MID_SELF_BUSY]							= "�����������£�����ʹ�ü���"
t_MidTipsList[MID_SELF_SPELLPRE]						= "������ʹ����������"
t_MidTipsList[MID_SELF_DEAD]							= "���Ѿ�����"
t_MidTipsList[MID_NOT_ENOUGH_ENERGY]					= "��������"
t_MidTipsList[MID_TARGET_TO_FAR]						= "Ŀ�겻�ڹ�����Χ��"
t_MidTipsList[MID_TRUMP_PER_FAIL_MOVE]					= "�ƶ��У��޷��ٻ�����"

t_MidTipsList[MID_MOVE_FAIL_STALL]						= "��̯״̬���޷��ƶ�"

t_MidTipsList[MID_PICK_ITEM_TOO_FAR]					= "����̫Զ���޷�ʰȡ"
t_MidTipsList[MID_SECONDARYSKILL_FAILED_INRIDE]			= "���״̬�У��޷����пƼ�����"
t_MidTipsList[MID_SECONDARYSKILL_FAILED_INFIGHT]		= "ս��״̬�У��޷����пƼ�����"
t_MidTipsList[MID_SECONDARYSKILL_FAILED_INCASTING]		= "�����У��޷����пƼ�����"
t_MidTipsList[MID_SECONDARYSKILL_FAILED_CANNTCONTROL]	= "�ܿ���״̬�У��޷����пƼ�����"
t_MidTipsList[MID_SECONDARYSKILL_FAILED_COLDDOWN]		= "������ȴ�У��޷�ʹ��"
t_MidTipsList[MID_CANNOT_CATCH_PET]						= "���ܶ�Ŀ��ʹ��"
t_MidTipsList[MID_CATCH_PET_DEAD]						= "Ŀ��������"
t_MidTipsList[MID_FAIL_WEI_XING]						= "�������Ŀ�괦�ڲ��ܸ���λ�ã�����ʧ��"
t_MidTipsList[MID_NPCSHOP_BATTLE_GLORY_SELLFAIL]		= "������������,����ʧ��!"
t_MidTipsList[MID_NPCSHOP_AOSHAN_BRAND_SELLFAIL]		= "����ս����²���,����ʧ��!"
t_MidTipsList[MID_NPCSHOP_LIFT_BRAND_SELLFAIL]			= "�������潱�²���,����ʧ��!"
t_MidTipsList[MID_TRUMP_PER_FAIL_BADSTATUS]				= "��״̬�£��޷��ٻ�����"
t_MidTipsList[MID_RESOLVE_WRONG_STATE]					= "��ǰ״̬�²��ֽܷ�"
t_MidTipsList[MID_RESOLVE_FAILED_INCASTING]				= "�����У��޷��ֽ�"
t_MidTipsList[MID_RESOLVE_FAILED_CANNTCONTROL]			= "�ܿ���״̬�У��޷��ֽ�"
t_MidTipsList[MID_RESOLVE_CONTAINER_LOCK]				= "�����������޷��ֽ�"
t_MidTipsList[MID_RIDE_PRE_FAILED_CLAN_LIMIT_CAPTION]	= "����ʧ�ܣ�������ֻ��ʱ���ռ�칫��᳤����ʹ��Ȩ��"
t_MidTipsList[MID_RIDE_PRE_FAILED_CLAN_LIMIT_MEMBER]	= "����ʧ�ܣ�������ֻ��ʱ���ռ�칫���Ա����ʹ��Ȩ��"
t_MidTipsList[MID_USE_ITEM_FAILED_CLAN_LIMIT_CAPTION]	= "����ʹ�ã�����Ʒֻ��ʱ���ռ�칫��᳤����ʹ��Ȩ��"
t_MidTipsList[MID_USE_ITEM_FAILED_CLAN_LIMIT_MEMBER]	= "����ʹ�ã�����Ʒֻ��ʱ���ռ�칫���Ա����ʹ��Ȩ��"
t_MidTipsList[MID_USE_BACK_CLAN_ITEM_FAILED_INFIGHT]	= "ս��״̬����ʹ�ù�����д��;�"
t_MidTipsList[MID_USE_PET_EGG_PET_FULL]					= "����������"
t_MidTipsList[MID_PET_MAP_NO_PET_FIGHT]					= "��ǰ��ͼ�����ٻ�����"
t_MidTipsList[MID_PET_MAP_NO_FAIRY_FIGHT]				= "��ǰ��ͼ�޷��ٻ�����"
t_MidTipsList[MID_MUST_MACHINE_COLLECT]					= "�ÿ����Ҫ�ɼ������ܲɼ�"
t_MidTipsList[MID_MACHINE_UNMATCH_COLLECT]				= "��ʹ�õĲɼ������ܲɼ��˿��"
t_MidTipsList[MID_MACHINE_UNMATCH_COLLECT_MORMAL]		= "��е״̬���ܽ��вɼ�����"
t_MidTipsList[MID_USE_MACHINE_ITEM_IN_MORPH]			= "��ǰ״̬�޷�ʹ�û�е"
t_MidTipsList[MID_USE_MORPH_ITEM_IN_MACHINE]			= "�ڻ�е״̬���޷�ʹ�ñ���ҩˮ"
t_MidTipsList[MID_ENTER_CLANISLAND_IN_FIGHT]			= "ս��״̬���ܴ���"
t_MidTipsList[MID_ENTER_CLANISLAND_IN_MACHINE]			= "��е״̬���ܴ���"
t_MidTipsList[MID_SUMMON_MUST_BE_CAPTAIN]				= "����ʹ���߱����Ƕӳ����"
t_MidTipsList[MID_SUMMON_MUST_BE_PRESIDENT]				= "����ʹ���߱����ǻ᳤�򸱻᳤���"
t_MidTipsList[MID_NOTENOUGH_COST_ITEM]					= "ȱ��ʹ�øü�����Ҫ�ĵ���"
t_MidTipsList[MID_CHATLINK_MAX_NUM]						= "���õ����������Ѵﵽ���"
t_MidTipsList[MID_CHATLINK_SAME_EXIST]					= "�Ѵ�����ͬ���͵�����"
t_MidTipsList[MID_NO_ATTACK_WORLD_CITY_CONTROL]			= "������Ч��������Դ���������ڣ���ǰ�����޷�������"
t_MidTipsList[MID_NO_ITEM_USE_IN_THIS_MAP]				= "�˵�ͼ����ʹ�ô���Ʒ"
t_MidTipsList[MID_NPCSHOP_SPARTA_GLORY_SELLFAIL]		= "������ҫ����,����ʧ��!"
t_MidTipsList[MID_NO_CHANGE_STALL_MODEL]				= "��û���ı����̯λ�����Ρ�"

SKILL_TARGET_NONE			= 0;
SKILL_TARGET_ENEMY_MON		= 1;
SKILL_TARGET_ENEMY_PLAYER	= 2;
SKILL_TARGET_MID_PLAYER		= 4;
SKILL_TARGET_MID_MON		= 8;
SKILL_TARGET_FRIEND_MON		= 16;
SKILL_TARGET_FRIEND_PLAYER	= 32;
SKILL_TARGET_SELF			= 64;
SKILL_TARGET_SELFPET		= 128;
SKILL_TARGET_FRIENDPET		= 256;
SKILL_TARGET_MIDPET			= 512;
SKILL_TARGET_ENEMYPET		= 1024;
SKILL_TARGET_TEAM_PLAYER	= 2048;

--ְҵСͼ��
careerIdxTexture = {
	"uires\\ui\\Za\\ZhiYeBiaoShi\\Zhan.tga",
	"uires\\ui\\Za\\ZhiYeBiaoShi\\Fa.tga",
	"uires\\ui\\Za\\ZhiYeBiaoShi\\Qiang.tga",
	"uires\\ui\\Za\\ZhiYeBiaoShi\\Ji.tga",
}

-- ����������Ϣ
local t_facesDesc = 
{
	["#001"] = { desc = "գ��", },
	["#002"] = { desc = "����", },
	["#003"] = { desc = "��Ƥ", },
	["#004"] = { desc = "����", },
	["#005"] = { desc = "�ɰ�", },
	["#006"] = { desc = "װ��", },
	["#007"] = { desc = "ŭ��", },
	["#008"] = { desc = "����", },
	["#009"] = { desc = "����", },
	["#010"] = { desc = "�ѹ�", },
	["#011"] = { desc = "����", },
	["#012"] = { desc = "��Ц", },
	["#013"] = { desc = "˯��", },
	["#014"] = { desc = "��", },
	["#015"] = { desc = "��", },
	["#016"] = { desc = "ɫ", },
	["#017"] = { desc = "�亹", },
	["#018"] = { desc = "��ˮ", },
	["#019"] = { desc = "��", },
	["#020"] = { desc = "��", },
	["#021"] = { desc = "��", },
	["#022"] = { desc = "��", },
	["#023"] = { desc = "��", },
	["#024"] = { desc = "����", },
	["#025"] = { desc = "����", },
	["#026"] = { desc = "��", },
	["#027"] = { desc = "��", },
	["#028"] = { desc = "�ɰ�", },
	["#029"] = { desc = "��", },
	["#030"] = { desc = "����", },
	["#031"] = { desc = "��ü", },
	["#032"] = { desc = "Ц", },
	["#033"] = { desc = "����", },
	["#034"] = { desc = "��", },
	["#035"] = { desc = "�۱�", },
	["#036"] = { desc = "ץ��", },
	["#037"] = { desc = "����", },
	["#038"] = { desc = "����", },
	["#039"] = { desc = "ɽ��", },
	["#040"] = { desc = "ŭ", },
	["#041"] = { desc = "��Ц", },
	["#042"] = { desc = "����", },
	["#043"] = { desc = "���", },
	["#044"] = { desc = "Ц", },
	["#045"] = { desc = "�ͻ�", },
	["#046"] = { desc = "�˷�", },
};

function GetFaceDesc( szKey )
	return t_facesDesc[szKey]["desc"];
end

-- ����������Ϣ
--[[
local t_Rotot_Face = 
	{
		{"#001","գ��"}, 
		{"#002","����"},
		{"#003","��Ƥ"},
		{"#004","����"},
		{"#005","�ɰ�"}, 
		{"#006","װ��"},
		{"#007","ŭ��"},
		{"#008","����"},
		{"#009","����"}, 
		{"#010","�ѹ�"},
		{"#011","����"},
		{"#012","��Ц"},
		{"#013","˯��"}, 
		{"#014","��"},
		{"#015","��"},
		{"#016","ɫ"},
		{"#017","�亹"},
		{"#018","��ˮ"},
		{"#019","��"}, 
		{"#020","��"}, 
		{"#021","��"},
		{"#022","��"},
		{"#023","��"}, 
		{"#024","����"},
		{"#025","����"}, 
		{"#026","��"},
		{"#027","��"},
		{"#028","�ɰ�"},
		{"#029","ʤ��"},
		{"#030","����"}, 
		{"#031","��ü"},  
		{"#032","Ц"},
		{"#033","����"},
		{"#034","��"}, 
		{"#035","�۱�"},
		{"#036","ץ��"},
		{"#037","����"},
		{"#038","����"},
		{"#039","ɽ��"},
		{"#040","ŭ"}, 
		{"#041","��Ц"},   
		{"#042","����"},
		{"#043","���"},
		{"#044","Ц"}, 
		{"#045","�ͻ�"},
		{"#046","�˷�"},

		{"#047","��ŭ"},
		{"#048","�ݰ�"},
		{"#049","��Ů����"},
		{"#050","˧�����"},
	};
--]]
local t_Rotot_Face = 
{
	{"#001","ŭ"},		{"#002","ɫ"},		{"#003","�ɰ�"},	{"#004","΢Ц"},	{"#005","����"},	{"#006","�ټ�"},
	{"#007","��ͷ"},	{"#008","����"},	{"#009","��Ц"},	{"#010","����"},	{"#011","����"},	{"#012","����"},
	{"#013","��"},		{"#014","ȭͷ"},	{"#015","����"},	{"#016","̾��"},	{"#017","��"},		{"#018","����"},
	{"#019","ҡ��"},	{"#020","С��"},	{"#021","�ٱ�ʺ"},	{"#022","����"},	{"#023","����"},	{"#024","�Է�"},
	{"#025","�ɶ�"},	{"#026","����"},	{"#027","�Ѫ"},	{"#028","����"},	{"#029","�ܶ�"},	{"#030","��"},
	{"#031","װ����"},	{"#032","����"},	{"#033","����"},	{"#034","����"},	{"#035","����"},	{"#036","����"},
	{"#037","��Ц"},	{"#038","��"},		{"#039","����"},	{"#040","ѣ��"},	{"#041","��"},		{"#042","��ͬ"},
	{"#043","�в�"},	{"#044","no"},		{"#045","˯��"},	{"#046","����"},	{"#047","����"},	{"#048","����"},
	{"#049","������"},	{"#050","����"},	{"#051","����"},	{"#052","·��"},	{"#053","������ "},	{"#054","����"},
	{"#055","����"},	{"#056","����"},	{"#057","��1"},		{"#058","��"},		{"#059","����"},	{"#060","ί��"},
	{"#061","����"},	{"#062","����"},	{"#063","�ջ�"},	{"#064","����"},	{"#065","����"},
};

function GetRototFaceTable()
	return t_Rotot_Face; 
end

local t_QuestStarTexUVList =
{

};

-- ����button����Ϣ
function SetButtonInfo( buttonInfo )
	local btn = getglobal( buttonInfo.szName );

	if buttonInfo.ClientUserData0 ~= nil then
		btn:SetClientUserData( 0, buttonInfo.ClientUserData0 );
	end

	if buttonInfo.ClientUserData1 ~= nil then
		btn:SetClientUserData( 1, buttonInfo.ClientUserData1 );
	end

	if buttonInfo.ClientUserData2 ~= nil then
		btn:SetClientUserData( 2, buttonInfo.ClientUserData2 );
	end

	if buttonInfo.ClientUserData3 ~= nil then
		btn:SetClientUserData( 3, buttonInfo.ClientUserData3 );
	end

	if buttonInfo.ClientString ~= nil then
		btn:SetClientString( buttonInfo.ClientString );
	end
end

-- ����ͷ��
function SetPhoto( nHairID, nFaceID, szHairIconName, szFaceIconName, nGender, nHairColor, szHairShadowTex, szBodyTex, szMask )
	if szMask == nil then
		szMask = "uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\ZhuJueTouXiang  MASK.tga";
	end

	local t_tgas =	{
						
						[szBodyTex] = 
						{
							[CLIENT_GENDER_MALE]	= { path = "uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\male_body.tga"	}, 
							[CLIENT_GENDER_FEMALE] = { path = "uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\female_body.tga" },
							-- TODO
							mask = szMask
						},

						[szFaceIconName] = 
						{	[CLIENT_GENDER_MALE]	= { path = "uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\552000" .. ( nFaceID ) .. ".tga" }, 
							[CLIENT_GENDER_FEMALE] = { path = "uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\552100" .. ( nFaceID ) .. ".tga" }, 
							mask = szMask
						},

						[szHairShadowTex] = 
						{
							[CLIENT_GENDER_MALE]	= { path = "uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\male\\hair_"..( nHairID + 1 ).."_shadow.tga"		}, 
							[CLIENT_GENDER_FEMALE] = { path = "uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\female\\hair_"..( nHairID + 1 ).."_shadow.tga"	}, 
							-- TODO
							mask = szMask
						},

						[szHairIconName] = 
						{	[CLIENT_GENDER_MALE]	= { path = "uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\male\\hair_" .. ( nHairID + 1 ).."_"..(nHairColor )..".tga" }, 
							[CLIENT_GENDER_FEMALE] = { path = "uires\\TuPianLei\\TouXiang\\JueSeTouXiang\\female\\hair_".. ( nHairID + 1 ).."_"..(nHairColor )..".tga" }, 
							mask = szMask
						},
					};
	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name );
		if data["mask"] ~= nil then
			tex:setMask( data["mask"] );
		end
		
		tex:SetTexture( data[nGender]["path"] );
		--tex:SetTexUV( 0, 0, 0, 0 );
		tex:SetTexRelUV( 1, 1 );
	end
end

-- @Desc:���ù�������λ��
-- @param: szScrollBarName	ΪҪ���õĹ�����
-- @param: nMaxViewLine		ΪҪһҳ������ʾ���������
-- @param: nValidDataNum	Ϊ��ǰҪ��ʾ����������
function SetScrollBar( szScrollBarName, nMaxViewLine, nValidDataNum )
	--ȷ������ѧϰ�����ʾ�ľ��弼��
	local slider		= getglobal( szScrollBarName );
	local nNum			= nValidDataNum - nMaxViewLine;
	if nNum > 0  then
		slider:SetMaxValue( nNum );
		slider:Show();
	else
		slider:SetMaxValue( 1 );
		slider:Hide();
	end

	slider:SetMinValue( 0 );
	slider:SetValueStep( 1 );
	if slider:GetValue() > slider:GetMaxValue() then
		slider:SetValue( slider:GetMaxValue() );
	end

end


--@Desc:�Ƴ���ǰѡ�е���Ʒ
function RemoveCheckItem( szParent )
	local nCheck = 0;
	for i=1 ,table.getn(t_ItemButtonCheckIndex) do
		if t_ItemButtonCheckIndex[i][1] == szParent then
			local nIndex =  (t_ItemButtonCheckIndex[i][2])%nSUNDRIESGridMax + 1;
			if nIndex > 0 and nIndex < nSUNDRIESGridMax + 1 then
				local stallButton = getglobal( szParent.."Item"..nIndex );
				stallButton:DisChecked();
				nCheck = i;
			end
		end
	end

	if nCheck ~= 0 then
		table.remove( t_ItemButtonCheckIndex, nCheck );
	end
end

--·��
local szSkillIconPath = "uires\\TuPianLei\\JiNeng\\JiNeng\\";
function GetSkillIconPath() --��ȡ����ͼ��·��
	return szSkillIconPath;
end

local szItemEquipIconPath = "uires\\TuPianLei\\WuPin\\";
function GetItemEquipIconPath( )
	--[[
	local IconPath = szItemEquipIconPath..nIconID..".tga";
	return IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH;
	--]]
	return szItemEquipIconPath;
end

function GetZhiNengtemEquipIconPath( nIconID )
	local IconPath = szItemEquipIconPath..nIconID..".tga";
	return IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH;
end

--��ȡ����ͼ��·��
local szPetIconPath = "uires\\TuPianLei\\TouXiang\\JingLing\\";
function GetPetIconPath()
	return szPetIconPath;
end

--��ȡNPCͷ��·��
--[[local szNpcPhotoIconPath = "uires\\TuPianLei\\TouXiang\\";
function GetNpcPhotoIconPath()
	return szNpcPhotoIconPath;
end
--]]

function SetPetHeadPhoto( PetDefInfo, tex )
	tex:SetTexture( "uires\\TuPianLei\\TouXiang\\GuaiWuTouXiang\\" .. PetDefInfo.IconID .. ".tga" );
	--[[if PetDefInfo.Type == SHITYPE then
		
	elseif PetDefInfo.Type == XIANTYPE then
		tex:SetTexture( "uires\\TuPianLei\\TouXiang\\JingLingTouXiang\\" .. PetDefInfo.MouldID .. ".tga" );
	end
	--]]
end

function SetPetModelView( PetDefInfo, tex )
	if PetDefInfo.Type == XIANTYPE then
		tex:SetTexture( "uires\\TuPianLei\\TouXiang\\JingLing\\" .. PetDefInfo.MouldID .. ".tga" );
	end
end

-- ��ȡ���·��
local szFittingIconPath = "uires\\TuPianLei\\JiNeng\\���\\";
function GetFittingIconPath()
	return szFittingIconPath;
end

-- ��ȡ����NPCͷ��·��
local szMonsterIconPath = "uires\\TuPianLei\\TouXiang\\GuaiWuTouXiang\\";
function GetMonsterIconPath( nIconId )
	return ( IsInExistence( szMonsterIconPath.. nIconId ..".tga" ) and szMonsterIconPath.. nIconId ..".tga" or
							szMonsterIconPath.."000000.tga" );
end

-- ��ȡbuffͼ��·��

local szBuffIconPath = "uires\\TuPianLei\\JiNeng\\Buff\\";
function GetBuffIconPath( nIconId )
	return ( IsInExistence( szBuffIconPath.. nIconId ..".tga" ) and ( szBuffIconPath.. nIconId ..".tga" ) or
							( szBuffIconPath.."0000.tga" ) );
end

-- ��ȡ����ͼ��
local szPetSkillPath = "uires\\TuPianLei\\JiNeng\\ChongWuJiNeng\\";
function GetPetSkillIconPath( nIconId )
	local IconPath = szPetSkillPath..nIconId..".tga";
	return IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH;
end

-- ��ȡ����ͷ��·��
function GetTrumpIconPath( )
	return "uires\\TuPianLei\\TouXiang\\JingLingTouXiang\\";
end
--����

function GetCareerName( nType )
	return t_raceList[nType] or "";
end

function GetPosition(nType)
	if nType == CLAN_POS_NOT_MEMBER then
		return "�ǻ�Ա";
	elseif nType == CLAN_POS_NORMAL then
		return "��Ա";
	elseif nType == CLAN_POS_TECH_OFFICER then
		return "�Ƽ���Ա";
	elseif nType == CLAN_POS_COLLECTION_OFFICER then
		return "�ɼ���Ա";
	elseif nType == CLAN_POS_FACTORY_OFFICER then
		return "������Ա";
	elseif nType == CLAN_POS_PET_OFFICER then
		return "�����Ա";
	elseif nType == CLAN_POS_WALL_OFFICER then
		return "������Ա";	
	elseif nType == CLAN_POS_VICE_PRESIDENT then
		return "���᳤";
	elseif nType == CLAN_POS_PRESIDENT then
		return "�᳤";
	elseif nType == CLAN_POS_GROUP_1 then
		return "߱ɱս�ų�";
	elseif nType == CLAN_POS_GROUP_2 then
		return "��ɱս�ų�"
	elseif nType == CLAN_POS_GROUP_3 then
		return "��ɱս�ų�";		
	elseif nType == CLAN_POS_GROUP_4 then
		return "��ɱս�ų�";		
	elseif nType == CLAN_POS_GROUP_5 then
		return "��ɱս�ų�";
	elseif nType == CLAN_POS_GROUP_6 then
		return "�ƽ�ս�ų�";
	end
end

function GetGender( nGender )
	if nGender == CLIENT_GENDER_MALE then
		return "��";
	elseif nGender == CLIENT_GENDER_FEMALE then
		return "Ů";
	end
end

function GetMidTipsList()
	return t_MidTipsList;
end

--��ʾ��Ϸ�����м���Ϣ
function ShowMidTips( szText )
	AddGameMiddleTips( szText );
end

-- ȷ��EditBox����������ֵ������0��ͷ
function CheckEditNumberInput( edit )
	local szInputText	= edit:GetText();
	-- �������ֵ���з�����ֵ,��EditBox�е�ֵ��Ϊ0
	if tonumber( szInputText ) == nil then
		return;
	end

	-- ��������ֵ����λ�Ƿ�Ϊ0
	if string.len( szInputText ) > 1 and string.sub( szInputText, 1, 1 ) == '0' then
		edit:SetText( string.sub( szInputText, 2, string.len( szInputText ) ) );
		return;
	end
end

-- װ�����Ͷ���
ARM_BOTHSWORD			= 100; 	--/* �Խ� */
ARM_KNIFE				= 101; 	--/* �� */
ARM_SWORD				= 103;	--/* �� */
ARM_SWORD_LEFT			= 104;  --/* ���ֽ� */
ARM_SHIELD				= 106; 	--/* �� */
ARM_MAGIC_STAFF_ICE		= 110; 	--/* ħ�� */
ARM_MAGIC_STAFF_FIRE	= 111;	--/* ��ħ�� */

ARM_MAGIC_FAQI			= 118; 	--/* ���� */
ARM_RIGHT_GUN			= 120; 	--/* ����ǹ */
ARM_BITH_GUN			= 121; 	--/* ����ǹ */
ARM_HEVY_SCATTERGUN		= 125; 	--/* �ػ�ǹ */
ARM_ENERGE				= 126; 	--/* ������ */
ARM_LIUDAN_GUN			= 127; 	--/* ���� */
ARM_GUNNER_WEAR_LEFT	= 128;	--/* ����ǹ */
ARM_HUOJIAN_GUN			= 132; 	--/* ����� */
ARM_GOD_STAFF_LIGHT		= 150; 	--/* ���� */
ARM_GOD_STAFF_THUNDER	= 151; 	--/* ʥ�� */
ARM_GOD_FAQI			= 158; 	--/* ʥ�� */
ARM_ANY_EX				= 9998;	--/* �κ������������Կ��� */
ARM_ANY					= 9999; --/* �κ����������Կ��� */

ARM_SECONDARY			= 111;

-- װ�����ͱ�
t_tagARM_TYPE =
{
	-- 103
	{ARM_SWORD,				ARM_SWORD,				"���ֽ�"},
	{ARM_KNIFE,				ARM_KNIFE,				"��"},
	{ARM_BOTHSWORD,			ARM_BOTHSWORD,			"�Խ�"},
	-- 104
	{ARM_SWORD_LEFT,		ARM_SWORD_LEFT,			"���ֽ�"},
	{ARM_SHIELD,			ARM_SHIELD,				"��"},
	{ARM_MAGIC_STAFF_ICE,	ARM_MAGIC_STAFF_ICE,	"����"},
	{ARM_MAGIC_STAFF_FIRE,	ARM_MAGIC_STAFF_FIRE,	"ħ��"},
	{ARM_MAGIC_FAQI,		ARM_MAGIC_FAQI,			"����"},
	{ARM_RIGHT_GUN,			ARM_RIGHT_GUN,			"����ǹ"},
	{ARM_BITH_GUN,			ARM_BITH_GUN,			"����ǹ"},
	{ARM_HEVY_SCATTERGUN,	ARM_HEVY_SCATTERGUN,	"�ػ�ǹ"},
	{ARM_GUNNER_WEAR_LEFT,	ARM_GUNNER_WEAR_LEFT,	"����ǹ"},
	{ARM_ENERGE,			ARM_ENERGE,				"��׼��"},
	{ARM_LIUDAN_GUN,		ARM_LIUDAN_GUN,			"����"},
	{ARM_HUOJIAN_GUN,		ARM_HUOJIAN_GUN,		"�����"},
	{ARM_GOD_STAFF_LIGHT,	ARM_GOD_STAFF_LIGHT,	"����"},
	{ARM_GOD_STAFF_THUNDER, ARM_GOD_STAFF_THUNDER,	"ʥ��"},
	{ARM_GOD_FAQI,			ARM_GOD_FAQI,			"ʥ��"},
	{ARM_ANY_EX,			ARM_ANY_EX,				"�κ�����"},
	{251, 254,				"����"},
	{261, 264,				"Ь��"},
	{271, 279,				"����"},
	{281, 284,				"����"},
	{291, 294,				"��ָ"},
	{201, 204,				"�·�"},
	{211, 214,				"ͷ��"},
	{221, 222,				"���"},
	{224, 224,				"���"},
	{241, 245,				"����"},
	{300, 399,				"ʱװ"},
	{111, 111,				"ħ��"},
	{112, 112,				"��������"},
}

-- TODO: �뿪��ͼ��ʱ��Ҫͳһ�ر����
function MainPlayerLeaveMap()
	for i = 1, MAX_CHAT_FRAME do
		local rich = getglobal( "FriendChatFrame" .. i .. "_RecordFrame_RecordDisplayRich" );
		rich:Clear();
	end
	if god_end then
		god_end()
	end
	if god_test_end then
		god_test_end()
	end
end

t_FrameExcept = 
	{ 
		["ChatFaceFrame"] = { "MainChatFrame_FaceBtn", "CBFFaceBtn" },
		["TmpHPShortCut"] = { "TmpShowHPShortCutBtn" },
		["TmpMPShortCut"] = { "TmpShowMpShortCutBtn" },
		["TypeHurtBtn"]	  = {"DamageCountFrameChangeTypeBtn"},
		["SendNearBtn"]   = {"DamageCountFrameSendDpsBtn"},
		["SecondarySkillDropDownFrame"] = {"SecondarySkillFrameBranchDropDownBtn","SecondarySkillFrameBranchBtn","SecondarySkillFramePartBtn","SecondarySkillFramePartDropDownBtn"},
		["SecondarySkillLearnDropDownFrame"] = {"SecondarySkillLearnDropDownFrame_FilterNormalStateBtn","SecondarySkillLearnDropDownFrame_OneLayerBtn","SecondarySkillLearnDropDownFrame_DownArrowBtn"},
		["TicketFrameBuyTimeSelectBtn"] = {"BuyTicketTimeSelectBtn","BuyTicketTimeBtn"},
		["TicketFrameSaleTimeSelectBtn"] = {"SaleTicketTimeSelectBtn","SaleTicketTimeBtn"},
		["SystemSettingFrame_SelectBtn"] = { "SystemSettingFrameSolutionHeadBtn", "SystemSettingFrameSolutionSelect_OneLayerBtn",
											"SystemSettingFrameSolutionDownBtn" },
		["SystemSettingFrame_SelectAtialiaseBtn"] = { "SystemSettingFrameAtialiaseSelect_OneLayerBtn", "SystemSettingFrameAtialiaseDownBtn",
											"SystemSettingFrameAtialiaseHeadBtn" },
		["FriendChatFaceFrame"] = { "FriendChatFrame1_SmileFaceBtn", "FriendChatFrame2_SmileFaceBtn", "FriendChatFrame3_SmileFaceBtn", 
									"FriendChatFrame4_SmileFaceBtn", "FriendChatFrame5_SmileFaceBtn", "FriendChatFrame6_SmileFaceBtn",
									"FriendMutilSendFrame_SmileFaceBtn", },


		["MainChatFaceFrame"] = { "MainChatFrame_FaceBtn", },


		["PetPhotoListFrame"]	= { "PetPhotoOpenListBtn", "PetPhotoListFrame_ScrollBar", "PetPhotoListFrame_ScrollUpBtn", 
									"PetPhotoListFrame_ScrollDownBtn", "PetPhotoListFrame_SliderBack", },
		["WorldmapSelectNameFrame"] = { "WorldMapFrame_AreaSelectBtn", "WorldMapFrame_AreaDownBtn", "WorldMapFrame_CopySelectBtn",
										"WorldMapFrame_CopyDownBtn" },
		["SystemSettingFrame_SelectShadowQualityBtn"] = {	"SystemSettingFrameShadowQualitySelect_OneLayerBtn",
															"SystemSettingFrameShadowQualityDownBtn",
															"SystemSettingFrameShadowQualityHeadBtn" },
		["ClanSetPosDropDownFrame"] = {"ClanSetPositionFrameDownBtn"},

		["SmallKeyboardFrame"]		= 
		{ ["checkFunc"] = function () 
							local szFocusFrame = GetFocusFrameName();
							return szFocusFrame == "PassWordEdit" or szFocusFrame == "LoginKeyBoardBtn";
						end 
		},
		["GMSelectNameFrame"]			= {"GMToolsFrameSelectBtn","GMToolsFrameDownBtn"},
		["LoginFrame_UserNameDropDownBtn"]	= {"LoginFrame_SelectUserNameBtn"},
		["ClanFrameProductSelectListBox"]	= {"ClanFrameProductProduceListSelectDownBtn"},
		["ClanProductFrameSelectListBox"]	= {"ClanProductFrameProduceListSelectDownBtn"},
		["TicketFrameGoldNumSelectBtn"]		= {"TicketFrame_GoldScrollDownBtn","TradeGoldSaleNumChooseBtn",},
		["TicketFrameSilverNumSelectBtn"]	= {"TicketFrame_SilverScrollDownBtn","TradeSilverSaleNumChooseBtn",},
		["TicketFrameTradeNumSelectBtn"]	= {"GoldListBtnSaleAccountNumBtn",},
		["VipAutoAssistHpSelectBtn"]		= {"VipAutoAssistHpScrollDownBtn","VipAutoAssistHpChooseBtn"},
		["VipAutoAssistMpSelectBtn"]		= {"VipAutoAssistMpScrollDownBtn","VipAutoAssistMpChooseBtn"},
		["YangHaiBangFrameFeedNumBtn"]		= {"YangHaiBangFrameChosenNumBtn","YangHaiBangFrameScrollDownBtn"},
	};

function IsInExceptFrame( szFrame )
	if t_FrameExcept[szFrame] ~= nil then
		for _, except in ipairs( t_FrameExcept[szFrame] ) do
			if isPointInFrame( except ) then
				return true;
			end
		end
	end
	
	local t_exceptInfo = t_FrameExcept[szFrame];
	if t_exceptInfo ~= nil then
		local checkFunc = t_exceptInfo["checkFunc"];
		if checkFunc ~= nil and checkFunc() then
			return true;
		end
	end

	return false
end

function GameSelectLeftButtonDown()
	-- �ټ�鵱ǰ�Ƿ���׷��״̬�����ǣ���ȡ��ԭ����׷��״̬���ٿ�ʼ�µ�׷��
	--[[
	if GameFollowManager:isWeiXing() then
		GameFollowManager:cancelWeiXing();
	end
	--]]
end

t_LeftButtonCloseFrame = {
	"ChatInsertChannelMenuFrame",
	"MainChatFaceFrame",
	"SystemSettingFrame_SelectShadowQualityBtn",
	"PetPhotoListFrame",
	"CreatingAutoTeamRightMenuFrame",
	"PetUI_PKRightOptionFrame",
	"WorldmapSelectNameFrame",
	"SystemSettingFrame_SelectBtn",
	"FriendFrame_RightOptionFrame",
	"FriendFrame_ChatListFrame",
	"MainPlayerRightMenu",
	"PetUI_RightOptionFrame",
	"SystemSettingFrame_SelectAtialiaseBtn",
	"PKSwitch_MainMenu",
	"PKSwitch_SubMenu",
	"TicketFrameBuyTimeSelectBtn",
	"TicketFrameSaleTimeSelectBtn", 
	"ChatFaceFrame",
	"TypeHurtBtn",
	"TypeDpsBtn",
	"TypeDamageBtn",
	"SendNearBtn",
	"SendTeamBtn",
	"SendPrivateBtn",
	"SendGuildBtn",
	"TmpHPShortCut",
	"TmpMPShortCut",
	"SortInfoListMenuFrame",
	"HundredLevelTownMainFrameSelect",
	"HundredLevelTownIntroduceSelect",
	"SecondarySkillDropDownFrame",
	"SecondarySkillLearnDropDownFrame",
	--"TicketFrameBuyTimeSelectBtn",
	"TicketFrameSaleTimeSelectBtn",
	"FriendChatFaceFrame",
	"ClanSetPosDropDownFrame",
	"ClanBasicMenu",
	"SmallKeyboardFrame",
	"GMSelectNameFrame",
	"LoginFrame_UserNameDropDownBtn",
	"ClanFrameProductSelectListBox",
	"ClanProductFrameSelectListBox",
	"ClanCollectTmpFrameRepairExpand",
	"ClanCollectTmpFrameEnergyExpand",
	"ClanCollectTmpFrameFoodExpand",
	"ClanCollectTmpFrameHPExpand",
	"ClanTankTmpFrameRepairExpand",
	"ClanTankTmpFrameEnergyExpand",
	"ClanTankTmpFrameFoodExpand",
	"ClanTankTmpFrameHPExpand",
	"MailOperationTipsFrame",
	"TicketFrameGoldNumSelectBtn",
	"TicketFrameSilverNumSelectBtn",
	"TicketFrameTradeNumSelectBtn",
	"VipAutoAssistHpSelectBtn",
	"VipAutoAssistMpSelectBtn",
	"YangHaiBangFrameFeedNumBtn",
	};

function GameClientLeftButtonDown()	
	local frame;
	for i = 1, table.getn( t_LeftButtonCloseFrame ) do
		frame = getglobal( t_LeftButtonCloseFrame[i] );
		if frame ~= nil and not isPointInFrame( t_LeftButtonCloseFrame[i] ) then
			if not IsInExceptFrame( t_LeftButtonCloseFrame[i] ) then
				frame:Hide();
			end
		end
	end
end

function GameClientRightButtonDown()
	local t_needChangeCursors = { "repair", "exchange", "disableExchange" };
	for _, szCursorState in ipairs( t_needChangeCursors ) do
		if GetCurrentCursor() == szCursorState then
			ChangeCursorState("normal");
			break;
		end
	end
end

function CloseAllFrames( szExceptName )
	local t_Frame = {
		"PetStrengthenConfirmSavePerfectFrame",
		"PetStrengthenConfirmStarBackFrame",
		"ConfirmLeaveTeamFrame",
		"ChatTextColorSettingFrame",
		"ChatPingBiSettingFrame",
		"ChatFaceAndActionFrame",
		"ChangeClanNameFrame",
		"ChangeRoleNameFrame",
		"ChatShortcutSendMsgFrame",
		"MainChatFaceFrame",
		"ChatInsertFrame",
		"MainChatFrame",
		"PetRongHeProcessFrame",
		"ReqirePetRongHeFrame",
		"PetRongHeSuccessFrame",
		"StallBuyerBuyItemFrame",
		"StallBuyerBuyPetFrame",
		"StallSeller_CloseStallFrmaeConfirmFrame",
		"OpenPetSkillConfirmFrame",
		"StallSellerShangJiaItemFrame",
		"StallSellerShangJiaPetFrame",
		--"ResetExistSkillConfirmFrame",
		"DropPetConfirmFrame",
		"AutoTeamBtnModelView",
		"ShortCutButtonsFrameXinShouBtn",
		"ReportBugConfirmFrame",
		"ReportBugFrame",
		"ReplaceBaoShi_ConfirmFrame",
		"DiDuiPlayerTipsFrame",
		"DiDuiPlayerTipsFrame",
		"NpcCommitItemFrame",
		"MultiCommitItemFrame",
		"PetAutoRecoverFrame",
		"PhotoFrameCaptainButton",
		"AutoTeamEnterPworldTimeDownFrame",
		"BaoShiHeChengFrame",
		"PrepareOpenHoleFrame",
		"BaoShiXiangQianFrame",
		"GameTrackSuDiFrame",
		"ChangeCareerFrame",
		"ForbitChgSelectCareerFrame",
		"ConfirmChangeCareerFrame",
		"GameCBTFrame",
		"GiftBtnUVAnimationTex",
		"SelectLeftHabitFrame",
		"WaitQueueFrame",
		"BuffTipsFrame",
		"RideTipsFrame",
		"PetSkillGridTipsFrame",
		"PetSkillTipsFrame",
		"SecondArmItemTipsFrame",
		"ArmItemTipsFrame",
		"LinkItemTipsFrame",
		"NormalItemTipsFrame",
		"ChatBugleFrame",
		"ExtraXinShouCenterGuideButton",
		"PworldSceneFrame",
		"ApplyCreateTeamFirstFrame",
		"WarnNotSelectDiffFrame",
		"ConfirmDifficultyFrame",
		"ConfirmDifficultyAndCareerFrame",
		"WarnAutoTeamFailFrame",
		"WarnTimeCountDownFrame",
		"ConfirmQuitAutoTeamFrame",
		"AutoTeamMemberTipsFrame",
		"CreatingAutoTeamFrame",
		"SuccessCreatTaskAutoTeamFrame",
		"SuccessCreatActiveAutoTeamFrame",
		"SearchTeamFrame",
		"ShopSellFrame",
		"ShopRepairFrame",
		"SellToShopConfirmFrame",
		"ShopSelectRepairFrame",
		"ShopExchangeConfirmFrame",
		"PetUI",
		"MainPlayerLeaveTeamBtn",
		"PhotoFrame",
		"FollowHintFrame",
		"SeePetZiZhiFrame",
		"GameSettingFrame",
		"TroopInviteFrame",
		"TroopInviteTeamFollowFrame",
		"IntonateBar",
		"BlockPrepareSlip",
		"ClanBuyBuildingFrame",
		"ClanCityRegionLevelUpFrame",
		"ClanBuyCityRegionFrame",
		"ClanLevelUpFrame",
		"ClanFrame_ConfirmFrame",
		"ClanDiplomatismFrame",
		"Clan_EditFrame",
		"RequestJoinClanFrame",
		"Clan_SetNiChengInputFrame",
		"Clan_CreateInputFrame",
		"ClanListFrame",
		"ApplyJoinClanFrame",
		"FadeTaskNameFrame",
		--"Pet_SkillGridExtendFrame",
		--"Pet_SkillLevelUpFrame",
		"Pet_SkillLearnFrame",
		"Pet_DevelopmentBuildUpFrame",
		--"Pet_ZengQiangFrame",
		"Pet_XiDianFrame",
		"WatchPetFrame",
		"FadeSceneNameFrame",
		"Shortcut_ThreeFrame",
		"SystemSettingFrame",
		"FriendFrame_ChatListFrame",
		"FriendFrame_ChatClueFrame",
		"SkillFrame",
		"SkillRuneResetFrame",
		"SkillRuneCombineFrame",
		"PetSkillResetFrame",
		"PetSkillLevelUpFrame",
		"PetGrowResetFrame",
		"EquipFrame",
		"RideFrame",
		"StashFrame",
		"PetFrame",
		"PlayerContainerFrame",
		"NpcTalkFrame",
		"ShopFrame",
		"GuideFrame",
		"TargetPrepareBar",
		"NpcLearnSkillFrame",
		"TaskTraceFrame",
		"ShortCutFrame",
		"ClanFrame",
		"TrustFrame",
		"MailHeadFrame",
		"MailSendItemFrame",
		"MailReadFrame",
		"StallSaleFrame",
		"StallBuyFrame",
		"TradeFrame",
		"FriendShipFrame",
		"GuideIcoFrame",
		"SkillLevelUpFrame",
		"PickUpFrame",
		"MinimapFrame",
		"Target",
		"PetFairyUI",
		"TaskAcceptFrame",
		"TaskNotFinishFrame",
		"TaskAwardFrame",
		"TaskLogFrame",
		"ExtendSkillRightFrame",
		"ExtendSkillLeftFrame",	  
		"ExtendSkillFrame",
		"ArmDurSignFrame",
		"CharacterSelectFrame",
		"BirthPlaceFrame",
		"ViewLockFrame",
		"LoginLockFrame",
		"ChangeMouseHabitLockFrame",
		"EffectFrame",
		"TeamMemberFrame",
		"TeamApplyListFrame",
		"TeamAroundPlayerFrame",
		"TeamQiYueFrame",
		"WorldMapFrame",
		"NpcLearnSkillFrame",
		"NpcQuestFrame",
		"PlayerMenu",
		"PworldLeaveTimeFrame",
		"PlayerPetMenu",
		"TeamMenu",
		"ChatMenu",
		"ChannelMenu",
		"ChatInsertChannelMenuFrame",
		"MainPlayerRightMenu",
		"QuitGameFrame",
		"ReallyQuitGameFrame",
		"ShowChangeRoleFrame",
		"ReviveFrame",
		"HelperReviveFrame",
		"SkillFittingFrame",
		"SkillInfoFrame",
		"SpellTipsFrame",
		"GameTooltip",
		"StaticPopup1",
		"StaticPopup2",
		"StaticPopup3",
		"StaticPopup4",
		"SystemSelectFrame",
		"ContactListTipsFrame",
		"QuestLogFrame",
		"QuestWatchFrame",
		"PushQuestFrame",
		"TeamRoleFrame",
		"ChangeNameFrame",
		"ChatInsertFrame",
		"ChatControlFrame",
		"TempShortCutFrame",
		"DiathesisFrame",
		"TipsEffectFrame",
		"PartyMemberBuffTooltip",
		"PromotionFrame",
		"PromontionTipsFrame",
		"TroopInviteFollowFrame",
		"WatchPlayerFrame",
		"ChatHistoryMsgMgrFrame",
		"TempFairyShortCutFrame",
		"TrustOrderListFrame",
		"GuideAwardFrame",
		"ChatShowBugleMsgFrame",
		--"RequestAddFriendFrame",
		"EquipDesignationFrame",
		"TrumpFrame",
		"MarketManagerFrame",
		"MarketBuyItemFrame",
		"MarketContributeFrame",
		"MarketTicketFrame",
		"MarketBuyTicketFrame",
		"MarketSaleTicketFrame",
		"ArmAdvRecastFrame",
		"MessageBoxFrame",
		"BlockMessageBox",
		"ConfirmCoinCertBox",
		"LastAutoFindFrame",
		"ExistenceExpFrame",
		"ExistenceExpInviteFrame",
		"ExistenceExp_ResourceDisplay",
		"InputFrame",
		"XPFrame",
		"ExchangeMoneyFrame",
		"ExchangeMoneyConfirmFrame",
		"DamageCountFrame",
		"RespactFrame",
		"GuideGiftFrame",
		"SortLineFrame",
		"SortInfoMsgFrame",
		"HundredLevelTownNotEnoughDialog",
		"HundredLevelTownNotTimeDialog",
		"HundredLevelTownConfirmUseFreeDialog",
		"HundredLevelTownPayDialog",
		"HundredLevelTownLeaveDialog",
		"HundredLevelTownLotteryRemindDialog",
		"HundredLevelTownFailDialog",
		"HundredLevelTownProcessingDialog",
		"HundredLevelTownFinishDialog",
		"HundredLevelTownIntroduce",
		"HundredLevelTownShowRandomItems",
		"HundredLevelTownMainFrame",
		"HundredLevelTownFinishFrame",
		"GameWheelLotteryFrame",
		"LevelUpTipFrame",
		"CameraCoverFrame",
		"SecondarySkillFrame",
		"SecondarySkillDropDownFrame",
		"SecondarySkillLearnFrame",
		"SecondarySkillLearnDropDownFrame",
		"ChatActionFrame",
		"EnchantFrame",
		"ClanApplyMenu",
		"ClanBasicMenu",
		"GMToolsFrame",
		"ClanLearnSkillFrame",
		"ClanStashFrame",
		"ClanProductFrame",
		"ContainerSetLockFrame",
		"ContainerOpenLockFrame",
		"ContainerSaveGetMoneyFrame",
		"ClanAddSpeedFrame",	
		"ClanSetPositionFrame",
		"ClanSetPosDropDownFrame",
		"CountdownReviveFrame",
		"BattleSummaryFrame",
		"AoShanBattleBossFrame",
		"BattleStartFrame",
		"BattleConfirmFrame",
		"ShortCutButtonsFrame",
		"ClanCollectTmpFrame",
		"ClanTankTmpFrame",
		"SelectSitFrame",
		"MapServerSelectFrame",
		"InfuseExpFrame",
		"ClanSkillTipFrame",
		"ExchangePointFrame",
		"ArmProceUnBindFrame",
		"ArmProceBindFrame",
		"GemProcePickFrame",
		"ArmProceStarFrame",
		"GemProcePickFrame",
		"KeySettingFrame",
		"ContainerChangeRoleFrame",
		"PworldDifficultyFrame",
		"PworldDifficultyTipsFrame",
		"TrumpLevelUpFrame",
		"TrumpStrengthFrame",
		"TrumpKeyInTipsFrame",
		"PworldScoreFrame",
		"WorldCityBuildFrame",
		"WorldCityCaptureFrame",
		"ContainerCancelLockFrame",
		"ContainerChangeLockFrame",
		"StartMudFrame",
		"OpenStashGridFrame",
		"GMSelectNameFrame",
		"VipWheelLotteryFrame",
		"TmpMachineBuffFrame",
		"OpenItemMapFrame",
		"TeamKickVoteFrame",
		"TeamVoteInfoFrame",
		"MarketContributeConfirmFrame",
		"ExistenceExpTotalResultFrame",
		"MessageBoxForGameFrame",
		"MessageWindowFrame",
		"VIPBarMessageFrame",
		"GMGiftsFrame",
		"GMMessageTipsFrame",
		"GMWebFrame",
		"GMWatchPetFrame",
		"ArmRecastFrame",
		"ArmRecastSelectAttrFrame",
		"ClanGiftFrame",
		"ClanOpenTouchTipFrame",
		"TmpItemPickTipsFrame",
		"DreamLandBossFrame",
		"DreamLandSelectLayerFrame",
		"DreamLandTeamFrame",
		"ArmWashFrame",
		"SparSorbFrame",
		"SparFrame",
		"GrowUpWayFrame",
		"GrowUpWayAttentionFrame",
		"MarketItemTipsFrame",
		"ArmProceFrame",
		"ArmProceTipsFrame",
		"QuestHelperFrame",
		"WatchSparFrame",
		"AutoAssistHintFrame",
		"AutoAssistFrame",
		"VipAutoAssistFrame",
		"CountDownFrame",
		"BountyHunterTask",
		"RegisterUserFrame",
		"FriendInfoFrame",
		"FriendMutilSendFrame",
		"MapRecorderFrame",
		"MapRecorderMessageFrame",
		"ConvoyDistanceFrame",
		"ClanIntellingenceFrame",
		"BetterEquipFrame",
		"ChoseClanCityFrame",
		"ChoseCityPosFrame",
		"ContinueRegisterUserFrame",
		"TestWarnFrame",
		"DelAllMailConfirmFrame",
		"ClanMachinePworldFrame",
		"ClanMachinePworldTimeShowFrame",
		"FindGayMessageBoxFrame",
		"IntelligenceRaceJoinFrame",
		"IntelligenceRaceFrame",
		"IntelligenceRaceHintFrame",
		"CowboyRankFrame",
		"ClanStrongInfoFrame",
		"ClanStrongWarResFrame",
		"BaseWarBannerFrame",
		"ClanBannerWordEditeFrame",
		"ClanDonateLogFrame",
		"PworldPortalFrame",
		"PworldPortalConfirmFrame",
		"ClanGetResFrame",
		"WarForcesFrame",
		"HappyCarnivalGuidFrame",
		"PetProceFrame",
		"DailyRewardFrame",
		"PworldPortalPrepareFrame",
		"OfflineExpBuffFrame",
		"RideMoveLevelFrame",
		"RideMoveMessageFrame",
		"MapAwardOreShowFrame",
		"PetAddLoveLevelMessageFrame",
		"MagicSmelterFrame",
		"TianFuFrame",
		"TianFuUpFrame",
		"TianFuTipsFrame",
		"ClanSkillTipsFrame",
		"WideGameTooltip",
		"CostMoneyConfirmFrame",
		"BaoXiangFrame",
		"ArmLevelUpFrame",
		"StrongWarContineKillFrame",
		"StrongWarInfoFrame",
		"NiXiReviveFrame",
		"WorldCityWarContineKillFrame",
		"WorldCityWarInfoFrame",
		"IsBusyFrame",
		"PlayerContainerSparEpurateFrame",
		"ChatAllBugleMsgFrame",
		"TeamMemberSummonFrame",
		"ClanMemberSummonFrame",
		"PworldPolyFrame",
		"ClanBannerGloryFrame",
		"ArmAdvWashFrame",
		"MachineWarInfoFrame",
		"MachineWarResultFrame",
		"MachineWarReadyTimeFrame",
		"VipFrame",
		"ArmAttrMoveFrame",
		"ArmAttrResetFrame",
		"PworldPolyPlayreInfoFrame",
		"AutoFiniQuestFrame",
		"AutoFiniQuestMiniFrame",
		"AutoFiniQuestConfigFrame",
		"AutoRecoveryFrame",
		"PetStarMoveFrame",
		"PetAttrEatFrame",
		"MessageBoxForPetFrame",
		"IdentifyFrame",
		"ClanGroupMemberInfoFrame",
		"ImportantItemConfirmFrame",
		"MessageBoxFrameForVip",
		"ArmProceStarMoveFrame",
		"PerfectStarTip",
		"PetStepUpFrame",
		"ClanBoonFrame",
		"SpartaPlayersFrame",
		"SpartaGetReadyFrame",
		"SpartaWarInfoFrame",
		"SpartaPworldGuidFrame",
		"SpartaApplyWaitFrame",
		"ArmAdvAttrMoveFrame",
		"BackUpFrame",
		"MessageBoxForItemFrame",
		"BackRemakeFrame",
		"ShoolGained_StudentFrame",
		"TaskGuidPopWindow",
		"FlowerEffectFrame",
		"FlowerReportFrame",
		"UseFlowerFrame",
		"SchoolChatFrame",
		"ShoolGainedFrame",
		"RegistStudentsFrame",
		"StudentRegistMsg",
		"SchoolInviteFrame",
		"SchoolJoinFrame",
		"SchoolInfoFrame",
		"XuKongBattleFrame",
		"XuKongBalanceFrame",
		"GuidToSchoolNpcFrame",
		"CardBoxFrame",
		"CardBoxExtendFrame",
		"ActivityLogFrame",
		"FlopRewardFrame",
		"CheckInRewardFrame",
		"DrawMysRewardFrame",
		"MysRewardFrame",
		"GiftCenterGuideFrame",
		"HangHaiFrame",
		"YangHaiBangFrame",
		"ClanDartDeliveryFrame",
		"OnlineStoreFrame",
		"ReturnGiftFrame",
		"ReturnArmSelectFrame",
		"ReturnGiftExpFrame",
		"ArmStepUpFrame",
		"RongYanSelectFrame",
		"RongYanStartFrame",
		"RongYanGuessFrame",
		"RongYanMiniFrame",
		"ArmBaseResetFrame",
		"ArmSkillActiveFrame",
		"ClanWarInfoFrame",
		"ClanWarResultFrame",
		"ClanWarMatchOpenApplyFrame",
		"ClanWarMatchFrame",
		"ClanWarPresentInviteFrame",
		"TradeShopFrame",
		"PworldCountFrame",
		"ArmSkillAttrMoveFrame",
		"ArmUpOrangeFrame",
		"ArmUpOrangeSetFrame",
		"GuChengBattleFrame",
		"TopCareerFrame",
		"TopCareerRuleFrame",
		"TopCareerGuessFrame",
		"TopCareerGuessInfoFrame",
		"TopCareerWinInfoFrame",
		"TopCareerInviteJoinFrame",
		"TopCareerTransFrame",
		"TopCareerSucMatchFrame",
		"TopCareerReadyGuidFrame",
		"PetImpUpFrame",
		"PetFosterChooseFrame",
		"PetFosterPlayFrame",
		"PetFosterFeedFrame",
		"PetFosterFrame",
		"ShopPriceWarnFrame",
		"TankWarFrame",
		"TankWarPlayreInfoFrame",
		"PetGiftBookUseConfirmFrame",
		"GemPackageSelectFrame",
		"PursueFrame",
		"RemoveRelationFrame",
		"OathUIFrame",
		"RemoveRelationCostFrame",
		"RongYanHighLvCallFrame",
		"RongYanHighLvStartFrame",
		"HighLvRongYanGuessFrame",
		"TransWarForcesFrame",
		"UnRealBackFrame",
		"SpanStrongSaleFrame",
		"SpanStrongWarInfoFrame",
		"ClanSpanStrongWarResFrame",
		"SpanStrongWarAwardFrame",
		"PetUnBindFrame",
		"ArmAttrRecreateFrame",
		"ShiKongInfoFrame",
		"ShiKongQueueTimeFrame",
		"PurchaseHomelandFrame",
		"SellHomelandFrame",
		"InquireHomelandFrame",
		"SetNameHomelandFrame",
		"SetStatusHomelandFrame",
		"SecondHomelandFrame",
		"PetHomelandFrame",
		"SeeMessageBoardHomelandFrame",
		"EditMessageBoardHomelandFrame",
		"SelectCameraModeGuideFrame",
		"GeiVigFrame",
	};

	-- ��������Ի���
	for i = 1, MAX_CHAT_FRAME do
		table.insert( t_Frame, "FriendChatFrame"..i );
	end

	for i = 1, table.getn( t_Frame ) do
		if t_Frame[i]~= szExceptName then
			local frame = getglobal( t_Frame[i] );
			if frame ~= nil and frame:IsShown() then
				frame:Hide();
			end
		end
	end
end

function MainPlayerDestroy()	 
	for i = 1, MAX_CENTER_BTS do
		local btn = getglobal( "XinShouCenterGuideButton"..i );
		btn:Hide();
	end

	CloseAllFrames("");
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;

	if TradeFrame:IsShown() then
		GameExchange:cancelExchange( ROLE_LOGOUT_REJECT );
		ClearTradeFrame();
	end

	if RequestAddFriendFrame:IsShown() then
		FriendShipFrame_Refuse_OnClick();
		RequestAddFriendFrame:Hide();
	end
	if ClanPworldIslandShowInfoFrame:IsShown() then
		SetClanPworldBegin( false );
		ClanPworldIslandShowInfoFrame:Hide();
	end
	if ClanMachinePworldTimeShowFrame:IsShown() then
		ClanMachinePworldTimeShowFrame:Hide();
	end

	-- �����ʾ��buf
	ClearBuffButton();
	ClearTargetBuffer();
	ClearPKTips();

	-- �������ϵͳ�е����������¼���е�
	ClearChatFrameRecordMessage();
	for i = 1, MAX_TEAM_MEMBER do
		local button = getglobal( "TeamRoleButton" .. i );
		for j = 1, MAX_TEAM_GOOD_BUFF do
			DelCollBuff( button:GetName().."Buff"..j );	
		end
	end

	if ContainerFrame_EndStallageBtn:IsShown() then
		GameStallManager:stall_End();
		StallSaleFrame_SaleHistoryBtnRich:Clear();
		ContainerFrame_EndStallageBtn:Hide();
		ContainerFrame_StallageBtn:Show();
	end
	
	GameFriendManager:reSetFriendList( "MainPlayerDestroy" );
	
	OfflineRemindFrame:Hide();
	ClearRideListWhenOffline();
	InitMailLocalVariable();
	InitChatFrame();
	-- ���ź��ʼ��������־����
	InitTaskLogFrameData();
	SkillMgr:reSetSkilList();
	
	-- ��������б�
	--Quest:clearQuest();
	-- ���ù�����Ϣ
	GameClanManager:resetClanInfo();
	-- ���ź��ʼ����¼��������
	initSelectFrameData();
	-- ���ÿ�ݼ�
	ResetShortCut();
	InitShotCutLocalVariable();
	-- ���������Ϣ
	GameWizard:zeroSetp();
	GameShortCut:resetShortCut();
	GameMailManager:resetMailData();

	-- �ر����еĶԻ����ݣ���������ʱ���ã�
	closePopWin();
    -- �������ѡ��Ч��
	SkillMgr:clearIntentSpell();
	-- �����ʷί�м�¼
	TrustHistoryFrame_DisplayRich:Clear();
	-- ������������
	ChatManager:resetChatManager();
	-- ���������Ʒ������״̬
	ClearItemLock();
	SingleLockItem();
	-- ��ʼ��������ܿ�����
	InitTaskAcceptFrame();
	-- ��ʼ���ײ���ʱ���������
	InitTmpFrameData();
	-- ��ʼ����������
	InitContainer();
	-- ��ʼ���ƺ�����
	InitEquipDesignation();
	-- ��ʼ��������������
	InitRespactFrame();
	-- ��ʼ����������
	InitTrumpFrame();
	-- ��ʼ����������
	InitSkillFrame();
	-- ��ʼ���������
	InitRideFrame();
	-- ɾ�����������¼
	initFriendData();
	-- ���ߵ�ʱ��������еĽ�������
	RejectAllExchangeInvite( ROLE_LOGOUT_REJECT );
	-- ���ߵ�ʱ��������е��������
	RejectAllRideInvite();
	-- ���߳�ʼ�����г�������
	initPetFrameData();
	initPetOperationFrameData();
	-- �������minimap���߼�����
	initMinimapData();
	-- ��ն����߼�����
	initTeamData();
	-- ���������츳������
	InitGeniusData();
	initPetDialogData();
	--IgnoreAllTeamRequests();
	-- ����˺�ͳ��
	DamageCountFrameReset();
	-- ���߳�ʼ���������
	InitGiftData();
	--[[
	if GameFollowManager:isWeiXing() then
		GameFollowManager:cancelWeiXing();
	end
	--]]
	--������а�����
	ClearRankInfoData();
	InitAutoTeamData();
	InitSkillData();
	clearOverlayRegion();
	-- ���߳�ʼ������ָ�������������
	InitGuideData();
	InitShortCutBtns();
	--��������
	ClearSecondarySkillData();

	local t_AutoRoundWorlldControl = GetAutoRoundWorlldControl();
	t_AutoRoundWorlldControl:init();
	
	local t_SelectLeftHabitFrameControl = GetSelectLeftHabitFrameControl();
	t_SelectLeftHabitFrameControl:init();

	local t_DropPackageCursorControl = GetDropPackageCursorControl();
	t_DropPackageCursorControl:init();
	initTrackInfo();
	initWizardXunFuPet();
	nAcceptTime = 0;
	clearNpcQuestList();

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local machine = mainplayer:getMonsterMachine();
	machine:cancelSkillSelectData();
	mainplayer:breakBTypePre();

	local t_StallBuyFrameBiaoJiStalls = GetStallBuyFrameBiaoJiStalls();
	t_StallBuyFrameBiaoJiStalls:clearAllBiaoJi();

	ClearRollInfo();
	InitChageShowModelAndTitleFrame();
	-- ��վ�������
	local gameTrump = mainplayer:getTrump();
	gameTrump:clearTrumpData();
	GameWheelLottery:clearData();
	GamePromotion:resetProData();
	InitPetSkillResetFrameData();
	InitSparFrameData();
	showUIFrames();
	AutoPworldManager:resetPworldState()
	GeniusMgr:Reset()
	GameClan_BaseWar:clearStrongWarSelfMaxContineKill()
	WorldCityManager:ClearWorldCitySelfContineKill();
	--���ؽ�ɫʱ����յ������
	InitStoreByChangePlayer()
end


RESULT_ATK_ADDMP = 5;					--/* ��������ʱ����MP */ 
RESULT_ICE_ATK = 14;					--/* �������� */ 
RESULT_FIRE_ATK = 15;      				--/* �𹥻��� */ 
RESULT_THUNDER_ATK = 16;      			--/* �׹����� */ 
RESULT_LIGHT_ATK = 27;					--/* �⹥���� */ 
RESULT_NIGHT_ATK = 28;					--/* �������� */ 
RESULT_PHSIC_ATK = 29;					--/* �������� */ 
RESULT_HPMAX = 11;						--/* ���HP */		 
RESULT_MPMAX = 26;						--/* ���MP */ 
RESULT_HITRATE = 18;					--/* ������ */ 
RESULT_FLEE = 19;						--/* ���� */ 	
RESULT_ICE_DEF = 20;					--/* ������ */ 
RESULT_FIRE_DEF = 21;					--/* ����� */ 
RESULT_THUNDER_DEF = 22;				--/* �׷��� */ 
RESULT_LIGHT_DEF = 23;					--/* ����� */ 
RESULT_NIGHT_DEF = 24;					--/* ������ */ 
RESULT_PHSIC_DEF = 25;					--/* ������� */ 
RESULT_DEF_ICEHURT = 31;				--/* �����˺� */
RESULT_DEF_FIREHURT = 32;				--/* �����˺� */
RESULT_DEF_THUNDERHURT = 33;			--/* �����˺� */
RESULT_DEF_LIGHTHURT = 34;				--/* �����˺� */
RESULT_DEF_NIGHTHURT = 35;				--/* �����˺� */
RESULT_DEF_PHISICHURT = 36;				--/* �������˺� */
RESULT_DEF_MAGICHURT = 37;				--/* �������˺� */
RESULT_ICEHURT = 38;					--/* ���˺� */
RESULT_FIREHURT = 39;					--/* ���˺� */
RESULT_THUNDERHURT = 40;				--/* ���˺� */
RESULT_LIGHTHURT = 41;					--/* ���˺� */
RESULT_NIGHTHURT = 42;					--/* ���˺� */
RESULT_PHISICHURT = 43;					--/* �����˺� */
RESULT_MAGICHURT = 44;					--/* �����˺� */
RESULT_HEAVY_HIT = 45;					--/* ���� */
RESULT_HP_ADD = 46;      				--/* ��Ѫ */
RESULT_MP_ADD = 47;      				--/* ��ħ */
RESULT_SELF_STATUS = 48;      			--/* ������ʩ��״̬ */	
RESULT_TMPFILL	= 50;					--/* �����ʱ��ҩ������ */
RESULT_DEF_HURT = 53;      				--/* �������˺� */
RESULT_HURT = 54;      					--/* �����˺� */		  
RESULT_MON_FLEE = 61;   				--/* ֻ�Թ��������õ����� */
RESULT_MON_DEF_HURT = 62;   			--/* ֻ�Ա������ʱ�����õĵֿ������˺� */
RESULT_HEAL_HP = 63;  					--/* ����Ѫ */
RESULT_HEAL_MP = 64;   					--/* ����ħ */
RESULT_ITEM_TASK = 66;  				--/* ������Ʒ���� */
RESULT_ARM_HPADD = 67;  				--/* װ�������ظ� */
RESULT_ARM_MPADD = 68;   				--/* װ��ħ���ظ� */	  	
RESULT_SKILL_BOOK = 75;					--/* ������ */
RESULT_USE_UNDISSOLVE = 76;				--/* ʹ�ò���ʧ */
RESULT_MAX_MOVE_POWER = 77;				--/* ����ƶ��� */
RESULT_FLY = 79;						--/* �����ٶ� */
RESULT_RIDE = 80;						--/* ��˵��� */
RESULT_RIDE_MOVE = 81;					--/* ����ƶ��ٶ� */
RESULT_RIDE_POWER_RESTORE = 82;			--/* ����ƶ����ָ��ٶ� */
RESULT_CALL_OWNER_MON = 83;				--/* ����ˢ�� */
RESULT_REVIVE_PET = 84;					--/* ���߸������( ս��) */
RESULT_ADD_EXP_PET = 85;				--/* �������ӳ��ﾭ�� */
RESULT_ADD_ARM_GRID = 86;				--/* ��������װ������ */
RESULT_ADD_SKILL_GRID = 87;				--/* �������Ӽ��ܸ��� */
RESULT_ADD_POWER = 88;					--/* ��������power */
RESULT_PET_ADD_LOVEVAL = 196; 			--/* ���������ҳ϶� */
RESULT_PET_SKILL_BOOK = 89;				--/* ���＼���� */
RESULT_PET_JIEJING = 90;				--/* ����ᾧ */
RESULT_PET_ADD_SKILLPOINT = 91;			--/* �������Ӽ��ܵ� */
RESULT_EXP = 92;						--/* ����ֵ�ӳ� */
RESULT_PET_HATCH = 93;					--/* ���ﵰ */

RESULT_XIAN_LIST = 94;					--/* ���Ӿ��������� */
RESULT_SHI_LIST = 95;					--/* �������������� */
RESULT_REVIVE_PET_LAOSI = 96;			--/* ���߸������(����) */
RESULT_ROLE_ADD_SKILLPOINT = 97;		--/* ��ɫ���Ӽ��ܵ� */
RESULT_YOUQIN = 98;						--/* ���� */
RESULT_GUAIQIN = 99;					--/* ���� */
RESULT_QIYUE = 100;						--/* ��Լ */
RESULT_COMM_COOL = 101;					--/* ������ȴ�ӳ� */
RESULT_SORB_HP = 102;					--/* ����hp */
RESULT_SORB_MP = 103;					--/* ����mp */
RESULT_HURT_REN = 104;					--/* �����˺��ӳ� */
RESULT_HURT_SHENGWU = 105;				--/* �������˺��ӳ� */
RESULT_HURT_BIANYIREN = 106;			--/* �Ա������˺��ӳ� */
RESULT_ADD_PREPARE = 107;				--/* ����ʱ���ӳ� */
RESULT_DEC_PREPARE = 108;				--/* ����ʱ����С */
RESULT_ADD_ATK_TARGET = 110;			--/* ���ӹ���Ŀ�� */
RESULT_ADD_EXP = 111;					--/* ��ɱ���Ӿ���ֵ */
RESULT_ADD_STATUS_TIME = 113;			--/* �ӳ��ü���״̬�ĳ�����Чʱ�� */
RESULT_ADD_PET_EXP = 114;				--/* ��ɱ���ӳ��ﾭ��ֵ */
RESULT_ADD_SKILL_POINT = 115;			--/* ��ɱ���Ӽ��ܵ� */
RESULT_ADD_KILL_MP = 116;				--/* ��ɱ����MP */
RESULT_ADD_BULL = 117;					--/* ��ɱ����BULL */
RESULT_ADD_KILL_COLLECT = 118;			--/* ��ɱ���ӻ����� */
RESULT_ADD_SHECHENG = 119;				--/* ������� */
RESULT_DESPISE = 120;					--/* ����,�����Ʒ��Խ�ͣ��˺�Խ�� */
RESULT_ADD_COLLECT_UPPER_LIMIT = 121;   --/* ���ӻ��������� */
RESULT_DEC_DEF = 122;					--/* ����ǿ�� */
RESULT_ADD_SKILL_LEVEL = 123;			--/* ����ؼ������Ӽ��ܼ��� */
RESULT_SELF_COOL = 124;					--/* ������ȴʱ����� */
RESULT_SKILL_RANGE = 125;				--/* �������ܷ�Χ */
RESULT_SKILL_CONSUME = 126;				--/* ������������ */
RESULT_DEPURATE = 127;					--/* ���� */
RESULT_BACK_HOME = 128;					--/* �س� */
RESULT_MAKE_TMP_ITEM = 129;				--/* ������ʱ���� */
RESULT_FITTINGS_LEARN = 130;			--/* ���ѧϰ */
RESULT_COOL_CLEAN = 131;				--/* ������ȴʱ����� */
RESULT_FIGHTVAL = 132;					--/* ս���� */
RESULT_DAMAGE = 133;					--/* �ƻ��� */
RESULT_SIT_HP_MODIFY = 134;				--/* ����hp���� */
RESULT_SIT_MP_MODIFY = 135;				--/* ����mp���� */
RESULT_SIT_EXP_MODIFY = 136;			--/* ����exp���� */
RESULT_SIT = 137;						--/* ���� */
RESULT_VIGOR = 138;						--/* ����vigor�ı������� */
RESULT_CONSUME_VIGOR_MODIFY = 139;		--/* ��������vigor���� */
RESULT_NOSIT_ADD_MP = 140;				--/* ����������mp */
RESULT_NOSIT_ADD_HP = 141;				--/* ����������hp */
RESULT_ARM_SKILL_LEVEL = 142;			--/* װ�����Ӽ��ܼ��� */
RESULT_DESPISE_MINI = 143;				--/* ����΢�� */
RESULT_DESPISE_WEAK = 144;				--/* �������� */
RESULT_DESPISE_SOFT = 145;				--/* ������� */
RESULT_DESPISE_NORM = 146;				--/* �����չ� */
RESULT_DESPISE_HARD = 147;				--/* ����Ӳ�� */
RESULT_DESPISE_XI = 148;				--/* ����ϡ�� */
RESULT_DESPISE_JIN = 149;				--/* ���Ӿ��� */
RESULT_DESPISE_BOSS = 150;				--/* ����Boss�� */
RESULT_DESPISE_JI = 151;				--/* ���Ӽ��� */
RESULT_PLAYER_ADD_EXP = 152;			--/* �������Ӿ��� */
RESULT_XP = 153;						--/* ����Ӱ��XPֵ���� */
RESULT_ADD_XP = 154;					--/* ����ֱ������xpֵ */
RESULT_DIATHESIS_UP = 155;				--/* �������� */
RESULT_MON_MAGIC_HURT = 156;			--/* ֻ�Ա������ʱ�����õĵֿ������˺� */
RESULT_ADD_MAX_DUR = 158;				--/* �;ö����� */
RESULT_ADD_BULL_DROP = 159;				--/* ���ҵ����� */
RESULT_UNIQUE_STATUS = 160;				--/* �ͷ�Ψһ��״̬ */
RESULT_XP_MAX = 161;					--/* ��������Ӱ��xp���� */
RESULT_ADD_STUFF_PACKAGE = 162;			--/* ���ϰ���չ */
RESULT_ADD_TASK_PACKAGE = 163;			--/* �������չ */
RESULT_ADD_SUNDRIES_PACKAGE = 164;		--/* �������չ */
RESULT_FAIRY_PIECE = 165;				--/* ����Ƭ */
RESULT_FAIRY_EGG = 166;					--/* �ϳɻ� */
RESULT_ARM_ADD_EXP = 167;				--/* װ��Ч�����ӽ�ɫ����ֵ */
RESULT_DO_SKILL = 168;					--/* ����ʩ�ż��� */
RESULT_MON_ICE_HURT = 169;				--/* ֻ�Ա������ʱ�����õĵֿ����˺� */
RESULT_MON_PHSIC_HURT = 170;			--/* ֻ�Ա������ʱ�����õĵֿ������˺� */
RESULT_ARM_STAR_SAFE = 171;				--/* װ��Ʒ���������շ�Ч�� */
RESULT_ARM_FIGHT_SAFE = 172;			--/* װ��ս�����������շ�Ч�� */
RESULT_GEM_INLAY_SAFE = 173;			--/* ��ʯ��Ƕ���շ�Ч�� */
RESULT_GEM_LEVEL_UP_STUFF = 201; 		--/* ��ʯ������resultval1 ��Ӧ�����ĵȼ� */	  
RESULT_GEM_PICK_STUFF = 202; 		--/* ��ʯժȡ��resultval1 ��Ӧժȡ�ĵȼ� */
RESULT_PHISIC_ICE_ATK = 174;			--/* ����������(ħ ��) */
RESULT_CALL_NPC = 175;					--/* ʹ�õ���  �ٻ�NPC */
RESULT_PULL = 176;						--/* ����/�ֵ���ǰ */
RESULT_TASK_ITEM_RATE = 177;			--/* ������������� */
RESULT_STUFF_ITEM_RATE = 178;			--/* ���Ӳ��ϵ����� */
RESULT_NO_BREAK_PRE = 179;				--/* �������ᱻ��� */
RESULT_DEC_MP = 180;					--/* ���ٶԷ�MP  */
RESULT_HEAVY_HIT_VAL = 181;				--/* �����˺���ֵ����   */
RESULT_ADD_DESIGNATION = 182;			--/* ��ɫ����һ���³ƺ� */
RESULT_ADD_TMPENERGE = 183;				--/* ������ʱ���ߵ����� */
RESULT_ADD_RESPACT = 184;				--/* ��ɫ�������� */
RESULT_FAIRY_BORN = 185;				--/* ������� */
RESULT_ADD_VIM = 186;					--/* ���Ӿ��� */
RESULT_ADD_EXP_ONTIME = 187;			--/* ��N��Ӿ��� ���� */
RESULT_HP_CHG_MP = 188;					--/* hpת����mp  */
RESULT_SKILL_FLUSH = 189;				--/* ���м�������CD--ˢ�¼��� */
RESULT_ADD_XP_ONTIME = 190;				--/* ��N���xp ����Ҫս��״̬ ���� */
RESULT_ADD_BASE_ATTR = 191;				--/* ����һ������ 1-�� 2-�� 3-�� 4-�� ���� */
RESULT_WATER_DEF = 267;					-- /* ˮ����|װ��Ч���ͳ����츳Ч����Val1��ʾ����ˮ������������Val2��ʾ��������Val3��ʾ������ */

RESULT_ADD_BASE_ATTR_HIT		= 197;	--/* һ�����Ծ�׼ */
RESULT_ADD_BASE_ATTR_DEF		= 198; 	--/* һ�����Է��� */
RESULT_ADD_BASE_ATTR_FLEE		= 199; 	--/* һ�����Զ��� */
RESULT_ADD_BASE_ATTR_STR		= 204; 	--/* һ���������� */
RESULT_ADD_BASE_ATTR_INT		= 205; 	--/* һ���������� */
RESULT_ADD_BASE_ATTR_STA		= 206; 	--/* һ���������� */
RESULT_ADD_BASE_ATTR_SPR		= 207; 	--/* һ�����Ծ��� */
RESULT_ADD_BASE_ATTR_AGILE		= 262;  --/* һ���������� */
RESULT_WATER_ATK				= 263; 	--/* ˮ������|װ��Ч�����츳Ч���������츳Ч��������Ч�������Ч����Val1��ʾ����ˮ��������������Val2��ʾ��������Val3��ʾ������ */
RESULT_ADD_BASE_ATTR_HEAVY_HIT	= 208; 	--/* һ�����Ա��� */		 
RESULT_TREASURE_PIC				= 214; 	--/* �ر�ͼ��ͼ���� */
RESULT_ITEM_CHAT				= 233;	--/* С����������Ʒ|С��������������㲥 */
RESULT_SECONDARY_SKILL_RECIPE	= 236;	--/* ѧϰ����� */
RESULT_SECONDARY_SKILL_PRODUCE_CHG = 235;	--/*����������б�ı�*/
RESULT_ADD_SUNDPKG_GRID			= 256;	--�������ﱳ������
RESULT_ADD_STUFFPKG_GRID		= 258;	-- ���Ӳ��ϱ�������
RESULT_BASE_ATTR_HEAVYHIT		= 271;	--/* ������|װ��Ч����Val1��ʾ���ӵ�ֵ */
RESULT_DEF_HEAVYHIT				= 272;	--/* �����ֿ�|װ��Ч����Val1��ʾ���ӵ�ֵ */
RESULT_MAGIC_HP_ADD				= 274; 	--/* ��������Ч��|����Ч����������= ����ħ������*������+��������Val1������Min��Val2������Max�� Val3������Min�� Val3������Max   */
RESULT_ELEMENT_ATK = 277; 				--/* Ԫ���˺�Ч��|val1��ʾ���� 1.�� 2.�� 3.�� 4.�� 5.��  val2��ʾ��С�� val3��ʾ���ֵval4-ʱ��:�� */
RESULT_ELEMENT_DEF = 278;		 		--/* Ԫ�ط���Ч��|val1��ʾ���� 1.�� 2.�� 3.�� 4.�� 5.��  val2��ʾ��С�� val3��ʾ���ֵval4-ʱ��:�� */
RESULT_IGNORE_DEF = 304;				--/* ����Ч��|���ӷ�����ֵ(��ʽ���ʵ�ʺ��ӵķ���) */
RESULT_REFLECTION_HP = 305; 			--/* ����Ч��|������ɵ�Ѫ����ʧֵ(��ʽ���ʵ��ֵ) */
RESULT_DEF_HP = 306;					--/* ����Ч��|�ֿ���ɵ�Ѫ���˺���ֵ(��ʽ���ʵ��ֵ) */
RESULT_MACHINE_ITEM = 308; 				--/* ����Ч��|�����е�ŵ������е���Ʒ��val 2�����е����ID */
RESULT_BEACH_EXP_MUL = 314;			 	--/* ��̲����ӳ�ϵ��(100Ϊ����)|val1Ϊϵ�� */
RESULT_ITEM_SWITCH = 315;				--/* itemresult|val1:1-���� 2-exp 3-��� val2-ת������ */'
RESULT_GEM = 321;					 	--/* ����result|��ʾ����Ʒ�Ǳ�ʯ */
RESULT_GEM_CHIP = 322;					--/* ����result|��ʾ����Ʒ�Ǳ�ʯ��Ƭ|val1��ʾ������ */
RESULT_WEAPON_MIN_PHSIC_ATK = 325; 	--/* ����������|val1 ��������val2 ���ӹ��� */
RESULT_WEAPON_MAX_PHSIC_ATK = 326; 		--/* ����������|val1 ��������val2 ���ӹ��� */
RESULT_WEAPON_MIN_MAGIC_ATK = 327; 		--/* ����ħ������|val1 ��������val2 ���ӹ��� */
RESULT_WEAPON_MAX_MAGIC_ATK = 328;	 	--/* ����ħ������|val1 ��������val2 ���ӹ��� */
RESULT_BASE_ATTR_HP = 329; 				--/* ����ֵ|val1 ������ֵval2 ������ֵ */
RESULT_BASE_ATTR_PHSIC_DEF = 330;		--/* �������|val1 ������ֵval2 ������ֵ */
RESULT_BASE_ATTR_MAGIC_DEF = 331;	 	--/* ħ������|val1 ������ֵval2 ������ֵ */
RESULT_BASE_ATTR_WIND_DEF = 332;	 	--/* ��Ԫ�ؿ���|val1 ������ֵval2 ������ֵ */
RESULT_BASE_ATTR_SOIL_DEF = 333; 		--/* ��Ԫ�ؿ���|val1 ������ֵval2 ������ֵ */
RESULT_BASE_ATTR_THUNDER_DEF = 334;	 	--/* ��Ԫ�ؿ���|val1 ������ֵval2 ������ֵ */
RESULT_BASE_ATTR_LIGHT_DEF = 335;	 	--/* ��Ԫ�ؿ���|val1 ������ֵval2 ������ֵ */
RESULT_BASE_ATTR_NIGHT_DEF = 336;	 	--/* ��Ԫ�ؿ���|val1 ������ֵval2 ������ֵ */
RESULT_BASE_ATTR_FLEE_LEVEL = 337; 		--/* ���ܵȼ�|val1 ������ֵval2 ������ֵ */
RESULT_BASE_ATTR_HIT_LEVEL = 338; 		--/* ���еȼ�|val1 ������ֵval2 ������ֵ */
RESULT_BASE_ATTR_HEAVY_HIT_VAL = 339;	--/* �����˺�|val1 ������ֵval2 ������ֵ */
RESULT_ARM_RECAST = 340;				--/* װ����������|val1 ��������װ������ߵȼ� */
RESULT_BASE_ATTR_PHSIC_ATK = 341;		--/* ������|�������޵Ĺ�����val1 ������ֵval2 ������ֵ */
RESULT_BASE_ATTR_MAGIC_ATK = 342;	 	--/* ħ������|�������޵Ĺ�����val1 ������ֵval2 ������ֵ */
RESULT_ARM_WASH_STUFF = 349; 			--/* װ��ϴ������|����Ч��val1�˲��Ͽ����õ�װ�����ȼ� */
RESULT_SPAR = 350;					 	--/* ��ͨ��ʯ|����Ч�� */
RESULT_SPAR_SHOP = 351;				 	--/* �̳Ǿ�ʯ|����Ч�� */
RESULT_SHANG_JI_TASK = 357;				--/* ����Ч��|�ͽ������|val1-�ͽ���ID����ʾ���Խ�ȡ��һ����ͽ����� */
RESULT_RUNE = 344;					--/* ��ʾ����Ʒ�Ƿ��� val1 ��ʾ���� 0�ͼ� 1�м� 2�߼� val2 ��ʾ��ɫ 1�� 2�� 3�� */
RESULT_MAP_RECORDER = 361; 			--/* �ռ������|����Ч�� ʹ��ʱ��λ������͵�ͼ val1 Ϊ�ɼ�¼���������(����Ϊ5) */
RESULT_RONGLU = 380;			 	--/* ����result|��ʾ��¯ */
RESULT_BAOXIANG = 383;			 	--/* ����Ч��|���䣬Val1 ��ʾ����ID */
RESULT_TILIAN = 385;				--/* ����Ч��|��ʯ����,val1-���ӷֽ�ɹ�����:�����val2-���Ļ�������,val3-���Ļ�������,val4-װ�����ȼ����� */
RESULT_SPAN_CHAT = 387;				--/* ����Ч��|������� */
RESULT_ARM_ATTR_MOVE = 396; 			--/* װ������ת�Ƶ���:val1-�ȼ�:��ʾ��ת�ƶ��ټ���װ�� */
RESULT_POFANG_LEVEL = 399; 			--/* ��̬�Ʒ��ȼ�|val1--ֵ */
RESULT_DIKANG_LEVEL = 400; 			--/* ��̬�ֿ��ȼ�|val1--ֵ */
RESULT_WEAPON_MIN_ATK = 408; 			--/* ����˫ϵ����|val1 ��������val2 ���ӹ��� */
RESULT_WEAPON_MAX_ATK = 409; 			--/* ����˫ϵ����|val1 ��������val2 ���ӹ��� */
RESULT_ITEM_TASK_RECYCLE = 410; 		--/* ����Ч��|��Ʒ��ȡ��������val1 ����ID */
RESULT_BACK_LEVELUP = 413; 				--/* ������������|Val1 ����������������һ���� ����1��ʾ��1��2�ĵ��� */
RESULT_BACK_CHG_TYPE = 414; 			--/* ���θı��������|val1--�ı������ͣ���ӦBackLevelDef->SubType */
RESULT_CHARM = 411;						--/*����ֵ����|val1--�Ӽ���������ֵ */
RESULT_LOST_TOTEM = 412;				--/*ʧ��ͼ�ڵ���|val1-�ٻ�������ʱ��(��),val2--��������,val3-�ٻ���ID,val4-��ö������� */
RESULT_ITEM_EXP_BUCHANG = 415; 			--/* ����Ч��|���߾��鲹������ */
RESULT_ITEM_ARM_BUCHANG = 416;		 	--/* ����Ч��|����װ���������� */
RESULT_ITEM_ARM_SELECT = 417; 			--/* ����Ч��|ѡ��ĳ�ֵ���:val-�ȼ�/10 */
RESULT_ITEM_CARDBOX = 418;			--/* ����Ч��|���Ʊ��� */
RESULT_ITEM_CLAN_CONTRI = 419;			--/* ����Ч��|���ṱ�׶ȣ�ʵ���ϰ�����ֵ */
RESULT_ARM_RECAST_MUST = 420;			--/* װ�������ض��ɹ�����|val1 ��������װ������ߵȼ� */
RESULT_ARM_WASH_STUFF_MUST = 421; 		--/* װ��ϴ���ض��ɹ�����|val1�˲��Ͽ����õ�װ�����ȼ� */
RESULT_LEGEND_SCROLL	= 429; 			--/* ����Ч��|�������val-����ID */
RESULT_CLAN_MEM_NUM_TOP	= 430;			--/* ������������|���Ἴ��Ч�� val1 ������������ֵ */
RESULT_PET_SILIAO = 432;				--	/* ��������:val1-�ظ������±� val2-����ɳ����� */
RESULT_PET_WANJU = 433;					--	/* �������:val1-�Ӷ��ٻ��ֶ� */
RESULT_PET_QIANLI = 434; 				--	/* ����Ǳ��ֵ����:val1-����Ǳ��ֵ */
RESULT_ITEM_BISAI = 435;			 	--	/* ���������Ǹ����� */
RESULT_PET_GENIUS_BOOK = 436;			-- 	/* �����츳��:��һ��result val1-val4��ʾ���� �ڶ���result val1-val2��ʾר������ID */
RESULT_PET_GENIUS_UP = 454;				-- 	/* ����Ч��:����Ʒ������,val1-����Ʒ��,0-3��ʾ��ͨ���ܳ� */
RESULT_ITEM_GEM_PAG = 462;				-- 	/* ����Ч��|��ʯ������,val1����GemPagDef���е�Id */
RESULT_ALL_ATTR = 463 				--/* ����Ч��|val1:װ������-ȫ���ԼӶ��ٵ� */
RESULT_ITEM_JIAJU = 464			--/* ����Ч��|�Ҿ�,�ܹ��ڼ�԰�аڷ�,val1-��Ӧ�Ĺ��� */
RESULT_ITEM_HOME_PET = 465			--/* ����Ч��|��԰����,val1-HomeMonSegDef�е�MonID,��ʾ�滻Ϊ�ĸ����� */
RESULT_ITEM_ZHONGZHI = 466			--/* ����Ч��|��ֲ����val1-��ֲʱˢ�Ĺ�ID,val2-����ʱ��(��) */
RESULT_WEAPON_ATK_MUL = 467; 			--/* ˫ϵ����������|val1 �ٷֱȣ�����˫ϵ�����İٷֱ����� */
RESULT_WEAPON_DEF_MUL = 468; 			--/* ˫ϵ����������|val1 �ٷֱȣ�����˫ϵ�����İٷֱ����� */
RESULT_SPAR_SORB_FREE = 469; 		--/* ����Դ���ܵ���|����ʯ���ܲ�����Ǯ */
RESULT_GEM_ADD_POWER_FREE = 470;		--/* �±�ʯ���ܵ���|����ʯ���ܲ�����Ǯ */

RESULT_NONE				= 0;
MAX_ITEM_RESULT			= 8;
MAX_ITEM_VALUE			= 4;
MAX_ITEM_KEYWORLD		= 5;
MAX_ITEM_SUITSETATTR	= 6;
MAX_ARM_HOLE			= 4;

STATE_INIT		= 0;
STATE_DIRLIST	= 1;
STATE_ROLELIST	= 2;
STATE_MAPLOADING = 3;
STATE_INWORLD	= 4;

t_quickResult = 
{
[RESULT_ICE_ATK] = { desc = "ħ������" },
};

t_tagRESULT =
{
    { RESULT_ICE_ATK,				"ħ������"},
    { RESULT_PHSIC_ATK, 			"������"},
    { RESULT_ICE_DEF,				"ħ������"},
    { RESULT_PHSIC_DEF, 			"�������"},
    { RESULT_HPMAX,					"����ֵ"},
    { RESULT_MPMAX,					"ħ��ֵ"},
    { RESULT_HITRATE,				"���еȼ�"},
    { RESULT_FLEE, 					"���ܵȼ�"},
    { RESULT_DEF_PHISICHURT,		"�����˺�����"},
    { RESULT_DEF_MAGICHURT,			"ħ���˺�����"},
    { RESULT_ARM_HPADD,				"����ֵÿ��ظ�"},
    { RESULT_ARM_MPADD,				"ħ��ֵÿ��ظ�"},

	{ RESULT_HEAVY_HIT, 			"������" },		-- ����
    { RESULT_HEAVY_HIT_VAL, 		"�����˺�" },	-- �����˺�����
    { RESULT_DAMAGE, 				"����ǿ��" },

	{ RESULT_ADD_BASE_ATTR_HIT, 		"��׼"},
    { RESULT_ADD_BASE_ATTR_DEF, 		"����"},
    { RESULT_ADD_BASE_ATTR_FLEE, 		"����"},
	{ RESULT_ADD_BASE_ATTR_STR, 		"����"},
    { RESULT_ADD_BASE_ATTR_INT, 		"����"},
    { RESULT_ADD_BASE_ATTR_STA, 		"����"},
    { RESULT_ADD_BASE_ATTR_SPR, 		"����"},
    { RESULT_ADD_BASE_ATTR_HEAVY_HIT,	"����"},

    { RESULT_ATK_ADDMP, 			"��������ʱ����ħ��ֵ" },
    { RESULT_FIRE_ATK, 				"��Ԫ���˺�" },
    { RESULT_THUNDER_ATK,			"��Ԫ���˺�" },
    { RESULT_LIGHT_ATK, 			"��Ԫ���˺�" },
    { RESULT_NIGHT_ATK, 			"��Ԫ���˺�" },
	{ RESULT_WATER_ATK,				"��Ԫ���˺�" },
    { RESULT_FIRE_DEF, 				"��Ԫ�ؿ���" },
    { RESULT_THUNDER_DEF, 			"��Ԫ�ؿ���" },
    { RESULT_LIGHT_DEF, 			"��Ԫ�ؿ���" },
    { RESULT_NIGHT_DEF, 			"��Ԫ�ؿ���" }, 
    { RESULT_DEF_ICEHURT, 			"ħ���˺�����" },
	{ RESULT_WATER_DEF, 			"��Ԫ�ؿ���" },
    { RESULT_DEF_FIREHURT, 			"��Ԫ�ؿ���" },
    { RESULT_DEF_THUNDERHURT, 		"��Ԫ�ؿ���" },
    { RESULT_DEF_LIGHTHURT, 		"��Ԫ�ؿ���" },
    { RESULT_DEF_NIGHTHURT, 		"��Ԫ�ؿ���" },	
    { RESULT_ICEHURT, 				"���˺�" },
    { RESULT_FIREHURT, 				"���˺�" },
    { RESULT_THUNDERHURT, 			"���˺�" },
    { RESULT_LIGHTHURT, 			"���˺�" },
    { RESULT_NIGHTHURT, 			"���˺�" },
    { RESULT_PHISICHURT, 			"�����˺�" },
    { RESULT_MAGICHURT, 			"�����˺�" },
	{ RESULT_TMPFILL,				"�����ʱ��ҩ������" },
    { RESULT_HP_ADD, 				"��Ѫ" },
    { RESULT_MP_ADD, 				"��ħ" },
    --{ RESULT_SELF_STATUS, 			"������ʩ��״̬" },	 
    { RESULT_DEF_HURT, 				"�������˺�" },
    { RESULT_HURT, 					"�����˺�" },		  
    { RESULT_MON_FLEE, 				"ֻ�Թ��������õ�����" },
    { RESULT_MON_DEF_HURT, 			"ֻ�Ա������ʱ�����õĵֿ������˺�" },
    { RESULT_HEAL_HP, 				"���Ƽӳ�" },
    { RESULT_HEAL_MP, 				"����ħ" },
    { RESULT_ITEM_TASK, 			"������Ʒ����" },	  	
    { RESULT_SKILL_BOOK, 			"������" },
    { RESULT_USE_UNDISSOLVE, 		"ʹ�ò���ʧ" },
    { RESULT_MAX_MOVE_POWER, 		"����ƶ���" },
    { RESULT_FLY, 					"�����ٶ�" },
    { RESULT_RIDE, 					"��˵���" },
    { RESULT_RIDE_MOVE, 			"����ƶ��ٶ�" },
    { RESULT_RIDE_POWER_RESTORE, 	"����ƶ����ָ��ٶ�" },
    { RESULT_CALL_OWNER_MON, 		"����ˢ��" },
    { RESULT_REVIVE_PET, 			"���߸������( ս��)" },
    { RESULT_ADD_EXP_PET, 			"�������ӳ��ﾭ��" },
    { RESULT_ADD_ARM_GRID, 			"��������װ������" },
    { RESULT_ADD_SKILL_GRID, 		"�������Ӽ��ܸ���" },
    { RESULT_ADD_POWER, 			"��������power" },
    { RESULT_PET_SKILL_BOOK, 		"���＼����" },
    { RESULT_PET_JIEJING, 			"����ᾧ" },
    { RESULT_PET_ADD_SKILLPOINT, 	"�������Ӽ��ܵ�" },
    { RESULT_EXP, 					"����ֵ�ӳ�" },
    { RESULT_PET_HATCH , 			"���ﵰ" },

    { RESULT_XIAN_LIST, 			"���Ӿ���������" },
    { RESULT_SHI_LIST, 				"���ӳ���������" },
    { RESULT_REVIVE_PET_LAOSI, 		"���߸������(����)" },
    { RESULT_ROLE_ADD_SKILLPOINT, 	"��ɫ���Ӽ��ܵ�" },
    { RESULT_YOUQIN, 				"����" },
    { RESULT_GUAIQIN, 				"����" },
    { RESULT_QIYUE, 				"��Լ" },
    { RESULT_COMM_COOL, 			"������ȴ�ӳ�" },
    { RESULT_SORB_HP, 				"��������ֵ" },
    { RESULT_SORB_MP, 				"����ħ��ֵ" },
    { RESULT_HURT_REN, 				"�����˺��ӳ�" },
    { RESULT_HURT_SHENGWU, 			"�������˺��ӳ�" },
    { RESULT_HURT_BIANYIREN, 		"�Ա������˺��ӳ�" },
    { RESULT_ADD_PREPARE, 			"����ʱ���ӳ�" },
    { RESULT_DEC_PREPARE, 			"����ʱ����С" },
    { RESULT_ADD_ATK_TARGET, 		"���ӹ���Ŀ��" },
    { RESULT_ADD_EXP, 				"��ɱ���Ӿ���ֵ" },
    { RESULT_ADD_STATUS_TIME, 		" �ӳ��ü���״̬�ĳ�����Чʱ��" },
    { RESULT_ADD_PET_EXP, 			"��ɱ���ӳ��ﾭ��ֵ" },
    { RESULT_ADD_SKILL_POINT, 		" ��ɱ���Ӽ��ܵ�" },
    { RESULT_ADD_KILL_MP, 			"��ɱ����ħ��ֵ" },
    { RESULT_ADD_BULL, 				"��ɱ����BULL" },
    { RESULT_ADD_KILL_COLLECT, 		"��ɱ����ս��" },
    { RESULT_ADD_SHECHENG, 			"�������" },
    { RESULT_DESPISE, 				" ����,�����Ʒ��Խ�ͣ��˺�Խ��" },
    { RESULT_DEC_DEF, 				"����ǿ��" },
    { RESULT_ADD_SKILL_LEVEL, 		"����ؼ������Ӽ��ܼ���" },
    { RESULT_SELF_COOL, 			"������ȴʱ�����" },
    { RESULT_SKILL_RANGE, 			"�������ܷ�Χ" },
    { RESULT_SKILL_CONSUME, 		"������������" },
    { RESULT_DEPURATE, 				"����" },
    --{ RESULT_BACK_HOME, 			"�س�" },
    { RESULT_MAKE_TMP_ITEM, 		"������ʱ����" },
    { RESULT_FITTINGS_LEARN, 		"���ѧϰ" },
    { RESULT_COOL_CLEAN, 			"������ȴʱ�����" },
    { RESULT_FIGHTVAL, 				"ս����" },
    { RESULT_SIT_HP_MODIFY, 		"��������ֵ����" },
    { RESULT_SIT_MP_MODIFY, 		"����mp����" },
    { RESULT_SIT_EXP_MODIFY, 		"����exp����" },
    { RESULT_SIT ,					"����" },
    { RESULT_VIGOR,					"����vigor�ı�������" },
    { RESULT_CONSUME_VIGOR_MODIFY,	"��������vigor����" },
    { RESULT_NOSIT_ADD_MP,			"����������ħ��ֵ" },
    { RESULT_NOSIT_ADD_HP,			"��������������ֵ" },
    { RESULT_ARM_SKILL_LEVEL,		"װ�����Ӽ��ܼ���" },
    { RESULT_DESPISE_MINI,			"����΢��" },
    { RESULT_DESPISE_WEAK,			"��������" },
    { RESULT_DESPISE_SOFT, 			"�������" },
    { RESULT_DESPISE_NORM, 			"�����չ�" },
    { RESULT_DESPISE_HARD, 			"����Ӳ��" },
    { RESULT_DESPISE_XI, 			"����ϡ��" },
    { RESULT_DESPISE_JIN, 			"���Ӿ���" },
    { RESULT_DESPISE_BOSS, 			"����Boss��" },
    { RESULT_DESPISE_JI, 			"���Ӽ���" },
    { RESULT_PLAYER_ADD_EXP, 		"�������Ӿ���" },
    { RESULT_XP, 					"����Ӱ��XPֵ����" },
    { RESULT_ADD_XP, 				"����ֱ������xpֵ" },
    { RESULT_DIATHESIS_UP, 			"��������" },
    { RESULT_MON_MAGIC_HURT, 		"ֻ�Ա������ʱ�����õĵֿ������˺�" },
    { RESULT_ADD_MAX_DUR, 			"�;ö�����" },
    { RESULT_ADD_BULL_DROP, 		""..MONEY_TYPE_MONEY_NAME.."������" },
    { RESULT_UNIQUE_STATUS, 		"�ͷ�Ψһ��״̬" },
    { RESULT_XP_MAX, 				"��������Ӱ��xp����" },
    { RESULT_ADD_STUFF_PACKAGE, 	"���ϰ���չ" },
    { RESULT_ADD_TASK_PACKAGE, 		"�������չ" },
    { RESULT_ADD_SUNDRIES_PACKAGE, 	"�������չ" },
    { RESULT_FAIRY_PIECE, 			"����Ƭ" },
    { RESULT_FAIRY_EGG, 			"�ϳɻ�" },
    { RESULT_ARM_ADD_EXP, 			"װ��Ч�����ӽ�ɫ����ֵ" },
    { RESULT_DO_SKILL, 				"����ʩ�ż���" },
    { RESULT_MON_ICE_HURT, 			"ֻ�Ա������ʱ�����õĵֿ����˺�" },
    { RESULT_MON_PHSIC_HURT, 		"ֻ�Ա������ʱ�����õĵֿ������˺�" },
    { RESULT_ARM_STAR_SAFE, 		"װ��Ʒ���������շ�Ч��" },
	
	{ RESULT_GEM_LEVEL_UP_STUFF, 	"��ʯ������resultval1 ��Ӧ�����ĵȼ�" },	  
	{ RESULT_GEM_PICK_STUFF,		"��ʯժȡ��resultval1 ��Ӧժȡ�ĵȼ�" },

    { RESULT_ARM_FIGHT_SAFE, 		"װ��ս�����������շ�Ч��" },
    { RESULT_GEM_INLAY_SAFE, 		"��ʯ��Ƕ���շ�Ч��" },
    { RESULT_PHISIC_ICE_ATK, 		"����" },
   -- { RESULT_CALL_NPC, 				"ʹ�õ���  �ٻ�NPC" },
    { RESULT_PULL, 					"����/�ֵ���ǰ" },
    { RESULT_TASK_ITEM_RATE, 		"�������������" },
    { RESULT_STUFF_ITEM_RATE, 		"���Ӳ��ϵ�����" },
    { RESULT_NO_BREAK_PRE, 			"�������ᱻ���" },
    { RESULT_DEC_MP = 180, 			"���ٶԷ�MP" },
	

    { RESULT_ADD_DESIGNATION, 			"��ɫ����һ���³ƺ�" },
    { RESULT_ADD_TMPENERGE, 			"������ʱ���ߵ�����" },
    { RESULT_ADD_RESPACT, 				"��ɫ��������" },
    { RESULT_FAIRY_BORN, 				"�������" },
    { RESULT_ADD_VIM, 					"���Ӿ���" },
    { RESULT_ADD_EXP_ONTIME, 			"��N��Ӿ��� ����" },
    { RESULT_HP_CHG_MP, 				"hpת����mp" },
    { RESULT_SKILL_FLUSH,				"���м�������CD--ˢ�¼���" },
    { RESULT_ADD_XP_ONTIME,				"��N���xp ����Ҫս��״̬ ����" },
    { RESULT_ADD_BASE_ATTR,				"����һ������ 1-�� 2-�� 3-�� 4-�� ����" },	  
    { RESULT_ADD_COLLECT_UPPER_LIMIT,	"����ս������" },
	{ RESULT_BASE_ATTR_HEAVYHIT,		"�����ȼ�"},
	{ RESULT_DEF_HEAVYHIT,				"�����ֿ�"},
	{ RESULT_IGNORE_DEF,				"�Ʒ��ȼ�"},
	{ RESULT_REFLECTION_HP,				"���˵ȼ�"},
	{ RESULT_DEF_HP,					"�ֿ��ȼ�"},
	{ RESULT_BASE_ATTR_HP,				"����ֵ"},
	{ RESULT_BASE_ATTR_PHSIC_DEF,		"�������"},
	{ RESULT_BASE_ATTR_MAGIC_DEF,		"ħ������"},
	{ RESULT_BASE_ATTR_WIND_DEF,		"��Ԫ�ؿ���"},
	{ RESULT_BASE_ATTR_SOIL_DEF,		"��Ԫ�ؿ���"},
	{ RESULT_BASE_ATTR_THUNDER_DEF,		"��Ԫ�ؿ���"},
	{ RESULT_BASE_ATTR_LIGHT_DEF,		"��Ԫ�ؿ���"},
	{ RESULT_BASE_ATTR_NIGHT_DEF,		"��Ԫ�ؿ���"},
	{ RESULT_BASE_ATTR_FLEE_LEVEL,		"���ܵȼ�"},
	{ RESULT_BASE_ATTR_HIT_LEVEL,		"���еȼ�"},
	{ RESULT_BASE_ATTR_HEAVY_HIT_VAL,	"�����˺�"},
	{ RESULT_BASE_ATTR_PHSIC_ATK,		"������"},
	{ RESULT_BASE_ATTR_MAGIC_ATK,		"ħ������"},
	{ RESULT_ADD_BASE_ATTR_AGILE,		"��֪"},
	{ RESULT_WEAPON_ATK_MUL,			"˫ϵ����"},
	{ RESULT_WEAPON_DEF_MUL,			"˫ϵ����"},
};

function GetOneItemResultConfig( data )
	local itemResultID	= data["resultID"];
	if itemResultID == RESULT_WEAPON_MIN_PHSIC_ATK or itemResultID == RESULT_WEAPON_MAX_PHSIC_ATK 
		or itemResultID == RESULT_WEAPON_MIN_MAGIC_ATK or itemResultID == RESULT_WEAPON_MAX_MAGIC_ATK 
		or itemResultID == RESULT_WEAPON_MIN_ATK or itemResultID == RESULT_WEAPON_MAX_ATK then
		return nil
	end
	local itemResultDescDef = getItemResultDesc( itemResultID )
	if itemResultDescDef ~= nil then
		return { itemResultDescDef.ResultID, itemResultDescDef.ResultDesc }
	else
		for index, oneItemResultConfig in ipairs( t_tagRESULT ) do
			if oneItemResultConfig[1] == itemResultID then
				return oneItemResultConfig;
			end
		end
	end
	

	return nil;
end

local t_NewEquipBaseAttr = 
{
	RESULT_BASE_ATTR_HP,
	RESULT_BASE_ATTR_PHSIC_DEF,
	RESULT_BASE_ATTR_MAGIC_DEF,
	RESULT_BASE_ATTR_WIND_DEF,
	RESULT_BASE_ATTR_SOIL_DEF,
	RESULT_BASE_ATTR_THUNDER_DEF,
	RESULT_BASE_ATTR_LIGHT_DEF,
	RESULT_BASE_ATTR_NIGHT_DEF,
	RESULT_BASE_ATTR_FLEE_LEVEL,
	RESULT_BASE_ATTR_HIT_LEVEL,
	RESULT_BASE_ATTR_HEAVY_HIT_VAL,
	RESULT_BASE_ATTR_PHSIC_ATK,
	RESULT_BASE_ATTR_MAGIC_ATK,
	RESULT_WEAPON_MIN_PHSIC_ATK,
	RESULT_WEAPON_MAX_PHSIC_ATK,
	RESULT_WEAPON_MIN_MAGIC_ATK,
	RESULT_WEAPON_MAX_MAGIC_ATK,
	RESULT_WEAPON_MIN_ATK,
	RESULT_WEAPON_MAX_ATK,
}
function IsEquipNewBaseAttr( resultID )
	for i=1,#t_NewEquipBaseAttr do
		if t_NewEquipBaseAttr[i] == resultID then
			return true;
		end
	end
	return false;
end

function GetOneItemResultAndValue( data )
	local itemDef = data["itemDef"];
	if itemDef == nil then
		return RESULT_NONE, 0;
	end

	for i = 1, MAX_ITEM_RESULT do
		local oneResult		= itemDef.Result[i-1]
		local itemResultID	= oneResult.ResultID;
		if itemResultID ~= RESULT_NONE then
			return itemResultID, GetOneItemResultFirstEffectValue( oneResult );
		end
	end

	return RESULT_NONE, 0;
end

function GetOneItemResultFirstEffectValue( oneResult )
	if oneResult.ResultVal1 ~= 0 then
		return oneResult.ResultVal1;
	end

	if oneResult.ResultVal2 ~= 0 then
		return oneResult.ResultVal2;
	end

	if oneResult.ResultVal3 ~= 0 then
		return oneResult.ResultVal3;
	end

	if oneResult.ResultVal4 ~= 0 then
		return oneResult.ResultVal4;
	end

	return 0;
end

local t_itemResultPostFix = 
{
[RESULT_HEAVY_HIT]		= "%",
[RESULT_HEAVY_HIT_VAL]	= "%",
[RESULT_BEACH_EXP_MUL]	= "%",
};
function GetItemResultPostfix( data )
	local itemResultID		= data["resultID"];
	local t_postFixConfig	= t_itemResultPostFix[itemResultID];
	if t_postFixConfig == nil then
		return "";
	end

	return t_postFixConfig;
end

local t_itemResultWanFenPostFix = 
{
	RESULT_HEAL_HP
};

function IsFormatWithWanFenWei( data )
	for _, configResultID in ipairs( t_itemResultWanFenPostFix ) do
		if configResultID == data["resultID"] then
			return true;
		end
	end

	return false;
end

function IsFormatWithFraction( data )
	local itemResultID		= data["resultID"];
	local t_postFixConfig	= t_itemResultPostFix[itemResultID];
	return t_postFixConfig ~= nil;
end

function FormatItemAttrValue( data )
	local nValue		= data["value"];
	local itemResultID	= data["resultID"];
	
	if IsFormatWithWanFenWei( { ["resultID"] = itemResultID } ) then
		nValue	= math.min( nValue / 100, 100 )
		return string.format( "%.2f", nValue ).."%";
	elseif IsFormatWithFraction( { ["resultID"] = itemResultID } ) then
		nValue	= math.min( nValue, 100 )
		return nValue.."%";
	end
	
	return nValue;
end

function FormatItemAddAttrValue( data )
	local nValue		= data["value"];
	local itemResultID	= data["resultID"];
	
	--[[
	if itemResultID == RESULT_HEAVY_HIT_VAL or itemResultID == RESULT_BASE_ATTR_HEAVY_HIT_VAL then
		nValue	= math.min( nValue, 100 )
		local interger, fraction = math.modf( nValue );
		--return ( interger.."."..math.floor( fraction * 100 ) ).."%";
		return interger.."."..math.floor( fraction * 100 );
	end
	]]	
	
	return math.floor( nValue );
end

local t_PercentResultID = { 53,54,391,392,393,394,395 }
function IsPercentResultID( resultID )
	for i=1,#t_PercentResultID do
		if t_PercentResultID[i] == resultID then
			return true
		end
	end
	return false
end

-- װ���ؼ���
ADD_ATTR_RESULT_NONE				= 0; 
ADD_ATTR_RESULT_ATK					= 1;	-- /* ���������� */
ADD_ATTR_RESULT_HP					= 2;	-- /* ���Ѫ������ */
ADD_ATTR_RESULT_MP					= 3;	-- /* ���ħ������ */
ADD_ATTR_RESULT_DAMAGE				= 4;	-- /* �ƻ��� */
ADD_ATTR_RESULT_EXP					= 5;	-- /* ս�����Ӿ��� */
ADD_ATTR_RESULT_PHSIC_DEF			= 6;	-- /* ��� */
ADD_ATTR_RESULT_ICE_DEF				= 7;	-- /* ħ�� */
ADD_ATTR_RESULT_ADD_DEF				= 8;	-- /* ˫�� */
ADD_ATTR_RESULT_MAX_DUR				= 9;	-- /* �;ö����� */
ADD_ATTR_RESULT_MIN_ATK				= 10;	-- /* ������С������ */
ADD_ATTR_RESULT_MIN_HP				= 11;	-- /* ���Ѫ��С������ */
ADD_ATTR_RESULT_MIN_MP				= 12;	-- /* ���ħ��С������ */
ADD_ATTR_RESULT_DEF_PHISICHURT		= 13;	-- /* �￹ */
ADD_ATTR_RESULT_DEF_ICEHURT			= 14;	-- /* ħ�� */
ADD_ATTR_RESULT_DEF_ICE_PHISIC		= 15;	-- /* ˫�� */
ADD_ATTR_RESULT_ENERGE_ATK			= 16;	-- /* ���������������� */
ADD_ATTR_RESULT_CANNON_ATK			= 17;	-- /* �ڹ��������� */
ADD_ATTR_RESULT_NOFIGHT_HPADD		= 18;	-- /* HP�ظ�����ֱ�/�룩��ս��״̬ */
ADD_ATTR_RESULT_NOFIGHT_MPADD		= 19;	-- /* MP�ظ�����ֱ�/�룩��ս��״̬ */
ADD_ATTR_RESULT_HPADD				= 20;	-- /* ��ʱ��HP�ظ�����ֱ�/�룩 */
ADD_ATTR_RESULT_MPADD				= 21;	-- /* ��ʱ��MP�ظ�����ֱ�/�룩 */
ADD_ATTR_RESULT_MIN_PHSIC_DEF		= 22;	-- /* ���С������ */
ADD_ATTR_RESULT_MIN_ICE_DEF			= 23;	-- /* ħ��С������ */
ADD_ATTR_RESULT_FIGHTVAL			= 24;	-- /* ս���� */
ADD_ATTR_RESULT_FLEE				= 25;	-- /* ������ */
ADD_ATTR_RESULT_SKILL_LEVEL			= 26;	-- /* ���Ӽ��ܵȼ� */
ADD_ATTR_RESULT_BADSTATUS_RATE		= 27;	-- /* ʩ�Ų���״̬�ɹ��� */
ADD_ATTR_RESULT_BADSTATUS_DEF		= 28;	-- /* �ֿ�����״̬ */
ADD_ATTR_RESULT_BADSTATUS_DEC_TIME	= 29;	-- /* ����״̬����ʱ���½� */
ADD_ATTR_RESULT_HEAVY_HIT			= 30;	-- /* ������ */
ADD_ATTR_RESULT_HITRATE				= 31;	-- /* ������ */
ADD_ATTR_RESULT_TASK_ITEM_RATE		= 32;	-- /* ������Ʒ������ */
ADD_ATTR_RESULT_STUFF_ITEM_RATE		= 33;   -- /* ���ϵ����� */
ADD_ATTR_RESULT_EGG_RATE			= 34;	-- /* ���ﵰ������ */
ADD_ATTR_RESULT_ADD_BULL			= 35;	-- /* �������ҵ����� */
ADD_ATTR_RESULT_MIN_EXP				= 36;	-- /* ս��С�����Ӿ��� */
ADD_ATTR_RESULT_ADD_PET_EXP			= 37;	-- /* ս���������Ӿ���ֵ��� */
ADD_ATTR_RESULT_ADD_SKILLPOINT		= 38;	-- /* ���Ӽ��ܵ��� */
ADD_ATTR_RESULT_FIGHT_HPADD			= 39;	-- /* HP�ظ�ֵս��״̬ */
ADD_ATTR_RESULT_FIGHT_MPADD			= 40;	-- /* MP�ظ�ֵս��״̬ */
ADD_ATTR_RESULT_HITRATE_RATE		= 41;	-- /* ������ */
ADD_ATTR_RESULT_PHSIC_ATK			= 42;	-- /* ������������ */
ADD_ATTR_RESULT_ICE_ATK				= 43;	-- /* ħ������������ */

ADD_ATTR_RESULT_HEAVY_HIT_VAL		= 44;	-- /* �����˺� */	
ADD_ATTR_RESULT_BASE_ATTR_HEAVYHIT	= 45; 	-- /* ������ */
ADD_ATTR_RESULT_DEF_HEAVY_HIT		= 46; 	-- /* �����ֿ� */
ADD_ATTR_RESULT_LILIANG				= 47; 	-- /* ���� */
ADD_ATTR_RESULT_ZHILI				= 48; 	-- /* ���� */
ADD_ATTR_RESULT_TIZHI				= 49;	-- /* ���� */
ADD_ATTR_RESULT_FANGHU				= 50;	-- /* ���� */
ADD_ATTR_RESULT_JINGSHEN			= 51;	-- /* ���� */
ADD_ATTR_RESULT_MINJIE				= 52;	-- /* ���� */
ADD_ATTR_RESULT_WATERATK			= 53;	-- /* ��ˮ������ */
ADD_ATTR_RESULT_FIREATK				= 54;	-- /* �ӻ𹥻��� */
ADD_ATTR_RESULT_THUNDERATK			= 55;	-- /* ���׹����� */
ADD_ATTR_RESULT_LIGHTATK			= 56;	-- /* �ӹ⹥���� */
ADD_ATTR_RESULT_NIGHTATK			= 57;	-- /* �Ӱ������� */
ADD_ATTR_RESULT_WATERDEF			= 58;	-- /* ��ˮ������ */
ADD_ATTR_RESULT_FIREDEF				= 59;	-- /* �ӻ������ */
ADD_ATTR_RESULT_THUNDERDEF			= 60;	-- /* ���׷����� */
ADD_ATTR_RESULT_LIGHTDEF			= 61;	-- /* �ӹ������ */
ADD_ATTR_RESULT_NIGHTDEF			= 62;	-- /* �Ӱ������� */
ADD_ATTR_RESULT_IGNORE_DEF			= 63; 	--/* ���ӷ�����ֵ(��ʽ���ʵ�ʺ��ӵķ���) */
ADD_ATTR_RESULT_REFLECTION_HP		= 64; 	--/* ������ɵ�Ѫ����ʧֵ(��ʽ���ʵ��ֵ) */
ADD_ATTR_RESULT_DEF_HP				= 65; 	--/* �ֿ���ɵ�Ѫ���˺���ֵ(��ʽ���ʵ��ֵ) */
ADD_ATTR_RESULT_HEAl_HP_MUL			= 66;	-- /* ���Ƽӳ� */
ADD_ATTR_RESULT_DEC_WEAR_LEVEL		= 73; 	-- /* ���ٴ����ȼ�Ҫ��val1 ���ٵĵȼ� */


t_tagArmRESULT =
{
    { ADD_ATTR_RESULT_ATK,					"����������" },
    { ADD_ATTR_RESULT_HP, 					"���Ѫ������" },
    { ADD_ATTR_RESULT_MP, 					"���ħ������" },
    { ADD_ATTR_RESULT_DAMAGE,				"�ƻ���" },
    { ADD_ATTR_RESULT_EXP,					"ս�����Ӿ���" },
    { ADD_ATTR_RESULT_PHSIC_DEF, 			"���" },
    { ADD_ATTR_RESULT_ICE_DEF,				"ħ��" },
    { ADD_ATTR_RESULT_ADD_DEF,				"˫��" },
    { ADD_ATTR_RESULT_MAX_DUR,				"�;ö�����" },
    { ADD_ATTR_RESULT_MIN_ATK,				"������С������" },
    { ADD_ATTR_RESULT_MIN_HP,				"���Ѫ��С������" },
    { ADD_ATTR_RESULT_MIN_MP,				"���ħ��С������" },
    { ADD_ATTR_RESULT_DEF_PHISICHURT,		"�����˺�����" },	-- "�￹"
    { ADD_ATTR_RESULT_DEF_ICEHURT,			"ħ���˺�����" },	-- "ħ��"
    { ADD_ATTR_RESULT_DEF_ICE_PHISIC,		"˫��" },
    { ADD_ATTR_RESULT_ENERGE_ATK,			"����������������" },
    { ADD_ATTR_RESULT_CANNON_ATK,			"�ڹ���������" },
    { ADD_ATTR_RESULT_NOFIGHT_HPADD,		"HP�ظ�����ֱ�/�룩��ս��״̬" },
    { ADD_ATTR_RESULT_NOFIGHT_MPADD,		"MP�ظ�����ֱ�/�룩��ս��״̬" },
    { ADD_ATTR_RESULT_HPADD,				"��ʱ��HP�ظ�����ֱ�/�룩" },
    { ADD_ATTR_RESULT_MPADD,				"��ʱ��MP�ظ�����ֱ�/�룩" },
    { ADD_ATTR_RESULT_MIN_PHSIC_DEF, 		"���С������" },
    { ADD_ATTR_RESULT_MIN_ICE_DEF,			"ħ��С������" },
    { ADD_ATTR_RESULT_FIGHTVAL, 			"ս����" },
    { ADD_ATTR_RESULT_FLEE, 				"������" },
    { ADD_ATTR_RESULT_SKILL_LEVEL,			"���Ӽ��ܵȼ�" },
    { ADD_ATTR_RESULT_BADSTATUS_RATE,		"ʩ�Ų���״̬�ɹ���" },
    { ADD_ATTR_RESULT_BADSTATUS_DEF, 		"�ֿ�����״̬" },
    { ADD_ATTR_RESULT_BADSTATUS_DEC_TIME,	"����״̬����ʱ���½�" },
    { ADD_ATTR_RESULT_HEAVY_HIT, 			"������" },
    { ADD_ATTR_RESULT_HITRATE,				"������" },
    { ADD_ATTR_RESULT_TASK_ITEM_RATE, 		"������Ʒ������" },
    { ADD_ATTR_RESULT_STUFF_ITEM_RATE,		"���ϵ�����" },
    { ADD_ATTR_RESULT_EGG_RATE, 			"���ﵰ������" },
    { ADD_ATTR_RESULT_ADD_BULL, 			"����"..MONEY_TYPE_BULL_NAME.."������" },
    { ADD_ATTR_RESULT_MIN_EXP,				"ս��С�����Ӿ���" },
    { ADD_ATTR_RESULT_ADD_PET_EXP,			"ս���������Ӿ���ֵ���" },
    { ADD_ATTR_RESULT_ADD_SKILLPOINT,		"���Ӽ��ܵ���" },
	{ ADD_ATTR_RESULT_FIGHT_HPADD,			"HP�ظ�ֵս��״̬" },
    { ADD_ATTR_RESULT_FIGHT_MPADD,			"MP�ظ�ֵս��״̬" },
    { ADD_ATTR_RESULT_HITRATE_RATE,			"������" },
    { ADD_ATTR_RESULT_PHSIC_ATK,			"������������" },
    { ADD_ATTR_RESULT_ICE_ATK,				"ħ������������" },
    { ADD_ATTR_RESULT_HEAVY_HIT_VAL,		"�����˺�" },
    { ADD_ATTR_RESULT_HEAl_HP_MUL,			"���Ƽӳ�" },
};

-- ��Ϊ����ȥ���ַ���ǰ��ͺ���ո��ַ�ʹ�õ�
function eraseSpace( str )
	local erasePreStr = "";

	for i = 1, string.len( str ) do
		if string.sub( str, i, i ) ~= " " and string.sub( str, i, i ) ~= "\n" then
			erasePreStr = string.sub( str, i, string.len( str ) );
			break;
		end
	end

	if erasePreStr == "" then
		return erasePreStr;
	end

	for i = string.len( erasePreStr ), 1, -1 do
		if string.sub( erasePreStr, i, i ) ~= " " then
			str = string.sub(  erasePreStr, 1, i );
			break;
		end
	end

	return str;
end

-- ��Ϊ��������������
function ScrollBar_OnMouseWheel()
	if not this:IsShown() then
		return;
	end

	local nScrollLine	= arg1;
	local absChange		= math.abs( arg1 );
	-- �����Ϲ���
	if arg1 > 0 then
		local nValue = this:GetValue() - absChange * this:GetValueStep();
		if nValue >= 0 then
			this:SetValue( nValue );
		else
			this:SetValue( 0 );
		end
	-- �����¹���
	else
		local nMaxValue = this:GetMaxValue();
		local nValue	= this:GetValue() + absChange * this:GetValueStep();

		if nValue <= nMaxValue then
			this:SetValue( nValue );
		else
			this:SetValue( nMaxValue );
		end
	end
end
--@Desc:�رյ�ǰ����
function CloseCurFrame( szCloseFrame )
	local Frame = getglobal( szCloseFrame );
	if Frame:IsShown() then
		Frame:Hide();
	end
end

--@Desc:�������и�����
function DisCheckAllHighLightBtn( szButtonName, nMax )
	for i=1, nMax do
		local CheckBtn = getglobal( szButtonName..i.."HighlightBtn");
		CheckBtn:DisChecked();
	end
end

function DisCheckAllBtn( szButtonName, nMax )
	for i=1, nMax do
		local CheckBtn = getglobal( szButtonName..i );
		CheckBtn:DisChecked();
	end
end

--@Desc:����Font�ؼ��ı�
function SetFontText( szFrame, nValue, t_Color )
	local font = getglobal(szFrame);
	font:SetText( nValue );
	if string.find( szFrame, "TrustInfo" ) ~= nil then
		return;
	end
	font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
end

function ShowIcon( szIconName )
	local icon = getglobal( szIconName );
	icon:Show();
end

--@Desc:����Font�ؼ��ı�
function SetRichText( szFrame, nValue, t_Color )
	local font = getglobal(szFrame);
	font:SetText( nValue, t_Color[1], t_Color[2], t_Color[3] );
end

--@Desc:����ְҵ��Font�ؼ��ı�
function SetCareeFontText( szFrame, nValue, t_Color )
	local font = getglobal(szFrame);
	font:SetText(GetCareerName(nValue));
	font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
end

--@Desc:���ù�ְ��Font�ؼ��ı�
function SetPositionFontText( szFrame, nValue, t_Color )
	local font = getglobal(szFrame);
	font:SetText(GetPosition(nValue));
	font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
end

--@Desc:����Texture�ؼ�����ʾ��Ʒͼ�꣩
function SetTexture( szTextureName, nIconID, t_Color )
	local texture = getglobal( szTextureName );
	assert( texture ~= nil, "�Ƿ�����!" .. szTextureName );

	local IconPath = GetItemEquipIconPath()..nIconID..".tga";
	-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	texture:SetTexture( IconPath );

	texture:Show();
end

function SetBoxTexture( szTextureName )
	local texture = getglobal( szTextureName );
	assert( texture ~= nil, "�Ƿ�����!" .. szTextureName );
	texture:Show();
end

--@Desc:�����ʼ�״̬ͼ��ĺ���
function SetMailState(  szFrame, nValue, t_Color  )
	local texture = getglobal(szFrame);
	local button  = getglobal( texture:GetParent() );
	texture:Show();
	if bitAnd( nValue,MAIL_FLAG_SYSTEM ) then 
		button:SetClientString("ϵͳ�ʼ�");
		if bitAnd( nValue,MAIL_FLAG_READ ) then
			local IconPath = GetItemEquipIconPath().."5500003.tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			texture:SetTexture( IconPath );
			
		else
			local IconPath = GetItemEquipIconPath().."5500004.tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			texture:SetTexture( IconPath );
		end
	else
		button:SetClientString("����ʼ�");
		if bitAnd( nValue,MAIL_FLAG_READ ) then
			local IconPath = GetItemEquipIconPath().."5500002.tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			texture:SetTexture( IconPath );
		else
			local IconPath = GetItemEquipIconPath().."5500001.tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			texture:SetTexture( IconPath );
		end
	end
end

function SetButton( szFrame , nValue, t_Color )
	local button = getglobal(szFrame);
	button:Show();
	if nValue ~= NONE and nValue ~= nil then
		button:SetClientUserData( 0, tonumber(nValue) );
	end
end

function SetRichFrame( szFrame, Value )
	local RichFrame = getglobal(szFrame);
	RichFrame:SetText( Value, 255, 255, 255 );
end

function SetMemFontFrame(  szFrame, Value, bOnLine )
	local MemFontFrame = getglobal( szFrame );
	MemFontFrame:SetText( Value );
	if not bOnLine then
		MemFontFrame:SetTextColor(120, 115, 100);
	end
end

--@Desc:����ڹ����е�ְλ
function GetPlayPositionInClan()
	local player = ActorMgr:getMainPlayer();
	if player == nil then return end;
    local szUserName	= player:getName();
	local ClanViewer	= GameClanManager:getClanViewer();
	local nClanMemNum	= ClanViewer.m_clanInfo.m_nClanMemNum;
	for i=1 , nClanMemNum do
		if szUserName == ClanViewer.m_Members[i-1].RoleMember.m_szRoleName then
			return ClanViewer.m_Members[i-1].Position;
		end
	end
	return "";
end

--@Desc:�Ƿ��ǻ᳤
function IsClanPresident()
	local player = ActorMgr:getMainPlayer();
	if player == nil then return end;
    local szUserName = player:getName();
	local ClanViewer = GameClanManager:getClanViewer();
	local szPresidentName = ClanViewer.m_PresidentInfo.m_szRoleName;
	if szUserName == szPresidentName then
		return true;
	end
	return false;
end

function MergeTables( t_Tabel1, t_Table2 )
	local t_SortTable = {};
	for i = 1, table.getn( t_Tabel1 ) do
		table.insert( t_SortTable, t_Tabel1[i]);
	end
	for i = 1, table.getn( t_Table2 ) do
		table.insert( t_SortTable, t_Table2[i]);
	end
	return t_SortTable;
end

-------------------------------------------------------------------------------
POSE_NONE					= 10; --// ����
--// սʿ
POSE_SHORTWEAPON_R		= 20; --// ������Ҷ�����
POSE_WEAPON_SHORTWEAPON	= 24; --// �����ֶ�����
--// ��ʦ
POSE_WAND				= 30; --// ����+����
--// ��ʦ
POSE_CHURCH_WAND		= 32; --// ����+����
--// ǹ��
POSE_GUN_R				= 40; --// ����ǹ
POSE_SCATTERGUN			= 42; --// ˫�ֻ�ǹ
POSE_DOUBLEGUN			= 47; --// ����ǹ
POSE_END				= 48;

ARM_FIGHTER				= 10; --//  սʿ����
ARM_WIZARD				= 11; --//  ��ʦ����
ARM_GUN					= 12; --//  ǹ��-ǹ
ARM_CANNON				= 13; --//  ǹ��-��
ARM_TEMP					= 14; --//  ��ʱ����
ARM_CHURCH				= 15; --//  ��ʦ����
ARM_RACE_SECONDARY		= 11;	--�����
-----------------------------------------------------------------------------
t_posestate =
{
--���� Id/100000	Id/10000				��̬					���� ����1 ս��8
-- սʿ
-- 100
	{ ARM_FIGHTER,	ARM_SWORD,				POSE_SHORTWEAPON_R,		0,		3},
-- 101
	{ ARM_FIGHTER,	ARM_KNIFE,				POSE_SHORTWEAPON_R,		0,		3},
-- 103
	{ ARM_FIGHTER,	ARM_BOTHSWORD,			POSE_WEAPON_SHORTWEAPON,0,		3},
-- 104
	{ ARM_FIGHTER,	ARM_SWORD_LEFT,			POSE_NONE,				0,		3},
-- 106
	{ ARM_FIGHTER,	ARM_SHIELD,				POSE_SHORTWEAPON_R,		0,		3},

-- ��ʦ
-- 110
	{ ARM_WIZARD,	ARM_MAGIC_STAFF_ICE,	POSE_WAND,				0,		3},
-- 111
	{ ARM_WIZARD,	ARM_MAGIC_STAFF_FIRE,	POSE_WAND,				0,		3},
-- 118
	{ ARM_WIZARD,	ARM_MAGIC_FAQI,			POSE_WAND,				0,		3},
	

--���� Id/100000	Id/10000				��̬
-- ǹ��
-- 120
	{ ARM_GUN,		ARM_RIGHT_GUN,			POSE_GUN_R,				8,		8},
-- 121
	{ ARM_GUN,		ARM_BITH_GUN,			POSE_DOUBLEGUN,			8,		8},
-- 125
	{ ARM_GUN,		ARM_HEVY_SCATTERGUN,	POSE_SCATTERGUN,		8,		8},
-- 127
	{ ARM_GUN,		ARM_LIUDAN_GUN,			POSE_SCATTERGUN,		8,		8},
-- 128
	{ ARM_GUN,		ARM_GUNNER_WEAR_LEFT,	POSE_NONE,				8,		8},
	
	-- ��˾
	-- 150
	{ ARM_CHURCH,	ARM_GOD_STAFF_LIGHT,	POSE_CHURCH_WAND,		0,		3},
	-- 151
	{ ARM_CHURCH,	ARM_GOD_STAFF_THUNDER,	POSE_CHURCH_WAND,		0,		3},
	-- 158
	{ ARM_CHURCH,	ARM_GOD_FAQI,			POSE_CHURCH_WAND,		0,		3},


	{ ARM_RACE_SECONDARY,ARM_SECONDARY,		POSE_WEAPON_SHORTWEAPON,0,		3},
}

function SetRichSlider( rich, szSlider, sliderTex, upbtn, downbtn )
	local rich			 = getglobal( rich );
	local slider		 = getglobal( szSlider );  
	local sliderThumbTex = getglobal( szSlider.."ThumbTexture" );	
	local slider_Tex	 = "";
	local upbtnTex		 = "";
	local downbtnTex	 = "";
	if upbtn ~="" and downbtn ~= "" then
		upbtnTex	= getglobal( upbtn );
		downbtnTex	= getglobal( downbtn );
	end
	if sliderTex ~= "" then
		slider_Tex	= getglobal( sliderTex );
	end

	local nViewLine	= rich:GetViewLines();
	local nAllLine	= rich:GetTextLines();
	rich:SetDispPos(rich:GetStartDispPos());
	slider:SetValue(0);
	slider:Hide();
	sliderThumbTex:Hide();

	if nAllLine - nViewLine > 0 then
		if slider_Tex ~= "" then
			slider_Tex:Show();
		end
		if upbtnTex ~="" and downbtnTex ~= "" then
			upbtnTex:Show();
			downbtnTex:Show();
		end
		local nMaxValue = nAllLine - nViewLine; 
		slider:Show();
		slider:SetMaxValue( nMaxValue );
		sliderThumbTex:SetSize( 15, slider:GetHeight()*(1-nMaxValue/(nMaxValue+nViewLine)));
		slider:SetMinValue(0);
		slider:SetValueStep(1);
	else
		if upbtnTex ~="" and downbtnTex ~= "" then
			upbtnTex:Hide();
			downbtnTex:Hide();
		end

		if sliderTex ~= "" then
			slider_Tex:Hide();
		end
	end
end
-- 

function OUTPUTDATA( szLogMsg )
	--[[local writeFile =  assert(io.open("log/MinimapLuaLog.txt","a+"));
	writeFile:write( "��"..os.date().."��".. szLogMsg.."\n");
	writeFile:close();--]]
end

-- ��������޸���Ϣʱ��Ӧ
function EmotionShowBtn_OnClick()
	local showbtn			= getglobal( this:GetParent().."_EmotionShowBtn");
	local emotionedit		= getglobal( this:GetParent().."_EmotionEdit");
	showbtn:Hide();
	emotionedit:Show();
	SetFocusFrame( this:GetParent().."_EmotionEdit" );
	emotionedit:SelectAllText();
end

-- �༭��Ϣʱ������Ӧ
function EmotionEdit_OnChar()
	local emotionshowtex = getglobal( this:GetParent().."_EmotionShowTex");
	emotionshowtex:Hide();
end

-- ���س���ʧȥ����ʱ��Ӧ
function EmotionEdit_OnFocusLost()
	local showbtn			= getglobal( this:GetParent().."_EmotionShowBtn");
	local emotionedit		= getglobal( this:GetParent().."_EmotionEdit");
	showbtn:Show();
	emotionedit:Hide();

	if this:GetParent() == "PetFrame" then
		Pet_HousecarlSetNameBtn_OnClick();
	end
end

function HideUIFrame( szParent )
	local parentframe = getglobal( szParent );
	parentframe:Hide();
end

local t_Suit = {};
function setSuitEquipId()

end

function ReplaceQuestDesc( szText )
	if szText == nil then
		return
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	local ReplaceInfo=
	{
		["#Name#"]		= MainPlayer:getName(),									--/*   */
		["#Gender#"]	= GetGender(MainPlayer:getGender()),					--/*   */
		["#Race#"]		= GetCareerName(MainPlayer:getRace()),					--/*   Ver.6 ͼ��id */
		["#b"]			= "\b",				
		["#t"]			= "\t",
		["#Gender1#"]	= MainPlayer:getGender() == CLIENT_GENDER_MALE and "���" or "���",
		["#Gender2#"]	= MainPlayer:getGender() == CLIENT_GENDER_MALE and "˧��" or "��Ů",
		["#Gender3#"]	= MainPlayer:getGender() == CLIENT_GENDER_MALE and "��ү" or "����"
	};

	for name, szRep in pairs(ReplaceInfo) do
		local v = string.find(szText,name);
		if v ~= 0 then
			szText = string.gsub(szText,name,szRep);
		end
	end

	return szText;
end

function ReplaceText( szText, srcText, desText )
	if szText == "" then
		return;
	end
	
	if srcText == ""  or desText == "" then
		return szText;
	end

	local v = string.find(szText,srcText);
	if v ~= nil and v ~= 0 then
		szText = string.gsub(szText,srcText,desText);
	end
	return szText;
end

-- �Ƿ���ָ���ؼ�����ʾ������
function IsInFrameAbs( szChildframe, szFrame )
	local childframe			= getglobal( szChildframe );
	local frame					= getglobal( szFrame );

	local nframeRealLeft		= frame:GetRealLeft();
	local nframeRealTop			= frame:GetRealTop();
	local nframeRealRight		= frame:GetRealLeft() + frame:GetWidth();
	local nframeRealBottom		= frame:GetRealTop() + frame:GetHeight();

	local nchildframeLeft	= childframe:GetRealLeft();
	local nchildframeWidth	= childframe:GetWidth();

	if ( nchildframeLeft >= nframeRealLeft and nchildframeLeft <= nframeRealRight )
		or ( nchildframeLeft + nchildframeWidth >= nframeRealLeft and nchildframeLeft + nchildframeWidth <= nframeRealRight ) then
		return true;
	end

	return false;
end

function PlayClickSound( szClickType, szBtnName )
	if szClickType == "RightButton" then
		if string.find( szBtnName, "PlayerContainerFrame", 1, true ) then
			local MainPlayer = ActorMgr:getMainPlayer();
			if MainPlayer == nil then
				return
			end

			local container = MainPlayer:getContainer();
			if container == nil then
				return
			end

			local Btn = getglobal( szBtnName );
			local Item = container:getItem( CONTAINER_TYPE_SUNDRIES, Btn:GetClientID()-1 );
			PlayItemSound( Item:getItemId() );
		end
	end
end

local t_ItemTypes =
	{
		{ "ҩˮ",			5000000,	5007199, "sounds\\UI\\shiyongyaoshui.wav" },
		{ "ʳ��",			5010000,	5011011, "" },
		{ "���ᡢ������",	5040001,	5041000, "sounds\\UI\\shiyongjuanzou.wav" },
		{ "������",			5050000,	5050004, "sounds\\UI\\shiyongbaoshi.wav" },
		{ "�����Ƽ�",		5051000,	5051999, "" },
		{ "�����",		5052000,	5052999, "" },
		{ "�һ���",			5060001,	5060003, "" },
		{ "����",			5061000,	5061007, "" },
		{ "����",			5600000,	5609999, "" },
		{ "���ﵰ",			5610000,	5619999, "" },
		{ "�������",		5620000,	5629999, "" },
		{ "װ������",		5200000,	5202111, "" },
		{ "װ����ʯ",		5210000,	5219170, "sounds\\UI\\shiyongbaoshi.wav" },
		{ "�������",		5230000,	5230005, "" },
		{ "���ܵ���",		5250000,	5251002, "" },
		{ "����",			5300000,	5310000, "" },
		{ "ί��",			5061200,	5061249, "" },
		{ "���＼����",		5231000,	5231999, "" },
		{ "�ƺ�",		5043001,	5043999, "" },
	}

function PlayItemSound( id )
	for i = 1, table.getn(t_ItemTypes) do
		if id >= t_ItemTypes[i][2] and id <= t_ItemTypes[i][3] and t_ItemTypes[i][4] ~= "" then
			playUISound( t_ItemTypes[i][4] );
		end
	end
end

-- ��ȡ��Ʒ����
function GetItemTypeById( id )
	for i = 1, table.getn(t_ItemTypes) do
		if id >= t_ItemTypes[i][2] and id <= t_ItemTypes[i][3] then
			return t_ItemTypes[i][1];
		end
	end
	return "����";
end

function AutoHideNomalTips( szFrameName )
	if NormalItemTipsFrame:IsShown() then
		if IsInFrameAbs( "NormalItemTipsFrame", szFrameName ) then
			NormalItemTipsFrame:Hide();
		end
	end
end

function ScirptCreateActor( nId, nActorType )
	if MapShower:getRealMapId() >= CLIENT_GAME_PWORLD_MAPID then
		local actor = ActorMgr:FindActor( nId );
		MapShower:updateMapNpcInfo( actor:getResId(), actor:getPlayerX() / 10, actor:getPlayerZ() / 10 );
	end
end	

function ScirptDestroyActor( nResID )
	if MapShower:getRealMapId() >= CLIENT_GAME_PWORLD_MAPID then
		MapShower:deleteMapNpcInfo( nResID );
	end
end

function CanExChange( item )
	if item == nil then
		return 3;
	end

	local itemdef = item:getItemDef();
	if itemdef.ExchgLevel > 0 then
		return 3;
	end

	if item:isItemBind() then
		return 3;
	end

	if itemdef.NoDeal ==  1 or itemdef.NoDrop == 1 then
		return 3;
	end

	return 0;
end

function GetExChangeErrorMsg( nError, szFrom )
	if szFrom == "" or szFrom == nil then
		szFrom = "other";
	end
	local t_ExChangeErrorMsg = 
		{
			[1] = { msg = "����Ʒ", ["mail"] = "�����ʼ�", ["stall"] = "�����ϼ�", ["exchange"] = "���ɽ���", ["other"] = "������ͨ" },
			[2] = { msg = "ί����Ʒ", ["mail"] = "�����ʼ�", ["stall"] = "�����ϼ�", ["exchange"] = "���ɽ���", ["other"] = "������ͨ" },
			[3] = { msg = "����ͨ��Ʒ", ["mail"] = "�����ʼ�", ["stall"] = "�����ϼ�", ["exchange"] = "���ɽ���", ["other"] = "������ͨ" },
		}
	return t_ExChangeErrorMsg[nError].msg..t_ExChangeErrorMsg[nError][szFrom];
end

-- д�� LUA LOG �ļ�
function LUALOGINFO( szLogMsg )
	local writeFile =  assert(io.open("log/lua.log","a+"));
	writeFile:write( "��"..os.date().."��".. szLogMsg.."\n");
	writeFile:close();
end

function ReSizeTooltip( szrich, szFrame, nWidthExtend )
	local rich	= getglobal( szrich );
	local frame = getglobal( szFrame );
	if rich == nil or frame == nil then
		return;
	end
	rich:resizeRect(112*GetScreenScaleY(), 20*GetScreenScaleY())
	local szText = rich:GetClientString();
	rich:SetText( szText, 86, 79, 54 );
	local nWidth = 1;
	for i = 1, rich:GetTextLines() do
		local nLineWidth = rich:getLineWidth( i - 1 );
		if nWidth < nLineWidth then
			nWidth = nLineWidth;
		end
	end

	local nHeight = rich:GetTotalHeight();
	nHeight = ( nHeight == 0 ) and 1 or nHeight;
	rich:resizeRect( nWidth, nHeight );
	rich:SetSize( math.ceil(nWidth / GetScreenScaleY()), math.ceil(nHeight / GetScreenScaleY()) );
	rich:SetText( szText, 86, 79, 54 );
	frame:SetSize(nWidth / GetScreenScaleY() + 20, nHeight / GetScreenScaleY() + 10);
end

-- ����ָ����ʾTIPS����
function HideGuideEffectTip( szExceptFrame ) 
	-- ��szExceptFrameΪnil��ʱ���������е�tips���
	-- ��szExceptFrame��Ϊnil��ʱ����ָ��tips��岻ͬ���������
	local t_hideGuideEffect = { "EffectFrame", "TipsEffectFrame", "PetDialogFrame" }
	for _, szFrame in ipairs( t_hideGuideEffect ) do
		local frame = getglobal( szFrame );
		if ( szExceptFrame == nil and frame:IsShown() ) 
			or ( szExceptFrame ~= nil and szExceptFrame ~= szFrame and frame:IsShown() ) then
			frame:Hide();
		end
	end
end

function GameStart()
	if not GameMgr:IsLastVideoError() then
		if GameMgr:isFirstRun() then
			--GameMgr:playVideo("resource\\9c.wmv")
			GameMgr:playVideo("resource\\wanyouyl.wmv")
		end
		GameMgr:playVideo("resource\\ljy.wmv")
	end
end

-- ������ǰ��Ʒ
function LockCurItem( item, szParent )
	if item:getItemId() ~= 0 then
		LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), szParent ); -- �����Ʒ��ӳɹ�����ס����Ʒ
	end
end

function isMainPlayerInWeixing()
	return GameFollowManager:isWeiXing();
end

local tSuitNameColor = {
							{130,255,135},	-- ǳ��
							{160,255,255},	-- ǳ��
							{255,185,255},	-- �ۺ�
							{255,115,0  },  -- ��ɫ
						}

local tItemNameColor = {
							[0] = GetColor()[COLOR_WHITE],
							[1] = GetColor()[COLOR_GREEN],
							[2] = GetColor()[COLOR_BULE],
							[3] = GetColor()[COLOR_PURPLE],
							[4] = {255,185,255},
							[5] = {255,185,255},
							[6] = {255,185,255},
							[7] = {255,115,0  },
							[8] = {255,115,0  },
						}
function GetItemNameColor( itemDef )
	if bitAnd( itemDef.CtrlFlag, ITEM_COLOR_GREEN_TOLUA ) then
		return GetColor()[ITEM_IMPORTANT_EXCELLENCE]
	elseif bitAnd( itemDef.CtrlFlag, ITEM_COLOR_BLUE_TOLUA ) then
		return GetColor()[ITEM_IMPORTANT_BEST]
	elseif bitAnd( itemDef.CtrlFlag, ITEM_COLOR_PURPLE_TOLUA ) then
		return GetColor()[ITEM_IMPORTANT_NONSUCH]
	elseif bitAnd( itemDef.CtrlFlag, ITEM_COLOR_PINK_TOLUA ) then
		return tSuitNameColor[3]
	end
	if itemDef.SuitSetID > 0 then
		local suitDef = getSuitSet( itemDef.SuitSetID )
		if tSuitNameColor[ suitDef.SuitSetType ] ~= nil then
			return tSuitNameColor[ suitDef.SuitSetType ]
		end
	end
	return tItemNameColor[itemDef.Important];
end

local t_ColorVal = {
	[0] = "0",	[1] = "1",	[2] = "2",	[3] = "3",
	[4] = "4",	[5] = "5",	[6] = "6",	[7] = "7",
	[8] = "8",	[9] = "9",	[10] = "a",	[11] = "b",
	[12] = "c",	[13] = "d",	[14] = "e",	[15] = "f",
}
function RGBColorToRichColor( t_Color )
	local szColor = "#c";
	for i = 1, table.getn( t_Color ) do
		local val1 = t_ColorVal[math.floor(t_Color[i]/16)];
		local val2 = t_ColorVal[t_Color[i]%16];
		szColor = szColor..val1..val2;
	end
	return szColor;
end

function UseItemFailedNotInMap( mapid, range )
	local szName = getMapName( mapid )
	if range == 0 then
		ShowMidTips( "����" .. szName .. "ʹ�ø���Ʒ��" )
	else
		ShowMidTips( "����" .. szName .. "��ָ������ʹ�ø���Ʒ��" )
	end
end

function getPlayerMoney( actor,moneyType )
	if actor == nil then return 0 end
	if moneyType == nil or moneyType == ATTR_ID_BULL then
		local levelupDef = getLevelUP( actor:getLv() )
		if levelupDef ~= nil then
			local leftNum = levelupDef.DailyUseBull - actor:getDailyUsedBull();
			leftNum = leftNum > 0 and leftNum or 0;
			if actor:getBull() > leftNum then
				return actor:getMoney() + leftNum;
			end
			return actor:getMoney() + actor:getBull();
		end
		return actor:getMoney() + actor:getBull()
	else
		return actor:getMoney()
	end
end