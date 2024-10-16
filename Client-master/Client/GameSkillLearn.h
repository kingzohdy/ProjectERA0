#pragma once
#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include <map>

const int MAX_SKILLLEARN = 300;

//tolua_begin 
enum
{
	SKILLLEARN_TYPE_PUBLIC,				// ����
};

enum
{
	SKILLLEARN_SUCCED,					// ѧϰ���ܳɹ�

	SKILLLEARN_FAIL_LVMIN,				// �ȼ�����
	SKILLLEARN_FAIL_CAREER,				// ְҵ������
	SKILLLEARN_FAIL_PREDIATHESIS,		// ǰ�����ʲ�����
	SKILLLEARN_FAIL_ITEM,				// ���߲�����
	SKILLLEARN_FAIL_SKILLPOINT,			// ���ܵ㲻��
	SKILLLEARN_FAIL_MONEY,				// ��Ǯ����
	SKILLLEARN_FAIL_QUEST,				// ���񲻷���
	SKILLLEARN_FAIL_NONEED,				// ѧϰ��ʽ������
	SKILLlEARN_FAIL_SKILLLV,			// ����ѧϰ�ȼ����� 
	SKILLlEARN_FAIL_EXP,				// ���鲻��
};

struct tagOneSkillLearnInfo
{
	int		m_nSkillId;
	int		m_nLv;
};

struct tagSkillLearnInfo
{
	int						m_nType;	// ���ͣ����л������������֣�
	int						m_nNum;		// ����
	tagOneSkillLearnInfo	m_SkillInfo[MAX_SKILLLEARN];				
};
//tolua_end

class GameSkillLearn//tolua_export
	: public Ogre::Singleton<GameSkillLearn>
{//tolua_export
public:
	GameSkillLearn(void);
	~GameSkillLearn(void);

public:
	//tolua_begin 
	// �����Ƿ��Զ�ѧϰ
	bool	isSkillAutoLearn(int skillId, int lv);
	// �����Ƿ���Ҫ������ѧϰ
	bool	isSkillNeedBookLearn(int skillId, int lv);
	// �����Ƿ����ѧϰ
	int		checkSkillLearn( int skillId, int lv );
	// ˢ�¼���ѧϰ�б�
	void	updateSkillLearnList();
	// ��ȡȫ��
	const tagSkillLearnInfo* getAllSkill( int type )const;
	// ��ȡ��ѧ
	const tagSkillLearnInfo* getCanLearnSkill( int type )const;
	
	//��ȡ��ָ���ļ���
	const tagOneSkillLearnInfo* getSkillInfo( int nSkillType, int nIndex, bool isAll ) const;
	// ����ѧϰ
	void skillLearn( int npcId, int skillId, int lv );
	// ��������
	void skillLvUp( int npcId, int skillId, int lv );
	// ����ѧϰ
	void diathesisLearn( int npcId, int diathesisId, int lv );
	// ��������
	void diathesisLvUp( int npcId, int diathesisId, int lv );
	// �츳ѧϰ
	void geniusLearn( int npcId, int geniusId, int lv );
	// �츳����
	void geniusLvUp( int npcId, int geniusId, int lv );
	// �츳����
	void geniusReset( int npcId );
	//tolua_end
private:
	std::map<int, tagSkillLearnInfo>	m_SkillLearnAll;	// ȫ��
	std::map<int, tagSkillLearnInfo>	m_SkillLearnCan;	// ��ѧ

	std::vector<tagSkillDef>			m_AllSkillDef;
};//tolua_export
