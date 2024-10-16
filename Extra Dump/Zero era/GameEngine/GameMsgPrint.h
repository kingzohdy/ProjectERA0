#pragma once
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include <list>

//tolua_begin
enum EMID_MESSAGE
{
	MID_NPCSHOP_MONEYNOTENOUGH_SELLFAIL = 1,		// 身上金钱不足,购买失败!
	MID_NPCSHOP_BULLNOTENOUGH_SELLFAIL,				// 身上布尔不足,购买失败!
	MID_NPCSHOP_EXCHANGEITEMNOTENOUGH_SELLFAIL,		// 兑换的牌子不足,购买失败!
	MID_NPCSHOP_MONEYNOTENOUGH_BUYSAVEFAIL,			// 身上金钱不足,回购失败!	
	MID_NPCSHOP_BULLNOTENOUGH_BUYSAVEFAIL,			// 身上布尔不足,回购失败!
	MID_NPCSHOP_BULLNOTENOUGH_REPAIRFAIL,			// 身上布尔不足,维修失败!
	MID_NPCSHOP_NOTNEEDREPAIR,						// 不需要修理
	
	MID_ARMPROCE_ARMSTAR,							// 装备品质提升
	MID_ARMPROCE_LEVEL,								// 装备等级提升
	MID_ARMPROCE_FIGHT,								// 装备战斗力提升
	MID_ARMPROCE_IDENTIFIABILITY,					// 装备鉴定成功
	MID_ARMPROCE_GEMLEVELUP,						// 宝石升级成功
	MID_ARMPROCE_GEMINLAY,							// 宝石镶嵌成功
	MID_ARMPROCE_GEMPICK,							// 宝石摘取成功
	MID_ARMPROCE_GEMPICK_FAIL,						// 宝石摘取失败
	MID_ARMPROCE_GEMINLAY_FAIL,						// 宝石镶嵌失败
	MID_ARMPROCE_GEMLEVELUP_FAIL,					// 宝石升级失败
	MID_ARMPROCE_IDENTIFIABILITY_FAIL,				// 装备鉴定失败
	MID_ARMPROCE_ARMSTAR_FAIL,						// 装备品质提升失败
	MID_ARMPROCE_LEVEL_FAIL,						// 装备等级提升失败
	MID_ARMPROCE_FIGHT_FAIL,						// 装备战斗力提升失败
	MID_ARMPROCE_NOTGEMEFFECTINLAY,					// 不是镶嵌宝石
	MID_ARMPROCE_NOTSAMEGEMTYPE,					// 宝石类型不一致
	MID_ARMPROCE_NOTSAMELEVEL,						// 宝石等级不一致
	MID_ARMPROCE_NOTGEM,							// 不是宝石不能放入
	MID_ARMPROCE_NOTINLAYGEM,						// 非镶嵌类宝石不可升级
	MID_ARMPROCE_NOTEQUIP,							// 请先放入装备
	MID_ARMPROCE_SAMEGEMTYPEINLAY,					// 待镶嵌列表已有同类型宝石！

