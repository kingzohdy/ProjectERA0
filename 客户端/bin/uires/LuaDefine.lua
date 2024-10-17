--定义全局变量及函数，封装的局部变量
DEFAULT_UI_WIDTH = 1024;
DEFAULT_UI_HEIGHT = 768;
g_monsterPinZhi = { [0] = "微", [1] = "弱", [2] = "软", [3] = "普", [4] = "硬", [5] = "稀", [6] = "精", [7] ="BOSS", [8]= "极" };
-----------------------------------------------------------晶石系统提示-相关-------------------------------------------------------------
--能源
DEFAULT_NENGYUAN_TASKID = 1085;
--晶石
DEFAULT_JINGSHI_TASKID = 1086;
--吸收
DEFAULT_XISHOU_TASKID = 1087;
-----------------------------------------------------------急你所急航空提示-相关----------------------------------------------------------
DEFAULT_WORLD_SEND_GUID_TASKID = 3131;
--------------------------------------------------------------公会情报攻防战相关----------------------------------------------------------
CLAN_FLAG_SPY_AVAILABLE_ONE = 1
CLNA_FLAG_TASK_AVAILABLE_TWO = 3
CLNA_FLAG_TASK_AVAILABLE_THREE = 7
CLAN_FLAG_TASK_SPY = 2
CLAN_FLAG_TASK_SPY_ANTI = 4
CLAN_SPY_LEVEL_LIMIT_MIN = 20
--------------------------------------------------------------赏金的GroupID--------------------------------------------------------------
TASK_SJ_GROUPID = 133
---------------------------------------------------------------分解相关---------------------------------------------------------------
--打开分解界面的等级限制
CONTAINER_RESOLVE_LEVEL_LIMIT_MIN = 20;
--------------------------------------------------------------------------------------------------------------------------------------

---------------------------------------------------------------宝石相关----------------------------------------------------------------
GEM_START_MAX = 4;
GEM_LEVEL_MAX = 7;
GEM_CHARGE_LEVEL_MIN = 3;
GEM_CHARGE_LEVEL_MAX = 15;
GEM_RESOLVE_LEVEL_MIN = 4;
--------------------------------------------------------------------------------------------------------------------------------------

-----------------------------------------------------------------战争势力图--------------------------------------------------------------
MAX_STRONG_NUM = 6
----------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------公会冶炼岛地图MapID------------------------------------------------
CLAN_EVERYDAY_PWORLD_MAP_ID_LUA = 60100
--------------------------------------------------------------------------------------------------------------------------------

-------------------------------------------------------------老玩家回归表里最大天数------------------------------------------------
RETURN_GIFT_MAX_DAY = 30
--------------------------------------------------------------------------------------------------------------------------------

-- 宝石效果
GEM_EffECT_STAR							= 0;	-- 升品质(星)宝石
GEM_EffECT_FIGHT						= 1;	-- 升战斗力等级宝石
GEM_EffECT_LEVEL						= 2;	-- 升装备等级宝石
GEM_EffECT_INLAY						= 3;	-- 镶嵌宝石

-- 登录失败原因
AUTH_REASON_PASSWD_ERROR				= 1; 	-- /* 密码不合法 */
AUTH_REASON_NO_ACCOUNT					= 2; 	-- /* 没有这个账号 */
AUTH_REASON_FREEZE						= 3; 	-- /* 账号被冻结 或账号不存在 */
AUTH_IFM_ACCOUNT_ERROR					= 4; 	-- /* 激活时,用户名不合法 */
AUTH_IFM_PASSWD_ERROR					= 5; 	-- /* 激活时,密码不合法 */
AUTH_REASON_FREEZE_TRYMAX				= 6; 	--/* 密码尝试次数过多冻结账号 */
AUTH_REASON_FREEZE_CHECKTOOL			= 7; 	--/* 检查工具冻结账号 */
AUTH_IFM_CERTID_ERROR					= 11; 	--/* 输入的身份证不正确 */
AUTH_IFM_AAS_ERROR						= 12; 	-- /* 用户属于防沉迷用户,已经激活过一个大区了 */
AUTH_IFM_NOUSE_SERVICE					= 14; 	--/* 无法使用服务 */
AUTH_IFM_ACCOUNT_EXSIT					= 15; 	-- /* 账户已经存在 */
AUTH_IFM_ACCOUNT_ACTIVATION				= 16; 	-- /* 账户已经激活游戏 */
AUTH_IFM_ACCOUNT_REGISTER_ERROR			= 17; 	-- /* 用户注册失败 */
AUTH_IFM_ACCOUNT_VALID_ERROR			= 18; 	-- /* 验证密码失败 */
AUTH_IFM_GET_ACCOUNT_INFO_ERR			= 20; 	-- /* 获取账户详细信息失败 */
AUTH_IFM_GAME_REGION_ERROR				= 21; 	-- /* 游戏大区编号不正确 */
AUTH_IFM_GAME_ACTIVE_ERROR				= 22; 	-- /* 激活游戏大区失败 */
AUTH_IFM_USE_IDCARD_ERROR				= 30; 	-- /* 用户身份证已经存在 */
AUTH_IFM_USE_IDCARD_REGIST_TIMES_ERROR	= 31; 	-- /* 该用户注册身份证超过50次 */
AUTH_IFM_AASUSE_IDCARD_EXIST_ERROR		= 32; 	-- /* 防沉迷用户注册身份证已存在 */
AUTH_IFM_USE_IDCARD_FORMAT_ERROR		= 39; 	-- /* 错误的身份证格式 */
AUTH_IFM_NEED_REGIST_CODE_ERROR			= 40; 	-- /* 内测期间，需要激活码,需要弹出激活码的对话框 */
AUTH_IFM_REGIST_CODE_ERROR				= 41; 	-- /* 激活码不正确，无法激活游戏 */
AUTH_IFM_ACCOUNT_REGISTER_SUCCES		= 42; 	-- /* 账号注册成功 */
AUTH_IFM_SYSTEM_BUSY					= 9999; -- /* 系统繁忙  */

-- 委托是否可以发布
CAN_DISTRIBUTE		= 1;
CAN_NOT_DISTRIBUTE	= 2;

-- 性别分类定义
GENDER_NO		= 0;
GENDER_MAN		= 1;
GENDER_WOMAN	= 2;

-- 键值
VK_BACK		= 8;
VK_DELETE	= 46;
VK_ESC		= 27;
VK_SPACE	= 32;
VK_UP		= 38;  
VK_DOWN		= 40;
-- 物品类型
ITEM_TYPE_TASK	= 3;
ITEM_NONE_BIND	= 0;

-- 消息提示 类型
CHATTIPS_TYPE			= 3;	-- 消息类型 聊天区提示

SYS_TIP				= 0;	--/* 提示信息 */
SYS_SVR				= 1; 	--/* 系统信息 */
SYS_COMBAT			= 2; 	--/* 战斗信息 */
SYS_MIDDLE			= 3;	--/* 中央三行滚动提示信息 */
SYS_MIDDLE2			= 4;	--/* 中央提示信息 */
SYS_BULLETIN		= 5;	--/* 公告信息 */
SYS_CHAT			= 6; 	--/* 聊天频道 */
SYS_POP_WINDOW		= 7;	--/* 弹出窗口提示信息 */ 
SYS_BUGLE			= 8;	--/* 小喇叭 */
SYS_MSG_STUFF		= 9; 	--/* 材料 */
SYS_MSG_TASK		= 10; 	--/* 任务物品 */
SYS_MSG_ITEM		= 11; 	--/* 道具 */
SYS_MSG_ARM			= 12; 	--/* 装备 */
SYS_MSG_CHAT_TIP	= 13;	--/* 聊天框提示信息 */
SYS_CLAN			= 14; 	--/* 公会频道 */
SYS_TEAM			= 15;	--/* 组队频道 */
SYS_ACTIVE			= 16;	--/* 活动频道 */
SYS_CURR			= 17;	--/* 始终当前频道页显示 */
SYS_SHOW 			= 18; 	--/* 炫耀频道 */

--全局变量
g_nCAREER_NON     = 0;
g_nCAREER_WARRIOR = 1;  --战士
g_nCAREER_MAGICOR = 2;  --法师
g_nCAREER_GUNER   = 3;	--枪手
g_nCAREER_CHURCH  = 4;	--牧师
g_nCAREER_WARRIOR_SINGEL = 17
g_nCAREER_WARRIOR_DOUBLE = 33
g_nCAREER_MAGICOR_SINGEL = 18
g_nCAREER_MAGICOR_DOUBLE = 34
g_nCAREER_GUNER_SINGEL = 19
g_nCAREER_GUNER_DOUBLE = 35
g_nCAREER_CHURCH_SINGEL = 20
g_nCAREER_CHURCH_DOUBLE = 36

MONEY_TYPE_MONEY_NAME			= "银币";   -- 游戏中流通货币
MONEY_TYPE_BULL_NAME			= "绑银";   -- 游戏中绑定货币
MONEY_TYPE_GODCOIN_NAME			= "金币";   -- 游戏中流通充值币
MONEY_TYPE_BIND_GODCOIN_NAME	= "金券";   -- 游戏中绑定充值币
MONEY_TYPE_GODCOIN				= "金币";	-- 金币
MONEY_TYPE_CONTRI_NAME			= "贡献度";

t_raceList =
{
	[g_nCAREER_WARRIOR]		= "战士",
	[g_nCAREER_MAGICOR]		= "法师",
	[g_nCAREER_GUNER]		= "枪手",
	[g_nCAREER_CHURCH]		= "祭司",

	[g_nCAREER_WARRIOR_SINGEL]	= "守护者",
	[g_nCAREER_WARRIOR_DOUBLE]	= "征服者",
	[g_nCAREER_MAGICOR_SINGEL]	= "聚能者",
	[g_nCAREER_MAGICOR_DOUBLE]	= "炼魂师",
	[g_nCAREER_GUNER_SINGEL]	= "猎杀者",
	[g_nCAREER_GUNER_DOUBLE]	= "毁灭者",
	[g_nCAREER_CHURCH_SINGEL]	= "圣佑者",
	[g_nCAREER_CHURCH_DOUBLE]	= "驭雷师",
};

FONTSYTLE_NORMAL	= 0;	--普通
FONTSYTLE_SHADOW	= 1;	--阴影
FONTSYTLE_BORDER	= 2;	--描边
FONTSYTLE_EMBOLDEN	= 3;	--加粗
TEAM_POSITION_MEMBER	= 0; -- 表示队友
TEAM_POSITION_CAPTAIN	= 1; -- 表示队长

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

ATTR_ID_MONEY		= 20;	-- 银币
ATTR_ID_BULL		= 21;	-- "绑银"
ATTR_ID_GODCOIN		= 60;	-- 金币
ATTR_ID_BINDGODCOIN	= 61;	-- 金券	
ATTR_ID_STASHMONEY	= 32;	-- 仓库银券	
ATTR_ID_STASHBULL	= 65;	-- 仓库银币
ATTR_ID_CONTRI		= 66;	-- 公会贡献度
ATTR_ID_SPARTA_GLORY	= 84;	-- 竞技场荣誉

LIST_TYPE_XIAN	= 7; 		--/* 精灵栏位 */
LIST_TYPE_SHI	= 8;		--/* 侍卫栏位 */
PET_SKILLGRID_MAX	= 10; -- 技能格
PET_ARMGRID_MAX		= 6;  -- 装备格
FAIRY_MAX_NUM		= 10; -- 法宝最大栏位 
GSACT_IN_MORPH	= 15;
GSACT_STALL		= 12;

-- 精灵状态
FAIRY_STAT_CAMP		= 1;  --/* 出征 */ 	
FAIRY_STAT_BACK		= 2;  --/* 召回 */
FAIRY_STAT_EVOLVE	= 4;  --/* 炼化 */
FAIRY_STAT_DROP		= 8;  --/* 丢弃 */

FAIRY_MAX_SKILL		= 4;  --/* 精灵技能上限 */	

-- 装备颜色枚举
GEM_COLOR_RED = 0; 
GEM_COLOR_BLUE = 1;
GEM_COLOR_GREEN = 2;
GEM_COLOR_COLOR = 3;

-- 联系人TIPS类型
CONTACT_TYPE_MAIL			= 1;
CONTACT_TYPE_MARKET			= 2;
CONTACT_TYPE_SENDITEM_MAIL	= 3;

t_GemColorDefind = 
{
	{ "红色",GEM_COLOR_RED }, 
	{ "蓝色",GEM_COLOR_BLUE }, 
	{ "绿色",GEM_COLOR_GREEN }, 
	{ "多彩",GEM_COLOR_COLOR }, 
};

USE_TARGET_PET = 1; 	--/* 目标是宠物 */			   

-- 默认物品图标路径
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

--@Used: 进度条的开始位置、进度条类型
startx					= 584;
RIDE_TYPE				= 1;
COLLECT_TYPE			= 2;

PET_HOUSECARL_TYPE		= 3;
PET_FAIRY_TYPE			= 4;
ITEM_USE_TYPE			= 5;
TRUMP_TYPE_PRE			= 6;

CHANNEL_TYPE			= 7;
SECONDARYSKILL_RESOLVE_TYPE	= 8;	--生活分解技能
SECONDARYSKILL_OTHER_TYPE = 9;		--其他生活技能
PREPARE_TYPE			= 10;
RESOLVE_TYPE			= 11;

-- 小喇叭的ID
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

--@Desc:装备加工
ARM_ACT_STAR			= 0;	--/* 装备品质提升 */
ARM_ACT_LEVEL			= 1;	--/* 装备等级提升 */
ARM_ACT_FIGHT			= 2;	--/* 装备战斗力提升 */
ARM_ACT_IDENTIFIABILITY = 3; 	--/* 装备鉴定 */
GEM_ACT_LEVEL_UP		= 4;	--/* 宝石升级 */
GEM_ACT_INLAY			= 5;	--/* 宝石镶嵌 */
GEM_ACT_PICK			= 6;	--/* 宝石摘取 */
SAVEITEMTYPE			= 1000;	--/* 保险符类型 */
MAX_STUFF_ARMPROCE		= 10;
ARMPROCE_SRC			= 11;	-- 装备类型
ARMPROCE_SAFEITEM		= 12;	-- 保险符类型
ARM_STAR_LIMIT			= 12;


MAXEQUIPPOS				= 22;	-- 最大装备位

GEM_EFFECT_STAR  = 0; --/* 升品质(星)宝石 */
GEM_EFFECT_FIGHT = 1; --/* 升战斗力等级宝石 */
GEM_EFFECT_LEVEL = 2; --/* 升装备等级宝石 */
GEM_EFFECT_INLAY = 3; --/* 镶嵌宝石 */
GEM_INLAY = 10;

UI_TYPE_UPGRADE_CLAN_CITY			= 18; 	-- /* 升级城市 */
UI_TYPE_UPGRADE_CLAN_CITY_BUILDING	= 19; 	-- /* 升级城市建筑 */
UI_TYPE_UPGRADE_CLAN				= 20; 	-- /* 升级公会 */

UI_TYPE_UPGRADE_TECH_CENTER			= 21; 	--/* 升级科技中心 */
UI_TYPE_UPGRADE_COLLECTION_CENTER	= 22; 	--/* 升级采集中心 */
UI_TYPE_UPGRADE_FACTORY				= 23; 	--/* 升级工厂 */
UI_TYPE_UPGRADE_FARM				= 24; 	--/* 升级兽栏 */
UI_TYPE_UPGRADE_WALL				= 48; 	--/* 升级城防 */


CLAN_BUILDING_CIVIL			= 0; 	-- /* 行政局 */
CLAN_BUILDING_FINANCE		= 1; 	-- /* 财务局 */
CLAN_BUILDING_ARM			= 2; 	-- /* 兵工厂 */
CLAN_BUILDING_TECHNOLOGY	= 3; 	-- /* 科技局 */
CLAN_CITY_MAP_ID		= 60000;
BEACH_MAP_ID			= 4403;
BRAVEISLAND_MAP_ID		= 4402;
CARNIVAL_MAP_ID			= 20100;

MOVE_DURATION				= 0.5;    -- /* 窗口的持续移动时间 */

--@desc:对应标签面板当前的x,y位置
--@used:切换标签时保持位置同步
EquipRealX	= 0;
EquipRealY	= 0;
SkillRealX	= 0;
SkillRealY	= 0;
PetRealX	= 0;
PetRealY	= 0;
--@desc:技能配件数量设定
MAX_SKILL_FITTINGS	= 3;
NONEFITTING			= -1;
FITTINGSLOT			= NONEFITTING;
FITTINGID			= NONEFITTING;
--@desc:技能类型：素质
SKILL_TYPE			= 0;
DIATHESIS_TYPE		= 1;
TRUMP_SKILL_TYPE = 1002;
TMP_TANK_SKILL_TYPE = 1003;
RUNE_SKILL_TYPE		= 1004;
--@desc:宠物类型设定
SHITYPE					= 1;  -- 侍卫
XIANTYPE				= 0;  -- 精灵
--@desc:对应EquipFrame.lua的table:t_RoleAttrInfo
--@used:获取角色的以下各项属性

NONE			= 0;
ONEPAGE			= 1;
MAIL_FLAG_SYSTEM	= 1;
MAIL_FLAG_READ		= 2;

