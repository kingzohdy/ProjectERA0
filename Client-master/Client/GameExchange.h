#pragma once
#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "Item.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameActor_Pet.h"

//tolua_begin
// �������׳�����Ϣ
struct tagSrcExchangePet
{
	int m_nListType;	// 0����, 1����, -1����û�г���                       
	int m_nGridIdx;		// ���ϵĸ���λ
	int m_nHeightID;
	int m_nLowID;
	
	bool isValid()
	{
		if( m_nListType == -1 || m_nGridIdx == -1 )
		{
			return false;
		}
		return true;
	}

	tagSrcExchangePet()
	{
		reset();
	}

	void reset()
	{
		m_nListType = -1;
		m_nGridIdx	= -1;
		m_nHeightID	= 0;
		m_nLowID	= 0;
	}
};

enum
{
	CLIENT_MAX_TRADE_MONEY	= INT_MAX,
	CLIENT_MAX_TRADE_DIST	= 10,
};
//tolua_end

class GameExchange//tolua_export
		: public NetMsgHandler, public Ogre::Singleton<GameExchange>
{//tolua_export
public:
	GameExchange(void);
	~GameExchange(void);

public:
	// s->c
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	virtual void update(unsigned int dtick);
	
	//tolua_begin
	
	// c->s
	void test( const char* pszContent );
	void helpTest( const char* pszContent );

	// ������
	void inviteExchange( const char* szName );
	// ��������
	void acceptExchange( const char* pszName );
	// �ܾ�����
	void rejectExchange( const char* pszName, int nRejectType );
	// ȡ������(�ͻ����Լ��رնԻ���)
	void cancelExchange( int nRejectType );
	// ��������
	void lockExchange();
	// ����ȷ��
	void dealExchange();
	// ȡ����������
	void cancelLock();
public:
	
	// ���ý��׵Ľ�Ǯ��Ŀ
	void addExchangeMoney( unsigned int nMoney );
	// ���һ��������Ʒ
	// ����( ��������λ�ã�Ŀ������������ )
	void addExchangeItem( int nIndex, int nListType, int nGrid, int nNum );
	// ���������Ʒ
	void delExchangeItem( int nIndex );
	// ���һ�����׳���
	// ����( ��������λ��, Ŀ������(tagLIST_TYPE 0����, 1����), ���ڳ��������� ) 
	void addExchangePet( int nIndex, int nListType, int nGrid, int nHeightID, int nLowID );
	// ���һ�����׳���
	void delExchangePet( int nIndex );

	// ��ȡ������Ʒ( nType 0Ϊ����1Ϊ���׷�, nIndexΪ�������ϵ�index )
	Item& getExchangItem( int nType, int nIndex );
	
	void synchronizeSelfPet();
    //TODO::�����޸�
	// ��ȡ���׳�����Ϣ( nType 0Ϊ����1Ϊ���׷�, nIndexΪ�������ϵ�index )
	// ע�⣺���ȡ����PetIDΪ0,��ʾ�����λû�г���Ͳ��õ����������ʲ鿴���
    tagRolePet_ToLua* getExchangePetInfo( int nType, int nIndex );
	// ��ȡ���׳��ﾲ̬��Ϣ( nType 0Ϊ����1Ϊ���׷�, nIndexΪ�������ϵ�index )
    
    // �Ƿ������ڽ����е���Ʒ
	bool isInExchangeItem( int nListType, int nGrid );
	// �Ƿ������ڽ����еĳ���
	bool isInExchangePet( int nListType, int nGrid );
	bool isInExchangePetByWID( int nHeightID, int nLowID );
	
	// ���˫���Ƿ����չ������
	bool canExchange( const char* pszName );

	// ��ȡ���׶�������
	const char* getExchangeName()
	{
		return m_strExchangeName.c_str();
	}
	// ��ü������׵Ľ�Ǯ
	unsigned int getSrcMoney()
	{
		return m_nSrcMoney;
	}
	// ��ȡ�Է����׵Ľ�Ǯ
	unsigned int getDesMoney()
	{
		return m_nDesMoney;
	}
	// ���������Ƿ�����
	bool isSrcLockExchange() 
	{
		return m_bSrcLockExchange;
	}
	// �Է��Ƿ�������
	bool isDesLockExchange()
	{
		return m_bDesLockExchange;
	}

	bool isSrcItemlistFull();
	bool isSrcPetlistFull();

	bool isInExchange();
	bool isConfirmExchage();
	
	//tolua_end
private:
	// ��������
	void endExchange();
private:
	Item				m_srcItemList[EXCHG_MAX_ITEM_NUM];	// �Լ�������Ʒ�б�
	Item				m_desItemList[EXCHG_MAX_ITEM_NUM];	// Ŀ�꽻����Ʒ�б�
	tagSrcExchangePet	m_srcPetList[EXCHG_MAX_PET_NUM];	// �����׵ĳ���
	tagRolePet_ToLua	m_desPetList[EXCHG_MAX_PET_NUM];	// Ŀ�꽻�׵ĳ���

	unsigned int		m_nSrcMoney;						// �Լ��Ľ�Ǯ
	unsigned int		m_nDesMoney;						// Ŀ�귽��Ǯ
	
	bool				m_bSrcLockExchange;					// ������������
	bool				m_bDesLockExchange;					// �Է���������
	std::string			m_strExchangeName;					// ���׶�����
	bool				m_bConfirmExchange;					// �Ƿ�ȷ�Ͻ���
};//tolua_export
