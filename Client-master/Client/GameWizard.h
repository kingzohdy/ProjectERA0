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
	GAMEWIZARD_CLOSEALL				= 1,		// 跳过新手提示		
	GAMEWIZARD_FIRST_UI				= 2,		// 第一个UI
	GAMEWIZARD_FIRST_TALK			= 4,		// 第一次对话
	GAMEWIZARD_FIRST_GETMISSION		= 8,		// 第一次任务领取
	GAMEWIZARD_FIRST_LEVELUP		= 16,		// 第一次升级
	GAMEWIZARD_FIRST_GETMONEY		= 32,		// 第一次获得金钱
	GAMEWIZARD_FIRST_GOCOPY			= 64,		// 第一次下副本
	GAMEWIZARD_FIRST_KILLMONSTER	= 128,		// 第一次杀怪

	GAMEWIZARD_FIRST_USEDSKILL		= 1001,		// 第一次使用技能
	GAMEWIZARD_FIRST_SAW_SHOULEI	= 1002,		// 第一次看到手雷
	GAMEWIZARD_FIRST_GIFT			= 1004,		// 第一个新手礼包
	GAMEWIZARD_FIRST_SAWPOWDER		= 1008,		// 第一次看到炸药桶
	GAMEWIZARD_FIRST_GETWEAPON		= 1016,		// 第一次接触武器
	GAMEWIZARD_FIRST_SAWXPITEM		= 1032,		// 第一次看到XP果
	GAMEWIZARD_FIRST_COLLECT		= 1064,		// 第一次采集
	GAMEWIZARD_FIRST_PRESENT		= 1128,		// 第一个礼品袋

	GAMEWIZARD_FIRST_STUDYSKILL		= 2002,		// 第一次学习技能
	GAMEWIZARD_FIRST_GETBOOTY		= 2004,		// 第一次拾取包裹
	GAMEWIZARD_FIRST_LOOT			= 2008,		// 第一次收集
	GAMEWIZARD_FIRST_SAWSWORD		= 2064,		// 第一次看到火焰刀
	GAMEWIZARD_FIRST_FLASHHELP		= 2128,		// 第一次FLASH指引

	GAMEWIZARD_FIRST_GETREAGENT		= 3001,		// 第一次获得药剂
	GAMEWIZARD_FIRST_FLY			= 3002,		// 第一次飞行
	GAMEWIZARD_FIRST_GETMONSETER	= 3004,		// 第一次刷怪
	GAMEWIZARD_FIRST_GETFOOD		= 3008,		// 第一次接触食物
	GAMEWIZARD_FIRST_GETPET			= 3016,		// 第一只仙子
	GAMEWIZARD_FIRST_POWERFULL		= 3032,		// 第一次XP涨满
	GAMEWIZARD_FIRST_SPECIALMISSION	= 3064,		// 第一次特殊行为
	GAMEWIZARD_FIRST_LIMITMISSION	= 3128,		// 第一次限时任务

	GAMEWIZARD_FIRST_SAW_JIJIASHOU	= 4001,		// 第一次看到临时机械
	GAMEWIZARD_FIRST_MACHINEPOWER	= 4002,		// 第一次看到临时机械弹药
	GAMEWIZARD_FIRST_MOVE			= 4004,		// 第一次移动	
	GAMEWIZARD_FIRST_ROTATE			= 4008,		// 第一次旋转视角
	GAMEWIZARD_FIRST_COMMMISSION	= 4016,		// 第一次提交任务
	GAMEWIZARD_FIRST_GETSKILLBOOK	= 4032,		// 第一次获取技能书
	GAMEWIZARD_SETSHORTCUT			= 4064,		// 第一次进游戏设置快捷键
	GAMEWIZARD_FIRST_OPENCONTAINER	= 4128,		// 第一次打开背包

	GAMEWIZARD_FIRST_OPENFRIEND		= 5001,		// 第一次打开好友界面
	GAMEWIZARD_FIRST_OPENEQUIP		= 5002,		// 第一次打开人物界面
	GAMEWIZARD_FIRST_OPENPET		= 5004,		// 第一次打开宠物界面
	GAMEWIZARD_FIRST_OPENRIDE		= 5008,		// 第一次打开坐骑界面
	GAMEWIZARD_FIRST_OPENTASK		= 5016,		// 第一次打开任务界面
	GAMEWIZARD_FIRST_OPENSTASH		= 5032,		// 第一次打开仓库界面
	GAMEWIZARD_FIRST_OPENMAIL		= 5064,		// 第一次打开邮件界面
	GAMEWIZARD_FIRST_OPENSHOP		= 5128,		// 第一次打开商店界面

	GAMEWIZARD_FIRST_OPENTRUST		= 6001,		// 第一次打开委托界面
	GAMEWIZARD_FIRST_OPENTRADE		= 6002,		// 第一次打开交易界面
	GAMEWIZARD_FIRST_OPENTLEARN		= 6004,		// 第一次打开技能学习界面
	GAMEWIZARD_FIRST_GETPETACTOR	= 6008,		// 第一次获得宠物
	GAMEWIZARD_FIRST_USEPETEGG		= 6016,		// 第一次使用宠物蛋

	GAMEWIZARD_FIRST_SEELEADER		= 6032,		// 第一次见到村长
	GAMEWIZARD_FIRST_SEETEACHER		= 6064,		// 第一次见到技能学习员
	GAMEWIZARD_FIRST_SEETEACHER2	= 8004,		// 第一次见到技能学习员
	GAMEWIZARD_FIRST_SEEMEDICASALER	= 6128,		// 第一次见到药品商人

	GAMEWIZARD_FIRST_SEECLANMANAGER	= 7001,		// 第一次见到公会管理员
	GAMEWIZARD_FIRST_SEEACTIMANAGER	= 7002,		// 第一次见到活动管理员
	GAMEWIZARD_FIRST_SEERIDESALER	= 7004,		// 第一次见到坐骑商人
	GAMEWIZARD_FIRST_SEEPETSALER	= 7008,		// 第一次见到宠物商人
	GAMEWIZARD_FIRST_SEEWEAPONSALER	= 7016,		// 第一次见到武器商人
	GAMEWIZARD_FIRST_SEEDEFANDSALER	= 7032,		// 第一次见到防御商人
	GAMEWIZARD_FIRST_SEEARMPROCER	= 7064,		// 第一次见到装备加工员
	GAMEWIZARD_FIRST_SEETRANSPORTER	= 7128,		// 第一次见到传送员

	GAMEWIZARD_FIRST_HAVEWEAPON		= 8001,		// 第一次领到武器
	GAMEWIZARD_FIRST_HAVERIDE		= 8002,		// 第一次装备上坐骑
	GAMEWIZARD_FIRST_SAWTASKLINK	= 8004,		// 第一次看到任务超链接
	GAMEWIZARD_FIRST_GETHP			= 8008,		// 第一次获得红药
	GAMEWIZARD_FIRST_GETMP			= 8016,		// 第一次获得蓝药
	GAMEWIZARD_FIRST_GETXIAN_ITEM	= 8032,		// 第一次获得精灵
	GAMEWIZARD_FIRST_HAVE_JING_LING	= 8064,		// 第一次使用精灵
	GAMEWIZARD_FIRST_OPENSKILL		= 8128,		// 第一次打开商店界面

	GAMEWIZARD_FIRST_EQUIPWEAPON	= 9001,		// TODO: 第一次装备武器
	GAMEWIZARD_FIRST_EQUIPCLOSE		= 9002,		// 第一次装备衣服
	GAMEWIZARD_FIRST_HAVECLOSE		= 9004,		// 第一次领到衣服
	GAMEWIZARD_FIRST_SHOPPING		= 9008,		// 第一次购买
	GAMEWIZARD_FIRST_USEHP			= 9016,		// 第一次使用红药
	GAMEWIZARD_FIRST_USEMP			= 9032,		// 第一次使用蓝药
	GAMEWIZARD_FIRST_LEARNSKILL		= 9064,		// 第一次学习技能
	GAMEWIZARD_FIRST_ENTERWORLD		= 9128,		// 第一次进入游戏

	GAMEWIZARD_FIRST_LOGINTIMECITY	= 10001,	// 第一次进时间城（非新手指引务删）
	GAMEWIZARD_FIRST_SETGAMEOPT		= 10002,	// 第一次进游戏设置游戏
	GAMEWIZARD_FIRST_GETGENIUSOPT	= 10004,	// 第一次获得天赋
	GAMEWIZARD_FIRST_GETQUEST_FROMUI	= 10008,	// 第一次从面板上面接受任务
	GAMEWIZARD_FIRST_FINISHQUEST_FROMUI	= 10016,	// 第一次从面板上完成任务
	GAMEWIZARD_FIRST_FIGHT				= 10032,	// 第一次战斗
	GAMEWIZARD_FIRST_USE_SWORD			= 10064,	// 第一次使用火焰刀
	GAMEWIZARD_FIRST_USEWEAPONITEM		= 10128,	// 第一次使用武器

	GAMEWIZARD_FIRST_EQUIP_TO_EQUIP_FRAME= 11001,	// 第一次装备了武器后显示到人物面板
	GAMEWIZARD_FIRST_PICKSWORD			= 11002,	// 第一次拾取火焰刀
	GAMEWIZARD_FIRST_PICKMACHINE		= 11004,	// 第一次拾取临时机械
	GAMEWIZARD_FIRST_USE_TMPMACHINE		= 11008,	// 第一次使用临时机械
	GAMEWIZARD_FIRST_SAWPAOTAI			= 11016,	// 第一次看到炮台
	GAMEWIZARD_FIRST_PICKPAOTAI			= 11032,	// 第一次拾取炮台
	GAMEWIZARD_FIRST_USE_TPAOTAI		= 11064,	// 第一次使用炮台
	GAMEWIZARD_FIRST_PICKSHOULEI		= 11128,	// 第一次拾取手雷
	GAMEWIZARD_FIRST_USE_TSHOULEI		= 12001,	// 第一次使用临时机械
	GAMEWIZARD_FIRST_SAW_ZHUCHENG_TRAN	= 12002,	// 第一次看到传送到时间城的大使
	GAMEWIZARD_FIRST_TALK_ZHUCHENG_TRAN	= 12004,	// 第一次与传送到时间城的大使对话
	GAMEWIZARD_FIRST_TRAN_TO_SKY_COPY	= 12008,	// 第一次与传送到天空岛副本
	GAMEWIZARD_FIRST_FLY_QUEST			= 12016,	// 第一次飞行的任务获取
	GAMEWIZARD_FIRST_CLICK_PET_ZHONG_CHENG_BTN_WHEN_LOW= 12032,	// 在第一次宠物忠诚度太低的时候点击驯服按钮
	GAMEWIZARD_FIRST_PET_FIGHT			= 12064,	// 第一次宠物出征
	GAMEWIZARD_FIRST_PET_HP_LOW			= 12128,	// 第一次宠物血量太低
	GAMEWIZARD_FIRST_BLINK_PET_BTN_WHEN_LOW= 13001,	// 在第一次宠物忠诚度太低的闪烁快捷栏宠物按钮
	GAMEWIZARD_FIRST_SELF_LEVEL_UP		= 13002,	// 第一次自己手动升级
	GAMEWIZARD_FIRST_TALK_TO_SKY_COPY_NPC= 13004,	// 在接到传送到天空岛副本任务后与NPC的第一次对话
	GAMEWIZARD_FIRST_TALK_TO_FLY_QUEST_NPC= 13008,	// 在接到飞行的任务后与NPC的第一次对话
	GAMEWIZARD_FIRST_CAMP_TRUMP	= 13016,	// 第一次出征精灵
	GAMEWIZARD_FIRST_GETRIDE_ITEM		= 13032,		// 第一次获得骑乘物品
	GAMEWIZARD_FIRST_BLINK_CON_STRENGTHEN_RIDE_ITEM_BTN= 13064,		// 第一次让背包中强化骑乘的道具闪烁
	GAMEWIZARD_FIRST_BLINK_STRENGTHEN_BTN	= 13128,		// 第一次强化骑乘	
	GAMEWIZARD_FIRST_PUT_STRENGTHEN_RIDE_TO_FRAME= 14001,	// 第一次将骑乘升级物品放到升级面板
	GAMEWIZARD_FIRST_SEE_COPY_TRAN_DOOR		= 14002,	// 第一次看到副本传送门	
	GAMEWIZARD_FIRST_TALK_COPY_TRAN_DOOR	= 14004,	// 第一次与副本传送门对话	
	GAMEWIZARD_FIRST_GET_CHENG_HAO			= 14008,	// 第一次获得称号	

	GAMEWIZARD_FIRST_SWORD_PWORLD_ANILM		= 14016,	// 第一次看到火焰刀副本动画
	GAMEWIZARD_FIRST_PAOTAI_PWORLD_ANILM	= 14032,	// 第一次看到炮台副本动画
	GAMEWIZARD_FIRST_SHOULEI_PWORLD_ANILM	= 14064,	// 第一次看到手雷副本动画
	GAMEWIZARD_FIRST_JIJIA_PWORLD_ANILM		= 14128,	// 第一次看到机甲兽副本动画
	GAMEWIZARD_FIRST_CURE_NPC				= 15001,	// 第一次救治飞龙
	GAMEWIZARD_FIRST_CURE_NPC_AFTER_TALK	= 15002,	// 在与飞龙对话后第一次救治飞龙
	GAMEWIZARD_FIRST_OPEN_BUY_CONFIRM		= 15004,	// 第一次点击确认购买面板
	GAMEWIZARD_SECOND_SELF_LEVEL_UP			= 15008,	// 第二次自己手动升级
	GAMEWIZARD_FIRST_BLINK_MAIN_BTN_GET_CHENG_HAO= 15016,	// 第一次获得称号让主角按钮闪动
	GAMEWIZARD_FIRST_BLINK_CHANRACTER_FRAME_LABLE_BTN_GET_CHENG_HAO	= 15032,	// 第一次获得称号让主角面板的称号标签闪烁
	GAMEWIZARD_FIRST_BLINK_CHENG_HAO_BTN_IN_CHENG_HAO_FRAME_GET_CHENG_HAO= 15064,// 第一次获得称号让称号面板的使用称号按钮闪烁
	GAMEWIZARD_SECOND_SAWSWORD				= 15128,	// 第二次看到火焰刀
	GAMEWIZARD_SECOND_USE_SWORD				= 16001,	// 第二次使用火焰刀
	GAMEWIZARD_FIRST_BLINK_LIAN_HUA_LABEL	= 16002,	// 第一次让炼化标签页闪烁
	GAMEWIZARD_FIRST_REPAIR					= 16004,	// 任务 1081 全部修理
	GAMEWIZARD_FIRST_BLINK_LIAN_HUA_CONFIRM_ZHU_RU_BTN	= 16008,	// 第一次让确认注入经验按钮闪烁
	GAMEWIZARD_FIRST_BLINK_LIAN_HUA_FRAME_LIAN_HUA_BTN	= 16016,	// 第一次让炼化按钮闪烁
	GAMEWIZARD_FIRST_EQUIP_NOT_ENOUGH_DUR	= 16032,	// 第一次提示装备耐久度不够
	GAMEWIZARD_FIRST_BLINK_REPAIR_ALL_BTN	= 16064,	// 第一次闪烁击全部修理按钮
	GAMEWIZARD_FIRST_BLINK_CONFIRM_REPAIR_BTN= 16128,	// 第一次确定修理的修理按钮
	GAMEWIZARD_SECOND_PICKSWORD				= 17001,	// 第二次拾取火焰刀
	GAMEWIZARD_FIRST_BLINK_SHORTCUT_RIDE_BTN= 17002,		// 第一次闪烁快捷栏骑乘按钮

	GAMEWIZARD_FIRST_BLINK_NPCTALK_LEARN_PET_SKILL_OPTION	= 17004,	// 第一次闪烁NPC对话面板上面的宠物技能学习选项
	GAMEWIZARD_FIRST_BLINK_CONTAINER_LEARN_PET_SKILL_BOOK_BTN= 17008,	// 第一次闪烁背包面板上面的宠物技能学习书图标
	GAMEWIZARD_FIRST_BLINK_PETSKILLFRAME_SKILL_GRID_BTN		= 17016,	// 第一次闪烁宠物技能学习面板上面的技能格子
	GAMEWIZARD_FIRST_BLINK_PETSKILLFRAME_LEARN_SKILL_BTN	= 17032,	// 第一次闪烁宠物技能学习面板上面的学习按钮

	GAMEWIZARD_FIRST_BLINK_NPCTALK_STRENGTHEN_PET_OPTION	= 17064,	// 第一次闪烁NPC对话面板上面的宠物技能强化选项
	GAMEWIZARD_FIRST_BLINK_CONTAINER_STRENGTHEN_ITEM_BTN	= 17128,	// 第一次闪烁背包面板上面的宠物强化石图标
	GAMEWIZARD_FIRST_BLINK_PESTRENGTHENFRAME_STRENGTHEN_BTN	= 18001,	// 第一次闪烁宠物强化面板上面的强化按钮
	
	GAMEWIZARD_FIRST_OPEN_CONTAINERFRAME_FIRST_GET_ADD_PET_EXP_ITEM	= 18002,// 第一次获得宠物经验果的时候打开背包
	GAMEWIZARD_FIRST_BLINK_CONTAINER_ADD_PET_EXP_ITEM_BTN	= 18004,	// 第一次闪烁背包面板上面的宠物经验果图标

	GAMEWIZARD_FIRST_BLINK_NPCTALK_LEVEL_UP_BAO_SHI_OPTION	= 18008,	// 第一次闪烁NPC对话面板上面的宝石升级选项
	GAMEWIZARD_FIRST_BLINK_LEVELUPBAOSHIFRAME_ITEM_BTN		= 18016,	// 第一次宝石升级面板上面的装备按钮
	GAMEWIZARD_FIRST_BLINK_LEVELUPBAOSHIFRAME_STUFF_BTN		= 18032,	// 第一次宝石升级面板上面的材料按钮
	GAMEWIZARD_FIRST_BLINK_LEVELUPBAOSHIFRAME_QIANGHUA_BTN	= 18064,	// 第一次宝石升级面板上面的强化按钮

	GAMEWIZARD_FIRST_BLINK_NPCTALK_XIANGQIAN_BAO_SHI_OPTION	= 18128,	// 第一次闪烁NPC对话面板上面的镶嵌宝石选项
	GAMEWIZARD_FIRST_BLINK_XIANGQIANBAOSHIFRAME_ITEM_BTN	= 19001,	// 第一次闪烁NPC第一次镶嵌宝石上面的装备按钮
	GAMEWIZARD_FIRST_BLINK_XIANGQIANBAOSHIFRAME_BAO_SHI_STUFF_BTN= 19002,	// 第一次闪烁NPC第一次镶嵌宝石上面的宝石材料按钮
	GAMEWIZARD_FIRST_BLINK_XIANGQIANBAOSHIFRAME_INSURE_STUFF_BTN= 19004,	// 第一次闪烁NPC第一次镶嵌宝石上面的镶嵌符按钮
	GAMEWIZARD_FIRST_BLINK_XIANGQIANBAOSHIFRAME_XIANGQIAN_BTN	= 19008,	// 第一次闪烁NPC第一次镶嵌宝石上面的镶嵌按钮

	GAMEWIZARD_FIRST_BLINK_CONTAINERFRAME_BOOTY_ITEM_BTN	= 19016,	// 第一次闪烁背包中由拾取东西而获取到的物品

	GAMEWIZARD_FIRST_BLINK_EXIST_WAIT_BTN			= 19032,/// 生存大考验第一次报名
	GAMEWIZARD_FIRST_BLINK_EXIST_INVITE_BTN			= 19064,/// 生存大考验第一次收到邀请
	GAMEWIZARD_FIRST_BLINK_EXIST_SCORE_BTN			= 19128,/// 生存大考验第一次进入
	GAMEWIZARD_FIRST_BLINK_BCT_PROCESS_BTN			= 20001,/// 百层塔第一次进入
	GAMEWIZARD_FIRST_BLINK_BCT_FINISH_BTN			= 20002,/// 百层塔第一次过关

	GAMEWIZARD_FIRST_BLINK_NPCTALK_ENTER_FUBEN_OPTION	= 20004,	// 第一次闪烁NPC对话面板上面的进入副本选项

	GAMEWIZARD_FIRST_BLINK_NPCTALK_STUDY_SKILL_OPTION	= 20008,	// 第一次闪烁NPC对话面板上面的学习技能选项
	GAMEWIZARD_FIRST_BLINK_NPCLEARNSKILL_STUDY_BTN		= 20016,	// 第一次闪烁技能学习面板的学习按钮


	GAMEWIZARD_FIRST_POP_SWORD_XINSHOU_HELP_BTN			= 20032,		// 弹出第一个火焰刀的新手提示
	GAMEWIZARD_SECOND_POP_SWORD_XINSHOU_HELP_BTN		= 20064,		// 弹出第二个火焰刀的新手提示
	GAMEWIZARD_FIRST_POP_JIJIA_XINSHOU_HELP_BTN			= 20128,		// 弹出第一个机甲兽的新手提示
	GAMEWIZARD_FIRST_POP_SHOULEI_XINSHOU_HELP_BTN		= 21001,		// 弹出第一个手雷的新手提示
	GAMEWIZARD_FIRST_POP_PAOTAI_XINSHOU_HELP_BTN		= 21002,		// 弹出第一个炮台的新手提示

	GAMEWIZARD_FIRST_POP_MOUSE_HABIT_XINSHOU_FRAME		= 21004,		// 弹出第一个修改鼠标习惯的新手提示


	GAMEWIZARD_FIRST_SEE_STUDY_SKILL_NPC				= 21008,		// 第一次看到技能学习NPC
	GAMEWIZARD_FIRST_BEGIN_LIAN_HUA						= 21016,		// 第一次炼化精灵

	GAMEWIZARD_FIRST_JIU_ZHI_DENG_LONG_CAO				= 21032,		// 第一次救治蓝色灯笼草
	GAMEWIZARD_FIRST_ADD_TIAN_FU						= 21064,		// 第一次加天赋

	GAMEWIZARD_FIRST_BLINK_PET_EGG_CONTAINER_BTN		= 21128,		// 第一次闪烁宠物蛋的背包按钮
	GAMEWIZARD_FIRST_GET_PET_BLINK_PET_SHORTCUT_BTN		= 22001,		// 第一次获得宠物闪烁宠物快捷键

	GAMEWIZARD_FIRST_LEVEL_UP_TRUMP						= 22002,		// 第一次升级精灵成功

	GAMEWIZARD_FIRST_BLINK_BCT_FAIL_BTN					= 22004,				/// 百层塔第一次失败
	GAMEWIZARD_FIRST_FIRST_SET_PET_USE_ITEM				= 22008,				/// 第一次设置宠物使用药品规则

	GAMEWIZARD_FIRST_CLOSE_SELF_LEVEL_UP				= 22016,	// 第一次S自己手动关闭升级提示
	GAMEWIZARD_SECOND_CLOSE_SELF_LEVEL_UP				= 22032,	// 第二次自己手动关闭手动升级提示
	GAMEWIZARD_FIRST_COMMIT_MAIL						= 22064,	// 第一次提交邮件
	GAMEWIZARD_FIRST_CLOSE_PET_ZHONG_CHENG_BTN_TIPS		= 22128,	// 第一次手动关闭驯服宠物新手指引tips

	GAMEWIZARD_FIRST_BLINK_AUTO_WEI_YANG_BTN			= 23001,	// 第一次闪烁自动喂养按钮
	GAMEWIZARD_FIRST_USE_NEW_SKILL						= 23002,	// 第一次使用新获得的技能
	GAMEWIZARD_FIRST_SET_CHAT_PING_BI					= 23004,	// 第一次设置频道屏蔽
	
	GAMEWIZARD_FIST_TRUMP_QUEST_XP_FULL					= 23008,	//第一次精灵任务XP满
	GAMEWIZARD_FIST_TRUMP_QUEST_FAIRY_MORPH				= 23016,	//第一次精灵任务精灵变身
	GAMEWIZARD_FIRST_SET_TUAN_DUICHAT_PING_BI			= 23032,	// 第一次设置团队频道屏蔽
	GAMEWIZARD_FIRST_SET_NEW_CHAT_PING_BI				= 23064,	// 第一次设置新的策划频道屏蔽

	GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_MAP                = 24001,    // 第一次打开公会地图
	GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_TECH               = 24002,    // 第一次打开公会科技
	GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_PRODUCT            = 24004,    // 第一次打开公会制造
	GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_LEARN_TECH         = 24008,     // 第一次学习科技

	GAMEWIZARD_FIRST_OPEN_SKILL_RUNE					= 24016,    // 第一次打开技能符文
	GAMEWIZARD_FIRST_BLINK_JIJIASHOU_CONTAINER_BTN		= 24032,	// 第一次获得机甲兽
	GAMEWIZARD_FIRST_USE_PET_HETI						= 24064,	// 第一次使用宠物合体
	GAMEWIZARD_FIRST_GET_BROKEN_JIJIASHOU_STEP1			= 24128,	// 第一获得损坏的机甲兽
	GAMEWIZARD_FIRST_GET_BROKEN_JIJIASHOU_STEP2			= 25001,	// 第一获得损坏的机甲兽
	GAMEWIZARD_FIRST_TRUMP_LEVEL_UP						= 25002,	// 第一次升级精灵
	GAME_WIZARD_FIRST_TRUMP_STAR_UP						= 25004,	// 第一次强化精灵
	GAME_WIZARD_FIRST_TRUMP_KEYIN						= 25008,	// 第一次精灵刻印
	GAMEWIZARD_FIRST_BLINK_GET_SPAR_CONTAINER_BTN		= 25016,	// 第一次获得晶石

	GAMEWIZARD_FIRST_CHANGE_PET_MODE					= 25032,	// 第一次变更宠物模式
	GAMEWIZARD_FIRST_SPARFRAME_SHOW						= 25064,	// 第一次打开能源界面
	GAMEWIZARD_FIRST_IN_TEAM							= 25128,	// 第一次进行组队
	GAMEWIZARD_FIRST_IN_AUTO_ASSIST						= 26001,	// 第一次打开挂机面板
	GAMEWIZARD_FIRST_GET_PK_TIPS						= 26002,	// 第一次显示PK提示
	GAMEWIZARD_FIRST_GET_RUNE_CONTAINER_BTN				= 26004,	// 第一次获得符文
	GAMEWIZARD_FIRST_OPEN_RUNE_RESET					= 26008,	// 第一次打开符文重置面板
	GAMEWIZARD_FIRST_ACCEPT_TASK_TO_WORLD_MAP_SEND		= 26016,	// 接受任务第一次乘坐急你所急航空
	GAMEWIZARD_FIRST_EQUIP_FENGYIN						= 26032,	// 第一次装备被封印
	GAMEWIZARD_FIRST_SPAR_EQURATE						= 26064,	// 第一次晶石提炼
	GAMEWIZARD_FIRST_GET_CLAN_PWORLD					= 26128,	// 第一次显示公会驱逐活动
	GAMEWIZARD_FIRST_GET_CLAN__BANNER_GLORY				= 27001,	// 第一次显示公会战旗荣耀
	GAMEWIZARD_FIRST_SET_AUTO_QUEST_DEFAULT				= 27002,	// 第一次设置自动任务默认值
	GAMEWIZARD_FIRST_SHOW_LOCK_ATTR_WARN				= 27004,	// 第一次显示属性锁定提示
	GAMEWIZARD_FIRST_SHOW_LOCK_ATTR_SAFE				= 27008,	// 第一次显示属性保护提示
	GAMEWIZARD_FIRST_IN_VIP_ASSIST						= 27016,	// 第一次打开高级辅助
	GAMEWIZARD_FIRST_SET_CLAN_GROUP_PINGBI				= 27032,	// 第一次登录游戏强制设置战团不屏蔽
	GAMEWIZARD_FIRST_QUEST_IF_YOU_CAN					= 27064,	// 第一次任务：如果你可以
	GAMEWIZARD_FIRST_QUEST_SAVE_OTHERS					= 27128,	// 第一次任务：救人
	GAMEWIZARD_FIRST_CLAN_DART_DELIVERY					= 28001,	// 第一次公会商运
	// END 10001	
};