	MID_ARMPROCE_NOTGEMPICKITEM,					// 没有摘取宝石所需要物品
	MID_ARMPROCE_NOUPDATEGEM,						// 放入要升级的宝石
	MID_ARMPROCE_NOGEMUPDATEITEM,					// 没有升级宝石所需要物品
	MID_ARMPROCE_NOTEQUIPITEM,						// 放入的物品不是装备
	MID_ARMPROCE_NOTHOLDEQUIP,						// 放入的装备没有带孔
	MID_ARMPROCET_NOTARMSAFEITEM,					// 背包里没有品质提升保险符
	MID_ARMPROCET_FIGHTSAFEITEM,					// 背包里没有战斗力提升保险符
	MID_ARMPROCET_GEMSAFEITEM,						// 背包里没有镶嵌符
	MID_ARMPROCET_FIGHTLIMIT,						// 装备战斗力等级不可高等于装备品质
	MID_ARMPROCET_NOTGEMPICK,						// 缺少对应的宝石摘取符
	MID_ARMPROCET_NOTSTARSTUFF,						// 放入的不是升品质的材料
	MID_ARMPROCET_NOTFIGHTSTUFF,					// 放入的不是升战斗力的材料
	MID_ARMPROCET_NOTEQUIPLEVELSTUFF,				// 放入的不是升装备等级的材料
	MID_ARMPROCET_NOTHOLEINLAY,						// 该装备已无可镶嵌的孔
	MID_ARMPROCET_NOTHOLEEQUIP,						// 该装备不是有孔装备
	MID_ARMPROCET_HOLEHASGEM,						// 该孔已镶嵌宝石
	MID_ARMPROCET_HOLECOLORDIFWITHGEM,				// 该孔颜色跟宝石颜色不匹配
	MID_ARMPROCET_INLAY_LEVELLIMIT,					// 宝石等级与装备等级不符
	MID_ARMPROCET_HOLEHASSAMEGEM,					// 该装备已镶嵌过这种宝石
	MID_ARMPROCET_NOTINLAYGEM,						// 该装备未镶嵌宝石
	MID_ARMPROCET_NOTEQUIPASKLEVELITEM,				// 该物品不是匹配的宝石升级符
	MID_ARMPROCET_NOTEQUIPASKPICKITEM,				// 该物品不是匹配的宝石摘取符
	MID_ARMPROCET_NOTCHOICEPICKGEM,					// 请选择你要摘取的宝石
	MID_ARMPROCET_CANNOTINDENTITY,					// 装备不可鉴定
	MID_ARMPROCET_NOTINDENTITYEQUIP,				// 装备未鉴定
	MID_ARMPROCET_NOTENOUGHBULL,					// 布尔不足
	MID_ARMPROCET_NOTNEXTITEMLEVEL,					// 装备不可升级

	MID_EXCHANGE_ACTIONFORBID,						//交易状态中不可执行此操作
	
	
	MID_EQUIP_NOTENOUGHEXP,							// 当前经验值不足, 升级失败
	MID_TRUST_NOTENOUGHLV,							// 等级未满三十级

	MID_TRUST_NOTENOUGHLVORITEM,					// 等级不足或当前委托需求物品不足
	MID_TRUST_HASTODAYLIMITFINISH,					// 已达本日委托限制次数
	MID_TRUST_MAX_ONETIME,							// 已达到一次所挂委托任务数目的上限

	MID_NPCSHOP_TRADENUMCANNOTNULL,					// 商品的数量不能为空
	MID_NPCSHOP_TRADEPRICECANNOTNULL,				// 商品的售价不能为空
	MID_NPCSHOP_TRADEBUYNUMCANNULL,					// 购买的数量不能为空
	MID_NPCSHOP_NOTCHOICEITEM,						// 请选择你要购买的物品！

	MID_NPCSHOP_NONEEDREPAIR,						// 不需维修

	MID_STALL_CANSTALLSTATE,						// 摆摊状态不能上架商品
	
	MID_MAIL_NOTENOUGHLV,							// 等级未满十级
	MID_MAIL_HASINMAILSENDITEM,						// 该物品已在待发邮件物品列表中

	MID_FIGHT_IN,									// 进入战斗状态
	MID_FIGHT_OUT,									// 离开战斗状态

	MID_SHORTCUT_MPNOTENOUGH,						// MP不足	
	MID_SHORTCUT_ENERGENOTENOUGH,					// 能量不足
	MID_SHORTCUT_NOTITEM,							// 物品已用完
	MID_SHORTCUT_CANNOTUSESPELL_BUF,				// 当前状态不能使用技能
	MID_SHORTCUT_CANNOTUSESPELL_MOVE,				// 当前状态下不能使用位移技能
	MID_SHORTCUT_CANNOTUSESPELL_INMORPH,			// 变身状态不能使用技能
	MID_SHORTCUT_CANNOTUSESPELL_XPLIMIT,			// 当前状态不能使用XP技能
	MID_SHORTCUT_CANNOTUSESPELL_INSTALL,			// 摆摊状态不能使用技能

