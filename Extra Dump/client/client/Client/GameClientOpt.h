#pragma once
#include "OgreSingleton.h"
#include <map>
#include "proto_comm.h"

enum
{
	SAVE_OPT_DIRECT		= 0,	// ����ָ��
	SAVE_OPT_SHORCUT,			// �����
	SAVE_OPT_SPSHORCT,			// ��ϼ�
	SAVE_OPT_GAMESETTING,		// ��Ϸ���� 
	SAVE_OPT_CAMERADIR,			// ������ǰ�ľ�ͷ���� 
	SAVE_OPT_TASKTRACE,			// ����׷������ 
	SAVE_OPT_PET_AUTO_USE_ITEM,	// �����Զ�ʹ��ҩƷ
	SAVE_OPT_LAST_COMMIT_MAIL_TIME,	// �ϴ��ύ�ʼ���ʱ��

	SAVE_OPT_LAST_PING_BI_WORLD_PIN_DAO,		// �ϴ���ѡ�����ε�����Ƶ��
	SAVE_OPT_LAST_PING_BI_CLAN_PIN_DAO,			// �ϴ���ѡ�����εĹ���Ƶ��
	SAVE_OPT_LAST_PING_BI_ZONG_HE_PIN_DAO,		// �ϴ���ѡ�����ε��ۺ�Ƶ��
	SAVE_OPT_LAST_PING_BI_TEAM_PIN_DAO,			// �ϴ���ѡ�����εĶ���Ƶ��
	SAVE_OPT_LAST_PING_BI_NEAR_PIN_DAO,			// �ϴ���ѡ�����εĸ���Ƶ��
	SAVE_OPT_LAST_PING_BI_PRIVATE_PIN_DAO,		// �ϴ���ѡ�����ε�˽��Ƶ��
	SAVE_OPT_MARKET_LATEST_GOLD_COIN_BUY,		// �������̳ǹ����б� 
	SAVE_OPT_MARKET_LATEST_TICK_BUY,			// �����ȯ�̳ǹ����б�
	SAVE_OPT_AUTO_ASSIST,						// �Զ���������
	SAVE_OPT_SHORTCUT_FULL_SKILL_INDEX,			// �����Full�󣬻�ü�����������
	SAVE_OPT_VIP_AUTO_ASSIST,					// VIP�߼�����
	SAVE_OPT_AUTO_QUEST,						// �Զ���������
	SAVE_OPT_AUTO_RECOVERY,						// �Զ��ظ�����
};

class GameClientOpt
	: public Ogre::Singleton<GameClientOpt>
{
public:
	GameClientOpt(void);
	~GameClientOpt(void);

public:
	// ���ñ������ݵĴ�С
	void setLevelSize( int nLevel, int nSize );
	// ���ñ��浽�������ϵ���Ϣ
	void setClientOpt( int nLevel, char* szData, bool bDelaySave = true );
	// ��ȡ���浽�������ϵ�������Ϣ
	bool loadClientOpt( int nLevel, int nVersion, char* szOutData );
	// ����������Ϣ
	void setOptContent( const char* szData );

	void update( unsigned int dtick );
private:
	std::map<int,int>		m_mapLevel; 
	int						m_OptLen;                         
	char					m_OptContent[MAX_OPT_LEN]; 
	unsigned int			m_nLastSend;
	bool					m_bNeedSend;
};