MAXMAILITEM		= 8;

MAX_SKILL_ICON	= 10
MAXSTALLITEM    = 30;
MAXSTALLPET		= 10;

STALL_TYPE_VIEW	= 2;

MAIL_HEAD_FIRST_PAGE	= 0; 	--/* 第一页 */
MAIL_HEAD_NEXT_PAGE		= 1; 	--/* 下页 */
MAIL_HEAD_CURRENT_PAGE	= 2; 	--/* 当前页 */
MAIL_HEAD_PREV_PAGE		= 3; 	--/* 上页 */

t_ItemButtonCheckIndex = {};	--/* 记录ItemTemplate点中的物品{{窗口1,栏位1},{窗口2,栏位3}}

--@desc:用于指定最多个数侍卫和精灵
MAX_PET_XIAN	= 10;
MAX_PET_SHI	= 5;

--@Desc:选中的任务ID
nCurSelectQuestId	= 0;
nCurSelectRecycleId = 0;

ATTR_ID_LILIANG			= 4; 	--/* 力量 */
ATTR_ID_ZHILI			= 8; 	--/* 智力 */		
ATTR_ID_TILI			= 12; 	--/* 体质 */		
ATTR_ID_JINGSHEN		= 13; 	--/* 精神 */		
ATTR_ID_FANGYU			= 10; 	--/* 防护 */		
ATTR_ID_SHANBI			= 9; 	--/* 闪躲 */		
ATTR_ID_MINGZHONG		= 11; 	--/* 暴击 */

ATTR_ID_PHSIC_ATK		= 35; 	--/* 物理攻击力 */
ATTR_ID_ICE_ATK			= 36; 	--/* 冰元素攻击力 */
ATTR_ID_BASE_ATTR_HEAVY = 73;	-- /* 爆击力 */
ATTR_ID_BASE_ATTR_DEFHEAVY = 74;	-- /* 暴击抵抗 */
ATTR_ID_HEAVY_VAL = 75;	-- /* 爆击伤害数值修正(增加的百分比) */

ATTR_ID_PHSIC_DEF		= 41; 	--/* 物理防御力 */
ATTR_ID_ICE_DEF			= 42; 	--/* 冰元素防御力 */
ATTR_ID_FIRE_DEF		= 43; 	--/* 火元素防御力 */
ATTR_ID_THUNDER_DEF		= 44; 	--/* 雷元素防御力 */
ATTR_ID_LIGHT_DEF		= 45; 	--/* 光元素防御力 */
ATTR_ID_NIGHT_DEF		= 46; 	--/* 暗元素防御力 */

ATTR_ID_DEF_PHSIC_HURT		= 47; 	--/* 物理抵抗力 */
ATTR_ID_DEF_ICE_HURT		= 48; 	--/* 冰元素抵抗力 */
ATTR_ID_DEF_FIRE_HURT		= 49; 	--/* 火元素抵抗力 */
ATTR_ID_DEF_THUNDER_HURT	= 50; 	--/* 雷元素抵抗力 */
ATTR_ID_DEF_LIGHT_HURT		= 51; 	--/* 光元素抵抗力 */
ATTR_ID_DEF_NIGHT_HURT		= 52; 	--/* 暗元素抵抗力 */

ATTR_ID_HEAVY			= 53; 	--/* 爆击率 */
ATTR_ID_HIT				= 54; 	--/* 命中 */
ATTR_ID_FLEE			= 55;	--/* 闪避 */
ATTR_ID_HPADD			= 56; 	--/* 生命回复 */
ATTR_ID_MPADD			= 57; 	--/* 魔法回复 */
ATTR_ID_HP				= 0; 	--/* HP */
ATTR_ID_HPMAX			= 1;
ATTR_ID_MP 				= 2; 	--/* MP */
ATTR_ID_FIGHT			= 33; 	--/* 战斗力 */
ATTR_ID_SKILLPOINT		= 19; 	--/* 技能点 */
ATTR_ID_DAMAGE			= 37;	--/* 破坏力 */
ATTR_ID_NOFIGHT_HPADD	= 38;	-- 	/* 非战斗状态HP恢复 */
ATTR_ID_NOFIGHT_MPADD	= 39;	-- 	/* 非战斗状态MP恢复 */
ATTR_ID_BADSTATUS_DEF	= 40;	--	/* 不良状态抗性 */
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
ATTR_ID_ICE_HURT		= 68;	-- /* 魔法伤害 */
ATTR_ID_HEAL_HP_ADD		= 69;	-- /* 治疗加修正 */
ATTR_ID_HEAL_HP_MUL		= 70;	-- /* 治疗乘修正 */
ATTR_ID_ATKEE_HEAL_HP_ADD = 71;	-- /* 被治疗加修正 */
ATTR_ID_ATKEE_HEAL_HP_MUL = 72; -- /* 被治疗乘修正 */
ATTR_ID_PHSIC_HURT			= 31; 

ATTR_ID_BATTLE_GLORY	= 76;	-- /* 战场荣誉 */
ATTR_ID_AOSHAN_BRAND	= 77; 	-- /* 绿洲牌子 */
ATTR_ID_LIFT_BRAND		= 78; 	-- /* 狂沙牌子 */

ATTR_ID_SCHOOL_VAL		= 87;	-- /* 师徒声望 */
ATTR_ID_ZHENGYI			= 89;   -- /* 正义点 */

ATTR_ID_REFLECTION_HP = 79;	-- /* 反伤等级 */
ATTR_ID_IGNORE_DEF = 80; 	-- /* 抵抗等级 */
ATTR_ID_DEF_HP = 81; 		-- /* 破防等级 */
ATTR_ID_PHSIC_MAX_ATK = 82; --	/* 物理上限攻击力 */
ATTR_ID_MAGIC_MAX_ATK = 83; --	/* 魔法上限攻击力 */
ATTR_ID_DODGE = 85; 		--/* 闪避率 */

COLOR_WHITE			= 0;	--白色
COLOR_GREEN			= 1;	--绿色
COLOR_BULE			= 2;	--蓝色
COLOR_PURPLE		= 3;	--紫色
COLOR_RED			= 4;	--红色
COLOR_BLACK			= 5;	--黑色
COLOR_GOLD			= 7;	--金色
COLOR_YELLOW		= 8;	--黄色

COLOR_WEAK_GREEN	= 9;	--淡绿
COLOR_WEAK_YELLOW	= 10;	--浅黄
COLOR_WEAK_PURPLE	= 11;	--浅色
COLOR_WEAK_BLUE		= 12;	--淡蓝

COLOR_ROSE_RED		= 13;	--玫瑰红
COLOR_SEA_GREEN		= 14;	--海绿
COLOR_FRESH_GREEN	= 15;	--鲜绿
COLOR_POWDER_RED	= 16;	--粉红
COLOR_VIRIDESCENCE	= 17;	--青绿
COLOR_WEAK_WRITE	= 18;   --淡白

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

--封装的局部变量
local t_MidTipsList = {};
t_MidTipsList[MID_NPCSHOP_MONEYNOTENOUGH_SELLFAIL]			= "身上".. MONEY_TYPE_MONEY_NAME.."不足,购买失败!";
t_MidTipsList[MID_NPCSHOP_BULLNOTENOUGH_SELLFAIL]			= "身上"..MONEY_TYPE_BULL_NAME.."不足,购买失败!";
t_MidTipsList[MID_NPCSHOP_EXCHANGEITEMNOTENOUGH_SELLFAIL]	= "兑换的牌子不足,购买失败!";
t_MidTipsList[MID_NPCSHOP_MONEYNOTENOUGH_BUYSAVEFAIL]		= "身上"..MONEY_TYPE_MONEY_NAME.."不足,回购失败!";

t_MidTipsList[MID_NPCSHOP_BULLNOTENOUGH_BUYSAVEFAIL]	= "身上"..MONEY_TYPE_BULL_NAME.."不足,回购失败!";
t_MidTipsList[MID_NPCSHOP_BULLNOTENOUGH_REPAIRFAIL]		= "身上"..MONEY_TYPE_BULL_NAME.."不足,维修失败!";
t_MidTipsList[MID_NPCSHOP_NOTNEEDREPAIR]				= "不需要维修!";
t_MidTipsList[MID_ARMPROCE_ARMSTAR]						= "装备品质提升";
t_MidTipsList[MID_ARMPROCE_LEVEL]						= "装备等级提升";
t_MidTipsList[MID_ARMPROCE_FIGHT]						= "装备战斗力提升";
t_MidTipsList[MID_ARMPROCE_IDENTIFIABILITY]				= "装备鉴定成功";
t_MidTipsList[MID_ARMPROCE_GEMLEVELUP]					= "宝石升级成功";
t_MidTipsList[MID_ARMPROCE_GEMINLAY]					= "宝石镶嵌成功";
t_MidTipsList[MID_ARMPROCE_GEMPICK]						= "宝石摘取成功";


t_MidTipsList[MID_ARMPROCE_NOTGEMEFFECTINLAY]			= "不是对应类型的宝石";
t_MidTipsList[MID_ARMPROCE_NOTSAMEGEMTYPE]				= "宝石类型不一致";
t_MidTipsList[MID_ARMPROCE_NOTSAMELEVEL]				= "宝石等级不一致";
t_MidTipsList[MID_ARMPROCE_NOTGEM]						= "不是宝石不能放入";

t_MidTipsList[MID_ARMPROCE_NOTEQUIP]					= " 没有放入装备";

t_MidTipsList[MID_ARMPROCE_NOTGEMPICKITEM]				= "没有摘取宝石所需要物品";
t_MidTipsList[MID_ARMPROCE_NOUPDATEGEM]					= "放入要升级的宝石";
t_MidTipsList[MID_ARMPROCE_NOGEMUPDATEITEM]				= "没有升级宝石所需要物品";
t_MidTipsList[MID_ARMPROCE_NOTEQUIPITEM]				= "放入的物品不是装备";
t_MidTipsList[MID_ARMPROCE_NOTHOLDEQUIP]				= "放入的装备没有带孔";


t_MidTipsList[MID_ARMPROCE_GEMPICK_FAIL]				= "宝石摘取失败 ";
t_MidTipsList[MID_ARMPROCE_GEMINLAY_FAIL]				= "宝石镶嵌失败 ";
t_MidTipsList[MID_ARMPROCE_GEMLEVELUP_FAIL]				= "宝石升级失败 ";
t_MidTipsList[MID_ARMPROCE_IDENTIFIABILITY_FAIL]		= "装备鉴定失败 ";
t_MidTipsList[MID_ARMPROCE_ARMSTAR_FAIL]				= "装备品质提升失败 ";
t_MidTipsList[MID_ARMPROCE_LEVEL_FAIL]					= "装备等级提升失败 ";
t_MidTipsList[MID_ARMPROCE_FIGHT_FAIL]					= "装备战斗力提升失败 ";
t_MidTipsList[MID_ARMPROCET_NOTENOUGHBULL]				= ""..MONEY_TYPE_BULL_NAME.."不足";

t_MidTipsList[MID_ARMPROCET_NOTARMSAFEITEM]				= "背包里没有品质提升保险符";
t_MidTipsList[MID_ARMPROCET_FIGHTSAFEITEM]				= "背包里没有战斗力提升保险符";
t_MidTipsList[MID_ARMPROCET_GEMSAFEITEM]				= "背包里没有镶嵌符";
t_MidTipsList[MID_ARMPROCET_FIGHTLIMIT]					= "装备战斗力等级不可高等于装备品质";
t_MidTipsList[MID_ARMPROCET_NOTGEMPICK]					= "缺少对应的宝石摘取符";
t_MidTipsList[MID_ARMPROCET_NOTSTARSTUFF]				= "放入的不是升品质的材料";
t_MidTipsList[MID_ARMPROCET_NOTFIGHTSTUFF]				= "放入的不是升战斗力的材料";
t_MidTipsList[MID_ARMPROCET_NOTEQUIPLEVELSTUFF]			= "放入的不是升装备等级的材料";
t_MidTipsList[MID_ARMPROCET_NOTHOLEINLAY]				= "该装备已无可镶嵌的孔";
t_MidTipsList[MID_ARMPROCET_NOTHOLEEQUIP]				= "该装备不是有孔装备";
t_MidTipsList[MID_ARMPROCET_HOLEHASGEM]					= "不可镶嵌";
t_MidTipsList[MID_ARMPROCET_NOTINLAYGEM]				= "该装备未镶嵌宝石";
t_MidTipsList[MID_ARMPROCET_NOTEQUIPASKLEVELITEM]		= "该物品不是匹配的宝石升级符";
t_MidTipsList[MID_ARMPROCET_NOTEQUIPASKPICKITEM]		= "该物品不是匹配的宝石摘取符";
t_MidTipsList[MID_ARMPROCET_NOTCHOICEPICKGEM]			= "请选择你要摘取的宝石";
t_MidTipsList[MID_ARMPROCET_CANNOTINDENTITY]			= "装备不可鉴定";
t_MidTipsList[MID_ARMPROCET_NOTINDENTITYEQUIP]			= "装备未鉴定";
t_MidTipsList[MID_ARMPROCET_NOTNEXTITEMLEVEL]			= "装备不可升级";

t_MidTipsList[MID_EQUIP_NOTENOUGHEXP]					= "当前经验值不足 升级失败";
t_MidTipsList[MID_TRUST_NOTENOUGHLV]					= "等级未满三十级";

t_MidTipsList[MID_STALL_NOTENOUGHLV]					= "等级未满二十级";
t_MidTipsList[MID_STALL_CANSTALLSTATE]					= "摆摊状态不能上架商品";

t_MidTipsList[MID_MAIL_NOTENOUGHLV]						= "等级未满十级";
t_MidTipsList[MID_MAIL_HASINMAILSENDITEM]				= "该物品已在待发邮件物品列表中";
t_MidTipsList[MID_TRUST_NOTENOUGHLVORITEM]				= "等级不足或当前委托需求物品不足";
t_MidTipsList[MID_TRUST_HASTODAYLIMITFINISH]			= "已达本日委托限制次数";

t_MidTipsList[MID_NPCSHOP_TRADENUMCANNOTNULL]			= "商品的数量不能为空";
t_MidTipsList[MID_NPCSHOP_TRADEPRICECANNOTNULL]			= "商品的售价不能为空";
t_MidTipsList[MID_NPCSHOP_TRADEBUYNUMCANNULL]			= "购买的数量不能为空";
t_MidTipsList[MID_NPCSHOP_NOTCHOICEITEM]				= "请选择你要购买的物品";

t_MidTipsList[MID_NPCSHOP_NONEEDREPAIR]					= "不需维修";
t_MidTipsList[MID_FIGHT_IN]								= "进入战斗状态";
t_MidTipsList[MID_FIGHT_OUT]							= "离开战斗状态";
t_MidTipsList[MID_SHORTCUT_MPNOTENOUGH]					= "当前魔法值不足";
t_MidTipsList[MID_SHORTCUT_ENERGENOTENOUGH]				= "当前能量不足";
t_MidTipsList[MID_SHORTCUT_NOTITEM]						= "该物品已用完";	
t_MidTipsList[MID_SHORTCUT_CANNOTUSESPELL_BUF]			= "当前状态不能使用技能";
t_MidTipsList[MID_SHORTCUT_CANNOTUSESPELL_MOVE]			= "当前状态下不能使用位移技能";	 
t_MidTipsList[MID_SHORTCUT_CANNOTUSESPELL_INMORPH]		= "变身状态不能使用技能";
t_MidTipsList[MID_SHORTCUT_CANNOTUSESPELL_INSTALL]		= "摆摊状态中无法施放技能";
t_MidTipsList[MID_PATH_FINDNONPC]						= "目标NPC不在当前地图";
t_MidTipsList[MID_XP_COOLDOWNTIME]						= "技能处于冷却状态";
t_MidTipsList[MID_XP_NOENOUGHMP]						= "宠物的MP不足";
t_MidTipsList[MID_XP_NOENOUGHXP]						= "技能所需魂能不足";
t_MidTipsList[MID_XP_NOENOUGHCOLLECT]					= "技能所需战意不足";
t_MidTipsList[MID_XP_NOINMAP]							= "只能在副本中使用";
t_MidTipsList[MID_MAIL_SENDER_NAME_NULL]				= "收件人姓名不能为空,邮件发送失败";
t_MidTipsList[MID_MAIL_TITLE_NULL]						= "邮件主题不能为空,邮件发送失败";
t_MidTipsList[MID_MAIL_NO_MONEY]						= "邮资不足,邮件发送失败";
t_MidTipsList[MID_TRADE_SELF_FIGHT]						= "处于战斗状态，不能进行交易";
t_MidTipsList[MID_TRADE_SELF_STALL]						= "摆摊状态中无法交易";
t_MidTipsList[MID_TRADE_SELF_FLY]						= "当前状态不能进行交易，请稍后再试";
t_MidTipsList[MID_TRADE_SELF_MORPH]						= "处于变身状态，不能进行交易";
t_MidTipsList[MID_TRADE_SELF_TRUMP_PRE]					= "正在召唤精灵，不能进行交易";
t_MidTipsList[MID_TRADE_CANNOT_REPEAT_CONFIRM_TRADE_]	= "不能重复确认交易";
t_MidTipsList[MID_TRADE_CANNOT_DIST_TOO_LONG]			= "距离过远无法申请交易";
t_MidTipsList[MID_TRADE_CANNOT_NOT_CAN_TRADE_OBJECT]	= "目标对象无法交易";