// 触发条件
enum
{
	WIZARD_SPRING_GETQUEST,			// 任务获取触发
	WIZARD_SPRING_QUESTCOMMIT,		// 任务提交触发
	WIZARD_SPRING_LEVELUP,			// 升级触发
	WIZARD_SPRING_OBJENTERVIEW,		// 对象进入视野触发
	WIZARD_SPRING_STUDY,			// 学习触发
	WIZARD_SPRING_OPENUI,			// 打开UI
};

#pragma pack(1)
struct tagOneDayGiftCond_ToLua
{
    unsigned char LVMin;                           	/*   最小等级 */
    unsigned char LVMax;                           	/*   最大等级 */
    unsigned char PloyNum;                         	/*   完成活动次数 */
    unsigned char OnlineCon;                       	/*   连续上线天数 */
    int OnlineTM;                        	/*  Ver.243 每天在线时间|单位:分 */
	int ContOnlineTM;                    	/*  Ver.1145 每天连续在线时间|单位:分 */
	int UserType;                        	/*  Ver.1145 用户类型|取值为GIFT_USER_TYPE */
	int ClanLevel;                       	/*  Ver.1185 公会等级 物品奖励只显示对应等级的 */
	int ClanTouch;                       	/*  Ver.1185 公会昨日摸宝次数 */
	int ClanCityLevel;                   	/*  Ver.1337 公会城市等级 */
	int TotalContri;                     	/*  Ver.1367 贡献度总数 */
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
    char Name[LVGIFT_NAME_LEN];           	/*  Ver.243 礼包名 */
    char TypeName[LVGIFT_NAME_LEN];       	/*  Ver.243 礼包类别名 */
    int GiftType;                       
    tagOneDayGiftCond_ToLua GiftCond;                       
    tagGiftItems_ToLua GiftItems[CAREER_MAX];          
    char GiftDesc[LVGIFT_DESC_LEN];           	/*  Ver.243 礼包描述 */
	int MoneyType;                       	/*  Ver.487  */
	int Money;                           	/*  Ver.487  */
	int Exp;                             	/*  Ver.1185 公会等级安定度或摸宝相关经验奖励 */
	int ClanContri;                      	/*  Ver.1185 公会贡献度奖励 */
};

