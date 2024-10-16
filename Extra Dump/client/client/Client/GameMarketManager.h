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
	unsigned int ItemID;                          	/*   物品 */
	unsigned int ItemNum;                         	/*   物品的个数 */
};

struct tagShopLimitTime_ToLua
{
	char SellBeginTime[DB_TIME_LEN];      	/*   限量物品的开始卖的时间 */
};

struct tagCSShopDef_ToLua
{
	unsigned int ShopID;                          	/*   商品id */
	char Name[RES_NAME_LEN];              	/*   商品名字 */
	char Type;                            	/*   商品的类型,对应zone_shopping.xml */
	char SubType;                         	/*   商品的子类型，对应zone_shopping.xml */
	char ShopDesc[MAX_MSG_DESC_LEN];      	/*   商品的描述 */
	unsigned int IconID;                          	/*   商品的图标 */
	tagShopItem_ToLua Items[SHOP_ITEM_COUNT];         
	char Flag;                            	/*   商品标记 Bind Macrosgroup:SHOP_ITEM_FLAG,*/
	char Sale;                            	/*   打折 100率的折扣,0表示不打折 */
	unsigned int OrderID;                         	/*   大于0的时候，才指定放到商城的那个位置 */
	char BuyType;                         	/*   1金券 2-金币 */
	unsigned int BuyPrice;                        	/*   在商城里面的购买价格 */
	unsigned int BuyUIType;                       	/*  Ver.409 快速购买商品显示的地方,请参考SHOPPING_BUY_UI_TYPE */
	char ShowDate[DATE_TIME_LEN];         	/*  Ver.449 商品的上架时间 */
	char HideDate[DATE_TIME_LEN];         	/*  Ver.449 商品的下架时间,如果没有配置上下架时间,默认上架 */
	int LimitDate;                       	/*  Ver.2332 隔多少天才能买一次 */
};

struct tagTradeRecord_ToLua
{
	char TradeType;                       	/*   0表示的是出售数据,1表示的是求购数据 */
    unsigned long long TradeID;                         	/*   交易的账单编号 */
    unsigned int Money;                           	/*   相关的银币 */
    unsigned int Gold;                            	/*   相关的金币 */
    unsigned int CountedDownTime;                 	/*   倒计时 秒 0的现实已撤单 */
    char OwnerName[ROLE_NAME_LEN];        	/*   空表示匿名 */
};

struct tagShopLimitDef_ToLua
{
	unsigned int ShopID;                          	/*   商品id 自增 */
	unsigned int LimitSellTotalNum;               	/*   限量购买的总数 */
	unsigned int LimitSelledNum;                  	/*   已经卖出去多少个,策划这个值不用填 */
	tagShopLimitTime_ToLua ShopLimitTime[MAX_SHOP_LIMIT_ITME_COUNT]; 	/*   限量刷新时间 */
	char ModifyFlag;                      	/*   回写标志位,程序用 */
	unsigned int FreshTime;                       	/*  Ver.228 刷新总数的时间,程序用 */
};

struct tagTradeRecordSortInfo_ToLua
{
	float UnitPrice;
	tagTradeRecord_ToLua TradeRecordData;
};
#pragma pack()

#define ONE_PAGE_NUM	12	//每页能显示的个数
#define ONE_PAGE_TRADE_LIST_NUM 14	//交易列表中，一页显示的数据条数

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
	int			key;			//标签ID
	int			subLabelCount;	//子标签数量
	SubTypeLabel subLabel[20];	//子标签数据
	char		labelName[16];	//标签标题
};

enum 
{
	TRADE_LIST_SALE_TYPE = 0,	//出售列表
	TRADE_LIST_BUY_TYPE = 1,	//求购列表
	TRADE_LIST_MY_TRADE_TYPE = 2	//我的订单
};

enum
{
	GET_PROPS_TYPE = 1,	//冲值礼包
	GET_ITEM_TYPE,		//9C的积分礼包
};

enum  
{
	SHOPPING_BUY_UI_SHANGCHENG_TOLUA = 0, 	/* 默认在商城面板购买商品 */
	SHOPPING_BUY_UI_PACKAGE_TOLUA = 1, 	/* 背包快捷购买 */
	SHOPPING_BUY_UI_RELIVE_TOLUA = 3, 	/* 复活快捷购买 */
};

enum
{
	BUY_TYPE_TICKET		= 1,	//金币
	BUY_TYPE_BINDGOLD	= 2,	//金券
};