t_MidTipsList[MID_TRADE_SELF_DEAD]						= "你已经死亡，不能进行交易";
t_MidTipsList[MID_TRADE_ACTOR_FIGHT]					= "对方处于战斗状态，不能进行交易";
t_MidTipsList[MID_TRADE_ACTOR_STALL]					= "对方处于摆摊状态，不能进行交易";
t_MidTipsList[MID_TRADE_ACTOR_FLY]						= "对方处于飞行状态，不能进行交易";
t_MidTipsList[MID_TRADE_ACTOR_DEAD]						= "对方已经死亡，不能进行交易";
t_MidTipsList[MID_TRADE_RELATION_XIONGSHOU]				= "对方在自己的凶手名单不能进行交易";
t_MidTipsList[MID_TRADE_RELATION_SUDI]					= "对方在自己的宿敌名单不能进行交易";
t_MidTipsList[MID_STALL_DISTANCE_LONG]					= "双方距离太远，不能查看对方的摊位信息";
t_MidTipsList[MID_STALL_SELF_FIGHT]						= "战斗状态下无法摆摊";
t_MidTipsList[MID_STALL_SELF_EMPTY]						= "没有商品上架，不能摆摊";
t_MidTipsList[MID_STALL_SELF_PK]						= "切磋状态下无法摆摊";
t_MidTipsList[MID_MULTIMAP_AUTOFIND_FAIL]				= "不能自动寻路到所选择的位置";
t_MidTipsList[MID_TRADE_DISTANCE_LONG]					= "距离太远取消交易";
t_MidTipsList[MID_TRADE_INVITE_TIME_OUT]				= "对方没有响应交易邀请";
t_MidTipsList[MID_TRADE_RELATION_BLACK]					= "对方在黑名单中不能进行交易";
t_MidTipsList[MID_STALL_DISTANCE_LONG]					= "双方距离太远，不能查看对方的摊位信息";
t_MidTipsList[MID_FOLLOW_START_FAIL]					= "当前状态不允许进行跟随操作";
t_MidTipsList[MID_TRUST_MAX_ONETIME]					= "已达到一次所挂委托任务数目的上限";
t_MidTipsList[MID_STALL_SEE_FIGHT]						= "战斗状态下无法浏览摊位";
t_MidTipsList[MID_STALL_SEE_PK]							= "切磋状态下无法浏览摊位";
t_MidTipsList[MID_STALL_CANNOT_EMPTY]					= "不能空置摊位，无法下架";
t_MidTipsList[MID_STALL_SEE_DEAD]						= "死亡状态下无法浏览摊位";
t_MidTipsList[MID_STALL_TOOMUCH_MONEY]					= "出售总额加当前携带金额，不得超出身上携带金额的上限";


t_MidTipsList[MID_STALL_SELF_RIDE]						= "骑乘状态，不能摆摊";
t_MidTipsList[MID_STALL_SELF_MORPH]						= "变身状态，不能摆摊";
t_MidTipsList[MID_STALL_SELF_TRUMP_PRE]					= "正在召唤精灵，不能摆摊";
t_MidTipsList[MID_STALL_SELF_NEAR_NPC]					= "严禁在NPC周围摆摊，请前往空旷区域";
t_MidTipsList[MID_STALL_SELF_NEAR_OTHER_STALL]			= "该位置已有摊位，请前往空旷区域";
t_MidTipsList[MID_STALL_SELF_DEAD]						= "死亡状态下无法摆摊";

t_MidTipsList[MID_ITEM_USE_FAIL_INPRE]					= "正在使用其它物品";
t_MidTipsList[MID_FRIEND_REPEAT_REQ]					= "你已经发送过申请，等待对方回复";
t_MidTipsList[MID_FRIEND_REPEAT_REQ]					= "该玩家已经在你的好友列表"; 
t_MidTipsList[MID_FRIEND_IS_ENEMY]						= "目标在凶手名单，无法添加好友，请移除后再添加";
t_MidTipsList[MID_FRIEND_IS_SU_DI]						= "对方在你的宿敌名单中，无法添加好友，请按“F”键打开宿敌名单查看";

t_MidTipsList[MID_FRIEND_IS_BLACK]						= "对方在你的黑名单中，无法添加好友，请按“F”键打开黑名单查看";

t_MidTipsList[MID_XP_NOCAMPTRUMP]						= "需要对应精灵处于出征状态！";		   
t_MidTipsList[MID_SHORTCUT_CANNOTUSESPELL_XPLIMIT]		= "当前状态不能使用XP技能"; 
t_MidTipsList[MID_ARMPROCET_HOLECOLORDIFWITHGEM]		= "该孔颜色跟宝石颜色不匹配";  
t_MidTipsList[MID_ARMPROCET_HOLEHASSAMEGEM]				= "该装备已镶嵌过这种宝石";
--t_MidTipsList[MID_FRIEND_REPEAT_REQ]					= "你已经发送过申请，等待对方回复";
t_MidTipsList[MID_FITTINGMGR_NOFITSTUFF]				= "该材料不能装备配件";
t_MidTipsList[MID_FITTINGMGR_NOTFITGROUP]				= "本技能不能镶嵌这个配件";
t_MidTipsList[MID_FITTINGMGR_CAREEALIMIT]				= "职业不符,不能镶嵌此配件";
t_MidTipsList[MID_FITTINGMGR_INLAYSPECIALLIMIT]			= "该镶嵌孔只能装备花类配件";
t_MidTipsList[MID_FITTINGMGR_INLAYNORMALLIMIT]			= "该镶嵌孔只能装备无花类配件";
t_MidTipsList[MID_FITTINGMGR_SAMEIDLIMIT]				= "不能镶嵌相同的配件";
t_MidTipsList[MID_FITTINGMGR_SAMETYPELIMIT]				= "不能镶嵌相同类型的配件";
t_MidTipsList[MID_FITTINGMGR_NEEDBULLLIMIT]				= "装备配件所需要的"..MONEY_TYPE_BULL_NAME.."不足"; 
t_MidTipsList[MID_USESKILL_INCOLLECT_FAIL]				= "正在采集，不能使用技能";
t_MidTipsList[MID_USEITEM_INCOLLECT_FAIL]				= "正在采集，不能使用该物品";
t_MidTipsList[MID_TRUMP_PER_FAIL_INFIGHT]				= "战斗状态下不能出征精灵";
t_MidTipsList[MID_TRUMP_PER_FAIL_INDEAD]				= "死亡状态下不能出征精灵";
t_MidTipsList[MID_ARMPROCET_INLAY_LEVELLIMIT]			= "宝石等级与装备等级不符";	
t_MidTipsList[MID_ARMPROCE_NOTINLAYGEM]					= "非镶嵌类宝石不可升级";
t_MidTipsList[MID_EXCHANGE_ACTIONFORBID]		= "交易状态中,不可执行此操作";	

t_MidTipsList[MID_SPELL_UNUSED_NOAMM]					= "缺少子弹";
t_MidTipsList[MID_SPELL_UNUSED_RIGHTARM]				= "缺少右手武器";
t_MidTipsList[MID_SPELL_UNUSED_LEFTARM]					= "缺少左手武器";
t_MidTipsList[MID_SPELL_UNUSED_SHOULDER]				= "缺少肩甲武器";
t_MidTipsList[MID_SPELL_UNUSED_BACK]					= "缺少背部武器";
t_MidTipsList[MID_SPELL_UNUSED_NONEEQUIP]				= "缺少技能对应的武器";
                                               		
t_MidTipsList[MID_SPELL_UNUSED_CAMP]					= "不能对目标使用";
t_MidTipsList[MID_SPELL_UNUSED_OBJ]						= "不能对目标使用";
t_MidTipsList[MID_SPELL_UNUSED_DEAD]					= "目标已死亡";
t_MidTipsList[MID_SPELL_UNUSED_MASK_TARGET]				= "目标不在视野内";
t_MidTipsList[MID_SPELL_UNUSED_MASK_POINT]				= "请选择正确的区域";
t_MidTipsList[MID_SPELL_UNUSED_INCOLLECT]				= "正在采集";
t_MidTipsList[MID_ITEM_USE_FAIL_NOTINAREA]				= "请在指定区域使用该物品！";
t_MidTipsList[MID_ARMPROCE_SAMEGEMTYPEINLAY]			= "待镶嵌列表已有同类型宝石！";
t_MidTipsList[MID_ITEM_USE_FAIL_IN_STALL]				= "摆摊状态中无法使用物品";

t_MidTipsList[MID_AUTOFIND_SELF_LIMIT]					= "身体受制，不能进行自动寻路";		 
t_MidTipsList[MID_SPELL_NOTARGET]						= "必须选择一个目标";	

t_MidTipsList[MID_TRUST_ALREADY_ORDER]					= "该委托已经挂单，不能重复挂单";
t_MidTipsList[MID_FIND_PATH_FAIL]						= "目标在副本中或不可到达区域，寻路失败";		
t_MidTipsList[MID_TRUMP_ISPRE]							= "正在出征精灵，请稍候";
t_MidTipsList[MID_TRUMP_BACK_FAIL]						= "正在使用技能，无法召回精灵";

t_MidTipsList[MID_MARKET_NOT_ENOUGH_GOLD]				= "没有足够的"..MONEY_TYPE_GODCOIN_NAME.."购买！";
t_MidTipsList[MID_MARKET_NOT_ENOUGH_BIND_GOLD]			= "没有足够的"..MONEY_TYPE_BIND_GODCOIN_NAME.."购买！";
t_MidTipsList[MID_MARKET_NOT_FRIEND]					= "对方为好友才能赠送！";
t_MidTipsList[MID_MARKET_SEND_TRADE_SUCESS]				= "交易发布成功！";
t_MidTipsList[MID_MARKET_TRADE_SUCESS]					= "交易成功！";
t_MidTipsList[MID_MARKET_DESTROY_SUCESS]				= "撤销订单成功！";
t_MidTipsList[MID_MARKET_PACKAGE_PRESENT_SUCESS]		= "赠送成功！";
t_MidTipsList[MID_MARKET_BUY_TIME_OUT]					= "该物品已失效！";

t_MidTipsList[MID_PET_PRE_FAIL_TRUMP]					= "正在召唤精灵，不能召唤宠物！"
t_MidTipsList[MID_PET_PRE_FAIL_USEITEM]					= "正在使用物品，不能召唤宠物！"
t_MidTipsList[MID_PET_PRE_FAIL_SKILLPRE]				= "正在进行技能吟唱，不能召唤宠物！"
t_MidTipsList[MID_PET_PRE_FAIL_RIDEPRE]					= "正在召唤骑乘，不能召唤宠物！"
t_MidTipsList[MID_PET_PRE_FAIL_COLLECT]					= "正在采集，不能召唤宠物！"
t_MidTipsList[MID_PET_PRE_FAIL_FIGHT]					= "战斗状态，不能召唤宠物！"
t_MidTipsList[MID_PET_PRE_FAIL_STALL]					= "摆摊状态，不能召唤宠物！"
t_MidTipsList[MID_PET_PRE_FAIL_IN_OTHER_RIDE]			= "处于他人坐骑，不能召唤宠物！"
t_MidTipsList[MID_PET_PRE_FAIL_IN_RIDE_MACHINE]			= "乘坐机械，不能召唤宠物！"
t_MidTipsList[MID_PET_FAIL_IN_FREEDOM_ANIL]				= "该宠物已经被您放生，不可进行该操作！"
t_MidTipsList[MID_PET_FAIL_FREEDOM_IN_FIGHT]			= "该宠物正在召唤中，不可放生！"
t_MidTipsList[MID_PET_PRE_FAIL_SOME_STATE]				= "当前状态，不能召唤宠物！"

t_MidTipsList[MID_TRUMP_PRE_FAIL_PET]					= "正在召唤宠物，不能召唤精灵！"
t_MidTipsList[MID_TRUMP_PRE_FAIL_USEITEM]				= "正在使用物品，不能召唤精灵！"
t_MidTipsList[MID_TRUMP_PRE_FAIL_SKILLPRE]				= "正在进行技能吟唱，不能召唤精灵！"
t_MidTipsList[MID_TRUMP_PRE_FAIL_COLLECT]				= "正在采集，不能召唤精灵！"
t_MidTipsList[MID_TRUMP_PRE_FAIL_RIDEPRE]				= "正在召唤骑乘，不能召唤精灵！"

t_MidTipsList[MID_RIDE_PRE_FAIL_TRUMP]					= "正在召唤精灵，不能使用骑乘！"
t_MidTipsList[MID_RIDE_PRE_FAIL_USEITEM]				= "正在使用物品，不能使用骑乘！"
t_MidTipsList[MID_RIDE_PRE_FAIL_SKILLPRE]				= "正在进行技能吟唱，不能使用骑乘！"
t_MidTipsList[MID_RIDE_PRE_FAIL_COLLECT]				= "正在采集，不能使用骑乘！"
t_MidTipsList[MID_RIDE_PRE_FAIL_PET]					= "正在召唤宠物，不能使用骑乘！"
t_MidTipsList[MID_RIDE_PRE_FAIL_NO_RIDE]				= "没有该骑乘"
t_MidTipsList[MID_RIDE_PRE_FAIL_SOME_STATE]				= "当前状态，不能使用骑乘！"

t_MidTipsList[MID_INFIGHT]								= "必须先脱离战斗"
t_MidTipsList[MID_NOINFIGHT]							= "必须先进入战斗"
t_MidTipsList[MID_NEED_TARGET]							= "必须选择一个目标"
t_MidTipsList[MID_NEED_MONSTER_TARGET]					= "不能对该目标使用这个物品"
t_MidTipsList[MID_NEED_NPC_TARGET]						= "必须选择一个NPC目标"
t_MidTipsList[MID_NEED_NPC_PLAYER]						= "请选中一名除您以外的玩家"	

t_MidTipsList[MID_APPLY_TEAM_PLAYER_IS_ENEMY]			= "对方在凶手名单，不能组队"
t_MidTipsList[MID_APPLY_TEAM_PLAYER_IS_TMPENEMY]		= "目标在你的宿敌名单中，无法组队，请按“F”键打开宿敌名单查看"
t_MidTipsList[MID_INVITE_TEAM_PLAYER_IS_TMPENEMY]		= "目标在你的宿敌名单中，无法组队，请按“F”键打开宿敌名单查看"

t_MidTipsList[MID_APPLY_TEAM_SELF_DEAD]					= "处于死亡状态，不能组队"	
t_MidTipsList[MID_APPLY_TEAM_OTHER_DEAD]				= "对方处于死亡状态，不能组队"	
t_MidTipsList[MID_APPLY_TEAM_OTHER_IN_BLACK]			= "对方在你的黑名单中，不能组队"
t_MidTipsList[MID_APPLY_TEAM_OTHER_IN_ZHUISHA]			= "不可与宿敌名单中的玩家组队"
t_MidTipsList[MID_APPLY_TEAM_OTHER_IN_XIONGSHOU]		= "不可与凶手名单中的玩家组队"
t_MidTipsList[MID_APPLY_TEAM_OTHER_IN_TEAM]				= "你已经拥有队伍"

t_MidTipsList[MID_USE_ITEM_FAILE_HPENOUGH]				= "当前生命值已满"
t_MidTipsList[MID_USE_ITEM_FAILE_MPENOUGH]				= "当前魔法值已满"
t_MidTipsList[MID_USE_ITEM_FAILE_LVENOUGH]				= "当前等级不能使用该物品！"

t_MidTipsList[MID_AUTO_TEAM_FAIL_AGREE_READY]			= "已经同意了，不能再次申请同意操作"
t_MidTipsList[MID_MARKET_NO_SEARCH]						= "该账单号不存在"

t_MidTipsList[MID_AUTO_TEAM_FAIL_TRAN_BY_EXCHANGE]		= "交易状态不能传送"
t_MidTipsList[MID_AUTO_TEAM_FAIL_TRAN_BY_STALL]			= "摆摊状态不能传送"
t_MidTipsList[MID_AUTO_TEAM_FAIL_TRAN_BY_DEAD]			= "死亡状态不能传送"
t_MidTipsList[MID_AUTO_TEAM_FAIL_TIME_OUT]				= "活动已经结束"

t_MidTipsList[MID_SECONDARYSKILL_WRONG_STATE]			= "当前状态下不能使用生活技能"
t_MidTipsList[MID_USEITEMPRE_WRONG_STATE]				= "当前状态下不能使用物品"

