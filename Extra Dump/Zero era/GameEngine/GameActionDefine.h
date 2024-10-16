#pragma once

enum EACT_POSE
{
	POSE_NONE					= 10,		// ����
	// սʿ	
	POSE_SHORTWEAPON_R			= 20,		// ������Ҷ�����
	POSE_WEAPON_SHORTWEAPON		= 24,		// �����ֶ�����
	// ��ʦ
	POSE_WAND					= 30,		// ����+����
	// ��ʦ	
	POSE_CHURCH_WAND			= 32,		// ����+����
	// ǹ��
	POSE_GUN_R					= 40,		// ����ǹ
	POSE_SCATTERGUN				= 42,		// ˫�ֻ�ǹ
	POSE_DOUBLEGUN				= 47,		// ����ǹ
	POS_TMPCAR					= 48,		// ��ʱ��е ����

	POSE_END					= 49,
};

enum EACTSTA_TYPE
{
	ACTSTA_NORMAL				= 1,		// ��������
	ACTSTA_LIVING				= 2,		// ����
	ACTSTA_READING				= 3,		// ����������
	ACTSTA_ATTACKONCE			= 4,		// һ���Թ���
	ACTSTA_ATTACKLOOP			= 5,		// ѭ������
	ACTSTA_OTHER				= 6,		// ����ս������

	ACTSTA_END					= 7,
};

enum EACTSTA_NORMAL
{
	// ����վ��
	ACTSTA_NORMAL_STAND				= 0,	// վ��
	ACTSTA_NORMAL_STAND2			= 1,	
	ACTSTA_NORMAL_STAND3			= 2,
	ACTSTA_NORMAL_FREE_STAND		= 3,	// վ��С����
	ACTSTA_NORMAL_FREE_STAND2		= 4,
	ACTSTA_NORMAL_FREE_STAND3		= 5,
	ACTSTA_NORMAL_GOIN_PRE_STAND	= 6,	// Ԥ��		
	ACTSTA_NORMAL_BACKUP_PRE_STAND	= 7,	// Ԥ��
	ACTSTA_NORMAL_PRE_STAND			= 8,	// ��ս
	ACTSTA_NORMAL_PRE_STAND2		= 9,	
	ACTSTA_NORMAL_PRE_STAND3		= 10,
	ACTSTA_NORMAL_RIDING_STAND		= 11,	// ���ս��

	// �����ƶ�
	ACTSTA_NORMAL_RUN				= 20,	// ��ͨ�ƶ�
	ACTSTA_NORMAL_WALK				= 21,	// ��
	ACTSTA_NORMAL_FLYUP				= 22,	// ����
	ACTSTA_NORMAL_FLY				= 23,	// ����
	ACTSTA_NORMAL_QUICK_FLY			= 24,	// ���ٷ���
	ACTSTA_NORMAL_FLYDOWN			= 25,	// ����
	ACTSTA_NORMAL_JUMP_UP			= 26,	// ��Ծ
	ACTSTA_NORMAL_JUMP_FLY			= 27,	// ��Ծ����
	ACTSTA_NORMAL_JUMP_DOWN			= 28,	// ��Ծ����
	ACTSTA_NORMAL_QUICK_RUN			= 29,	// ����	
	ACTSTA_NORMAL_PHAS				= 30,	// ˲��
	ACTSTA_NORMAL_ROLL				= 31,	// ����

	// �����ܻ�
	ACTSTA_NORMAL_HIT				= 40,	// ��ͨ�ܻ�
	ACTSTA_NORMAL_HIT2				= 41,	// Ԥ��
	ACTSTA_NORMAL_HIT3				= 42,	// Ԥ��
	ACTSTA_NORMAL_DEFENSE			= 43,	// ���Ʒ���
	ACTSTA_NORMAL_DODGE				= 44,	// ����
	ACTSTA_NORMAL_HIT_FLY			= 45,	// �����ܻ�
	ACTSTA_NORMAL_HIT_BACK			= 46,	// ���1
	ACTSTA_NORMAL_HIT_BACK2			= 47,	// ���2
	ACTSTA_NORMAL_HIT_BACK3			= 48,	// ���3
	ACTSTA_NORMAL_HIT_DOWN			= 49,	// ��
	ACTSTA_NORMAL_HIT_UP			= 50,	// ����
	ACTSTA_NORMAL_HIT_BUMP			= 51,	// ���ײ��

	// ��������
	ACTSTA_NORMAL_DEAD				= 60,	// ����
	ACTSTA_NORMAL_DEAD2				= 61,	// Ԥ��	
	ACTSTA_NORMAL_DEAD_BONE			= 62,	// ������̬
	ACTSTA_NORMAL_FLY_DEAD			= 63,	// ��������
	ACTSTA_NORMAL_BACK_DEAD			= 64,	// �������
	ACTSTA_NORMAL_BACK_DEAD2		= 65,	// �������2
	ACTSTA_NORMAL_BACK_DEAD3		= 66,	// �������3
	ACTSTA_NORMAL_BUMP_DEAD3		= 66,	// ײ������
	ACTSTA_NORMAL_FIRE_DEAD			= 68,	// ����
	ACTSTA_NORMAL_BROKEN_DEAD		= 69,	// ������

	ACTSTA_NORMAL_NPC_TALK			= 80,	// NPC�Ի�
	ACTSTA_NORMAL_END				= 70,
};

// ��ȡ����ID( ��̬id, ��������id, ��ΪId)
inline int getActId( int posId, int typeId, int actionId )
{
	assert( posId  < POSE_END );
	assert( typeId < ACTSTA_END );
	return posId*1000 + typeId * 100 + actionId;
}

// ��ȡ����ID
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

// ���ݶ���ID��ȡ��̬����
inline int getPoseType( int actionId )
{
	return actionId/1000;
}

// ���ݶ���ID��ö�������(���棬������)
inline int getActionType( int actionId )
{
	return actionId%1000/100;
}

// ��ȡ�Ӷ���ID
inline int getSlotActionId( int actionId )
{
	return actionId%1000%100;
}

inline int getSlotActionIdByShortId( int actionId )
{
	return actionId/100; 
}

// �Ƿ���ѭ����������
inline bool isLoopAction( int actionId )
{
	// ѭ�������ͼ�����������Ϊѭ�����Ŷ���
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