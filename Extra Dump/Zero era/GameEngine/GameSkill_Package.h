#pragma once

#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include <map>
#include <set>

enum{
	ACTION_HIT_NONE	= 0,	
	ACTION_HIT_MISS,		// ����
	ACTION_HIT_INVALID,		// ��Ч
	ACTION_HIT_HIT,			// �ܻ�
	ACTION_HIT_HEAVY,		// �ػ�
	ACTION_HIT_BACK,		// ����
	ACTION_HIT_BACKDEAD,	// ��������
	ACTION_HIT_DEAD,		// ����
};

class GameSkill_Package
{
public:
	GameSkill_Package(void);
	~GameSkill_Package(void);

public:
	// �Ƿ�������˶���ʾðѪ�˺�
	bool showDamageForAll();
	// ִ�м��ܰ�(���ض�����������)
	void	doPackage();
	int		doPackage(int id);
	// ���ö���
	void setTargetState(int id, const CSHITS& Package);
	// ˢ��Ŀ��HP��MP
	void updateTargetValue();
	// ��ȡ�յ���ʱ�̵�ʱ��
	unsigned int getTime();
	
private:
	// ���㼼���������˴��
	void calSkillHitBack( GameActor* pObj, int nHitValue );

public:
	CSHITS			m_HitValue;			
	int				m_HitType;			// �ܻ��˺�Ʈ������
	int				m_nAttackerID;		// ������ID
	unsigned int	m_uStartTime;
	unsigned int	m_uStatusIdx;
	std::map<int, tagCSOneHit *>	m_Hits;
	std::set<int>		m_hitbackIds;
};
