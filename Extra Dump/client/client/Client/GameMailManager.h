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
	tagLong_ToLua	m_WID;                             	/*   邮件全局唯一ID */
	char			m_szRecv[ROLE_NAME_LEN_SAVE];       /*   收件人 */
	char			m_szSend[ROLE_NAME_LEN_SAVE];       /*   发件人 */
	int				m_nTime;                            /*   发件时间 */
	char			m_szTitle[NAME_LEN];				/*   标题 */
	int				m_nFlags;                           /*   标志 Bind Macrosgroup:MAIL_FLAG,*/
};

struct tagMailDetail_ToLua
{
	char			m_szText[SHORT_DESC_LEN];           /*   正文 */
	int				m_ItemNum;                         	/*   邮寄的物品数量 */                            
	Item			m_Items[MAIL_MAX_ITEM];            	/*   邮寄的物品 */
	unsigned int	m_uGold;                            /*   邮寄的金币 */
};

struct tagMailData
{
	tagMailHead_ToLua		m_MailHead;			// 邮件头
	tagMailDetail_ToLua		m_MailDetail;		// 邮件主体
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

	// 阅读邮件
	void mail_Read( int nHeight, int nLow );
	// 删除邮件
	void mail_Delete( int nHeight, int nLow );
	// 发送邮件
	void mail_Send( const char* szToName, const char* szTitle, const char* szText, int nListType, int nGridIndex );
	// 收取邮件
	void mail_GetGoods( int nHeight, int nLow, int nIndex );
	// 设置发送邮件内容
	void addSendItem( const Item& item, int nIndex, int nNum );
	// 移除要发送邮件的物品
	void delSendItem( int nIndex );
	// 添加发送的金钱
	void addSendGold( int nGold );
	// 获得即将发送的物品信息
	const tagMailSendInfo& getSendData()const;
	// 清空发送物品
	void clearSendItem();
	// 获取一页的邮件头数据
	const tagPageMail* getOnePageMail( int nPage );
	// 获取邮件的某一格的所有物品
	void mail_GetMoney( int nHeight, int nLow );
	// 获取邮件里所有的金币和物品
	void mail_GetAll( int nHeight, int nLow,int nDel=0 );
	// 删除角色邮箱里所有邮件
	void mail_Delete_All( void );
	// 取出邮箱里的所有物品
	void mail_Get_All( void );
	// 重置邮件数据
	void  resetMailData();
	// 预判断邮件发送状况
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