t_MidTipsList[MID_USEITEM_IN_FLY_STATE]					= "当前状态不能使用物品，请稍后再试"
t_MidTipsList[MID_AUTOFIND_IN_FLY_STATE]				= "当前状态不能自动寻路，请稍后再试"
t_MidTipsList[MID_SHORTCUT_IN_FLY_STATE]				= "当前状态不能使用技能，请稍后再试"
t_MidTipsList[MID_PWORLD_RESTOREITEM]					= "该副本不能使用恢复性物品"
t_MidTipsList[MID_MAP_NOSKILL]							= "当前地图不能使用该技能"
t_MidTipsList[MID_SAFE_ENV_NOSKILL]						= "安全区内不能使用该技能"
t_MidTipsList[MID_SELF_BUSY]							= "正在做其他事，不能使用技能"
t_MidTipsList[MID_SELF_SPELLPRE]						= "你正在使用其他技能"
t_MidTipsList[MID_SELF_DEAD]							= "你已经死亡"
t_MidTipsList[MID_NOT_ENOUGH_ENERGY]					= "能量不足"
t_MidTipsList[MID_TARGET_TO_FAR]						= "目标不在攻击范围内"
t_MidTipsList[MID_TRUMP_PER_FAIL_MOVE]					= "移动中，无法召唤精灵"

t_MidTipsList[MID_MOVE_FAIL_STALL]						= "摆摊状态中无法移动"

t_MidTipsList[MID_PICK_ITEM_TOO_FAR]					= "距离太远，无法拾取"
t_MidTipsList[MID_SECONDARYSKILL_FAILED_INRIDE]			= "骑乘状态中，无法进行科技生产"
t_MidTipsList[MID_SECONDARYSKILL_FAILED_INFIGHT]		= "战斗状态中，无法进行科技生产"
t_MidTipsList[MID_SECONDARYSKILL_FAILED_INCASTING]		= "吟唱中，无法进行科技生产"
t_MidTipsList[MID_SECONDARYSKILL_FAILED_CANNTCONTROL]	= "受控制状态中，无法进行科技生产"
t_MidTipsList[MID_SECONDARYSKILL_FAILED_COLDDOWN]		= "技能冷却中，无法使用"
t_MidTipsList[MID_CANNOT_CATCH_PET]						= "不能对目标使用"
t_MidTipsList[MID_CATCH_PET_DEAD]						= "目标已死亡"
t_MidTipsList[MID_FAIL_WEI_XING]						= "您或跟随目标处于不能跟随位置，跟随失败"
t_MidTipsList[MID_NPCSHOP_BATTLE_GLORY_SELLFAIL]		= "身上荣誉不足,购买失败!"
t_MidTipsList[MID_NPCSHOP_AOSHAN_BRAND_SELLFAIL]		= "身上战魂徽章不足,购买失败!"
t_MidTipsList[MID_NPCSHOP_LIFT_BRAND_SELLFAIL]			= "身上生存奖章不足,购买失败!"
t_MidTipsList[MID_TRUMP_PER_FAIL_BADSTATUS]				= "该状态下，无法召唤精灵"
t_MidTipsList[MID_RESOLVE_WRONG_STATE]					= "当前状态下不能分解"
t_MidTipsList[MID_RESOLVE_FAILED_INCASTING]				= "吟唱中，无法分解"
t_MidTipsList[MID_RESOLVE_FAILED_CANNTCONTROL]			= "受控制状态中，无法分解"
t_MidTipsList[MID_RESOLVE_CONTAINER_LOCK]				= "背包上锁，无法分解"
t_MidTipsList[MID_RIDE_PRE_FAILED_CLAN_LIMIT_CAPTION]	= "乘骑失败，此坐骑只有时间城占领公会会长才有使用权限"
t_MidTipsList[MID_RIDE_PRE_FAILED_CLAN_LIMIT_MEMBER]	= "乘骑失败，此坐骑只有时间城占领公会成员才有使用权限"
t_MidTipsList[MID_USE_ITEM_FAILED_CLAN_LIMIT_CAPTION]	= "不可使用，此物品只有时间城占领公会会长才有使用权限"
t_MidTipsList[MID_USE_ITEM_FAILED_CLAN_LIMIT_MEMBER]	= "不可使用，此物品只有时间城占领公会成员才有使用权限"
t_MidTipsList[MID_USE_BACK_CLAN_ITEM_FAILED_INFIGHT]	= "战斗状态不可使用公会城市传送卷"
t_MidTipsList[MID_USE_PET_EGG_PET_FULL]					= "宠物栏已满"
t_MidTipsList[MID_PET_MAP_NO_PET_FIGHT]					= "当前地图不可召唤宠物"
t_MidTipsList[MID_PET_MAP_NO_FAIRY_FIGHT]				= "当前地图无法召唤精灵"
t_MidTipsList[MID_MUST_MACHINE_COLLECT]					= "该矿产需要采集车才能采集"
t_MidTipsList[MID_MACHINE_UNMATCH_COLLECT]				= "您使用的采集车不能采集此矿产"
t_MidTipsList[MID_MACHINE_UNMATCH_COLLECT_MORMAL]		= "机械状态不能进行采集操作"
t_MidTipsList[MID_USE_MACHINE_ITEM_IN_MORPH]			= "当前状态无法使用机械"
t_MidTipsList[MID_USE_MORPH_ITEM_IN_MACHINE]			= "在机械状态，无法使用变身药水"
t_MidTipsList[MID_ENTER_CLANISLAND_IN_FIGHT]			= "战斗状态不能传送"
t_MidTipsList[MID_ENTER_CLANISLAND_IN_MACHINE]			= "机械状态不能传送"
t_MidTipsList[MID_SUMMON_MUST_BE_CAPTAIN]				= "卷轴使用者必须是队长身份"
t_MidTipsList[MID_SUMMON_MUST_BE_PRESIDENT]				= "卷轴使用者必须是会长或副会长身份"
t_MidTipsList[MID_NOTENOUGH_COST_ITEM]					= "缺少使用该技能需要的道具"
t_MidTipsList[MID_CHATLINK_MAX_NUM]						= "设置的链接数量已达到最大"
t_MidTipsList[MID_CHATLINK_SAME_EXIST]					= "已存在相同类型的链接"
t_MidTipsList[MID_NO_ATTACK_WORLD_CITY_CONTROL]			= "攻击无效，还有能源增幅器存在，当前建筑无法被攻击"
t_MidTipsList[MID_NO_ITEM_USE_IN_THIS_MAP]				= "此地图不能使用此物品"
t_MidTipsList[MID_NPCSHOP_SPARTA_GLORY_SELLFAIL]		= "殿堂荣耀不足,购买失败!"
t_MidTipsList[MID_NO_CHANGE_STALL_MODEL]				= "你没法改变这个摊位的外形～"

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

--职业小图标
careerIdxTexture = {
	"uires\\ui\\Za\\ZhiYeBiaoShi\\Zhan.tga",
	"uires\\ui\\Za\\ZhiYeBiaoShi\\Fa.tga",
	"uires\\ui\\Za\\ZhiYeBiaoShi\\Qiang.tga",
	"uires\\ui\\Za\\ZhiYeBiaoShi\\Ji.tga",
}

-- 表情描述信息
local t_facesDesc = 
{
	["#001"] = { desc = "眨眼", },
	["#002"] = { desc = "呲牙", },
	["#003"] = { desc = "调皮", },
	["#004"] = { desc = "生气", },
	["#005"] = { desc = "可爱", },
	["#006"] = { desc = "装酷", },
	["#007"] = { desc = "怒火", },
	["#008"] = { desc = "可怜", },
	["#009"] = { desc = "脸红", },
	["#010"] = { desc = "难过", },
	["#011"] = { desc = "伤心", },
	["#012"] = { desc = "嘻笑", },
	["#013"] = { desc = "睡觉", },
	["#014"] = { desc = "牙", },
	["#015"] = { desc = "亲", },
	["#016"] = { desc = "色", },
	["#017"] = { desc = "冷汗", },
	["#018"] = { desc = "口水", },
	["#019"] = { desc = "擦", },
	["#020"] = { desc = "吐", },
	["#021"] = { desc = "捏", },
	["#022"] = { desc = "嘘", },
	["#023"] = { desc = "敲", },
	["#024"] = { desc = "禁言", },
	["#025"] = { desc = "得意", },
	["#026"] = { desc = "骂", },
	["#027"] = { desc = "酷", },
	["#028"] = { desc = "可爱", },
	["#029"] = { desc = "骂", },
	["#030"] = { desc = "亲亲", },
	["#031"] = { desc = "皱眉", },
	["#032"] = { desc = "笑", },
	["#033"] = { desc = "可怜", },
	["#034"] = { desc = "晕", },
	["#035"] = { desc = "扣鼻", },
	["#036"] = { desc = "抓狂", },
	["#037"] = { desc = "鼓掌", },
	["#038"] = { desc = "鄙视", },
	["#039"] = { desc = "山贼", },
	["#040"] = { desc = "怒", },
	["#041"] = { desc = "大笑", },
	["#042"] = { desc = "加油", },
	["#043"] = { desc = "大哭", },
	["#044"] = { desc = "笑", },
	["#045"] = { desc = "送花", },
	["#046"] = { desc = "兴奋", },
};

function GetFaceDesc( szKey )
	return t_facesDesc[szKey]["desc"];
end

-- 表情描述信息
--[[
local t_Rotot_Face = 
	{
		{"#001","眨眼"}, 
		{"#002","呲牙"},
		{"#003","调皮"},
		{"#004","生气"},
		{"#005","可爱"}, 
		{"#006","装酷"},
		{"#007","怒火"},
		{"#008","可怜"},
		{"#009","脸红"}, 
		{"#010","难过"},
		{"#011","伤心"},
		{"#012","嘻笑"},
		{"#013","睡觉"}, 
		{"#014","牙"},
		{"#015","亲"},
		{"#016","色"},
		{"#017","冷汗"},
		{"#018","口水"},
		{"#019","擦"}, 
		{"#020","吐"}, 
		{"#021","捏"},
		{"#022","嘘"},
		{"#023","敲"}, 
		{"#024","禁言"},
		{"#025","得意"}, 
		{"#026","骂"},
		{"#027","酷"},
		{"#028","可爱"},
		{"#029","胜利"},
		{"#030","亲亲"}, 
		{"#031","皱眉"},  
		{"#032","笑"},
		{"#033","可怜"},
		{"#034","晕"}, 
		{"#035","扣鼻"},
		{"#036","抓狂"},
		{"#037","鼓掌"},
		{"#038","鄙视"},
		{"#039","山贼"},
		{"#040","怒"}, 
		{"#041","大笑"},   
		{"#042","加油"},
		{"#043","大哭"},
		{"#044","笑"}, 
		{"#045","送花"},
		{"#046","兴奋"},

		{"#047","愤怒"},
		{"#048","拜拜"},
		{"#049","美女过来"},
		{"#050","帅哥过来"},
	};
--]]
local t_Rotot_Face = 
{
	{"#001","怒"},		{"#002","色"},		{"#003","可爱"},	{"#004","微笑"},	{"#005","流汗"},	{"#006","再见"},
	{"#007","猪头"},	{"#008","疑问"},	{"#009","大笑"},	{"#010","害羞"},	{"#011","惊讶"},	{"#012","哭泣"},
	{"#013","吐"},		{"#014","拳头"},	{"#015","尴尬"},	{"#016","叹气"},	{"#017","吐"},		{"#018","闭嘴"},
	{"#019","摇摆"},	{"#020","小样"},	{"#021","抠鼻屎"},	{"#022","爱你"},	{"#023","抱抱"},	{"#024","吃饭"},
	{"#025","可恶"},	{"#026","饥饿"},	{"#027","飙血"},	{"#028","傲慢"},	{"#029","奋斗"},	{"#030","滚"},
	{"#031","装可怜"},	{"#032","吸烟"},	{"#033","无语"},	{"#034","发颤"},	{"#035","瞪眼"},	{"#036","咒骂"},
	{"#037","坏笑"},	{"#038","囧"},		{"#039","喇叭"},	{"#040","眩晕"},	{"#041","雷"},		{"#042","赞同"},
	{"#043","有才"},	{"#044","no"},		{"#045","睡觉"},	{"#046","崩溃"},	{"#047","坐等"},	{"#048","阴险"},
	{"#049","拍死你"},	{"#050","疑问"},	{"#051","幽灵"},	{"#052","路过"},	{"#053","我想死 "},	{"#054","心碎"},
	{"#055","蛋定"},	{"#056","鼓掌"},	{"#057","加1"},		{"#058","汗"},		{"#059","伤心"},	{"#060","委屈"},
	{"#061","勾引"},	{"#062","可怜"},	{"#063","菊花"},	{"#064","蛋糕"},	{"#065","礼物"},
};

function GetRototFaceTable()
	return t_Rotot_Face; 
end

local t_QuestStarTexUVList =
{

};

-- 设置button的信息
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

-- 设置头像
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

-- @Desc:设置滚动条的位置
-- @param: szScrollBarName	为要设置的滚动条
-- @param: nMaxViewLine		为要一页所能显示的最大行数
-- @param: nValidDataNum	为当前要显示的数据行数
function SetScrollBar( szScrollBarName, nMaxViewLine, nValidDataNum )
	--确定技能学习面板显示的具体技能
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


--@Desc:移除当前选中的物品
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

--路径
local szSkillIconPath = "uires\\TuPianLei\\JiNeng\\JiNeng\\";
function GetSkillIconPath() --获取技能图标路径
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

--获取宠物图像路径
local szPetIconPath = "uires\\TuPianLei\\TouXiang\\JingLing\\";
function GetPetIconPath()
	return szPetIconPath;
end

--获取NPC头像路径
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

-- 获取配件路径
local szFittingIconPath = "uires\\TuPianLei\\JiNeng\\配件\\";
function GetFittingIconPath()
	return szFittingIconPath;
end

-- 获取怪物NPC头像路径
local szMonsterIconPath = "uires\\TuPianLei\\TouXiang\\GuaiWuTouXiang\\";
function GetMonsterIconPath( nIconId )
	return ( IsInExistence( szMonsterIconPath.. nIconId ..".tga" ) and szMonsterIconPath.. nIconId ..".tga" or
							szMonsterIconPath.."000000.tga" );
end

-- 获取buff图标路径

local szBuffIconPath = "uires\\TuPianLei\\JiNeng\\Buff\\";
function GetBuffIconPath( nIconId )
	return ( IsInExistence( szBuffIconPath.. nIconId ..".tga" ) and ( szBuffIconPath.. nIconId ..".tga" ) or
							( szBuffIconPath.."0000.tga" ) );
end

-- 获取素质图标
local szPetSkillPath = "uires\\TuPianLei\\JiNeng\\ChongWuJiNeng\\";
function GetPetSkillIconPath( nIconId )
	local IconPath = szPetSkillPath..nIconId..".tga";
	return IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH;
end

-- 获取精灵头像路径
function GetTrumpIconPath( )
	return "uires\\TuPianLei\\TouXiang\\JingLingTouXiang\\";
end
--函数

function GetCareerName( nType )
	return t_raceList[nType] or "";
end

function GetPosition(nType)
	if nType == CLAN_POS_NOT_MEMBER then
		return "非会员";
	elseif nType == CLAN_POS_NORMAL then
		return "会员";
	elseif nType == CLAN_POS_TECH_OFFICER then
		return "科技官员";
	elseif nType == CLAN_POS_COLLECTION_OFFICER then
		return "采集官员";
	elseif nType == CLAN_POS_FACTORY_OFFICER then
		return "工厂官员";
	elseif nType == CLAN_POS_PET_OFFICER then
		return "宠物官员";
	elseif nType == CLAN_POS_WALL_OFFICER then
		return "郊区官员";	
	elseif nType == CLAN_POS_VICE_PRESIDENT then
		return "副会长";
	elseif nType == CLAN_POS_PRESIDENT then
		return "会长";
	elseif nType == CLAN_POS_GROUP_1 then
		return "弑杀战团长";
	elseif nType == CLAN_POS_GROUP_2 then
		return "暗杀战团长"
	elseif nType == CLAN_POS_GROUP_3 then
		return "狂杀战团长";		
	elseif nType == CLAN_POS_GROUP_4 then
		return "灭杀战团长";		
	elseif nType == CLAN_POS_GROUP_5 then
		return "绝杀战团长";
	elseif nType == CLAN_POS_GROUP_6 then
		return "黄金战团长";
	end
end

function GetGender( nGender )
	if nGender == CLIENT_GENDER_MALE then
		return "男";
	elseif nGender == CLIENT_GENDER_FEMALE then
		return "女";
	end
end

function GetMidTipsList()
	return t_MidTipsList;
end

--显示游戏窗口中间消息
function ShowMidTips( szText )
	AddGameMiddleTips( szText );
end

-- 确保EditBox的数字输入值不会以0起头
function CheckEditNumberInput( edit )
	local szInputText	= edit:GetText();
	-- 若输入的值含有非数字值,则将EditBox中的值置为0
	if tonumber( szInputText ) == nil then
		return;
	end

	-- 检查输入的值的首位是否为0
	if string.len( szInputText ) > 1 and string.sub( szInputText, 1, 1 ) == '0' then
		edit:SetText( string.sub( szInputText, 2, string.len( szInputText ) ) );
		return;
	end
end

-- 装备类型定义
ARM_BOTHSWORD			= 100; 	--/* 对剑 */
ARM_KNIFE				= 101; 	--/* 刀 */
ARM_SWORD				= 103;	--/* 剑 */
ARM_SWORD_LEFT			= 104;  --/* 左手剑 */
ARM_SHIELD				= 106; 	--/* 盾 */
ARM_MAGIC_STAFF_ICE		= 110; 	--/* 魔杖 */
ARM_MAGIC_STAFF_FIRE	= 111;	--/* 火魔杖 */

