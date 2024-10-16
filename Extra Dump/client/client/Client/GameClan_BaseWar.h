#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

#pragma pack(1)
//tolua_begin
#define STRONG_STAT_NO_TOLUA		STRONG_STAT_NO				/*  */
#define STRONG_STAT_IDLE_TOLUA		STRONG_STAT_IDLE			/* ����ʱ�� */
#define STRONG_STAT_SALE_TOLUA		STRONG_STAT_SALE			/* ����ʱ�� */
#define STRONG_STAT_TONGJI_TOLUA	STRONG_STAT_TONGJI			/* ͳ��ʱ�� */
#define STRONG_STAT_SELECT_TOLUA	STRONG_STAT_SELECT			/* ��Ҳ�ѯʱ�� */
#define STRONG_STAT_FIGHT_TOLUA		STRONG_STAT_FIGHT			/* ����ʱ�� */

#define CLAN_TECH_CATEGORY_SHAPE_TOLUA CLAN_TECH_CATEGORY_SHAPE	/* ��״�Ƽ�*/
#define CLAN_TECH_CATEGORY_SHADING_TOLUA CLAN_TECH_CATEGORY_SHADING	/* ���ƿƼ�*/
#define CLAN_TECH_CATEGORY_MARK_TOLUA CLAN_TECH_CATEGORY_MARK/* ��ǿƼ�*/


struct tagBannerDef_ToLua
{
	short Type;                           
	short SubType;                        
	char TechLimit;                      
	int ClanMoney;                      
	int IconID;                         
	char Name[RES_NAME_LEN];              	/*  Ver.1593 xxx��״,xxx���Ƶ� */
};
struct tagStrongMon_ToLua
{
	int Flag;                            	/*  Ver.1616 1-��ʾ��������Դ�� */
	int MemID;                          
	unsigned int ClanID;                         
	tdr_ulonglong ClanWID;                        
	int X;                               	/*  Ver.1598  */
	int Y;                               	/*  Ver.1598  */
};
struct tagStrongSaleInfo_ToLua
{
	int NpcID;                          
	int MapID;                           	/*  Ver.1563  */
	int SaleStat;                       
	int ClanJoin;                       
	int ClanPostion;                    
	int ClanMoney;                      
	int AtkClanMoney;                   
	tdr_ulonglong AtkClanWid;                     
};

struct tagStrongInfoOne_ToLua
{
	int MapID;                          
	int IsOpen;                          	/*  Ver.1613 0-δ���� 1-���� */
	tdr_ulonglong ClanWid;                        
};
struct tagAwardOre_ToLua
{
	int OreID;                          
	int Num;                            
}; 
struct tagMapAwardOreDef_ToLua
{
	int MapID;                          
	tagAwardOre_ToLua Ore[MAP_AWARD_ORE_LIST];        
}; 
struct tagGloryOne_ToLua
{
	int MapID;                          
	int Open;                           
	int EndTime;                        
	int StartTime;                      
	int x;                              
	int y;                              
};

struct tagStrongGlory_ToLua
{
	int Num;                             	/*  Ver.2226  */
	tagGloryOne_ToLua Glory[MAX_STRONG_POINT];         	/*  Ver.2226  */
};

//tolua_end
#pragma pack()

struct BaseWarMapInfo
{
	int nBaseWarMapID;
	int nMapID;
};

#define  MAX_BASE_WAR_MAP 6
#define  MAX_STRONG_POINT_LV 3

const BaseWarMapInfo BaseWarMapList[] = {
	{ 21300, 2202 },
	{ 21400, 2200 },
	{ 21500, 2300 },
	{ 21600, 2400 },
	{ 21700, 2500 },
	{ 21800, 3404 },
};

