#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "OgreXMLData.h"
#include "GameDefineTolua.h"
#include "GameWizard.h"

// using namespace Ogre;

#pragma pack(1)

//tolua_begin

struct tagShopItem_ToLua
{
	unsigned int ItemID;                          	/*   ��Ʒ */
	unsigned int ItemNum;                         	/*   ��Ʒ�ĸ��� */
};

struct tagShopLimitTime_ToLua
{
	char SellBeginTime[DB_TIME_LEN];      	/*   ������Ʒ�Ŀ�ʼ����ʱ�� */
};

struct tagCSShopDef_ToLua
{
	unsigned int ShopID;                          	/*   ��Ʒid */
	char Name[RES_NAME_LEN];              	/*   ��Ʒ���� */
	char Type;                            	/*   ��Ʒ������,��Ӧzone_shopping.xml */
	char SubType;                         	/*   ��Ʒ�������ͣ���Ӧzone_shopping.xml */
	char ShopDesc[MAX_MSG_DESC_LEN];      	/*   ��Ʒ������ */
	unsigned int IconID;                          	/*   ��Ʒ��ͼ�� */
	tagShopItem_ToLua Items[SHOP_ITEM_COUNT];         
	char Flag;                            	/*   ��Ʒ��� Bind Macrosgroup:SHOP_ITEM_FLAG,*/
	char Sale;                            	/*   ���� 100�ʵ��ۿ�,0��ʾ������ */
	unsigned int OrderID;                         	/*   ����0��ʱ�򣬲�ָ���ŵ��̳ǵ��Ǹ�λ�� */
	char BuyType;                         	/*   1��ȯ 2-��� */
	unsigned int BuyPrice;                        	/*   ���̳�����Ĺ���۸� */
	unsigned int BuyUIType;                       	/*  Ver.409 ���ٹ�����Ʒ��ʾ�ĵط�,��ο�SHOPPING_BUY_UI_TYPE */
	char ShowDate[DATE_TIME_LEN];         	/*  Ver.449 ��Ʒ���ϼ�ʱ�� */
	char HideDate[DATE_TIME_LEN];         	/*  Ver.449 ��Ʒ���¼�ʱ��,���û���������¼�ʱ��,Ĭ���ϼ� */
	int LimitDate;                       	/*  Ver.2332 �������������һ�� */
};

struct tagTradeRecord_ToLua
{
	char TradeType;                       	/*   0��ʾ���ǳ�������,1��ʾ���������� */
    unsigned long long TradeID;                         	/*   ���׵��˵���� */
    unsigned int Money;                           	/*   ��ص����� */
    unsigned int Gold;                            	/*   ��صĽ�� */
    unsigned int CountedDownTime;                 	/*   ����ʱ �� 0����ʵ�ѳ��� */
    char OwnerName[ROLE_NAME_LEN];        	/*   �ձ�ʾ���� */
};

struct tagShopLimitDef_ToLua
{
	unsigned int ShopID;                          	/*   ��Ʒid ���� */
	unsigned int LimitSellTotalNum;               	/*   ������������� */
	unsigned int LimitSelledNum;                  	/*   �Ѿ�����ȥ���ٸ�,�߻����ֵ������ */
	tagShopLimitTime_ToLua ShopLimitTime[MAX_SHOP_LIMIT_ITME_COUNT]; 	/*   ����ˢ��ʱ�� */
	char ModifyFlag;                      	/*   ��д��־λ,������ */
	unsigned int FreshTime;                       	/*  Ver.228 ˢ��������ʱ��,������ */
};

struct tagTradeRecordSortInfo_ToLua
{
	float UnitPrice;
	tagTradeRecord_ToLua TradeRecordData;
};
#pragma pack()

#define ONE_PAGE_NUM	12	//ÿҳ����ʾ�ĸ���
#define ONE_PAGE_TRADE_LIST_NUM 14	//�����б��У�һҳ��ʾ����������

struct SubTypeLabel
{
	SubTypeLabel()
	{
		key = 0;
		memset(labelName, 0, sizeof(labelName));
	};

	int		key;
	char	labelName[16];
};

struct TypeLabel
{
	int			key;			//��ǩID
	int			subLabelCount;	//�ӱ�ǩ����
	SubTypeLabel subLabel[20];	//�ӱ�ǩ����
	char		labelName[16];	//��ǩ����
};

enum 
{
	TRADE_LIST_SALE_TYPE = 0,	//�����б�
	TRADE_LIST_BUY_TYPE = 1,	//���б�
	TRADE_LIST_MY_TRADE_TYPE = 2	//�ҵĶ���
};

enum
{
	GET_PROPS_TYPE = 1,	//��ֵ���
	GET_ITEM_TYPE,		//9C�Ļ������
};

enum  
{
	SHOPPING_BUY_UI_SHANGCHENG_TOLUA = 0, 	/* Ĭ�����̳���幺����Ʒ */
	SHOPPING_BUY_UI_PACKAGE_TOLUA = 1, 	/* ������ݹ��� */
	SHOPPING_BUY_UI_RELIVE_TOLUA = 3, 	/* �����ݹ��� */
};

enum
{
	BUY_TYPE_TICKET		= 1,	//���
	BUY_TYPE_BINDGOLD	= 2,	//��ȯ
};

enum tagSHOP_ITEM_FLAG_TOLUA 
{
	SHOP_ITEM_FLAG_NO_TOLUA = 0, 	/* �ޱ�� */
	SHOP_ITEM_FLAG_DISCOUNT_TOLUA = 1, 	/* ���� */
	SHOP_ITEM_FLAG_HOT_TOLUA = 2, 	/* ���� */
	SHOP_ITEM_FLAG_LIMIT_TOLUA = 3, 	/* ���� */
	SHOP_ITEM_FLAG_NEW_TOLUA = 4, 	/* ��Ʒ */
	SHOP_ITEM_FLAG_PANIC_TOLUA = 5, 	/* ���� */
};

enum
{
	RECORD_SORT_NUM			= 1,	//���׳���������ѯ
	RECORD_SORT_PRICE		= 2,	//���׳��ۼ۸��ѯ
	RECORD_SORT_UNIT_PRICE	= 3,	//���׵��۲�ѯ
};

enum
{
	RECORD_SELL_TRADE_LIST	= 1,	//��ҳ����б�
	RECORD_BUY_TRADE_LIST	= 2,	//���ҳ����б�
	RECORD_MY_TRADE_LIST	= 3,	//�ҵĶ����б�
};
#define MAX_CAN_BUY_NUM 99

#define SHOP_ITEM_COUNT_TOLUA SHOP_ITEM_COUNT

long string2time(const char * szTime);

//tolua_end
bool operator== ( const tagTradeRecordSortInfo_ToLua& recordInfo, const tdr_ulonglong tradeID );

bool TradeRecordCompare( const tagTradeRecordSortInfo_ToLua& lhs, const tagTradeRecordSortInfo_ToLua& rhs );
bool UnitPriceLessThan( const tagTradeRecordSortInfo_ToLua& lhs, const tagTradeRecordSortInfo_ToLua& rhs );

typedef std::vector<TypeLabel> VTYPELABEL;
typedef std::map<int,VTYPELABEL> TYPELABELMAP;

typedef std::vector<SHOPDEF> VSHOPITEM;

typedef std::vector<SHOPLIMITDEF> VSSHOPLIMIT;