ARM_MAGIC_FAQI			= 118; 	--/* 法器 */
ARM_RIGHT_GUN			= 120; 	--/* 右手枪 */
ARM_BITH_GUN			= 121; 	--/* 对手枪 */
ARM_HEVY_SCATTERGUN		= 125; 	--/* 重机枪 */
ARM_ENERGE				= 126; 	--/* 能量场 */
ARM_LIUDAN_GUN			= 127; 	--/* 榴弹炮 */
ARM_GUNNER_WEAR_LEFT	= 128;	--/* 副手枪 */
ARM_HUOJIAN_GUN			= 132; 	--/* 火箭炮 */
ARM_GOD_STAFF_LIGHT		= 150; 	--/* 律杖 */
ARM_GOD_STAFF_THUNDER	= 151; 	--/* 圣杖 */
ARM_GOD_FAQI			= 158; 	--/* 圣器 */
ARM_ANY_EX				= 9998;	--/* 任何武器，不可以空手 */
ARM_ANY					= 9999; --/* 任何武器，可以空手 */

ARM_SECONDARY			= 111;

-- 装备类型表
t_tagARM_TYPE =
{
	-- 103
	{ARM_SWORD,				ARM_SWORD,				"右手剑"},
	{ARM_KNIFE,				ARM_KNIFE,				"刀"},
	{ARM_BOTHSWORD,			ARM_BOTHSWORD,			"对剑"},
	-- 104
	{ARM_SWORD_LEFT,		ARM_SWORD_LEFT,			"左手剑"},
	{ARM_SHIELD,			ARM_SHIELD,				"盾"},
	{ARM_MAGIC_STAFF_ICE,	ARM_MAGIC_STAFF_ICE,	"法杖"},
	{ARM_MAGIC_STAFF_FIRE,	ARM_MAGIC_STAFF_FIRE,	"魔杖"},
	{ARM_MAGIC_FAQI,		ARM_MAGIC_FAQI,			"法器"},
	{ARM_RIGHT_GUN,			ARM_RIGHT_GUN,			"右手枪"},
	{ARM_BITH_GUN,			ARM_BITH_GUN,			"对手枪"},
	{ARM_HEVY_SCATTERGUN,	ARM_HEVY_SCATTERGUN,	"重机枪"},
	{ARM_GUNNER_WEAR_LEFT,	ARM_GUNNER_WEAR_LEFT,	"左手枪"},
	{ARM_ENERGE,			ARM_ENERGE,				"瞄准镜"},
	{ARM_LIUDAN_GUN,		ARM_LIUDAN_GUN,			"榴弹炮"},
	{ARM_HUOJIAN_GUN,		ARM_HUOJIAN_GUN,		"火箭炮"},
	{ARM_GOD_STAFF_LIGHT,	ARM_GOD_STAFF_LIGHT,	"律杖"},
	{ARM_GOD_STAFF_THUNDER, ARM_GOD_STAFF_THUNDER,	"圣杖"},
	{ARM_GOD_FAQI,			ARM_GOD_FAQI,			"圣器"},
	{ARM_ANY_EX,			ARM_ANY_EX,				"任何武器"},
	{251, 254,				"徽章"},
	{261, 264,				"鞋子"},
	{271, 279,				"护符"},
	{281, 284,				"项链"},
	{291, 294,				"戒指"},
	{201, 204,				"衣服"},
	{211, 214,				"头盔"},
	{221, 222,				"肩甲"},
	{224, 224,				"肩甲"},
	{241, 245,				"面饰"},
	{300, 399,				"时装"},
	{111, 111,				"魔杖"},
	{112, 112,				"生活器具"},
}

-- TODO: 离开地图的时候要统一关闭面板
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
	-- 再检查当前是否处于追踪状态，若是，先取消原来的追踪状态，再开始新的追踪
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

	-- 插入聊天对话框
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

	-- 清除显示的buf
	ClearBuffButton();
	ClearTargetBuffer();
	ClearPKTips();

	-- 清除好友系统中的所有聊天记录所有的
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
	-- 顶号后初始化任务日志数据
	InitTaskLogFrameData();
	SkillMgr:reSetSkilList();
	
	-- 清空任务列表
	--Quest:clearQuest();
	-- 重置公会信息
	GameClanManager:resetClanInfo();
	-- 顶号后初始化登录界面数据
	initSelectFrameData();
	-- 重置快捷键
	ResetShortCut();
	InitShotCutLocalVariable();
	-- 清空配置信息
	GameWizard:zeroSetp();
	GameShortCut:resetShortCut();
	GameMailManager:resetMailData();

	-- 关闭所有的对话泡泡（主角析构时调用）
	closePopWin();
    -- 清除技能选择效果
	SkillMgr:clearIntentSpell();
	-- 清空历史委托记录
	TrustHistoryFrame_DisplayRich:Clear();
	-- 重置聊天数据
	ChatManager:resetChatManager();
	-- 解除所有物品的锁定状态
	ClearItemLock();
	SingleLockItem();
	-- 初始化任务接受框数据
	InitTaskAcceptFrame();
	-- 初始化底部临时快捷栏数据
	InitTmpFrameData();
	-- 初始化背包数据
	InitContainer();
	-- 初始化称号数据
	InitEquipDesignation();
	-- 初始化声望奖章数据
	InitRespactFrame();
	-- 初始化精灵数据
	InitTrumpFrame();
	-- 初始化技能数据
	InitSkillFrame();
	-- 初始化骑乘数据
	InitRideFrame();
	-- 删除所有聊天记录
	initFriendData();
	-- 下线的时候清空所有的交易申请
	RejectAllExchangeInvite( ROLE_LOGOUT_REJECT );
	-- 下线的时候清空所有的骑乘邀请
	RejectAllRideInvite();
	-- 下线初始化所有宠物数据
	initPetFrameData();
	initPetOperationFrameData();
	-- 下线清空minimap的逻辑数据
	initMinimapData();
	-- 清空队伍逻辑数据
	initTeamData();
	-- 下线重置天赋的数据
	InitGeniusData();
	initPetDialogData();
	--IgnoreAllTeamRequests();
	-- 清空伤害统计
	DamageCountFrameReset();
	-- 下线初始化礼包数据
	InitGiftData();
	--[[
	if GameFollowManager:isWeiXing() then
		GameFollowManager:cancelWeiXing();
	end
	--]]
	--清空排行榜数据
	ClearRankInfoData();
	InitAutoTeamData();
	InitSkillData();
	clearOverlayRegion();
	-- 下线初始化新手指引所有相关数据
	InitGuideData();
	InitShortCutBtns();
	--清空生活技能
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
	-- 清空精灵数据
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
	--返回角色时，清空店铺相关
	InitStoreByChangePlayer()
end


RESULT_ATK_ADDMP = 5;					--/* 修正攻击时增加MP */ 
RESULT_ICE_ATK = 14;					--/* 冰攻击力 */ 
RESULT_FIRE_ATK = 15;      				--/* 火攻击力 */ 
RESULT_THUNDER_ATK = 16;      			--/* 雷攻击力 */ 
RESULT_LIGHT_ATK = 27;					--/* 光攻击力 */ 
RESULT_NIGHT_ATK = 28;					--/* 暗攻击力 */ 
RESULT_PHSIC_ATK = 29;					--/* 物理攻击力 */ 
RESULT_HPMAX = 11;						--/* 最大HP */		 
RESULT_MPMAX = 26;						--/* 最大MP */ 
RESULT_HITRATE = 18;					--/* 命中率 */ 
RESULT_FLEE = 19;						--/* 闪避 */ 	
RESULT_ICE_DEF = 20;					--/* 冰防御 */ 
RESULT_FIRE_DEF = 21;					--/* 火防御 */ 
RESULT_THUNDER_DEF = 22;				--/* 雷防御 */ 
RESULT_LIGHT_DEF = 23;					--/* 光防御 */ 
RESULT_NIGHT_DEF = 24;					--/* 暗防御 */ 
RESULT_PHSIC_DEF = 25;					--/* 物理防御 */ 
RESULT_DEF_ICEHURT = 31;				--/* 抗冰伤害 */
RESULT_DEF_FIREHURT = 32;				--/* 抗火伤害 */
RESULT_DEF_THUNDERHURT = 33;			--/* 抗雷伤害 */
RESULT_DEF_LIGHTHURT = 34;				--/* 抗光伤害 */
RESULT_DEF_NIGHTHURT = 35;				--/* 抗暗伤害 */
RESULT_DEF_PHISICHURT = 36;				--/* 抗物理伤害 */
RESULT_DEF_MAGICHURT = 37;				--/* 抗法术伤害 */
RESULT_ICEHURT = 38;					--/* 冰伤害 */
RESULT_FIREHURT = 39;					--/* 火伤害 */
RESULT_THUNDERHURT = 40;				--/* 雷伤害 */
RESULT_LIGHTHURT = 41;					--/* 光伤害 */
RESULT_NIGHTHURT = 42;					--/* 暗伤害 */
RESULT_PHISICHURT = 43;					--/* 物理伤害 */
RESULT_MAGICHURT = 44;					--/* 法术伤害 */
RESULT_HEAVY_HIT = 45;					--/* 暴击 */
RESULT_HP_ADD = 46;      				--/* 加血 */
RESULT_MP_ADD = 47;      				--/* 加魔 */
RESULT_SELF_STATUS = 48;      			--/* 对自身施加状态 */	
RESULT_TMPFILL	= 50;					--/* 填充临时弹药或能量 */
RESULT_DEF_HURT = 53;      				--/* 抗最终伤害 */
RESULT_HURT = 54;      					--/* 最终伤害 */		  
RESULT_MON_FLEE = 61;   				--/* 只对怪物有作用的闪避 */
RESULT_MON_DEF_HURT = 62;   			--/* 只对被怪物打时有作用的抵抗最终伤害 */
RESULT_HEAL_HP = 63;  					--/* 治疗血 */
RESULT_HEAL_MP = 64;   					--/* 治疗魔 */
RESULT_ITEM_TASK = 66;  				--/* 触发物品任务 */
RESULT_ARM_HPADD = 67;  				--/* 装备生命回复 */
RESULT_ARM_MPADD = 68;   				--/* 装备魔法回复 */	  	
RESULT_SKILL_BOOK = 75;					--/* 技能书 */
RESULT_USE_UNDISSOLVE = 76;				--/* 使用不消失 */
RESULT_MAX_MOVE_POWER = 77;				--/* 最大移动力 */
RESULT_FLY = 79;						--/* 飞行速度 */
RESULT_RIDE = 80;						--/* 骑乘道具 */
RESULT_RIDE_MOVE = 81;					--/* 骑乘移动速度 */
RESULT_RIDE_POWER_RESTORE = 82;			--/* 骑乘移动力恢复速度 */
RESULT_CALL_OWNER_MON = 83;				--/* 道具刷怪 */
RESULT_REVIVE_PET = 84;					--/* 道具复活宠物( 战死) */
RESULT_ADD_EXP_PET = 85;				--/* 道具增加宠物经验 */
RESULT_ADD_ARM_GRID = 86;				--/* 道具增加装备格子 */
RESULT_ADD_SKILL_GRID = 87;				--/* 道具增加技能格子 */
RESULT_ADD_POWER = 88;					--/* 道具增加power */
RESULT_PET_ADD_LOVEVAL = 196; 			--/* 宠物增加忠诚度 */
RESULT_PET_SKILL_BOOK = 89;				--/* 宠物技能书 */
RESULT_PET_JIEJING = 90;				--/* 宠物结晶 */
RESULT_PET_ADD_SKILLPOINT = 91;			--/* 宠物增加技能点 */
RESULT_EXP = 92;						--/* 经验值加成 */
RESULT_PET_HATCH = 93;					--/* 宠物蛋 */

RESULT_XIAN_LIST = 94;					--/* 增加精灵栏数量 */
RESULT_SHI_LIST = 95;					--/* 增加侍卫栏数量 */
RESULT_REVIVE_PET_LAOSI = 96;			--/* 道具复活宠物(老死) */
RESULT_ROLE_ADD_SKILLPOINT = 97;		--/* 角色增加技能点 */
RESULT_YOUQIN = 98;						--/* 友侵 */
RESULT_GUAIQIN = 99;					--/* 怪侵 */
RESULT_QIYUE = 100;						--/* 契约 */
RESULT_COMM_COOL = 101;					--/* 公共冷却延长 */
RESULT_SORB_HP = 102;					--/* 吸收hp */
RESULT_SORB_MP = 103;					--/* 吸收mp */
RESULT_HURT_REN = 104;					--/* 对人伤害加成 */
RESULT_HURT_SHENGWU = 105;				--/* 对生物伤害加成 */
RESULT_HURT_BIANYIREN = 106;			--/* 对变异人伤害加成 */
RESULT_ADD_PREPARE = 107;				--/* 吟唱时间延长 */
RESULT_DEC_PREPARE = 108;				--/* 吟唱时间缩小 */
RESULT_ADD_ATK_TARGET = 110;			--/* 增加攻击目标 */
RESULT_ADD_EXP = 111;					--/* 击杀增加经验值 */
RESULT_ADD_STATUS_TIME = 113;			--/* 延长该技能状态的持续生效时间 */
RESULT_ADD_PET_EXP = 114;				--/* 击杀增加宠物经验值 */
RESULT_ADD_SKILL_POINT = 115;			--/* 击杀增加技能点 */
RESULT_ADD_KILL_MP = 116;				--/* 击杀增加MP */
RESULT_ADD_BULL = 117;					--/* 击杀增加BULL */
RESULT_ADD_KILL_COLLECT = 118;			--/* 击杀增加积累数 */
RESULT_ADD_SHECHENG = 119;				--/* 增加射程 */
RESULT_DESPISE = 120;					--/* 藐视,怪物的品质越低，伤害越大 */
RESULT_ADD_COLLECT_UPPER_LIMIT = 121;   --/* 增加积累数上限 */
RESULT_DEC_DEF = 122;					--/* 攻击强度 */
RESULT_ADD_SKILL_LEVEL = 123;			--/* 宠物关键字增加技能级别 */
RESULT_SELF_COOL = 124;					--/* 自我冷却时间调整 */
RESULT_SKILL_RANGE = 125;				--/* 调整技能范围 */
RESULT_SKILL_CONSUME = 126;				--/* 调整技能消耗 */
RESULT_DEPURATE = 127;					--/* 净化 */
RESULT_BACK_HOME = 128;					--/* 回城 */
RESULT_MAKE_TMP_ITEM = 129;				--/* 制造临时道具 */
RESULT_FITTINGS_LEARN = 130;			--/* 配件学习 */
RESULT_COOL_CLEAN = 131;				--/* 技能冷却时间归零 */
RESULT_FIGHTVAL = 132;					--/* 战斗力 */
RESULT_DAMAGE = 133;					--/* 破坏力 */
RESULT_SIT_HP_MODIFY = 134;				--/* 打坐hp修正 */
RESULT_SIT_MP_MODIFY = 135;				--/* 打坐mp修正 */
RESULT_SIT_EXP_MODIFY = 136;			--/* 打坐exp修正 */
RESULT_SIT = 137;						--/* 打坐 */
RESULT_VIGOR = 138;						--/* 激活vigor的被动技能 */
RESULT_CONSUME_VIGOR_MODIFY = 139;		--/* 打坐消耗vigor修正 */
RESULT_NOSIT_ADD_MP = 140;				--/* 不打坐增加mp */
RESULT_NOSIT_ADD_HP = 141;				--/* 不打坐增加hp */
RESULT_ARM_SKILL_LEVEL = 142;			--/* 装备增加技能级别 */
RESULT_DESPISE_MINI = 143;				--/* 藐视微怪 */
RESULT_DESPISE_WEAK = 144;				--/* 藐视弱怪 */
RESULT_DESPISE_SOFT = 145;				--/* 藐视软怪 */
RESULT_DESPISE_NORM = 146;				--/* 藐视普怪 */
RESULT_DESPISE_HARD = 147;				--/* 藐视硬怪 */
RESULT_DESPISE_XI = 148;				--/* 藐视稀怪 */
RESULT_DESPISE_JIN = 149;				--/* 藐视精怪 */
RESULT_DESPISE_BOSS = 150;				--/* 藐视Boss怪 */
RESULT_DESPISE_JI = 151;				--/* 藐视极怪 */
RESULT_PLAYER_ADD_EXP = 152;			--/* 人物增加经验 */
RESULT_XP = 153;						--/* 素质影响XP值增加 */
RESULT_ADD_XP = 154;					--/* 道具直接增加xp值 */
RESULT_DIATHESIS_UP = 155;				--/* 素质升级 */
RESULT_MON_MAGIC_HURT = 156;			--/* 只对被怪物打时有作用的抵抗法术伤害 */
RESULT_ADD_MAX_DUR = 158;				--/* 耐久度上限 */
RESULT_ADD_BULL_DROP = 159;				--/* 银币掉落率 */
RESULT_UNIQUE_STATUS = 160;				--/* 释放唯一性状态 */
RESULT_XP_MAX = 161;					--/* 被动技能影响xp上限 */
RESULT_ADD_STUFF_PACKAGE = 162;			--/* 材料包扩展 */
RESULT_ADD_TASK_PACKAGE = 163;			--/* 任务包扩展 */
RESULT_ADD_SUNDRIES_PACKAGE = 164;		--/* 杂物包扩展 */
RESULT_FAIRY_PIECE = 165;				--/* 魂碎片 */
RESULT_FAIRY_EGG = 166;					--/* 合成魂 */
RESULT_ARM_ADD_EXP = 167;				--/* 装备效果增加角色经验值 */
RESULT_DO_SKILL = 168;					--/* 道具施放技能 */
RESULT_MON_ICE_HURT = 169;				--/* 只对被怪物打时有作用的抵抗冰伤害 */
RESULT_MON_PHSIC_HURT = 170;			--/* 只对被怪物打时有作用的抵抗物理伤害 */
RESULT_ARM_STAR_SAFE = 171;				--/* 装备品质升级保险符效果 */
RESULT_ARM_FIGHT_SAFE = 172;			--/* 装备战斗力升级保险符效果 */
RESULT_GEM_INLAY_SAFE = 173;			--/* 宝石镶嵌保险符效果 */
RESULT_GEM_LEVEL_UP_STUFF = 201; 		--/* 宝石升级符resultval1 对应升级的等级 */	  
RESULT_GEM_PICK_STUFF = 202; 		--/* 宝石摘取符resultval1 对应摘取的等级 */
RESULT_PHISIC_ICE_ATK = 174;			--/* 攻击力上升(魔 物) */
RESULT_CALL_NPC = 175;					--/* 使用道具  召唤NPC */
RESULT_PULL = 176;						--/* 拉人/怪到面前 */
RESULT_TASK_ITEM_RATE = 177;			--/* 增加任务掉落率 */
RESULT_STUFF_ITEM_RATE = 178;			--/* 增加材料掉落率 */
RESULT_NO_BREAK_PRE = 179;				--/* 吟唱不会被打断 */
RESULT_DEC_MP = 180;					--/* 减少对方MP  */
RESULT_HEAVY_HIT_VAL = 181;				--/* 暴击伤害数值修正   */
RESULT_ADD_DESIGNATION = 182;			--/* 角色增加一个新称号 */
RESULT_ADD_TMPENERGE = 183;				--/* 增加临时道具的能量 */
RESULT_ADD_RESPACT = 184;				--/* 角色增加声望 */
RESULT_FAIRY_BORN = 185;				--/* 精灵出生 */
RESULT_ADD_VIM = 186;					--/* 增加精力 */
RESULT_ADD_EXP_ONTIME = 187;			--/* 隔N秒加经验 被动 */
RESULT_HP_CHG_MP = 188;					--/* hp转化成mp  */
RESULT_SKILL_FLUSH = 189;				--/* 所有技能立即CD--刷新技能 */
RESULT_ADD_XP_ONTIME = 190;				--/* 隔N秒加xp 不需要战斗状态 被动 */
RESULT_ADD_BASE_ATTR = 191;				--/* 增加一级属性 1-力 2-智 3-体 4-精 被动 */
RESULT_WATER_DEF = 267;					-- /* 水防御|装备效果和宠物天赋效果，Val1表示基础水防御加修正，Val2表示乘修正，Val3表示加修正 */