enum tagSHOP_ITEM_FLAG_TOLUA 
{
	SHOP_ITEM_FLAG_NO_TOLUA = 0, 	/* 无标记 */
	SHOP_ITEM_FLAG_DISCOUNT_TOLUA = 1, 	/* 打折 */
	SHOP_ITEM_FLAG_HOT_TOLUA = 2, 	/* 热销 */
	SHOP_ITEM_FLAG_LIMIT_TOLUA = 3, 	/* 限量 */
	SHOP_ITEM_FLAG_NEW_TOLUA = 4, 	/* 新品 */
	SHOP_ITEM_FLAG_PANIC_TOLUA = 5, 	/* 抢购 */
};

enum
{
	RECORD_SORT_NUM			= 1,	//交易出售数量查询
	RECORD_SORT_PRICE		= 2,	//交易出售价格查询
	RECORD_SORT_UNIT_PRICE	= 3,	//交易单价查询
};

enum
{
	RECORD_SELL_TRADE_LIST	= 1,	//金币出售列表
	RECORD_BUY_TRADE_LIST	= 2,	//银币出售列表
	RECORD_MY_TRADE_LIST	= 3,	//我的订单列表
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
	//请求购买
	void requestBuy(int id,int num,int giftFlag,const char* GiftToPlayerName,const char* szDesc,int nBuyFlag=0);
	//请求从背包赠送物品
	void requestPackagePresent(int nGridIndex,int num,const char* GiftToPlayerName,const char* szDesc);
	//点券交易
	void requestTrade( int type,unsigned long long tradeId );
	//发布交易单
	void requestSendTradeRecord(int type, int nMoney,int nGold,int nTime,int nAnonymous);
	//请求查询点券
	void requestQueryTianyuan();
	//请求全部转入背包
	void requestMoveAllToBag();
	//请求查询
	void searchTradeByID(char* szID,int nOperate);
	//撤销我的订单
	void destroyTradeRecord(unsigned long long nTradeID,int nType);
	//void destroyTradeRecord(int nHeigh,int nLow,int nType);
	bool checkMyTradeRecord( unsigned long long nTradeID );
	//获得商品数据
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
	
	//请求兑换点券
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

	//请求查询点券出售列表
	void requestQueryTradeSell(unsigned int nBeginNum,unsigned int nTotalNum);
	//请求查询点券求购列表
	void requestQueryTradeBuy(unsigned int nBeginNum,unsigned int nTotalNum);
	//请求查询我的订单
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

	VSHOPITEM	m_MarketItem;	//商城商品
	VSHOPITEM	m_CurSelItem;	//当前选择的类别下的物品
	SHOPMSGDEF	m_showMsg;		//公告消息
	TYPELABELMAP m_TypeLabel;	//标签数据
	VSSHOPLIMIT m_ShopLimit;

	//点券出售列表
	TRADERECORD m_TradeRecord[ONE_PAGE_TRADE_LIST_NUM];
	int			m_nTradeNum;
	int			m_nTotalNum;

	//elmer
	int			m_RecordSortType;
	int			m_RecordSortFlag;
	//金币交易数据
	int			m_nSellRecordNum;
	int			m_nSellRecordIndex;
	int			m_nSellTotalNum;
	std::vector<tagTradeRecordSortInfo_ToLua>	m_sellRecordVec;
	std::vector<tagTradeRecordSortInfo_ToLua>	m_sellChosenVec;
	//银币交易数据
	int			m_nBuyRecordNum;
	int			m_nBuyRecordIndex;
	int			m_nBuyTotalNum;
	std::vector<tagTradeRecordSortInfo_ToLua>	m_buyRecordVec;
	std::vector<tagTradeRecordSortInfo_ToLua>	m_buyChosenVec;
	//我的订单
	bool		m_bMyTradeFlag;
	std::vector<tagTradeRecordSortInfo_ToLua>   m_myTradeVec;


	unsigned int	m_uLastRequestTick;
	
	//我的订单
	TRADERECORD m_myTradeRecord[MAX_TRADE_RECORD];
	int			m_nMyTradeNum;
	int			m_nCurMyPage;

	int			m_nCurDianquan;

	time_t		m_LastRefreshTime;
	//购买预览服装
	unsigned int	m_uLastBuyTick;
	bool		m_bBuyPreviewItem;

	// 最近购买
	int			m_LatestGoldCoinBuy[ONE_PAGE_NUM];	// 最近金币购买
	int			m_LatestTicketBuy[ONE_PAGE_NUM];		// 最近金券购买
};//tolua_export