class CMarketManager//tolua_export
	:public NetMsgHandler,public Ogre::Singleton<CMarketManager>
{//tolua_export
public:
	CMarketManager();
	~CMarketManager();
	
	void create();
	virtual int	HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

	void resetMarketInfo();
	bool onGetXmlResponse( const CSSHOPPINGGETXMLFILERES& xmlData );
	void ParserTypeLabel( Ogre::XMLNode node, VTYPELABEL& vLabel );
	void ParserSubType( Ogre::XMLNode node, TypeLabel* label );

	//tolua_begin
	int	 getLabelCount( int nBuyType );
	int	 getSubLabelCount( int nBuyType, int labelIndex );
	TypeLabel* getTypeLabel( int nBuyType, int labelIndex );
	int getCurSaleTotalPage(int nType,int nSubType);
	//������
	void requestBuy(int id,int num,int giftFlag,const char* GiftToPlayerName,const char* szDesc,int nBuyFlag=0);
	//����ӱ���������Ʒ
	void requestPackagePresent(int nGridIndex,int num,const char* GiftToPlayerName,const char* szDesc);
	//��ȯ����
	void requestTrade( int type,unsigned long long tradeId );
	//�������׵�
	void requestSendTradeRecord(int type, int nMoney,int nGold,int nTime,int nAnonymous);
	//�����ѯ��ȯ
	void requestQueryTianyuan();
	//����ȫ��ת�뱳��
	void requestMoveAllToBag();
	//�����ѯ
	void searchTradeByID(char* szID,int nOperate);
	//�����ҵĶ���
	void destroyTradeRecord(unsigned long long nTradeID,int nType);
	//void destroyTradeRecord(int nHeigh,int nLow,int nType);
	bool checkMyTradeRecord( unsigned long long nTradeID );
	//�����Ʒ����
	tagCSShopDef_ToLua* getMarketItem( int nIndex,int nSelType,int nSelSubType );
	void onSelectType( int nSelType,int nSelSubType,int nBuyType );
	void showTradeInfo( int nCurSelTradeType,int nPage );
	tagCSShopDef_ToLua* getItemByID( int shopItemID );
	int getCanByNum( int shopItemID );
	int getSubTypeItemNum(int nSelType,int nSelSubType,int nBuyType);

	tagTradeRecord_ToLua* getTradeRecord( int nIndex );

	int	getTotalRecordNum();

	void selectMarketItem(int nItemID);

	void requestOpenMarket();

	int	getShopItemNum();
	tagCSShopDef_ToLua* getShopItemByIndex( int nIdex );
	
	//����һ���ȯ
	void requestExchangePoint( int nNum );

	void searchItems( const char* name,int nBuyType );

	tagShopLimitDef_ToLua* getShopLimit( int shopID );

	const char* getNextRefreshTime( const tagShopLimitDef_ToLua* limit );

	const char* getShowMsg();

	int getCurDianquan(){ return m_nCurDianquan; }

	bool CheckShowDate();
	
	void SetBuyPreviewState( bool state );


	const tagTradeRecordSortInfo_ToLua* getOneTradeInfoList( int nPage, int nRow, int listType  ) const;
	void	sortTradeRecord( int nType, int nFlag,int tradeType );
	int		getCurTradeTotalPage(int nType );
	float	getUnitPriceMin( int nType );
	void	setTradeRecordByNum( int tradeType, int num);
	void	ChangeRecordName( const char* oldName, const char* newName );
	int		getShopIDByID( int buyType, int ItemID );
	void	openAdLink( const char* url );
	//tolua_end

	//�����ѯ��ȯ�����б�
	void requestQueryTradeSell(unsigned int nBeginNum,unsigned int nTotalNum);
	//�����ѯ��ȯ���б�
	void requestQueryTradeBuy(unsigned int nBeginNum,unsigned int nTotalNum);
	//�����ѯ�ҵĶ���
	void requestQueryMyRecord(int nNum, int nTotalNum);

	bool onFreshResponse(const CSSHOPPINGFRESHRECORDRES& record);

	bool onModifyResponse(const CSSHOPPINGMODIFYRECORDRES& record);

	bool onQueryTradeSellResponse(const CSSHOPPINGQUERYTRADESELLRES& tradeSells);

	bool onQueryTradeBuyResponse(const CSSHOPPINGQUERYTRADEBUYRES& tradeBuys);

	bool onPackagePresentResponse(const CSSHOPPINGPACKAGEPRESENTRES& packagePresent);

	bool onTradeResponse(const CSSHOPPINGTRADERES& tradeRes);

	bool onSendTradeRecordResponse(const CSSHOPPINGSENDTRADERECORDRES& sendRecord);

	bool onQueryMyTradeRecordResponse(const CSSHOPPINGQUERYMYTRADERECORDRES& myRecord);

	bool onSearchTradeRespone(const CSSHOPPINGQUERYRECORDBYIDRES& record);

	bool onExchangePointResponse( const CSSHOPPINGEXCHANGERES& exchangePoint );

	bool onDestroyTradeRecord(const CSSHOPPINGDESTROYTRADERECORDRES& record);

	void setMyDisplayRecord();

	bool isInShowTime( const SHOPDEF& shopdef );

	void checkLatestBuyData();

	char* getLatestGoldCoinBuy(){ return (char*)m_LatestGoldCoinBuy; }
	char* getLatestTicketBuy(){ return (char*)m_LatestTicketBuy; }

	void getLatestType( int& nType, int& nSubType, int nBuyType );
	const char* getTypeName( int nType, int nBuyType );
	//elmer /**************/
	void update();
	void modifyTradeRecordData( tagTradeRecord recordData ,int recordType );
	int  getRecordSortType() { return m_RecordSortType; }
	int  getRecordSortFlag(){ return m_RecordSortFlag; }

private:
	void setRecordInfo( tagTradeRecordSortInfo_ToLua* pRecordInfo, const void* pInfo );
	/**************/
private:

	VSHOPITEM	m_MarketItem;	//�̳���Ʒ
	VSHOPITEM	m_CurSelItem;	//��ǰѡ�������µ���Ʒ
	SHOPMSGDEF	m_showMsg;		//������Ϣ
	TYPELABELMAP m_TypeLabel;	//��ǩ����
	VSSHOPLIMIT m_ShopLimit;

	//��ȯ�����б�
	TRADERECORD m_TradeRecord[ONE_PAGE_TRADE_LIST_NUM];
	int			m_nTradeNum;
	int			m_nTotalNum;

	//elmer
	int			m_RecordSortType;
	int			m_RecordSortFlag;
	//��ҽ�������
	int			m_nSellRecordNum;
	int			m_nSellRecordIndex;
	int			m_nSellTotalNum;
	std::vector<tagTradeRecordSortInfo_ToLua>	m_sellRecordVec;
	std::vector<tagTradeRecordSortInfo_ToLua>	m_sellChosenVec;
	//���ҽ�������
	int			m_nBuyRecordNum;
	int			m_nBuyRecordIndex;
	int			m_nBuyTotalNum;
	std::vector<tagTradeRecordSortInfo_ToLua>	m_buyRecordVec;
	std::vector<tagTradeRecordSortInfo_ToLua>	m_buyChosenVec;
	//�ҵĶ���
	bool		m_bMyTradeFlag;
	std::vector<tagTradeRecordSortInfo_ToLua>   m_myTradeVec;


	unsigned int	m_uLastRequestTick;
	
	//�ҵĶ���
	TRADERECORD m_myTradeRecord[MAX_TRADE_RECORD];
	int			m_nMyTradeNum;
	int			m_nCurMyPage;

	int			m_nCurDianquan;

	time_t		m_LastRefreshTime;
	//����Ԥ����װ
	unsigned int	m_uLastBuyTick;
	bool		m_bBuyPreviewItem;

	// �������
	int			m_LatestGoldCoinBuy[ONE_PAGE_NUM];	// �����ҹ���
	int			m_LatestTicketBuy[ONE_PAGE_NUM];		// �����ȯ����
};//tolua_export