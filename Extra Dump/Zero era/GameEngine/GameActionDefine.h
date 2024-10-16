#pragma once

enum EACT_POSE
{
	POSE_NONE					= 10,		// 空手
	// 战士	
	POSE_SHORTWEAPON_R			= 20,		// 左盾牌右短武器
	POSE_WEAPON_SHORTWEAPON		= 24,		// 左右手短武器
	// 法师
	POSE_WAND					= 30,		// 法杖+法器
	// 牧师	
	POSE_CHURCH_WAND			= 32,		// 法杖+法器
	// 枪手
	POSE_GUN_R					= 40,		// 单手枪
	POSE_SCATTERGUN				= 42,		// 双手机枪
	POSE_DOUBLEGUN				= 47,		// 对手枪
	POS_TMPCAR					= 48,		// 临时机械 拉车

	POSE_END					= 49,
};

enum EACTSTA_TYPE
{
	ACTSTA_NORMAL				= 1,		// 基本动作
	ACTSTA_LIVING				= 2,		// 生活
	ACTSTA_READING				= 3,		// 吟唱，集气
	ACTSTA_ATTACKONCE			= 4,		// 一次性攻击
	ACTSTA_ATTACKLOOP			= 5,		// 循环攻击
	ACTSTA_OTHER				= 6,		// 特殊战斗动作

	ACTSTA_END					= 7,
};

enum EACTSTA_NORMAL
{
	// 常规站立
	ACTSTA_NORMAL_STAND				= 0,	// 站立
	ACTSTA_NORMAL_STAND2			= 1,	
	ACTSTA_NORMAL_STAND3			= 2,
	ACTSTA_NORMAL_FREE_STAND		= 3,	// 站立小动作
	ACTSTA_NORMAL_FREE_STAND2		= 4,
	ACTSTA_NORMAL_FREE_STAND3		= 5,
	ACTSTA_NORMAL_GOIN_PRE_STAND	= 6,	// 预留		
	ACTSTA_NORMAL_BACKUP_PRE_STAND	= 7,	// 预留
	ACTSTA_NORMAL_PRE_STAND			= 8,	// 备战
	ACTSTA_NORMAL_PRE_STAND2		= 9,	
	ACTSTA_NORMAL_PRE_STAND3		= 10,
	ACTSTA_NORMAL_RIDING_STAND		= 11,	// 骑乘战立

	// 常规移动
	ACTSTA_NORMAL_RUN				= 20,	// 普通移动
	ACTSTA_NORMAL_WALK				= 21,	// 走
	ACTSTA_NORMAL_FLYUP				= 22,	// 升空
	ACTSTA_NORMAL_FLY				= 23,	// 飞行
	ACTSTA_NORMAL_QUICK_FLY			= 24,	// 高速飞行
	ACTSTA_NORMAL_FLYDOWN			= 25,	// 降落
	ACTSTA_NORMAL_JUMP_UP			= 26,	// 跳跃
	ACTSTA_NORMAL_JUMP_FLY			= 27,	// 跳跃飞行
	ACTSTA_NORMAL_JUMP_DOWN			= 28,	// 跳跃飞行
	ACTSTA_NORMAL_QUICK_RUN			= 29,	// 疾走	
	ACTSTA_NORMAL_PHAS				= 30,	// 瞬移
	ACTSTA_NORMAL_ROLL				= 31,	// 翻滚

	// 常规受击
	ACTSTA_NORMAL_HIT				= 40,	// 普通受击
	ACTSTA_NORMAL_HIT2				= 41,	// 预留
	ACTSTA_NORMAL_HIT3				= 42,	// 预留
	ACTSTA_NORMAL_DEFENSE			= 43,	// 盾牌防御
	ACTSTA_NORMAL_DODGE				= 44,	// 闪避
	ACTSTA_NORMAL_HIT_FLY			= 45,	// 空中受击
	ACTSTA_NORMAL_HIT_BACK			= 46,	// 打飞1
	ACTSTA_NORMAL_HIT_BACK2			= 47,	// 打飞2
	ACTSTA_NORMAL_HIT_BACK3			= 48,	// 打飞3
	ACTSTA_NORMAL_HIT_DOWN			= 49,	// 打倒
	ACTSTA_NORMAL_HIT_UP			= 50,	// 爬起
	ACTSTA_NORMAL_HIT_BUMP			= 51,	// 打飞撞击

	// 常规死亡
	ACTSTA_NORMAL_DEAD				= 60,	// 死亡
	ACTSTA_NORMAL_DEAD2				= 61,	// 预留	
	ACTSTA_NORMAL_DEAD_BONE			= 62,	// 死亡姿态
	ACTSTA_NORMAL_FLY_DEAD			= 63,	// 空中死亡
	ACTSTA_NORMAL_BACK_DEAD			= 64,	// 打飞死亡
	ACTSTA_NORMAL_BACK_DEAD2		= 65,	// 打飞死亡2
	ACTSTA_NORMAL_BACK_DEAD3		= 66,	// 打飞死亡3
	ACTSTA_NORMAL_BUMP_DEAD3		= 66,	// 撞击死亡
	ACTSTA_NORMAL_FIRE_DEAD			= 68,	// 烧死
	ACTSTA_NORMAL_BROKEN_DEAD		= 69,	// 被打碎

	ACTSTA_NORMAL_NPC_TALK			= 80,	// NPC对话
	ACTSTA_NORMAL_END				= 70,
};

// 获取动作ID( 姿态id, 动作类型id, 行为Id)
inline int getActId( int posId, int typeId, int actionId )
{
	assert( posId  < POSE_END );
	assert( typeId < ACTSTA_END );
	return posId*1000 + typeId * 100 + actionId;
}

// 获取动作ID
inline int getActId( int posId, int actionId )
{
	if( actionId == -1 )
	{
		return actionId;
	}
	//assert( posId < POSE_END );
	//assert( actionId > 100 && actionId < ACTSTA_END * 100 );
	return posId*1000 + actionId;
}

// 根据动作ID获取姿态类型
inline int getPoseType( int actionId )
{
	return actionId/1000;
}

// 根据动作ID获得动作类型(常规，攻击等)
inline int getActionType( int actionId )
{
	return actionId%1000/100;
}

// 获取子动作ID
inline int getSlotActionId( int actionId )
{
	return actionId%1000%100;
}

inline int getSlotActionIdByShortId( int actionId )
{
	return actionId/100; 
}

// 是否是循环攻击动作
inline bool isLoopAction( int actionId )
{
	// 循环攻击和集气，吟唱都为循环播放动作
	if( actionId/100 == ACTSTA_ATTACKLOOP || actionId/100 == ACTSTA_READING )
	{
		return true;
	}
	return false;
}

inline int getLoopActionTyoe( int actionId )
{
	return actionId/100;
}