	MID_PATH_FINDNONPC,								// 目标NPC不在当前地图
	MID_XP_COOLDOWNTIME,							// 技能处于冷却状态
	MID_XP_NOENOUGHMP,								// 宠物的MP不足
	MID_XP_NOENOUGHXP,								// 玩家的XP不足
	MID_XP_NOENOUGHCOLLECT,							// 玩家的积累数不足
	MID_XP_NOINMAP,									// 只能在副本中使用
	MID_XP_NOCAMPTRUMP,								// 当前没有出征精灵

	MID_MAIL_SENDER_NAME_NULL,						// 邮件发送者名字为空
	MID_MAIL_TITLE_NULL,							// 邮件主题为空
	MID_MAIL_NO_MONEY,								// 发送邮件所需邮资不足

	MID_TRADE_SELF_FIGHT,							// 主角正处于战斗状态，不能进行交易
	MID_TRADE_SELF_STALL,							// 主角正处于摆摊状态，不能进行交易
	MID_TRADE_SELF_FLY,								// 主角正处于飞行状态，不能进行交易
	MID_TRADE_SELF_DEAD,							// 主角正处于死亡状态，不能进行交易
	MID_TRADE_SELF_MORPH,							// 主角正处于变身状态，不能进行交易
	MID_TRADE_SELF_TRUMP_PRE,						// 主角正在召唤精灵，不能进行交易
	MID_TRADE_CANNOT_REPEAT_CONFIRM_TRADE_,			// 不能重复确认交易
	MID_TRADE_CANNOT_DIST_TOO_LONG,					// 距离过远无法申请交易
	MID_TRADE_CANNOT_NOT_CAN_TRADE_OBJECT,			// 目标对象无法交易

	MID_TRADE_ACTOR_FIGHT,							// 请求交易的那方正处于战斗状态，不能进行交易
	MID_TRADE_ACTOR_STALL,							// 请求交易的那方正处于摆摊状态，不能进行交易
	MID_TRADE_ACTOR_FLY,							// 请求交易的那方正处于飞行状态，不能进行交易
	MID_TRADE_ACTOR_DEAD,							// 请求交易的那方正处于死亡状态，不能进行交易
	MID_TRADE_DISTANCE_LONG,						// 双方距离太远，不能进行交易
	MID_TRADE_INVITE_TIME_OUT,						// 对方长时间没响应交易邀请

	MID_TRADE_RELATION_XIONGSHOU,					// 对方是自己的仇人不能进行交易
	MID_TRADE_RELATION_SUDI,						// 对方在自己的宿敌列表不能进行交易
	MID_TRADE_RELATION_BLACK,						// 对方在黑名单中不能进行交易

	MID_STALL_DISTANCE_LONG,						// 双方距离太远，不能查看对方的摊位信息
	MID_STALL_SELF_EMPTY,							// 没有商品上架，不能进行摆摊
	MID_STALL_SELF_FIGHT,							// 主角正处于战斗状态，不能进行摆摊
	MID_STALL_SELF_PK,								// 主角正处于PK状态，不能进行摆摊
	MID_STALL_SELF_RIDE,							// 主角处于骑乘状态，不能摆摊
	MID_STALL_SELF_MORPH,							// 主角正处于变身状态，不能查看对方摊位信息
	MID_STALL_SEE_FIGHT,							// 主角正处于战斗状态，不能查看对方摊位信息
	MID_MULTIMAP_AUTOFIND_FAIL,						// 跨地图自动寻路失败
	MID_USESKILL_INCOLLECT_FAIL,					// 正在采集，不能使用技能
	MID_USEITEM_INCOLLECT_FAIL,						// 正在采集，不能使用物品
	MID_STALL_SELF_TRUMP_PRE,						// 正在召唤精灵，不能摆摊
	MID_STALL_SELF_NEAR_NPC,						// 距离NPC太近，不能摆摊
	MID_STALL_SELF_NEAR_OTHER_STALL,				// 距离其他摊位太近，不能摆摊
	MID_STALL_SELF_DEAD,							// 主角正处于死亡状态，不能进行摆摊
	MID_STALL_NOTENOUGHLV,							// 等级未满二十级
	MID_STALL_SEE_PK,								// 切磋状态，不能查看对方摊位信息
	MID_STALL_CANNOT_EMPTY,							// 不能空置摊位，无法下架
	MID_STALL_SEE_DEAD,								// 死亡状态下无法浏览摊位
	MID_STALL_TOOMUCH_MONEY,						// 出售总额加当前携带金额，不得超出身上携带金额的上限

