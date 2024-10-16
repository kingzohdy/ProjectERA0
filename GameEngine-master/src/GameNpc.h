#pragma once

#include "GameActor.h"
#include "GameMsgPrint.h"

struct tagNpcQusetInfo   
{
	std::vector<int>	    m_HaveAcceptDynamicQuestId;	// ��ӵ�нӵĶ�̬�����б�
	std::vector<int>	    m_HaveAcceptQuestId;	// ��ӵ�нӵ������б�
	std::vector<int>	    m_HaveFiniQuestId;		// ��ӵ�н��������б�
	std::vector<CYCLEGRADE>	m_HaveAcceptRecycle;	// ��ӵ�еĽӵĻ��������б�
	std::vector<CYCLEGRADE>	m_HaveFiniRecycle;		// ��ӵ�н��Ļ��������б�	
	int					    m_nQuestShow;			// ͷ����ʾ�ı�־
	
};

// ��ȡ�����б�

bool isFixGetTask( int nQuestID );
extern void getQuestList( int npcId );
extern tagNpcQusetInfo& getNpcQuestInfo( int npcId ); 

//tolua_begin
void clearNpcQuestList();
enum
{
	MONSTER_ORANGE,
	MONSTER_RED,
	MONSTER_WHITE,
	MONSTER_GREEN,	
	MONSTER_YELLOW
};
// ��ȡͷ����־
int getNpcQuestShow( int npcId );
int getMonsterRadius( int nResId );
int getMonsterSubtype( int nResId );
int getMonsterLevel( int nResId );
int getMonsterNameColor( int nResId );
const char* getMonsterName( int nResId );
int getMonsterIcon( int nResId );
int getMonsterMaxHP( int nResId );
const char* getNpcDesc( int npcId );
const char* GetNpcName( int nResid );
int getMonMachineMaxEnergy(int resid);
//tolua_end

class GameNPC : public GameActor//tolua_export
{//tolua_export

public:
	GameNPC();
	// �������
	void onBirth( int nBirthTime ); 
	// �����λ
	virtual void CmdMoveBack( int to_x, int to_z );
	virtual bool UpdateMoveBack();
	// ����������
	virtual void setSubType();
	// ���ù�������(������������������)
	virtual void setAtkType();
	virtual void updateUI( int nChangeValue );
	virtual void Update(unsigned int dtick);
public:
	// ��ԭ����
	void reSetAngle();
	// ��ȡNPC��������
	const char* getNpcFunDes()
	{
		return m_strNpcFunctionDes.c_str();
	}

	bool isPatrol();

public:
	int					m_nPlayFreeActTime;		// �������ж���ʱ��
	int					m_nNpcAngle;
	std::string			m_strNpcFunctionDes;	// NPC��������

	int					m_nQusetShow;
	bool				m_bIsPatrol;			// �Ƿ���Ѳ��npc
private:
	int					m_nMoveBackTime;
	bool				m_lastXunluo;
};//tolua_export