struct tagLVGift_ToLua
{
    int LV;                             
    char Name[LVGIFT_NAME_LEN];           	/*  Ver.243 礼包名 */
    tagGiftItems_ToLua GiftItems[CAREER_MAX];          
    char GiftDesc[LVGIFT_DESC_LEN];           	/*  Ver.243 礼包描述 */
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
    unsigned char Lv;                    /*   领取过奖励的等级 */
};

struct tagRoleGiftInfo_ToLua
{
    tagGiftLvList_ToLua GiftLvList[MAX_LV_GIFT];         	/*   领取过的等级的奖励 */
    char OneDayNum; 
    tagGiftInfo_ToLua GiftInfoOneDay[MAX_ONEDAY_GIFT]; 	/*   已经领取过的每日奖励 */
    int OneDayOnlineTM;                  	/*  Ver.243 今天在线时间|秒 */
    int OnlineTMUpTM;                    	/*  Ver.243 今天在线时间更新的时间 */
};
struct tagCoverItem_ToLua
{
	int ItemID;                         
	int Num;                            
	int Rate;                           
};
struct tagContCoverGift_ToLua
{
	int Day;                             					/*   连续签到几天 */
    char Name[RES_NAME_LEN];              					/*   名字 */
	tagCoverItem_ToLua Items[MAX_CONT_COVER_GIFT_ITEM];		/*   奖励道具 */
};
struct tagNewRoleGift_ToLua
{
	int ID;                             
	char Name[RES_NAME_LEN];              	/*   名字 */
	int Sec;                             	/*   多少秒之后领取 */
	tagGiftItem_ToLua Items[MAX_NEW_ROLE_GIFT_ITEM];   	/*   奖励道具 */
};
enum
{
    CLIENT_GIFT_TYPE_LV = GIFT_TYPE_LV, 	        /* 等级奖励 */
    CLIENT_GIFT_TYPE_ONEDAY = GIFT_TYPE_ONEDAY, 	/* 每日奖励 */
	CLIENT_GIFT_TYPE_CONTI = GIFT_TYPE_CONTI,		/* 连续每日在线多久奖励 */
	CLIENT_GIFT_TYPE_ACCGIFT =GIFT_TYPE_ACCGIFT,	/* 特殊奖励  问卷奖励 */
	CLIENT_GIFT_TYPE_PROPGIFT = GIFT_TYPE_PROPGIFT,	/* 充值奖励 */
	CLIENT_GIFT_TYPE_CONTI_GROUP = GIFT_TYPE_CONTI_GROUP, 	/* 连续在线组奖励 */
	CLIENT_GIFT_TYPE_CDKEY = GIFT_TYPE_CDKEY,				/* CDKey 礼包 */
	CLIENT_GIFT_TYPE_CONT_COVER = GIFT_TYPE_CONT_COVER, 	/* 连续签到礼包 */
};