	MID_FOLLOW_START_FAIL,							// 不能跟随
	MID_ITEM_USE_FAIL_INPRE,						// 正在使用其它物品
	MID_ITEM_USE_FAIL_NOTINAREA,					// 只能在指定区域使用
	MID_ITEM_USE_FAIL_IN_STALL,						// 处于摆摊状态无法使用物品

	MID_FRIEND_REPEAT_REQ,							// 重复申请加某人为好友
	MID_FRIEND_IS_ENEMY,							// 该玩家是你的仇人不能加为好友
	MID_FRIEND_IS_SU_DI,							// 该玩家在你的追杀名单中不能加为好友
	MID_FRIEND_IS_BLACK,							// 该玩家在你的黑名单中，不能加为好友

	MID_FITTINGMGR_NOFITSTUFF,						// 该材料不能装备配件
	MID_FITTINGMGR_NOTFITGROUP,						// 本技能不能镶嵌这个配件
	MID_FITTINGMGR_CAREEALIMIT,						// 职业不符,不能镶嵌此配件
	MID_FITTINGMGR_INLAYSPECIALLIMIT,				// 该镶嵌孔只能装备花类配件
	MID_FITTINGMGR_INLAYNORMALLIMIT,				// 该镶嵌孔只能装备无花类配件
	MID_FITTINGMGR_SAMEIDLIMIT,						// 不能镶嵌相同id的配件
	MID_FITTINGMGR_SAMETYPELIMIT,					// 不能镶嵌相同类型的配件
	MID_FITTINGMGR_NEEDBULLLIMIT,					// 装备配件所需要的布尔不足

	MID_TRUMP_PER_FAIL_INFIGHT,						// 死亡状态下不能出征精灵
	MID_TRUMP_PER_FAIL_INDEAD,						// 战斗状态下不能出征精灵
	MID_TRUMP_ISPRE,								// 正在出征精灵，请稍候
	MID_TRUMP_BACK_FAIL,							// 当前状态不能召回精灵

	MID_SPELL_UNUSED_NOAMM,							// 缺少子弹
	MID_SPELL_UNUSED_RIGHTARM,						// 缺少右手武器
	MID_SPELL_UNUSED_LEFTARM,						// 缺少左手武器
	MID_SPELL_UNUSED_SHOULDER,						// 缺少肩甲武器
	MID_SPELL_UNUSED_BACK,							// 缺少背部武器
	MID_SPELL_UNUSED_NONEEQUIP,						// 缺少武器

	MID_SPELL_UNUSED_CAMP,							// 施法对象阵营不符合
	MID_SPELL_UNUSED_OBJ,							// 施法对象不符合
	MID_SPELL_UNUSED_DEAD,							// 目标死亡
	MID_SPELL_UNUSED_MASK_TARGET,					// 目标中间有魔法阻挡
	MID_SPELL_UNUSED_MASK_POINT,					// 地点中间有魔法阻挡
	MID_SPELL_UNUSED_INCOLLECT,						// 在采集状态下
	