RESULT_ADD_BASE_ATTR_HIT		= 197;	--/* 一级属性精准 */
RESULT_ADD_BASE_ATTR_DEF		= 198; 	--/* 一级属性防护 */
RESULT_ADD_BASE_ATTR_FLEE		= 199; 	--/* 一级属性躲闪 */
RESULT_ADD_BASE_ATTR_STR		= 204; 	--/* 一级属性力量 */
RESULT_ADD_BASE_ATTR_INT		= 205; 	--/* 一级属性智力 */
RESULT_ADD_BASE_ATTR_STA		= 206; 	--/* 一级属性体质 */
RESULT_ADD_BASE_ATTR_SPR		= 207; 	--/* 一级属性精神 */
RESULT_ADD_BASE_ATTR_AGILE		= 262;  --/* 一级属性敏捷 */
RESULT_WATER_ATK				= 263; 	--/* 水攻击力|装备效果、天赋效果、宠物天赋效果、技能效果和配件效果，Val1表示基础水攻击力加修正，Val2表示乘修正，Val3表示加修正 */
RESULT_ADD_BASE_ATTR_HEAVY_HIT	= 208; 	--/* 一级属性暴击 */		 
RESULT_TREASURE_PIC				= 214; 	--/* 藏宝图地图坐标 */
RESULT_ITEM_CHAT				= 233;	--/* 小喇叭消耗物品|小喇叭聊天服务器广播 */
RESULT_SECONDARY_SKILL_RECIPE	= 236;	--/* 学习生活技能 */
RESULT_SECONDARY_SKILL_PRODUCE_CHG = 235;	--/*生活技能生产列表改变*/
RESULT_ADD_SUNDPKG_GRID			= 256;	--增加杂物背包格子
RESULT_ADD_STUFFPKG_GRID		= 258;	-- 增加材料背包格子
RESULT_BASE_ATTR_HEAVYHIT		= 271;	--/* 暴击力|装备效果，Val1表示增加的值 */
RESULT_DEF_HEAVYHIT				= 272;	--/* 暴击抵抗|装备效果，Val1表示增加的值 */
RESULT_MAGIC_HP_ADD				= 274; 	--/* 法术治疗效果|技能效果，治疗量= 人物魔法攻击*乘修正+加修正，Val1乘修正Min，Val2乘修正Max， Val3加修正Min， Val3加修正Max   */
RESULT_ELEMENT_ATK = 277; 				--/* 元素伤害效果|val1表示类型 1.火 2.雷 3.光 4.暗 5.氺  val2表示最小至 val3表示最大值val4-时间:分 */
RESULT_ELEMENT_DEF = 278;		 		--/* 元素防御效果|val1表示类型 1.火 2.雷 3.光 4.暗 5.氺  val2表示最小至 val3表示最大值val4-时间:分 */
RESULT_IGNORE_DEF = 304;				--/* 道具效果|忽视防御，值(公式算的实际忽视的防御) */
RESULT_REFLECTION_HP = 305; 			--/* 道具效果|反射造成的血量损失值(公式算的实际值) */
RESULT_DEF_HP = 306;					--/* 道具效果|抵抗造成的血量伤害，值(公式算的实际值) */
RESULT_MACHINE_ITEM = 308; 				--/* 道具效果|怪物机械放到背包中的物品，val 2怪物机械定义ID */
RESULT_BEACH_EXP_MUL = 314;			 	--/* 海滩经验加成系数(100为基数)|val1为系数 */
RESULT_ITEM_SWITCH = 315;				--/* itemresult|val1:1-银币 2-exp 3-金卷 val2-转换多少 */'
RESULT_GEM = 321;					 	--/* 道具result|表示此物品是宝石 */
RESULT_GEM_CHIP = 322;					--/* 道具result|表示此物品是宝石碎片|val1表示能量点 */
RESULT_WEAPON_MIN_PHSIC_ATK = 325; 	--/* 下限物理攻击|val1 基础攻击val2 附加攻击 */
RESULT_WEAPON_MAX_PHSIC_ATK = 326; 		--/* 上限物理攻击|val1 基础攻击val2 附加攻击 */
RESULT_WEAPON_MIN_MAGIC_ATK = 327; 		--/* 下限魔法攻击|val1 基础攻击val2 附加攻击 */
RESULT_WEAPON_MAX_MAGIC_ATK = 328;	 	--/* 上限魔法攻击|val1 基础攻击val2 附加攻击 */
RESULT_BASE_ATTR_HP = 329; 				--/* 生命值|val1 基础数值val2 附加数值 */
RESULT_BASE_ATTR_PHSIC_DEF = 330;		--/* 物理防御|val1 基础数值val2 附加数值 */
RESULT_BASE_ATTR_MAGIC_DEF = 331;	 	--/* 魔法防御|val1 基础数值val2 附加数值 */
RESULT_BASE_ATTR_WIND_DEF = 332;	 	--/* 风元素抗性|val1 基础数值val2 附加数值 */
RESULT_BASE_ATTR_SOIL_DEF = 333; 		--/* 土元素抗性|val1 基础数值val2 附加数值 */
RESULT_BASE_ATTR_THUNDER_DEF = 334;	 	--/* 电元素抗性|val1 基础数值val2 附加数值 */
RESULT_BASE_ATTR_LIGHT_DEF = 335;	 	--/* 光元素抗性|val1 基础数值val2 附加数值 */
RESULT_BASE_ATTR_NIGHT_DEF = 336;	 	--/* 暗元素抗性|val1 基础数值val2 附加数值 */
RESULT_BASE_ATTR_FLEE_LEVEL = 337; 		--/* 闪避等级|val1 基础数值val2 附加数值 */
RESULT_BASE_ATTR_HIT_LEVEL = 338; 		--/* 命中等级|val1 基础数值val2 附加数值 */
RESULT_BASE_ATTR_HEAVY_HIT_VAL = 339;	--/* 暴击伤害|val1 基础数值val2 附加数值 */
RESULT_ARM_RECAST = 340;				--/* 装备重铸材料|val1 可以重铸装备的最高等级 */
RESULT_BASE_ATTR_PHSIC_ATK = 341;		--/* 物理攻击|无上下限的攻击力val1 基础数值val2 附加数值 */
RESULT_BASE_ATTR_MAGIC_ATK = 342;	 	--/* 魔法攻击|无上下限的攻击力val1 基础数值val2 附加数值 */
RESULT_ARM_WASH_STUFF = 349; 			--/* 装备洗炼材料|道具效果val1此材料可作用的装备最大等级 */
RESULT_SPAR = 350;					 	--/* 普通晶石|道具效果 */
RESULT_SPAR_SHOP = 351;				 	--/* 商城晶石|道具效果 */
RESULT_SHANG_JI_TASK = 357;				--/* 道具效果|赏金令道具|val1-赏金类ID，表示可以接取哪一类的赏金任务 */
RESULT_RUNE = 344;					--/* 表示此物品是符文 val1 表示级别 0低级 1中级 2高级 val2 表示颜色 1红 2蓝 3绿 */
RESULT_MAP_RECORDER = 361; 			--/* 空间记忆器|道具效果 使用时定位坐标或传送地图 val1 为可记录的最大数量(上限为5) */
RESULT_RONGLU = 380;			 	--/* 道具result|表示熔炉 */
RESULT_BAOXIANG = 383;			 	--/* 道具效果|宝箱，Val1 表示宝箱ID */
RESULT_TILIAN = 385;				--/* 道具效果|晶石提炼,val1-增加分解成功概率:万分率val2-消耗货币类型,val3-消耗货币数量,val4-装备最大等级限制 */
RESULT_SPAN_CHAT = 387;				--/* 道具效果|跨服聊天 */
RESULT_ARM_ATTR_MOVE = 396; 			--/* 装备属性转移道具:val1-等级:表示能转移多少级的装备 */
RESULT_POFANG_LEVEL = 399; 			--/* 动态破防等级|val1--值 */
RESULT_DIKANG_LEVEL = 400; 			--/* 动态抵抗等级|val1--值 */
RESULT_WEAPON_MIN_ATK = 408; 			--/* 下限双系攻击|val1 基础攻击val2 附加攻击 */
RESULT_WEAPON_MAX_ATK = 409; 			--/* 上限双系攻击|val1 基础攻击val2 附加攻击 */
RESULT_ITEM_TASK_RECYCLE = 410; 		--/* 道具效果|物品接取环组任务val1 环组ID */
RESULT_BACK_LEVELUP = 413; 				--/* 背饰升级道具|Val1 几级背饰用来升下一级， 例如1表示，1升2的道具 */
RESULT_BACK_CHG_TYPE = 414; 			--/* 背饰改变种类道具|val1--改变后的类型，对应BackLevelDef->SubType */
RESULT_CHARM = 411;						--/*魅力值道具|val1--加减多少魅力值 */
RESULT_LOST_TOTEM = 412;				--/*失落图腾道具|val1-召唤怪生存时间(秒),val2--经验上限,val3-召唤怪ID,val4-获得多少声望 */
RESULT_ITEM_EXP_BUCHANG = 415; 			--/* 道具效果|离线经验补偿道具 */
RESULT_ITEM_ARM_BUCHANG = 416;		 	--/* 道具效果|离线装备补偿道具 */
RESULT_ITEM_ARM_SELECT = 417; 			--/* 道具效果|选择某种道具:val-等级/10 */
RESULT_ITEM_CARDBOX = 418;			--/* 道具效果|卡牌宝箱 */
RESULT_ITEM_CLAN_CONTRI = 419;			--/* 道具效果|公会贡献度，实例上包含数值 */
RESULT_ARM_RECAST_MUST = 420;			--/* 装备重铸必定成功材料|val1 可以重铸装备的最高等级 */
RESULT_ARM_WASH_STUFF_MUST = 421; 		--/* 装备洗炼必定成功材料|val1此材料可作用的装备最大等级 */
RESULT_LEGEND_SCROLL	= 429; 			--/* 道具效果|传奇卷轴val-道具ID */
RESULT_CLAN_MEM_NUM_TOP	= 430;			--/* 公会人数上限|公会技能效果 val1 公会人数上限值 */
RESULT_PET_SILIAO = 432;				--	/* 宠物饲料:val1-回复多少温饱 val2-宠物成长上限 */
RESULT_PET_WANJU = 433;					--	/* 宠物玩具:val1-加多少欢乐度 */
RESULT_PET_QIANLI = 434; 				--	/* 宠物潜力值道具:val1-多少潜力值 */
RESULT_ITEM_BISAI = 435;			 	--	/* 比赛服的那个东东 */
RESULT_PET_GENIUS_BOOK = 436;			-- 	/* 宠物天赋书:第一个result val1-val4表示技能 第二个result val1-val2表示专属宠物ID */
RESULT_PET_GENIUS_UP = 454;				-- 	/* 道具效果:宠物品质升级,val1-宠物品质,0-3表示普通到杰出 */
RESULT_ITEM_GEM_PAG = 462;				-- 	/* 道具效果|宝石包道具,val1代表GemPagDef表中的Id */
RESULT_ALL_ATTR = 463 				--/* 道具效果|val1:装备属性-全属性加多少点 */
RESULT_ITEM_JIAJU = 464			--/* 道具效果|家具,能够在家园中摆放,val1-对应的怪物 */
RESULT_ITEM_HOME_PET = 465			--/* 道具效果|家园宠物,val1-HomeMonSegDef中的MonID,表示替换为哪个宠物 */
RESULT_ITEM_ZHONGZHI = 466			--/* 道具效果|种植道具val1-种植时刷的怪ID,val2-成熟时间(秒) */
RESULT_WEAPON_ATK_MUL = 467; 			--/* 双系攻击乘修正|val1 百分比，增加双系攻击的百分比修正 */
RESULT_WEAPON_DEF_MUL = 468; 			--/* 双系防御乘修正|val1 百分比，增加双系防御的百分比修正 */
RESULT_SPAR_SORB_FREE = 469; 		--/* 新能源充能道具|给晶石充能不消耗钱 */
RESULT_GEM_ADD_POWER_FREE = 470;		--/* 新宝石充能道具|给宝石充能不消耗钱 */

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
[RESULT_ICE_ATK] = { desc = "魔法攻击" },
};

