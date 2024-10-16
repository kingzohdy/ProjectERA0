#pragma once
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameDefineTolua.h"
#include "Item.h"
#include "NetMessage.h"
#include <map>
#include <math.h>

//tolua_begin 

#define MAIL_FLAG_GOLD_TOLUA MAIL_FLAG_GOLD
struct tagMailHead_ToLua
{
	tagLong_ToLua	m_WID;                             	/*   �ʼ�ȫ��ΨһID */
	char			m_szRecv[ROLE_NAME_LEN_SAVE];       /*   �ռ��� */
	char			m_szSend[ROLE_NAME_LEN_SAVE];       /*   ������ */
	int				m_nTime;                            /*   ����ʱ�� */
	char			m_szTitle[NAME_LEN];				/*   ���� */
	int				m_nFlags;                           /*   ��־ Bind Macrosgroup:MAIL_FLAG,*/
};

struct tagMailDetail_ToLua
{
	char			m_szText[SHORT_DESC_LEN];           /*   ���� */
	int				m_ItemNum;                         	/*   �ʼĵ���Ʒ���� */                            
	Item			m_Items[MAIL_MAX_ITEM];            	/*   �ʼĵ���Ʒ */
	unsigned int	m_uGold;                            /*   �ʼĵĽ�� */
};

struct tagMailData
{
	tagMailHead_ToLua		m_MailHead;			// �ʼ�ͷ
	tagMailDetail_ToLua		m_MailDetail;		// �ʼ�����
};

struct tagPageMail
{
	int				m_nPageId;
	int				m_nMailNum;
	int				m_nTotalHeadNum;
	tagMailData		m_MainData[MAIL_MAX_HEAD_PAGE_NUM];
};
struct tagMailSendInfo
{
	int		m_nGold;
	int		m_nItemNum;
	Item	m_Items[MAIL_MAX_ITEM];   
};

enum 
{
	CLIENT_MAIL_PACKAGE_ID			= MAIL_PACKAGE_ID,
	CLIENT_MAIL_BINDED_PACKAGE_ID	= MAIL_BINDED_PACKAGE_ID
};
//tolua_end

class GameMailManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameMailManager>
{//tolua_export
public:
	GameMailManager(void);
	~GameMailManager(void);

public:
	
	// c->s	
	//tolua_begin

	// �Ķ��ʼ�
	void mail_Read( int nHeight, int nLow );
	// ɾ���ʼ�
	void mail_Delete( int nHeight, int nLow );
	// �����ʼ�
	void mail_Send( const char* szToName, const char* szTitle, const char* szText, int nListType, int nGridIndex );
	// ��ȡ�ʼ�
	void mail_GetGoods( int nHeight, int nLow, int nIndex );
	// ���÷����ʼ�����
	void addSendItem( const Item& item, int nIndex, int nNum );
	// �Ƴ�Ҫ�����ʼ�����Ʒ
	void delSendItem( int nIndex );
	// ��ӷ��͵Ľ�Ǯ
	void addSendGold( int nGold );
	// ��ü������͵���Ʒ��Ϣ
	const tagMailSendInfo& getSendData()const;
	// ��շ�����Ʒ
	void clearSendItem();
	// ��ȡһҳ���ʼ�ͷ����
	const tagPageMail* getOnePageMail( int nPage );
	// ��ȡ�ʼ���ĳһ���������Ʒ
	void mail_GetMoney( int nHeight, int nLow );
	// ��ȡ�ʼ������еĽ�Һ���Ʒ
	void mail_GetAll( int nHeight, int nLow,int nDel=0 );
	// ɾ����ɫ�����������ʼ�
	void mail_Delete_All( void );
	// ȡ���������������Ʒ
	void mail_Get_All( void );
	// �����ʼ�����
	void  resetMailData();
	// Ԥ�ж��ʼ�����״��
	int checkMailSend(unsigned int nMoney );
	//tolua_end

	void changeMailSendUserName( const char* pszOldName, const char* pszNewName );
public:
	// s->c
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

private:
	std::vector<tagMailData>	m_MailData;
	tagPageMail					m_PageMail;
	tagMailSendInfo				m_SendData;
	std::vector<tdr_ulonglong>	m_MailRead;
};//tolua_export