	MID_SPELL_NOTARGET,								// 没有目标
	MID_AUTOFIND_SELF_LIMIT,						// 主角状态受制，不能进行自动寻路

	MID_TRUST_ALREADY_ORDER,						// 该委托已经挂单，不能重复挂单
	MID_FIND_PATH_FAIL,								// 寻路失败，目标点不可达
	MID_MARKET_NOT_ENOUGH_GOLD,						//点券不够
	MID_MARKET_NOT_ENOUGH_BIND_GOLD,				//绑定点券不够
	MID_MARKET_NOT_FRIEND,							//不是赠送好友
	MID_MARKET_SEND_TRADE_SUCESS,					//发布交易成功
	MID_MARKET_TRADE_SUCESS,						//交易成功
	MID_MARKET_DESTROY_SUCESS,						//撤销订单成功
	MID_MARKET_PACKAGE_PRESENT_SUCESS,				//赠送成功
	MID_MARKET_BUY_TIME_OUT,						//购买物品以失效

	MID_PET_PRE_FAIL_TRUMP,							// 正在召唤精灵，不能召唤宠物
	MID_PET_PRE_FAIL_USEITEM,						// 正在使用物品，不能召唤宠物
	MID_PET_PRE_FAIL_SKILLPRE,						// 正在进行技能吟唱，不能召唤宠物
	MID_PET_PRE_FAIL_RIDEPRE,						// 正在召唤骑乘，不能召唤宠物
	MID_PET_PRE_FAIL_COLLECT,						// 当前状态，不能召唤宠物
	MID_PET_PRE_FAIL_FIGHT,							// 战斗状态，不能召唤宠物
	MID_PET_PRE_FAIL_STALL,							// 摆摊状态，不能召唤宠物
	MID_PET_PRE_FAIL_IN_OTHER_RIDE,					// 处于他人坐骑，不能召唤宠物
	MID_PET_PRE_FAIL_IN_RIDE_MACHINE,				// 乘坐机械，不能召唤宠物
	MID_PET_FAIL_IN_FREEDOM_ANIL,					// 该宠物已经被您放生，不可进行该操作！
	MID_PET_FAIL_FREEDOM_IN_FIGHT,					// 该宠物正在出征，不可放生
	MID_PET_PRE_FAIL_SOME_STATE,					// 当前状态，不能召唤宠物

	MID_TRUMP_PRE_FAIL_PET,							// 正在召唤宠物，不能召唤精灵
	MID_TRUMP_PRE_FAIL_USEITEM,						// 正在使用物品，不能召唤精灵
	MID_TRUMP_PRE_FAIL_SKILLPRE,					// 正在进行技能吟唱，不能召唤精灵
	MID_TRUMP_PRE_FAIL_COLLECT,						// 当前状态，不能召唤精灵
	MID_TRUMP_PRE_FAIL_RIDEPRE,						// 正在召唤骑乘，不能召唤精灵

	MID_RIDE_PRE_FAIL_TRUMP,						// 正在召唤精灵，不能使用骑乘
	MID_RIDE_PRE_FAIL_USEITEM,						// 正在使用物品，不能使用骑乘
	MID_RIDE_PRE_FAIL_SKILLPRE,						// 正在进行技能吟唱，不能使用骑乘
	MID_RIDE_PRE_FAIL_COLLECT,						// 当前状态，不能使用骑乘
	MID_RIDE_PRE_FAIL_PET,							// 正在召唤宠物，不能使用骑乘
	MID_RIDE_PRE_FAIL_NO_RIDE,						// 没有该骑乘
	MID_RIDE_PRE_FAIL_SOME_STATE,					// 当前状态，不能使用骑乘

	MID_INFIGHT,									// 在战斗状态，不能使用
	MID_NOINFIGHT,									// 在非战斗状态，不能使用

	MID_NEED_TARGET,								// 必须选择一个目标
	MID_NEED_MONSTER_TARGET,						// 必须选择一个怪物目标
	MID_NEED_NPC_TARGET,							// 必须选择一个NPC目标	
	MID_NEED_NPC_PLAYER,							// 必须选择一个玩家	