t_tagRESULT =
{
    { RESULT_ICE_ATK,				"魔法攻击"},
    { RESULT_PHSIC_ATK, 			"物理攻击"},
    { RESULT_ICE_DEF,				"魔法防御"},
    { RESULT_PHSIC_DEF, 			"物理防御"},
    { RESULT_HPMAX,					"生命值"},
    { RESULT_MPMAX,					"魔法值"},
    { RESULT_HITRATE,				"命中等级"},
    { RESULT_FLEE, 					"闪避等级"},
    { RESULT_DEF_PHISICHURT,		"物理伤害减免"},
    { RESULT_DEF_MAGICHURT,			"魔法伤害减免"},
    { RESULT_ARM_HPADD,				"生命值每秒回复"},
    { RESULT_ARM_MPADD,				"魔法值每秒回复"},

	{ RESULT_HEAVY_HIT, 			"暴击率" },		-- 暴击
    { RESULT_HEAVY_HIT_VAL, 		"暴击伤害" },	-- 暴击伤害提升
    { RESULT_DAMAGE, 				"攻击强度" },

	{ RESULT_ADD_BASE_ATTR_HIT, 		"精准"},
    { RESULT_ADD_BASE_ATTR_DEF, 		"敏捷"},
    { RESULT_ADD_BASE_ATTR_FLEE, 		"躲闪"},
	{ RESULT_ADD_BASE_ATTR_STR, 		"力量"},
    { RESULT_ADD_BASE_ATTR_INT, 		"智力"},
    { RESULT_ADD_BASE_ATTR_STA, 		"体质"},
    { RESULT_ADD_BASE_ATTR_SPR, 		"精神"},
    { RESULT_ADD_BASE_ATTR_HEAVY_HIT,	"暴击"},

    { RESULT_ATK_ADDMP, 			"修正攻击时增加魔法值" },
    { RESULT_FIRE_ATK, 				"风元素伤害" },
    { RESULT_THUNDER_ATK,			"电元素伤害" },
    { RESULT_LIGHT_ATK, 			"光元素伤害" },
    { RESULT_NIGHT_ATK, 			"暗元素伤害" },
	{ RESULT_WATER_ATK,				"土元素伤害" },
    { RESULT_FIRE_DEF, 				"风元素抗性" },
    { RESULT_THUNDER_DEF, 			"电元素抗性" },
    { RESULT_LIGHT_DEF, 			"光元素抗性" },
    { RESULT_NIGHT_DEF, 			"暗元素抗性" }, 
    { RESULT_DEF_ICEHURT, 			"魔法伤害减免" },
	{ RESULT_WATER_DEF, 			"土元素抗性" },
    { RESULT_DEF_FIREHURT, 			"风元素抗性" },
    { RESULT_DEF_THUNDERHURT, 		"电元素抗性" },
    { RESULT_DEF_LIGHTHURT, 		"光元素抗性" },
    { RESULT_DEF_NIGHTHURT, 		"暗元素抗性" },	
    { RESULT_ICEHURT, 				"冰伤害" },
    { RESULT_FIREHURT, 				"火伤害" },
    { RESULT_THUNDERHURT, 			"雷伤害" },
    { RESULT_LIGHTHURT, 			"光伤害" },
    { RESULT_NIGHTHURT, 			"暗伤害" },
    { RESULT_PHISICHURT, 			"物理伤害" },
    { RESULT_MAGICHURT, 			"法术伤害" },
	{ RESULT_TMPFILL,				"填充临时弹药或能量" },
    { RESULT_HP_ADD, 				"加血" },
    { RESULT_MP_ADD, 				"加魔" },
    --{ RESULT_SELF_STATUS, 			"对自身施加状态" },	 
    { RESULT_DEF_HURT, 				"抗最终伤害" },
    { RESULT_HURT, 					"最终伤害" },		  
    { RESULT_MON_FLEE, 				"只对怪物有作用的闪避" },
    { RESULT_MON_DEF_HURT, 			"只对被怪物打时有作用的抵抗最终伤害" },
    { RESULT_HEAL_HP, 				"治疗加成" },
    { RESULT_HEAL_MP, 				"治疗魔" },
    { RESULT_ITEM_TASK, 			"触发物品任务" },	  	
    { RESULT_SKILL_BOOK, 			"技能书" },
    { RESULT_USE_UNDISSOLVE, 		"使用不消失" },
    { RESULT_MAX_MOVE_POWER, 		"最大移动力" },
    { RESULT_FLY, 					"飞行速度" },
    { RESULT_RIDE, 					"骑乘道具" },
    { RESULT_RIDE_MOVE, 			"骑乘移动速度" },
    { RESULT_RIDE_POWER_RESTORE, 	"骑乘移动力恢复速度" },
    { RESULT_CALL_OWNER_MON, 		"道具刷怪" },
    { RESULT_REVIVE_PET, 			"道具复活宠物( 战死)" },
    { RESULT_ADD_EXP_PET, 			"道具增加宠物经验" },
    { RESULT_ADD_ARM_GRID, 			"道具增加装备格子" },
    { RESULT_ADD_SKILL_GRID, 		"道具增加技能格子" },
    { RESULT_ADD_POWER, 			"道具增加power" },
    { RESULT_PET_SKILL_BOOK, 		"宠物技能书" },
    { RESULT_PET_JIEJING, 			"宠物结晶" },
    { RESULT_PET_ADD_SKILLPOINT, 	"宠物增加技能点" },
    { RESULT_EXP, 					"经验值加成" },
    { RESULT_PET_HATCH , 			"宠物蛋" },

    { RESULT_XIAN_LIST, 			"增加精灵栏数量" },
    { RESULT_SHI_LIST, 				"增加宠物栏数量" },
    { RESULT_REVIVE_PET_LAOSI, 		"道具复活宠物(老死)" },
    { RESULT_ROLE_ADD_SKILLPOINT, 	"角色增加技能点" },
    { RESULT_YOUQIN, 				"友侵" },
    { RESULT_GUAIQIN, 				"怪侵" },
    { RESULT_QIYUE, 				"契约" },
    { RESULT_COMM_COOL, 			"公共冷却延长" },
    { RESULT_SORB_HP, 				"吸收生命值" },
    { RESULT_SORB_MP, 				"吸收魔法值" },
    { RESULT_HURT_REN, 				"对人伤害加成" },
    { RESULT_HURT_SHENGWU, 			"对生物伤害加成" },
    { RESULT_HURT_BIANYIREN, 		"对变异人伤害加成" },
    { RESULT_ADD_PREPARE, 			"吟唱时间延长" },
    { RESULT_DEC_PREPARE, 			"吟唱时间缩小" },
    { RESULT_ADD_ATK_TARGET, 		"增加攻击目标" },
    { RESULT_ADD_EXP, 				"击杀增加经验值" },
    { RESULT_ADD_STATUS_TIME, 		" 延长该技能状态的持续生效时间" },
    { RESULT_ADD_PET_EXP, 			"击杀增加宠物经验值" },
    { RESULT_ADD_SKILL_POINT, 		" 击杀增加技能点" },
    { RESULT_ADD_KILL_MP, 			"击杀增加魔法值" },
    { RESULT_ADD_BULL, 				"击杀增加BULL" },
    { RESULT_ADD_KILL_COLLECT, 		"击杀增加战意" },
    { RESULT_ADD_SHECHENG, 			"增加射程" },
    { RESULT_DESPISE, 				" 藐视,怪物的品质越低，伤害越大" },
    { RESULT_DEC_DEF, 				"攻击强度" },
    { RESULT_ADD_SKILL_LEVEL, 		"宠物关键字增加技能级别" },
    { RESULT_SELF_COOL, 			"自我冷却时间调整" },
    { RESULT_SKILL_RANGE, 			"调整技能范围" },
    { RESULT_SKILL_CONSUME, 		"调整技能消耗" },
    { RESULT_DEPURATE, 				"净化" },
    --{ RESULT_BACK_HOME, 			"回城" },
    { RESULT_MAKE_TMP_ITEM, 		"制造临时道具" },
    { RESULT_FITTINGS_LEARN, 		"配件学习" },
    { RESULT_COOL_CLEAN, 			"技能冷却时间归零" },
    { RESULT_FIGHTVAL, 				"战斗力" },
    { RESULT_SIT_HP_MODIFY, 		"打坐生命值修正" },
    { RESULT_SIT_MP_MODIFY, 		"打坐mp修正" },
    { RESULT_SIT_EXP_MODIFY, 		"打坐exp修正" },
    { RESULT_SIT ,					"打坐" },
    { RESULT_VIGOR,					"激活vigor的被动技能" },
    { RESULT_CONSUME_VIGOR_MODIFY,	"打坐消耗vigor修正" },
    { RESULT_NOSIT_ADD_MP,			"不打坐增加魔法值" },
    { RESULT_NOSIT_ADD_HP,			"不打坐增加生命值" },
    { RESULT_ARM_SKILL_LEVEL,		"装备增加技能级别" },
    { RESULT_DESPISE_MINI,			"藐视微怪" },
    { RESULT_DESPISE_WEAK,			"藐视弱怪" },
    { RESULT_DESPISE_SOFT, 			"藐视软怪" },
    { RESULT_DESPISE_NORM, 			"藐视普怪" },
    { RESULT_DESPISE_HARD, 			"藐视硬怪" },
    { RESULT_DESPISE_XI, 			"藐视稀怪" },
    { RESULT_DESPISE_JIN, 			"藐视精怪" },
    { RESULT_DESPISE_BOSS, 			"藐视Boss怪" },
    { RESULT_DESPISE_JI, 			"藐视极怪" },
    { RESULT_PLAYER_ADD_EXP, 		"人物增加经验" },
    { RESULT_XP, 					"素质影响XP值增加" },
    { RESULT_ADD_XP, 				"道具直接增加xp值" },
    { RESULT_DIATHESIS_UP, 			"素质升级" },
    { RESULT_MON_MAGIC_HURT, 		"只对被怪物打时有作用的抵抗法术伤害" },
    { RESULT_ADD_MAX_DUR, 			"耐久度上限" },
    { RESULT_ADD_BULL_DROP, 		""..MONEY_TYPE_MONEY_NAME.."掉落率" },
    { RESULT_UNIQUE_STATUS, 		"释放唯一性状态" },
    { RESULT_XP_MAX, 				"被动技能影响xp上限" },
    { RESULT_ADD_STUFF_PACKAGE, 	"材料包扩展" },
    { RESULT_ADD_TASK_PACKAGE, 		"任务包扩展" },
    { RESULT_ADD_SUNDRIES_PACKAGE, 	"杂物包扩展" },
    { RESULT_FAIRY_PIECE, 			"魂碎片" },
    { RESULT_FAIRY_EGG, 			"合成魂" },
    { RESULT_ARM_ADD_EXP, 			"装备效果增加角色经验值" },
    { RESULT_DO_SKILL, 				"道具施放技能" },
    { RESULT_MON_ICE_HURT, 			"只对被怪物打时有作用的抵抗冰伤害" },
    { RESULT_MON_PHSIC_HURT, 		"只对被怪物打时有作用的抵抗物理伤害" },
    { RESULT_ARM_STAR_SAFE, 		"装备品质升级保险符效果" },
	
	{ RESULT_GEM_LEVEL_UP_STUFF, 	"宝石升级符resultval1 对应升级的等级" },	  
	{ RESULT_GEM_PICK_STUFF,		"宝石摘取符resultval1 对应摘取的等级" },

    { RESULT_ARM_FIGHT_SAFE, 		"装备战斗力升级保险符效果" },
    { RESULT_GEM_INLAY_SAFE, 		"宝石镶嵌保险符效果" },
    { RESULT_PHISIC_ICE_ATK, 		"攻击" },
   -- { RESULT_CALL_NPC, 				"使用道具  召唤NPC" },
    { RESULT_PULL, 					"拉人/怪到面前" },
    { RESULT_TASK_ITEM_RATE, 		"增加任务掉落率" },
    { RESULT_STUFF_ITEM_RATE, 		"增加材料掉落率" },
    { RESULT_NO_BREAK_PRE, 			"吟唱不会被打断" },
    { RESULT_DEC_MP = 180, 			"减少对方MP" },
	

    { RESULT_ADD_DESIGNATION, 			"角色增加一个新称号" },
    { RESULT_ADD_TMPENERGE, 			"增加临时道具的能量" },
    { RESULT_ADD_RESPACT, 				"角色增加声望" },
    { RESULT_FAIRY_BORN, 				"精灵出生" },
    { RESULT_ADD_VIM, 					"增加精力" },
    { RESULT_ADD_EXP_ONTIME, 			"隔N秒加经验 被动" },
    { RESULT_HP_CHG_MP, 				"hp转化成mp" },
    { RESULT_SKILL_FLUSH,				"所有技能立即CD--刷新技能" },
    { RESULT_ADD_XP_ONTIME,				"隔N秒加xp 不需要战斗状态 被动" },
    { RESULT_ADD_BASE_ATTR,				"增加一级属性 1-力 2-智 3-体 4-精 被动" },	  
    { RESULT_ADD_COLLECT_UPPER_LIMIT,	"增加战意上限" },
	{ RESULT_BASE_ATTR_HEAVYHIT,		"暴击等级"},
	{ RESULT_DEF_HEAVYHIT,				"暴击抵抗"},
	{ RESULT_IGNORE_DEF,				"破防等级"},
	{ RESULT_REFLECTION_HP,				"反伤等级"},
	{ RESULT_DEF_HP,					"抵抗等级"},
	{ RESULT_BASE_ATTR_HP,				"生命值"},
	{ RESULT_BASE_ATTR_PHSIC_DEF,		"物理防御"},
	{ RESULT_BASE_ATTR_MAGIC_DEF,		"魔法防御"},
	{ RESULT_BASE_ATTR_WIND_DEF,		"风元素抗性"},
	{ RESULT_BASE_ATTR_SOIL_DEF,		"土元素抗性"},
	{ RESULT_BASE_ATTR_THUNDER_DEF,		"电元素抗性"},
	{ RESULT_BASE_ATTR_LIGHT_DEF,		"光元素抗性"},
	{ RESULT_BASE_ATTR_NIGHT_DEF,		"暗元素抗性"},
	{ RESULT_BASE_ATTR_FLEE_LEVEL,		"闪避等级"},
	{ RESULT_BASE_ATTR_HIT_LEVEL,		"命中等级"},
	{ RESULT_BASE_ATTR_HEAVY_HIT_VAL,	"暴击伤害"},
	{ RESULT_BASE_ATTR_PHSIC_ATK,		"物理攻击"},
	{ RESULT_BASE_ATTR_MAGIC_ATK,		"魔法攻击"},
	{ RESULT_ADD_BASE_ATTR_AGILE,		"感知"},
	{ RESULT_WEAPON_ATK_MUL,			"双系攻击"},
	{ RESULT_WEAPON_DEF_MUL,			"双系防御"},
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

-- 装备关键字
ADD_ATTR_RESULT_NONE				= 0; 
ADD_ATTR_RESULT_ATK					= 1;	-- /* 攻击力上升 */
ADD_ATTR_RESULT_HP					= 2;	-- /* 最大血量上升 */
ADD_ATTR_RESULT_MP					= 3;	-- /* 最大魔法上升 */
ADD_ATTR_RESULT_DAMAGE				= 4;	-- /* 破坏力 */
ADD_ATTR_RESULT_EXP					= 5;	-- /* 战斗增加经验 */
ADD_ATTR_RESULT_PHSIC_DEF			= 6;	-- /* 物防 */
ADD_ATTR_RESULT_ICE_DEF				= 7;	-- /* 魔防 */
ADD_ATTR_RESULT_ADD_DEF				= 8;	-- /* 双防 */
ADD_ATTR_RESULT_MAX_DUR				= 9;	-- /* 耐久度上限 */
ADD_ATTR_RESULT_MIN_ATK				= 10;	-- /* 攻击力小幅上升 */
ADD_ATTR_RESULT_MIN_HP				= 11;	-- /* 最大血量小幅上升 */
ADD_ATTR_RESULT_MIN_MP				= 12;	-- /* 最大魔法小幅上升 */
ADD_ATTR_RESULT_DEF_PHISICHURT		= 13;	-- /* 物抗 */
ADD_ATTR_RESULT_DEF_ICEHURT			= 14;	-- /* 魔抗 */
ADD_ATTR_RESULT_DEF_ICE_PHISIC		= 15;	-- /* 双抗 */
ADD_ATTR_RESULT_ENERGE_ATK			= 16;	-- /* 能量场攻击力上升 */
ADD_ATTR_RESULT_CANNON_ATK			= 17;	-- /* 炮攻击力上升 */
ADD_ATTR_RESULT_NOFIGHT_HPADD		= 18;	-- /* HP回复（万分比/秒）非战斗状态 */
ADD_ATTR_RESULT_NOFIGHT_MPADD		= 19;	-- /* MP回复（万分比/秒）非战斗状态 */
ADD_ATTR_RESULT_HPADD				= 20;	-- /* 随时间HP回复（万分比/秒） */
ADD_ATTR_RESULT_MPADD				= 21;	-- /* 随时间MP回复（万分比/秒） */
ADD_ATTR_RESULT_MIN_PHSIC_DEF		= 22;	-- /* 物防小幅上升 */
ADD_ATTR_RESULT_MIN_ICE_DEF			= 23;	-- /* 魔防小幅上升 */
ADD_ATTR_RESULT_FIGHTVAL			= 24;	-- /* 战斗力 */
ADD_ATTR_RESULT_FLEE				= 25;	-- /* 闪避率 */
ADD_ATTR_RESULT_SKILL_LEVEL			= 26;	-- /* 增加技能等级 */
ADD_ATTR_RESULT_BADSTATUS_RATE		= 27;	-- /* 施放不良状态成功率 */
ADD_ATTR_RESULT_BADSTATUS_DEF		= 28;	-- /* 抵抗不良状态 */
ADD_ATTR_RESULT_BADSTATUS_DEC_TIME	= 29;	-- /* 不良状态持续时间下降 */
ADD_ATTR_RESULT_HEAVY_HIT			= 30;	-- /* 爆击率 */
ADD_ATTR_RESULT_HITRATE				= 31;	-- /* 命中率 */
ADD_ATTR_RESULT_TASK_ITEM_RATE		= 32;	-- /* 任务物品掉落率 */
ADD_ATTR_RESULT_STUFF_ITEM_RATE		= 33;   -- /* 材料掉落率 */
ADD_ATTR_RESULT_EGG_RATE			= 34;	-- /* 宠物蛋掉落率 */
ADD_ATTR_RESULT_ADD_BULL			= 35;	-- /* 增加银币掉落率 */
ADD_ATTR_RESULT_MIN_EXP				= 36;	-- /* 战斗小幅增加经验 */
ADD_ATTR_RESULT_ADD_PET_EXP			= 37;	-- /* 战斗宠物增加经验值获得 */
ADD_ATTR_RESULT_ADD_SKILLPOINT		= 38;	-- /* 增加技能点获得 */
ADD_ATTR_RESULT_FIGHT_HPADD			= 39;	-- /* HP回复值战斗状态 */
ADD_ATTR_RESULT_FIGHT_MPADD			= 40;	-- /* MP回复值战斗状态 */
ADD_ATTR_RESULT_HITRATE_RATE		= 41;	-- /* 命中率 */
ADD_ATTR_RESULT_PHSIC_ATK			= 42;	-- /* 物理攻击力上升 */
ADD_ATTR_RESULT_ICE_ATK				= 43;	-- /* 魔法攻击力上升 */

ADD_ATTR_RESULT_HEAVY_HIT_VAL		= 44;	-- /* 暴击伤害 */	
ADD_ATTR_RESULT_BASE_ATTR_HEAVYHIT	= 45; 	-- /* 暴击力 */
ADD_ATTR_RESULT_DEF_HEAVY_HIT		= 46; 	-- /* 暴击抵抗 */
ADD_ATTR_RESULT_LILIANG				= 47; 	-- /* 力量 */
ADD_ATTR_RESULT_ZHILI				= 48; 	-- /* 智力 */
ADD_ATTR_RESULT_TIZHI				= 49;	-- /* 体质 */
ADD_ATTR_RESULT_FANGHU				= 50;	-- /* 防护 */
ADD_ATTR_RESULT_JINGSHEN			= 51;	-- /* 精神 */
ADD_ATTR_RESULT_MINJIE				= 52;	-- /* 敏捷 */
ADD_ATTR_RESULT_WATERATK			= 53;	-- /* 加水攻击力 */
ADD_ATTR_RESULT_FIREATK				= 54;	-- /* 加火攻击力 */
ADD_ATTR_RESULT_THUNDERATK			= 55;	-- /* 加雷攻击力 */
ADD_ATTR_RESULT_LIGHTATK			= 56;	-- /* 加光攻击力 */
ADD_ATTR_RESULT_NIGHTATK			= 57;	-- /* 加暗攻击力 */
ADD_ATTR_RESULT_WATERDEF			= 58;	-- /* 加水防御力 */
ADD_ATTR_RESULT_FIREDEF				= 59;	-- /* 加火防御力 */
ADD_ATTR_RESULT_THUNDERDEF			= 60;	-- /* 加雷防御力 */
ADD_ATTR_RESULT_LIGHTDEF			= 61;	-- /* 加光防御力 */
ADD_ATTR_RESULT_NIGHTDEF			= 62;	-- /* 加暗防御力 */
ADD_ATTR_RESULT_IGNORE_DEF			= 63; 	--/* 忽视防御，值(公式算的实际忽视的防御) */
ADD_ATTR_RESULT_REFLECTION_HP		= 64; 	--/* 反射造成的血量损失值(公式算的实际值) */
ADD_ATTR_RESULT_DEF_HP				= 65; 	--/* 抵抗造成的血量伤害，值(公式算的实际值) */
ADD_ATTR_RESULT_HEAl_HP_MUL			= 66;	-- /* 治疗加成 */
ADD_ATTR_RESULT_DEC_WEAR_LEVEL		= 73; 	-- /* 减少穿戴等级要求val1 减少的等级 */


t_tagArmRESULT =
{
    { ADD_ATTR_RESULT_ATK,					"攻击力上升" },
    { ADD_ATTR_RESULT_HP, 					"最大血量上升" },
    { ADD_ATTR_RESULT_MP, 					"最大魔法上升" },
    { ADD_ATTR_RESULT_DAMAGE,				"破坏力" },
    { ADD_ATTR_RESULT_EXP,					"战斗增加经验" },
    { ADD_ATTR_RESULT_PHSIC_DEF, 			"物防" },
    { ADD_ATTR_RESULT_ICE_DEF,				"魔防" },
    { ADD_ATTR_RESULT_ADD_DEF,				"双防" },
    { ADD_ATTR_RESULT_MAX_DUR,				"耐久度上限" },
    { ADD_ATTR_RESULT_MIN_ATK,				"攻击力小幅上升" },
    { ADD_ATTR_RESULT_MIN_HP,				"最大血量小幅上升" },
    { ADD_ATTR_RESULT_MIN_MP,				"最大魔法小幅上升" },
    { ADD_ATTR_RESULT_DEF_PHISICHURT,		"物理伤害减免" },	-- "物抗"
    { ADD_ATTR_RESULT_DEF_ICEHURT,			"魔法伤害减免" },	-- "魔抗"
    { ADD_ATTR_RESULT_DEF_ICE_PHISIC,		"双抗" },
    { ADD_ATTR_RESULT_ENERGE_ATK,			"能量场攻击力上升" },
    { ADD_ATTR_RESULT_CANNON_ATK,			"炮攻击力上升" },
    { ADD_ATTR_RESULT_NOFIGHT_HPADD,		"HP回复（万分比/秒）非战斗状态" },
    { ADD_ATTR_RESULT_NOFIGHT_MPADD,		"MP回复（万分比/秒）非战斗状态" },
    { ADD_ATTR_RESULT_HPADD,				"随时间HP回复（万分比/秒）" },
    { ADD_ATTR_RESULT_MPADD,				"随时间MP回复（万分比/秒）" },
    { ADD_ATTR_RESULT_MIN_PHSIC_DEF, 		"物防小幅上升" },
    { ADD_ATTR_RESULT_MIN_ICE_DEF,			"魔防小幅上升" },
    { ADD_ATTR_RESULT_FIGHTVAL, 			"战斗力" },
    { ADD_ATTR_RESULT_FLEE, 				"闪避率" },
    { ADD_ATTR_RESULT_SKILL_LEVEL,			"增加技能等级" },
    { ADD_ATTR_RESULT_BADSTATUS_RATE,		"施放不良状态成功率" },
    { ADD_ATTR_RESULT_BADSTATUS_DEF, 		"抵抗不良状态" },
    { ADD_ATTR_RESULT_BADSTATUS_DEC_TIME,	"不良状态持续时间下降" },
    { ADD_ATTR_RESULT_HEAVY_HIT, 			"暴击率" },
    { ADD_ATTR_RESULT_HITRATE,				"命中率" },
    { ADD_ATTR_RESULT_TASK_ITEM_RATE, 		"任务物品掉落率" },
    { ADD_ATTR_RESULT_STUFF_ITEM_RATE,		"材料掉落率" },
    { ADD_ATTR_RESULT_EGG_RATE, 			"宠物蛋掉落率" },
    { ADD_ATTR_RESULT_ADD_BULL, 			"增加"..MONEY_TYPE_BULL_NAME.."掉落率" },
    { ADD_ATTR_RESULT_MIN_EXP,				"战斗小幅增加经验" },
    { ADD_ATTR_RESULT_ADD_PET_EXP,			"战斗宠物增加经验值获得" },
    { ADD_ATTR_RESULT_ADD_SKILLPOINT,		"增加技能点获得" },
	{ ADD_ATTR_RESULT_FIGHT_HPADD,			"HP回复值战斗状态" },
    { ADD_ATTR_RESULT_FIGHT_MPADD,			"MP回复值战斗状态" },
    { ADD_ATTR_RESULT_HITRATE_RATE,			"命中率" },
    { ADD_ATTR_RESULT_PHSIC_ATK,			"物理攻击力上升" },
    { ADD_ATTR_RESULT_ICE_ATK,				"魔法攻击力上升" },
    { ADD_ATTR_RESULT_HEAVY_HIT_VAL,		"暴击伤害" },
    { ADD_ATTR_RESULT_HEAl_HP_MUL,			"治疗加成" },
};

-- 此为用于去除字符串前面和后面空格字符使用的
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

-- 此为用鼠标滚动滚动条
function ScrollBar_OnMouseWheel()
	if not this:IsShown() then
		return;
	end

	local nScrollLine	= arg1;
	local absChange		= math.abs( arg1 );
	-- 若向上滚动
	if arg1 > 0 then
		local nValue = this:GetValue() - absChange * this:GetValueStep();
		if nValue >= 0 then
			this:SetValue( nValue );
		else
			this:SetValue( 0 );
		end
	-- 若向下滚动
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
--@Desc:关闭当前窗口
function CloseCurFrame( szCloseFrame )
	local Frame = getglobal( szCloseFrame );
	if Frame:IsShown() then
		Frame:Hide();
	end
end

--@Desc:隐藏所有高亮条
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

--@Desc:设置Font控件文本
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

--@Desc:设置Font控件文本
function SetRichText( szFrame, nValue, t_Color )
	local font = getglobal(szFrame);
	font:SetText( nValue, t_Color[1], t_Color[2], t_Color[3] );
end

--@Desc:设置职业的Font控件文本
function SetCareeFontText( szFrame, nValue, t_Color )
	local font = getglobal(szFrame);
	font:SetText(GetCareerName(nValue));
	font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
end

--@Desc:设置官职的Font控件文本
function SetPositionFontText( szFrame, nValue, t_Color )
	local font = getglobal(szFrame);
	font:SetText(GetPosition(nValue));
	font:SetTextColor(t_Color[1], t_Color[2], t_Color[3]);
end

--@Desc:设置Texture控件（显示物品图标）
function SetTexture( szTextureName, nIconID, t_Color )
	local texture = getglobal( szTextureName );
	assert( texture ~= nil, "非法数据!" .. szTextureName );

	local IconPath = GetItemEquipIconPath()..nIconID..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	texture:SetTexture( IconPath );

	texture:Show();
end

function SetBoxTexture( szTextureName )
	local texture = getglobal( szTextureName );
	assert( texture ~= nil, "非法数据!" .. szTextureName );
	texture:Show();
end

--@Desc:设置邮件状态图标的函数
function SetMailState(  szFrame, nValue, t_Color  )
	local texture = getglobal(szFrame);
	local button  = getglobal( texture:GetParent() );
	texture:Show();
	if bitAnd( nValue,MAIL_FLAG_SYSTEM ) then 
		button:SetClientString("系统邮件");
		if bitAnd( nValue,MAIL_FLAG_READ ) then
			local IconPath = GetItemEquipIconPath().."5500003.tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			texture:SetTexture( IconPath );
			
		else
			local IconPath = GetItemEquipIconPath().."5500004.tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			texture:SetTexture( IconPath );
		end
	else
		button:SetClientString("玩家邮件");
		if bitAnd( nValue,MAIL_FLAG_READ ) then
			local IconPath = GetItemEquipIconPath().."5500002.tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			texture:SetTexture( IconPath );
		else
			local IconPath = GetItemEquipIconPath().."5500001.tga";
			-- 如果没找到物品的图标则使用默认图标显示
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

--@Desc:玩家在公会中的职位
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

--@Desc:是否是会长
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
POSE_NONE					= 10; --// 空手
--// 战士
POSE_SHORTWEAPON_R		= 20; --// 左盾牌右短武器
POSE_WEAPON_SHORTWEAPON	= 24; --// 左右手短武器
--// 法师
POSE_WAND				= 30; --// 法杖+法器
--// 牧师
POSE_CHURCH_WAND		= 32; --// 法杖+法器
--// 枪手
POSE_GUN_R				= 40; --// 单手枪
POSE_SCATTERGUN			= 42; --// 双手机枪
POSE_DOUBLEGUN			= 47; --// 对手枪
POSE_END				= 48;

ARM_FIGHTER				= 10; --//  战士武器
ARM_WIZARD				= 11; --//  法师武器
ARM_GUN					= 12; --//  枪手-枪
ARM_CANNON				= 13; --//  枪手-炮
ARM_TEMP					= 14; --//  临时武器
ARM_CHURCH				= 15; --//  牧师武器
ARM_RACE_SECONDARY		= 11;	--生活技能
-----------------------------------------------------------------------------
t_posestate =
{
--左手 Id/100000	Id/10000				姿态					类型 休闲1 战备8
-- 战士
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

-- 法师
-- 110
	{ ARM_WIZARD,	ARM_MAGIC_STAFF_ICE,	POSE_WAND,				0,		3},
-- 111
	{ ARM_WIZARD,	ARM_MAGIC_STAFF_FIRE,	POSE_WAND,				0,		3},
-- 118
	{ ARM_WIZARD,	ARM_MAGIC_FAQI,			POSE_WAND,				0,		3},
	

--右手 Id/100000	Id/10000				姿态
-- 枪手
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
	
	-- 祭司
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
	writeFile:write( "【"..os.date().."】".. szLogMsg.."\n");
	writeFile:close();--]]
