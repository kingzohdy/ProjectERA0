#pragma once
#include "OgreSingleton.h"
#include "Item.h"  
#include "GameActor_Pet.h"
#include "GameDefineTolua.h"

//tolua_begin 
struct tagStallPetEntity_ToLua
{
	int					m_nDefID;                          
	tagPetId_ToLua		m_WID;                            
	int					m_nListType;                       
	int					m_nGridIdx;                        
	int					m_nStallGridIdx;                   
	tagRolePet_ToLua	m_RolePet;  
	unsigned int		m_uPrice;  
	tagLong_ToLua		m_GridGID;
};

struct tagStallData_ToLua
{
	char					m_szRoleName[NAME_LEN];
	char					m_szStallName[STALL_MAX_NAME_LEN]; 
	char					m_szStallAd[STALL_MAX_NAME_LEN];
	int						m_nItemNum;                        
	Item					m_ItemData[STALL_MAX_ITEM];       
	int						m_nPetNum;                         
	tagStallPetEntity_ToLua m_PetData[STALL_MAX_PET];         
};

enum
{
	CLIENT_STALL_MAX_PET		= STALL_MAX_PET,
	CLIENT_STALL_MAX_ITEM_PAGE	= STALL_MAX_ITEM_PAGE,
	CLIENT_STALL_MAX_ITEM		= STALL_MAX_ITEM,
	CLIENT_MIN_STALL_LEVEL		= 20,
};

enum
{
	CLIENT_STALL_START	= 0,
	CLIENT_STALL_END	= 1,
};

enum
{
	CLIENT_STALL_TYPE_ITEM	= STALL_TYPE_ITEM,
	CLIENT_STALL_TYPE_PET	= STALL_TYPE_PET,
};

enum
{
	CLIENT_MIN_STALL_DIST_BETWEEN_NPC = 300,
	CLIENT_MIN_STALL_DIST_BETWEEN_OTHER_STALL = 300,
};

enum
{
	STALL_CHG_ITEM_INSTALL_FAILED	= 0,
	STALL_CHG_ITEM_INSTALL_SUCCESS	= 1,
};

enum
{
	STALL_CHG_AD_FAILED		= 0,
	STALL_CHG_AD_SUCCESS	= 1,
};

enum
{
	CLIENT_STALL_MAP = 1000,
	CLIENT_MAX_MONEY = MAX_MONEY,
};

unsigned int getMaxUint();
//tolua_end

class GameStallManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameStallManager>
{//tolua_export
public:
	GameStallManager(void);
	~GameStallManager(void);

public:

	// c->s
	
	//tolua_begin 
	// ��ʼ��̯
	bool isSellGoodsMoneyTooMuch();
	void stall_Start( const char* szStallName );
	// ������̯
	void stall_End();
	// ̯λ����
	void stall_ChangeName( const char* szName );
	// ̯λ������
	void stall_ChangeAd( const char* pszAd );
	// ������̯λ������Э��
	void localChangeAd( const char* pszAd );
	// ̯λ�鿴
	void stall_See( int nMemId );
	// ���� stalltype 0 ��Ʒ 1 ����
	void stall_Buy( int nStallType, int nGridIdx, int nNum, const char* szName );
	// ������λ
	void stall_AddGrid();
	// ��̯״̬�¼ܳ���
	void	delPetInStall( int nGrid );
	// ��̯״̬���¼ܵ���
	void	delItemInStall( int nGrid );
	// ���ð�̯��Ϣ
	void resetAllStallInfo();
	void resetStallInfo();
	// ��������ȡ�Է�̯λ��Ϣ
	void resetSeeStallInfo();
	// ��ȡ��̯����λ��Ϣ
	int getStallGridNum();
	// ��ȡ̯������λ��Ϣ
	int getSeeStallItemNum();
	//tolua_end

	// s->c
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	virtual void update(unsigned int dtick);

public:
	void ValidateLastStallItems();
	void ValidateLastStallPets();
	//tolua_begin
	void ValidateLastStallGoods();
	// �Ƿ�û���κ���Ʒ�ϼ�
	bool isNoneGoodsOnStall();
	// �Ƿ���NPC�������
	bool isNpcTooNear();
	// �Ƿ�������̯λ�������
	bool isOtherStallTooNear();
	// ��Ӱ�̯��Ʒ
	void addStallItem( int nStallGridIdx, int nListType, int nGrid, int nNum, unsigned int nPrice );
	// ��̯״̬�ϼܵ���
	void addStallItemInStall( int nStallGridIdx, int nListType, int nGrid, int nNum, unsigned int nPrice );
	// �Ƴ���̯��Ʒ
	void delLastStallItem( int nIndex );
	void delStallItem( int nIndex );
	// �Ƴ���̯����
	void delStallPet( int nIndex );
	// ��Ӱ�̯����
	void addStallPet( int nStallIndex, int nType, int nIndex, unsigned int nPrice );
	// ��̯״̬��Ӱ�̯����
	void addStallPetInStall( int nStallIndex, int nType, int nIndex, unsigned int nPrice );
	// ���̯λ�ϵ���Ʒ�Ƿ��Ѱ���
	bool isStallItemListFull();
	// �Ƿ�������̯λ�ϼܵ���Ʒ
	bool isInStallItem( int nListType, int nGrid );
	// ���̯λ�ϵĳ����Ƿ��Ѱ���
	bool isStallPetListFull();
	// �Ƿ�����̯λ�ϼܵĳ���
	bool isInStallIPet( int nType, int nGrid );
	bool isInStallIPet( int nType, int nHeightID, int nLowID );

	// ��ȡ�Լ�������̯����Ʒ
	void SaveLastStallGoodsToStall();
	void PutLastStallGoodsToStall();
	const tagStallData_ToLua& getSelfStallItem() const;
	const tagStallData_ToLua& getSelfLastStallItem() const;
	// ��ȡ�鿴�����̯����Ʒ
	const tagStallData_ToLua& getSeeStallItem() const;
	// nType 0��ʾ�Լ���1��ʾ�Է�
	bool	getItemLink( int nType, int GridIndex );
	bool	canSeeStall();
	// ���˫���Ƿ����չ������
	bool	canStartStall( );

	bool	isInStallArea();
	// �Ƿ�����¼���Ʒ
	bool	canRemoveGoods();

	Item*	getSaledItem( int idx );
	//tolua_end

private:
	void OnStallSeeRes( const STALLSEERES& stallSeeResData );
	void OnStallBuyerChgRes( const STALLINFOCHG& stallBuyChgResData );
	void OnStallSellerChgRes( const STALLINFOCHG& stallSellerChgData );
	void OnStallUpRes( const STALLUPRES& stallUpResData );
	void OnStallDownRes( const STALLDOWNRES& stallDownResData );

private:
	tagStallData_ToLua	m_SelfStallItem;		// �Լ�������̯����Ʒ
	tagStallData_ToLua	m_LastSelfStallItem;		// �Լ�������̯����Ʒ
	tagStallData_ToLua	m_SeeStallItem;			// �鿴�İ�̯��Ʒ
	int					m_StallGridNum;			// ��̯����λ��

	std::vector<Item>	m_SaledItem;			// �Ѿ�����ȥ����Ʒ
};//tolua_export