	MID_APPLY_TEAM_PLAYER_IS_ENEMY,					// 对方是你的仇人，不能申请入队
	MID_APPLY_TEAM_PLAYER_IS_TMPENEMY,				// 对方在你的追杀名单，不能申请入队
	MID_INVITE_TEAM_PLAYER_IS_TMPENEMY,				// 对方在你的追杀名单，不能申请入队
	MID_APPLY_TEAM_SELF_DEAD,						// 本人已经死亡无法组队
	MID_APPLY_TEAM_OTHER_DEAD,						// 对方已经死亡无法组队
	MID_APPLY_TEAM_OTHER_IN_BLACK,					// 对方在你的黑名单中，无法组队
	MID_APPLY_TEAM_OTHER_IN_ZHUISHA,				// 不可与追杀名单中的玩家组队
	MID_APPLY_TEAM_OTHER_IN_XIONGSHOU,				// 不可与凶手名单中的玩家组队
	MID_APPLY_TEAM_OTHER_IN_TEAM,					// 你已经拥有队伍

	MID_USE_ITEM_FAILE_HPENOUGH,					// 当前生命值已满,无需要使用
    MID_USE_ITEM_FAILE_MPENOUGH,					// 当前魔法值已满,无需要使用
    MID_USE_ITEM_FAILE_LVENOUGH,					// 等级不足,无法使用

	MID_AUTO_TEAM_FAIL_AGREE_READY,					// 已经处于准备状态，不能再次进行同意操作
	MID_MARKET_NO_SEARCH,							// 商城没有查找到订单

	MID_AUTO_TEAM_FAIL_TRAN_BY_EXCHANGE,			// 交易状态不能传送
	MID_AUTO_TEAM_FAIL_TRAN_BY_STALL,				// 摆摊状态不能传送
	MID_AUTO_TEAM_FAIL_TRAN_BY_DEAD,				// 死亡状态不能传送
	MID_AUTO_TEAM_FAIL_TIME_OUT,					// 超时不能传送

	MID_SECONDARYSKILL_WRONG_STATE,					//处于不能使用生活技能的状态的下
	MID_USEITEMPRE_WRONG_STATE,						//处于不能使用物品吟唱的状态的下

	MID_USEITEM_IN_FLY_STATE,						//在飞行的时候 不能使用道具
	MID_AUTOFIND_IN_FLY_STATE,						//在飞行状态不能自动寻路
	MID_SHORTCUT_IN_FLY_STATE,						//飞行状态下 不能使用技能

	MID_PWORLD_RESTOREITEM,							//副本中不能使用恢复性道具
	MID_MAP_NOSKILL,								//地图中不能使用技能
	MID_SAFE_ENV_NOSKILL,							//安全区不能使用技能
	MID_SELF_BUSY,									//正在做其他事
	MID_SELF_DEAD,									//你已经死亡
	MID_SELF_SPELLPRE,								//正在吟唱技能
	MID_CANNT_USE_INPK,								//切磋状态下不能使用此技能
	MID_NOT_ENOUGH_ENERGY,							//能量不足
	MID_TARGET_TO_FAR,								//目标太远

	MID_TRUMP_PER_FAIL_MOVE,						//移动中，不能召唤精灵
	MID_MOVE_FAIL_STALL,							// 摆摊状态无法移动

	MID_PICK_ITEM_TOO_FAR,							// 距离太远，无法拾取
	MID_SECONDARYSKILL_FAILED_INRIDE,				//骑乘状态中，无法进行科技生产
	MID_SECONDARYSKILL_FAILED_INFIGHT,				//战斗状态中，无法进行科技生产
	MID_SECONDARYSKILL_FAILED_INCASTING,			//吟唱中，无法进行科技生产
	MID_SECONDARYSKILL_FAILED_CANNTCONTROL,			//受控制状态中，无法进行科技生产
	MID_SECONDARYSKILL_FAILED_COLDDOWN,				// 生活技能冷却中