enum 
{
	CDKEY_FAIL_NO_TOLUA = 0, 	/* 成功 */
	CDKEY_FAIL_VAILD_TOLUA = 1, 	/* cdkey无效 */
	CDKEY_FAIL_USE_TOLUA = 2, 	/* cdkey已经使用过了 */
	CDKEY_FAIL_OVERDUE_TOLUA = 3, 	/* cdkey过期 */
	CDKEY_FAIL_DUMP_TOLUA = 4, 	/* 重复领取相同类型的礼包 */
	CDKEY_FAIL_SYS_TOLUA = 5, 	/* 系统错误 */
	CDKEY_FAIL_MANY_TOLUA = 6, 	/* 多个角色同时在使用同一个cdkey */
	CDKEY_FAIL_LEVEL_LOW_TOLUA = 7, 	/* 等级过低 */
	CDKEY_FAIL_LEVEL_HIGH_TOLUA = 8, 	/* 等级过高 */
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
	char Name[LVGIFT_NAME_LEN];           	/*   礼包名 */
	unsigned char Day;                             	/*   连续在线几天 */
	unsigned char Hour;                            	/*   每天在线几小时 */
	unsigned char IsNext;                          	/*   领完了 下一次能否再领 0-否  1-是 */
	tagGiftItems_ToLua GiftItems[CAREER_MAX];          
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   礼包描述 */
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
	int Type;                            	/*   礼包类型4-特殊礼包(称号,熊猫等) 5-问卷礼包 */
	int Flag;                            	/*   此字段不用填写, 由服务器写入 */
	char Name[LVGIFT_NAME_LEN];           	/*   礼包名 */
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   礼包描述 */
	tagGiftItem_ToLua GiftItems[MAX_ACC_GIFT_LIST];   
	tdr_datetime_t SetGiftTime;                     	/*  Ver.1221 发放时间 */
	tdr_datetime_t GetGiftTime;                     	/*  Ver.1221 领取时间 */
};