end

-- 鼠标点击待修改信息时响应
function EmotionShowBtn_OnClick()
	local showbtn			= getglobal( this:GetParent().."_EmotionShowBtn");
	local emotionedit		= getglobal( this:GetParent().."_EmotionEdit");
	showbtn:Hide();
	emotionedit:Show();
	SetFocusFrame( this:GetParent().."_EmotionEdit" );
	emotionedit:SelectAllText();
end

-- 编辑信息时输入响应
function EmotionEdit_OnChar()
	local emotionshowtex = getglobal( this:GetParent().."_EmotionShowTex");
	emotionshowtex:Hide();
end

-- 按回车等失去焦点时响应
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
		["#Race#"]		= GetCareerName(MainPlayer:getRace()),					--/*   Ver.6 图标id */
		["#b"]			= "\b",				
		["#t"]			= "\t",
		["#Gender1#"]	= MainPlayer:getGender() == CLIENT_GENDER_MALE and "哥哥" or "姐姐",
		["#Gender2#"]	= MainPlayer:getGender() == CLIENT_GENDER_MALE and "帅哥" or "美女",
		["#Gender3#"]	= MainPlayer:getGender() == CLIENT_GENDER_MALE and "大爷" or "姑娘"
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

-- 是否在指定控件的显示区域内
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
		{ "药水",			5000000,	5007199, "sounds\\UI\\shiyongyaoshui.wav" },
		{ "食物",			5010000,	5011011, "" },
		{ "卷轴、技能书",	5040001,	5041000, "sounds\\UI\\shiyongjuanzou.wav" },
		{ "辅助类",			5050000,	5050004, "sounds\\UI\\shiyongbaoshi.wav" },
		{ "蒸汽科技",		5051000,	5051999, "" },
		{ "活动道具",		5052000,	5052999, "" },
		{ "兑换物",			5060001,	5060003, "" },
		{ "杂物",			5061000,	5061007, "" },
		{ "坐骑",			5600000,	5609999, "" },
		{ "宠物蛋",			5610000,	5619999, "" },
		{ "精灵魂器",		5620000,	5629999, "" },
		{ "装备改造",		5200000,	5202111, "" },
		{ "装备宝石",		5210000,	5219170, "sounds\\UI\\shiyongbaoshi.wav" },
		{ "宠物改造",		5230000,	5230005, "" },
		{ "功能道具",		5250000,	5251002, "" },
		{ "任务",			5300000,	5310000, "" },
		{ "委托",			5061200,	5061249, "" },
		{ "宠物技能书",		5231000,	5231999, "" },
		{ "称号",		5043001,	5043999, "" },
	}

function PlayItemSound( id )
	for i = 1, table.getn(t_ItemTypes) do
		if id >= t_ItemTypes[i][2] and id <= t_ItemTypes[i][3] and t_ItemTypes[i][4] ~= "" then
			playUISound( t_ItemTypes[i][4] );
		end
	end
end

-- 获取物品类型
function GetItemTypeById( id )
	for i = 1, table.getn(t_ItemTypes) do
		if id >= t_ItemTypes[i][2] and id <= t_ItemTypes[i][3] then
			return t_ItemTypes[i][1];
		end
	end
	return "其它";
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
			[1] = { msg = "绑定物品", ["mail"] = "不可邮寄", ["stall"] = "不可上架", ["exchange"] = "不可交易", ["other"] = "不可流通" },
			[2] = { msg = "委托物品", ["mail"] = "不可邮寄", ["stall"] = "不可上架", ["exchange"] = "不可交易", ["other"] = "不可流通" },
			[3] = { msg = "非流通物品", ["mail"] = "不可邮寄", ["stall"] = "不可上架", ["exchange"] = "不可交易", ["other"] = "不可流通" },
		}
	return t_ExChangeErrorMsg[nError].msg..t_ExChangeErrorMsg[nError][szFrom];
end

-- 写入 LUA LOG 文件
function LUALOGINFO( szLogMsg )
	local writeFile =  assert(io.open("log/lua.log","a+"));
	writeFile:write( "【"..os.date().."】".. szLogMsg.."\n");
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

-- 隐藏指引提示TIPS窗口
function HideGuideEffectTip( szExceptFrame ) 
	-- 在szExceptFrame为nil的时候隐藏所有的tips面板
	-- 在szExceptFrame不为nil的时候将与指定tips面板不同的面板隐藏
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

-- 锁定当前物品
function LockCurItem( item, szParent )
	if item:getItemId() ~= 0 then
		LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), szParent ); -- 如果物品添加成功则锁住该物品
	end
end

function isMainPlayerInWeixing()
	return GameFollowManager:isWeiXing();
end

local tSuitNameColor = {
							{130,255,135},	-- 浅绿
							{160,255,255},	-- 浅蓝
							{255,185,255},	-- 粉红
							{255,115,0  },  -- 橙色
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
		ShowMidTips( "请在" .. szName .. "使用该物品！" )
	else
		ShowMidTips( "请在" .. szName .. "的指定区域使用该物品！" )
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