	MID_CANNOT_CATCH_PET,							// 不是可以捕捉的宠物
	MID_CATCH_PET_DEAD,								// 捕捉的宠物已死亡
	MID_FAIL_WEI_XING,								// 停止尾行
	MID_NPCSHOP_BATTLE_GLORY_SELLFAIL,				// 荣誉不足,购买失败!
	MID_NPCSHOP_AOSHAN_BRAND_SELLFAIL,				// 绿洲牌子不足,购买失败!
	MID_NPCSHOP_LIFT_BRAND_SELLFAIL,				// 狂沙牌子不足,购买失败!
	MID_TRUMP_PER_FAIL_BADSTATUS,					// 该状态下，无法召唤精灵
	MID_RESOLVE_WRONG_STATE,						// 处于不能使用分解的状态的下
	MID_RESOLVE_FAILED_INCASTING,					//吟唱中，无法分解
	MID_RESOLVE_CONTAINER_LOCK,						//背包上锁，无法分解
	MID_RESOLVE_FAILED_CANNTCONTROL,				//受控制状态中，无法进行科技生产
	MID_RIDE_PRE_FAILED_CLAN_LIMIT_CAPTION,			//乘骑失败，此坐骑只有时间城占领公会会长才有使用权限
	MID_RIDE_PRE_FAILED_CLAN_LIMIT_MEMBER,			//乘骑失败，此坐骑只有时间城占领公会成员才有使用权限
	MID_USE_ITEM_FAILED_CLAN_LIMIT_CAPTION,			//使用失败，此物品只有时间城占领公会会长才有使用权限
	MID_USE_ITEM_FAILED_CLAN_LIMIT_MEMBER,			//使用失败，此物品只有时间城占领公会会长才有使用权限
	MID_USE_BACK_CLAN_ITEM_FAILED_INFIGHT,			//战斗状态不可使用公会城市传送卷
	MID_USE_PET_EGG_PET_FULL,						//宠物栏已满
	MID_PET_MAP_NO_PET_FIGHT,						//当前地图宠物不可出战
	MID_PET_MAP_NO_FAIRY_FIGHT,						//当前地图无法召唤精灵

	MID_MUST_MACHINE_COLLECT,						//必须采集车采集
	MID_MACHINE_UNMATCH_COLLECT,					//采集车不匹配
	MID_MACHINE_UNMATCH_COLLECT_MORMAL,				//采集车不匹配但采集车本身采集类型为0
	MID_USE_MACHINE_ITEM_IN_MORPH,					//变身中无法使用怪物机械
	MID_USE_MORPH_ITEM_IN_MACHINE,					//在机械上使用变身药水

	MID_ENTER_CLANISLAND_IN_FIGHT,					//战斗状态不能传送
	MID_ENTER_CLANISLAND_IN_MACHINE,				//机械状态不能传送
	MID_SUMMON_MUST_BE_CAPTAIN,						//传送卷轴使用必须是队长
	MID_SUMMON_MUST_BE_PRESIDENT,					//传送卷轴使用必须是会长
	MID_NOTENOUGH_COST_ITEM,						//技能需要消耗的道具数量不足
	MID_CHATLINK_MAX_NUM,							//设置的链接数量已达到最大
	MID_CHATLINK_SAME_EXIST,						//已存在相同类型的链接

	MID_NO_ATTACK_WORLD_CITY_CONTROL,				//攻击无效，还有能源增幅器存在，当前建筑无法被攻击
	MID_NO_ITEM_USE_IN_THIS_MAP,					//此地图不能使用此物品

	MID_NPCSHOP_SPARTA_GLORY_SELLFAIL,				// 殿堂荣耀不足,购买失败!

	MID_NO_CHANGE_STALL_MODEL,						//你没法改变这个摊位的外形～
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