struct tagAccGiftInfo_ToLua
{
	int Count;                          
	tagAccGift_ToLua AccGifts[MAX_ACC_GIFT];
};

/* 积分送礼包定义 */
struct tagPropsDef_ToLua
{
	unsigned int GroupID;                         	/*   组ID|同一个日期阶段为一组|组id需要唯一(不能与旧或已失效的ID相同) */
	tdr_ulonglong CashUp;                          	/*   冲值上限,例如冲值20000点券才能领这个礼包 */
	char BeginDate[DB_TIME_LEN];          	/*   冲值的时间段定义|开始日期 */
	char EndDate[DB_TIME_LEN];            	/*   冲值的时间段定义|结束日期 20110620到20110621表示2天有效 */
	char Name[32];                        	/*   名称|同一组中,只需要在最小的CashUp行中填写即可 */
	int IconID;                          	/*  Ver.1930 礼包图标 */
	char Title[64];                       	/*   标题|同一组中,只需要在最小的CashUp行中填写即可 */
	char DesHead[512];                    	/*   描述头部|同一组中,只需要在最小的CashUp行中填写即可 */
	char DesTail[512];                    	/*   描述尾部|同一组中,只需要在最小的CashUp行中填写即可 */
	tagGiftItem_ToLua PropsGiftItem[PROPS_GIFT_ITEM_NUM];
};