class GameClan_BaseWar//tolua_export
	:public NetMsgHandler, public Ogre::Singleton<GameClan_BaseWar>
{//tolua_export
public:
	GameClan_BaseWar(void);
	~GameClan_BaseWar(void);
public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	//tolua_begin
	const char* getStrongPointName();				//��ȡ��ǰ��ͼ�ݵ�����
	int			getStrongPointLV();					//��ȡ��ǰ��ͼ�ݵ�ȼ�
	int			getStrongPointBaseUse();			//��ȡ��ǰ��ͼ���ĵ׼�
	const char*	getStrongPointOwnerName();			//��ȡ��ǰ��ͼ�ݵ�ռ�칫������
	int			getStrongPointStat();				//��ȡ��ǰ�ݵ㾺��״̬
	int			getStrongPworldResLimit();			//��ȡ��ǰ�ݵ�ս��ͼ��Դ����
	bool		IsJoinStrongSale();					//��ȡ�Լ������Ƿ�����˾���
	int			getMyClanSaleMoney();				//��ȡ�Լ����ᰵ���ʽ�
	int			getSaleWinerPrice();				//��ȡ���ĳɹ��������
	const char*	getSaleWinerName();					//��ȡ���ĳɹ���������

	bool		IsInBaseWarPworld();				//��ȡ��ǰ�Ƿ��ھݵ�ս��
	int			getPworldAtkResNum();				//��ȡ��ǰ������Դ��
	int			getPworldDefResNum();				//��ȡ��ǰ�ط���Դ��
	tdr_ulonglong getAtkClanWID();					//��ȡ��ǰ����WID
	tdr_ulonglong getDefClanWID();					//��ȡ��ǰ�ط�WID

	tdr_ulonglong getCurMapStrongOwnerWID( int mapID );	//��ȡ��ǰ��ͼռ�칫��WID
	const char*	getCurMapStrongOWnerName( int mapID );				//��ȡ��ǰ��ͼռ�칫��Name
	bool		IsCurMapHasStrongPoint( int mapID );	//��ǰ��ͼ�Ƿ��Ǿݵ��ͼ

	int			getBannerNumByType( int nType );	//����������ȡĳһ�������������
	tagBannerDef_ToLua* getBannerDef( int index );	//��ȡ���Ķ���

	tagStrongMon_ToLua getBaseWarResMonInfo( int index );	//��ȡ�ݵ�ս��ͼ��Դ��Info
	tdr_ulonglong	getBaseWarResMonClanWID( int index );	//��ȡ�ݵ�ս��ͼ��Դ��ClanID
	int			getBaseWarResMonMemID( int index );	//��ȡ�ݵ�ս��ͼ��Դ��memID

	//c->s
	void		EnterStrongSale( int nMoney );		//ȷ�ϲμӾ���
	void		SetBanner( int mapID, int npcID, int nStyle, int nBack, int nSign );	//��������
	void		SetBannerXuanYan( int mapID, const char* pszXuanYan );	//������������
	void		RequestAllStrongInfo();							//����ս������ͼ��Ϣ
	void		RequestAward( int strongID, int type );			//��ȡ��������
	void		EnterStrongWar( int mapID );					//����ݵ�

	int			 getAllStrongInfoNum();					//��ȡս������ͼ��ϢNum
	tagStrongSaleInfo_ToLua* getStrongSaleInfoByIndex( int index );		//����������ȡս������ͼ������Ϣ
	tagStrongInfoOne_ToLua* getStrongOccupInfoByIndex( int index );		//����������ȡս������ͼռ����Ϣ

	unsigned int	getStrongOwnClanFengHaoColor( const tdr_ulonglong ClanWid );		//��ȡ�ݵ�ӵ���߷����ɫ

	int			getStrongAwardMoney( int mapID );			//��ȡ�ݵ㽱����Ǯ
	int			getStrongAwardBuffID( int mapID );			//��ȡ�ݵ�����Buff����
	int			getStrongAwardBuffLV( int mapID );			//��ȡ�ݵ�ռ�콱��buffLV
	int			getStrongAwardClanContri( int mapID );		//��ȡ�ݵ㽱�����ṱ�׶�

	void		setStrongBaseWarSaleInfoByIndex( int index );	//�ݵ�����ͼ������ĵ�ʱ�򣬸����������þ��ľݵ���Ϣ
	int			getStrongPointLVByMapID( int mapID );					//��ȡ��ǰ��ͼ�ݵ�ȼ�
	int			getStrongPointBaseUseByMapID( int mapID );			//��ȡ��ǰ��ͼ���ĵ׼�
	int			getCityWarFightStat();								//��ȡʱ���������״̬

	bool		hasStrongOpen();
	bool		hasStrongPoint(GameActor *pActor);						//��ҹ��ᵱǰ�Ƿ�ռ�оݵ�
	tagMapAwardOreDef_ToLua*	getMapAwardOreDef( int nMapID );		//��ȡ��ͼռ�콱��

	//�ݵ�ս���ս����Ϣ
	int			getStrongWarSelfResGetNum();				//��ȡ�Լ���Դ��ռ�����
	int			getStrongWarSelfKill();						//��ȡ�Լ�ɱ����
	int			getStrongWarSelfMaxContineKill();			//��ȡ�Լ������ɱ��
	int			getStrongWarSelfAssist();					//��ȡ�Լ�������
	int			getStrongWarSelfEnterTime();				//��ȡ�Լ�����ս��ʱ��
	int			getStrongWarSelfCurContineKill();			//��ȡ�Լ���ɱ��
	int			getStrongWarSelfDie();						//��ȡ�Լ�������
	void		clearStrongWarSelfMaxContineKill();			//clear�Լ���ɱ��

	int			getStrongWarPlayerNum();							//��ȡ�ݵ�ս����������Ŀ

	const char* getStrongWarPlayerName( int index );				//��ȡ�������
	int			getStrongWarPlayerRaceId( int index );				//��ȡ���ְҵid
	int			getStrongWarPlayerResGetNum( int index );			//��ȡ�����Դ��ռ�����
	int			getStrongWarPlayerKill( int index );				//��ȡ���ɱ����
	int			getStrongWarPlayerMaxContineKill( int index );		//��ȡ��������ɱ��
	int			getStrongWarPlayerAssist( int index );				//��ȡ���������
	int			getStrongWarPlayerEnterTime( int index );			//��ȡ��ҽ���ս��ʱ��
	const char* getStrongWarPlayerClanName( int index );			//��ȡ��ҹ�����

	int			getStrongWarWinnerMoneyType();						//��ȡ�ݵ�ʤ����������Ǯ����
	int			getStrongWarWinnerMoney();							//��ȡ�ݵ�ʤ����������Ǯ����
	int			getStrongWarFailMoneyType();						//��ȡʧ�ܷ�������Ǯ����
	int			getStrongWarFailMoney();							//��ȡʧ�ܷ�������Ǯ����

	int			getStrongWarWinnerItemID();							//��ȡ�ݵ�ս��ʤ���Ľ�����ƷID
	int			getStrongWarWinnerItemNum();						//��ȡ�ݵ�ս��ʤ��������Ʒ����
	int			getStrongWarFailItemID();							//��ȡ�ݵ�սʧ�ܷ�������ƷID
	int			getStrongWarFailItemNum();							//��ȡ�ݵ�սʧ�ܷ�������Ʒ����

	int			getStrongWarWinnereExp();							//��ȡʧʤ������exp
	int			getStrongWarFailExp();								//��ȡʧ�ܷ�����exp

	void		LeaveStrongWar();									//�뿪�ݵ�ս��

	int			getStrongWarTodayEnteredID();						//��ȡ��ҽ��ս���ľݵ�ս��¼ID
	const char*	getStrongWarTodayEnteredStrongName();				//��ȡ��ҽ��ս���ľݵ�ս��¼�ݵ�Name

	tagStrongGlory_ToLua	getStrongBannerGloryInfo();		//��ȡ��Ӧ��ͼ��ս����ҫ��Ϣ

	//tolua_end
	void		SetBaseWarResMonInfo();					//���þݵ�ս��Դ�ֵ���Ϣ
private:
	std::vector<tagStrongInfoOne> m_StrongPointsVec;	//ȫ���ݵ���Ϣ�б�	
	tagStrongPworldInfo m_StrongPworldInfo;				//�ݵ�ս��������Դ��Ϣ��¼
	tagStrongSaleInfo m_CurStrongSaleInfo;				//��ǰ���ľݵ���Ϣ
	std::vector<tagBannerDef*> m_BannerDefVec;			//���������б�
	tagStrongAllInfo m_StrongAllInfo;				//ս������ͼ��Ϣ
	tagStrongWarOne m_StrongWarOne;					//���˾ݵ�ս����Ϣ
	tagStrongWarAll m_StrongWarAll;					//�����˵ľݵ�ս����Ϣ
	bool m_StrongWarIsInFight;						//�Ƿ��ھݵ�սս����

	int m_StrongWarTodayEnterID;					//��ҽ��ս���ľݵ�ս��¼ID

	tagStrongGlory_ToLua	m_StrongGlory;					//ս����ҫ��Ϣ
};//tolua_export