/* 充值礼包的领取信息 */
struct tagPropsGiftGetInfo_ToLua
{
	tdr_ulonglong CashUp;                          	/*   PropsDef里面定义的CashUp */
	int GetTime;                         	/*   领取时间 */
};

/* 积分礼品 */
struct tagPropsGift_ToLua
{
	unsigned int Uin;                            
	unsigned int GroupID;                         	/*   PropsDef表中定义的组ID */
	tdr_ulonglong CashNow;                         	/*   当前充值点数 */
	tdr_datetime_t LastCashTime;                    	/*   充值点数最后更新的时间 */
	int GiftGetNum;                      	/*   已领取礼包数量 */
	tagPropsGiftGetInfo_ToLua GiftGetInfo[PROPS_LIMIT_NUM];    	/*   已领取礼包信息 */
};

struct tagPropGiftInfo_ToLua
{
	int DefCount;                        	/*   礼包Def数量 */
	tagPropsDef_ToLua PropsDef[NEW_PROPSDEF_RECORD_MAX];
	int Count;                           	/*   礼包数量 */
	tagPropsGift_ToLua PropsGift[PROPS_LIMIT_MONTH];  
};

struct tagContiGroup_ToLua
{
	int ID;                             
	int Day;                            
};

struct tagGiftOne_ToLua
{
	int Day;                             	/*   连续几天送这个礼包  */
	int ItemID1;                        
	int ItemNum1;                       
	int ItemID2;                        
	int ItemNum2;                       
	int ItemID3;                        
	int ItemNum3;                       
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   礼包描述 */
};

struct tagOnlineGiftGroup_ToLua
{
	int ID;                              	/*   自增 */
	int Time;                            	/*   单位|分 */
	char Name[LVGIFT_NAME_LEN];           	/*   礼包名 */
	int OneNum;                         
	tagGiftOne_ToLua GiftOnes[MAX_GIFT_GROUP];       
};

struct tagMediaGiftItems_ToLua
{
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   描述 */
	tagGiftItem_ToLua GiftItems[MEDIAGIFT_OPTIONS_ITEM_MAX];
};

struct tagPreViewGift_ToLua
{
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   描述 */
	int MediaGiftID;                    
};


struct tagMediaGiftDef_ToLua
{
	int MediaGiftID;                    
	char GiftDesc[LVGIFT_NAME_LEN];       	/*   礼包名 */
	tagMediaGiftItems_ToLua SelectAllGift;  /*   必获取道具 */
	tagMediaGiftItems_ToLua SelectOneGift;  /*   N选1，选择性获取道具 */
	tagPreViewGift_ToLua PreViewGift;		/*   子媒体礼包预览 */
};

struct tagLtrGiftItem_ToLua
{
	int ItemID;                         
	int ItemNum;                        
	int Rate;                            	/*   得到这个道具的概率（百万分率） */
};

struct tagLtrGiftItems_ToLua
{
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   描述 */
	tagLtrGiftItem_ToLua GiftItems[MEDIAGIFT_OPTIONS_ITEM_MAX];
};

struct tagNextLtrGift_ToLua
{
	char Desc[LVGIFT_DESC_LEN];           	/*   描述 */
	int LtrID;                          
};

struct tagLtrGiftDef_ToLua
{
	int LtrID;                          
	char GiftDesc[LVGIFT_DESC_LEN];       	/*   礼包描述 */
	unsigned char MaxTime;                         	/*   一次循环的最大次数 */
	unsigned char RangeTime;                       	/*   当前项抽奖范围（此礼包前N次抽奖范围） */
	tagLtrGiftItems_ToLua LtrGiftItems;                    	/*   抽奖随机获得道具 */
	tagNextLtrGift_ToLua NextLtrGift;                     	/*   下一次抽奖 */
	unsigned char EverLoop;                        	/*   是否无限循环，1 无限循环，0 只能循环1次 */
};

struct tagBuChangDef_ToLua
{
	int Day;                             	/*   流失天数*/
	int Bull;                            	/*   补偿绑银*/
	tagGiftItem_ToLua GiftItem[MAX_BUCHANG_ITEM];      	/*   补偿道具*/
	int Modulus;                         	/*  Ver.2090 经验补偿系数|百分比*/
};
struct tagArmBuChangDef_TuLua
{
	short Lvl;                             	/*   等级段|等级/10,从0开始 */
	short Career;                          	/*   职业 */
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
	// 向服务器领取奖励请求
	void sendAwardRequest( int nAwardID );
	// 向服务器发送领取礼包请求
	void sendGiftRequest( int nLv, int nGiftId, int nType );	
	//tolua_end

	// 处理服务器发回的消息
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

public:	
	// 对象进入视野
	virtual void objEnterView(GameActor *actor);
	// 对象离开视野
	virtual void objLeaveView(GameActor *actor);
	// 使用道具
	virtual void useContainerItem( int itemId, int listType, int GridIdx );
	// 更新任务NPC
	void updateQuestNpc( int nQuestState, int nQuestId, int nRecycleID );
	// 任务完成触发
	void springByFinishQuest( int nQuestId );
	// 任务获取触发
	void springByGetQuest( int nQuestId, int nRecycleID );
	// 任务提交触发
	void springByQuestCommit( int nQuestId );
	// 任务丢弃触发
	void springByQuestDrop( int nQuestId );
	// 升级触发
	void springByLevelUp( int nLv );
	// 对象进入视野触发
	void springByObjectEnter( int objType, int objResId, int actorId, int dis );
	// 进入场景触发
	void springByEnterMap( int mapId );
	// 学习技能触发
	void springByStudySkill( int skillId );
	
	// 获取服务器上配置信息
	void setSetp( const char* szStep );
	// 获取配置信息
	char* getSetp();
	//tolua_begin
	// 清空配置信息
	void zeroSetp();
	// 处理步骤
	void doStep( int nStep );

	void undoStep( int nStep );
	// 步骤是否已经处理过
	bool isDo( int nStep );
	// 关闭NPC对话泡泡
	void hideNpcTalkByResId( int nResId );
	//显示NPC对话泡泡
	void showNpcTalkByResId( int id );
	// 删除NPC对话泡泡
	void delNpcTalkByActorId( int id );
	// 设置NPC对话泡泡
	void setNpcTalkByActorId( int nId, const char* szTalk, const char* szDrawBack, bool bShowOnce, int nOffsetX, int nOffsetY, 
						const char* szTexPath, bool bShowTex, int nTexOffsetX, int nTexOffsetY, int nDoStep = 0, 
						int nDuration = 10000 );


	// 删除怪物的对话泡泡
	void delActorTalkByActorId( int nActorId );
	// 删除怪物的对话泡泡
	void delActorTalkByActorId( GameActor* pActor );


	// 设置怪物的对话泡泡
	void setActorTalkByActorId( int nActorId, const char* szTalk, const char* szDrawBack, bool bShowOnce, int nOffsetX, int nOffsetY, 
							const char* szTexPath, bool bShowTex, int nTexOffsetX, int nTexOffsetY, int nDoStep = 0, 
							int nDuration = 10000 );

	// 设置某些类型怪物对话泡泡
	void setGroupActorsByResId( int nResId, const char* szTalk, const char* szDrawBack, bool bShowOnce, int nOffsetX, int nOffsetY, 
		const char* szTexPath, bool bShowTex, int nTexOffsetX, int nTexOffsetY, int nDoStep = 0, 
		int nDuration = 10000 );

	// 删除某些类型怪物对话泡泡
	void delGroupActorsByResId( int nResId );
	// 删除所有掉落包裹
	void delAllDropItemPop( );

	// 设置飞龙对话泡泡
	void setFlyMonsterTalk( int nActorId, const char* szTalk, const char* szDrawBack, const char* szTexPath );

	const tagContOnlineGiftDef_ToLua* getContOnlineGiftDef( int nID );

	tagPosition_ToLua getPlayTopWindowPos( int actorId );
	//tolua_end

	// 设置怪物的对话泡泡
	void setActorTalkByActorId( GameActor* pActor, const char* szTalk, const char* szDrawBack, bool bShowOnce, 
		int nOffsetX, int nOffsetY, const char* szTexPath, bool bShowTex, int nTexOffsetX, 
		int nTexOffsetY, int nDoStep = 0, int nDuration = 10000 );

public:
    //tolua_begin 
    // 获得每日奖励数量
    int getOneDayGiftNum()
	{ 
		return (int)m_vectOneDayGift.size(); 
	}

    // 获得等级奖励数量
    int getLVGiftNum(){ return (int)m_vectLVGift.size(); }
    // 获得每日奖励信息
    tagOneDayGift_ToLua* getOneDayGiftDef( int nIndex );
    // 获得等级奖励信息
    tagLVGift_ToLua* getLVGiftDef( int nIndex );
    // 获得玩家已领取奖励信息
    tagRoleGiftInfo_ToLua* getRoleGiftInfo();
    // 获得剩余的等待时间
    int getLeavingTime( int nTotalTime );
    // 能否领取奖励
    bool canGetAwardGift();
    // 是否当天时间
    bool isToday( int nTime );
    // 设置玩家登陆时间
    void setRoleLoginTm( int nLoginTm )
	{ 
		m_nRoleLoginTm = nLoginTm; 
	}
	tagMiscContOnlineInfo_ToLua* getContOnlineInfo();
	tagAccGiftInfo_ToLua* getAccGiftInfo();

	//初始化礼包
	void InitGiftData();
	//充值礼包
	tagPropGiftInfo_ToLua* getPropGiftInfo();
	// 连续在线礼包
	int getContiNum();
	tagContiGroup_ToLua* getContiData( int nIdx );
	const tagOnlineGiftGroup_ToLua* getOnlineGiftGroupDef( int nID );
	tagGiftOne_ToLua* getGiftInfo( int nID,int nDay );
	// CDKey 礼包
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

	// 刷新运营礼包
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
	
	// 连续签到奖励
	const tagContCoverGift_ToLua* getContCoverGiftDef( int nDays );
	void  contGiftCheckIN( int nDays );
	int   getContCoverMaxDays();
	const tagNewRoleGift_ToLua* getNewRoleGiftDef( int nID );
	void  getNewRoleGift( int nID );
	// MT礼包
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
	tagAccGiftInfo					m_AccGiftInfo;		//特殊礼包
	tagPropGiftInfo					m_PropGiftInfo;		//充值礼包
	std::vector<tagContiGroup>		m_vContiGroup;		//连续在线礼包
	char							m_PreCreateGift;
	bool							m_bShowPreCreate;
	int								m_nLastRequestTime;

	std::vector<tagOneDayGift*>		m_vVipBarGift;		//特权网吧连续在线奖励
	std::vector<tagOneDayGift*>		m_vectClanGift;		//公会成员福利

	int								m_UserType;

	ROLELTRINFO						m_RoleLtrInfo;

	Ogre::Entity*					m_pDirEntity;
	char							m_szID[24];
};//tolua_export
