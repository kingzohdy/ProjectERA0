#include "GameClan_BaseWar.h"
#include "GameDBData.h"
#include "GameClanManager.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameNpc.h"
#include "GameMap.h"
#include "OgreScriptLuaVM.h"
#include "GameManager.h"
#include "ClanInfo.h"
#include "WorldCity.h"
#include <windows.h> 
GameClan_BaseWar::GameClan_BaseWar(void)
{
	m_StrongWarIsInFight = false;
	m_StrongWarTodayEnterID = 0;
	memset( &m_StrongWarAll, 0, sizeof(m_StrongWarAll) );
	memset( &m_StrongWarOne, 0, sizeof(m_StrongWarOne) );
	DBDataManager::getSingleton().loadBannerDef( m_BannerDefVec );
	INetworkManager::getSingleton().RegisterMsgHandler( STRONG_SVR, this );
}
GameClan_BaseWar::~GameClan_BaseWar(void)
{
}

int GameClan_BaseWar::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if ( msgid == STRONG_SVR )
	{
		const STRONGSVR& Msg = msgbody.StrongSvr;
		switch( Msg.Cmd )
		{
		case STRONG_SVR_OP_SALE_INFO:
			{
				const STRONGSALEINFO& saleInfo = Msg.Data.SaleInfo;
				m_CurStrongSaleInfo = saleInfo;
				char buffer[256];
				sprintf( buffer, "OpenClanBaseWarSale(%d)", saleInfo.NpcID );
				GameManager::getSingleton().getScriptVM()->callString( buffer );
			}
			break;
		case STRONG_SVR_OP_STRONG_INFO:
			{
				const STRONGPOINTINFOS& pointInfos = Msg.Data.StrongPointInfo;
				m_StrongPointsVec.clear();
				for ( int i = 0; i < pointInfos.Num; i++ )
				{
					const STRONGINFOONE& strongInfoOne = pointInfos.StrongInfos[i];  
					m_StrongPointsVec.push_back(strongInfoOne);
					ActorManager::getSingleton().setMapOwnerWID( strongInfoOne.MapID, strongInfoOne.ClanWid );
				}

				MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
				if ( pMain != NULL && hasStrongPoint( pMain ) )
				{
					GameManager::getSingleton().SendUIEvent("GE_STRONG_INFO");
				}
				ActorManager::getSingleton().setClanFengHao();
				ActorManager::getSingleton().updateAllPlayerTitleInfo();
			}
			break;
		case STRONG_SVR_OP_PWORLD_INFO:
			{
				const STRONGPWORLDINFO& pworldInfo = Msg.Data.StrongPworldInfo;
				m_StrongPworldInfo = pworldInfo;
				SetBaseWarResMonInfo();
			}
			break;
		case STRONG_SVR_OP_BANNER_CHG:
			{
				const BANNERCHG& bannerChg = Msg.Data.BannerChg;
				GameActor *pActor = ActorManager::getSingleton().FindActor( bannerChg.MemID );
				if ( pActor != NULL )
				{
					pActor->m_ViewBanner.GloryEffect = bannerChg.Effect;
					pActor->m_ViewBanner.Sign = bannerChg.Sign;
					pActor->m_ViewBanner.Back = bannerChg.Back;
					pActor->m_ViewBanner.Style = bannerChg.Style;
					pActor->setBannerInfo();
					sprintf( pActor->m_ViewBanner.XuanYan, "%s", bannerChg.XuanYan );
					((GameNPC*)pActor)->m_strNpcFunctionDes = pActor->m_ViewBanner.XuanYan;
				}
			}
			break;
		case STRONG_SVR_OP_STRONG_ACTIVE:
			{
				//ͬһʱ�̿���6���ݵ�ȫ������
				const STRONGACTIVE& strongActive = Msg.Data.Active;
				int activeLVS[MAX_STRONG_POINT_LV];
				memset( &activeLVS, 0, sizeof(activeLVS) );
				//ͳ�Ƶ�ǰ�������ļ����ȼ��ľݵ�
				for ( int i = 0; i < strongActive.Num; i++ )
				{
					int mapID = strongActive.MapIDs[i];
					const tagStrongPointDef* strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapID );
					if ( strongPworldDef->Lvl > 0 )
					{
						int lv = strongPworldDef->Lvl;
						activeLVS[ lv - 1 ] = 1;
					}
					for( std::vector<tagStrongInfoOne>::iterator itr = m_StrongPointsVec.begin(), end = m_StrongPointsVec.end() ; itr != end; ++itr )
					{
						if ( itr->MapID == mapID )
						{
							itr->IsOpen = 1;
						}
					}
				}
				//Ϊ����ĵȼ����Ư������
				for ( int i = 0; i < MAX_STRONG_POINT_LV; i++ )
				{
					if ( activeLVS[i] == 1 )
					{
						char buffer[256];
						sprintf( buffer, "StrongPointActive(%d)", i + 1 );
						GameManager::getSingleton().getScriptVM()->callString( buffer );
					}
				}
			}
			break;
		case STRONG_SVR_OP_ALL_INFO:
			{
				m_StrongAllInfo = Msg.Data.AllInfo;
				GameManager::getSingleton().getScriptVM()->callString( "GetStrongAllInfo()" );
			}
			break;
		case STRONG_SVR_OP_WAR_ONE:
			{
				m_StrongWarOne = Msg.Data.WarOne;
				if ( !m_StrongWarIsInFight )
				{
					GameManager::getSingleton().getScriptVM()->callString( "StrongWarBegin()" );
				}
				GameManager::getSingleton().getScriptVM()->callString( "PlayersInfoChanges()" );
			}
			break;
		case STRONG_SVR_OP_WAR_ALL:
			{
				m_StrongWarAll = Msg.Data.WarAll;
				if ( (!m_StrongWarIsInFight) && m_StrongWarAll.End != 1 )
				{
					GameManager::getSingleton().getScriptVM()->callString( "StrongWarBegin()" );
				}
				//�ݵ�ս����
				if ( m_StrongWarAll.End == 1 )
				{
					GameManager::getSingleton().getScriptVM()->callString( "StrongWarEnd()" );
				}
				GameManager::getSingleton().getScriptVM()->callString( "PlayersInfoChanges()" );
			}
			break;
		case STRONG_SVR_OP_STRONGID:
			{
				m_StrongWarTodayEnterID = Msg.Data.StrongID.ID;
			}
			break;
		case STRONG_SVR_OP_STRONGLORY:
			{
				memcpy( &m_StrongGlory, &Msg.Data.StrongGlory, sizeof(m_StrongGlory) );
				GameManager::getSingleton().getScriptVM()->callString( "SetClanBannerGloryInfo()" );
			}
			break;
		}
	}
	return 0;
}
//��ȡ��ǰ��ͼ�ݵ�����
const char* GameClan_BaseWar::getStrongPointName()
{
	int mapId = m_CurStrongSaleInfo.MapID;
	const tagStrongPointDef* strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		return "";
	}
	return strongPworldDef->Name;
}
//��ȡ��ǰ��ͼ�ݵ�ȼ�
int GameClan_BaseWar::getStrongPointLV()
{
	int mapId = m_CurStrongSaleInfo.MapID;
	const tagStrongPointDef* strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->Lvl;
}
int GameClan_BaseWar::getStrongPointLVByMapID( int mapID )
{
	const tagStrongPointDef* strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapID );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->Lvl;
}
int GameClan_BaseWar::getStrongPointBaseUseByMapID( int mapID )
{
	const tagStrongPointDef* strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapID );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->BaseUse;
}
//��ȡ��ǰ��ͼ���ĵ׼�
int GameClan_BaseWar::getStrongPointBaseUse()
{
	int mapId = m_CurStrongSaleInfo.MapID;
	const tagStrongPointDef* strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->BaseUse;
}
//��ȡ��ǰ��ͼ�ݵ�ռ�칫������
const char* GameClan_BaseWar::getStrongPointOwnerName()
{
	int mapId = m_CurStrongSaleInfo.MapID;
	tdr_ulonglong nOwnerWId = 0;
	const char* OwnerName ="";
	for( std::vector<tagStrongInfoOne>::iterator itr = m_StrongPointsVec.begin(), end = m_StrongPointsVec.end() ; itr != end; ++itr )
	{
		if ( itr->MapID == mapId )
		{
			nOwnerWId = itr->ClanWid;
		}
	}
	if ( nOwnerWId != 0 )
	{
		OwnerName = GameClanManager::getSingleton().clanGetClanNameByWid( nOwnerWId );
	}
	return OwnerName;
}
//��ȡ��ǰ�ݵ㾺��״̬
int GameClan_BaseWar::getStrongPointStat()
{
	return m_CurStrongSaleInfo.SaleStat;
}

//ȷ�ϲμӾ���
void GameClan_BaseWar::EnterStrongSale( int nMoney )
{
	tagCSPkgBody msgbody;
	msgbody.StrongClt.Cmd = STRONG_CLT_OP_SALE;
	msgbody.StrongClt.Data.Sale.MapID = m_CurStrongSaleInfo.MapID;
	msgbody.StrongClt.Data.Sale.Money = nMoney;
	SendGameServerMsg( STRONG_CLT, msgbody );
}
//��ȡ��ǰ�ݵ�ս��ͼ��Դ����
int GameClan_BaseWar::getStrongPworldResLimit()
{
	int mapId = m_CurStrongSaleInfo.MapID;
	const tagStrongPointDef* strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
		{
			if ( BaseWarMapList[i].nBaseWarMapID == pMain->getCurMap()->getRealId() )
			{
				mapId = BaseWarMapList[i].nMapID;
			}
		}
		strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
		if ( strongPworldDef == NULL )
		{
			return 0;
		}
	}
	return strongPworldDef->ResLimit;
}
int	GameClan_BaseWar::getStrongAwardMoney( int mapID )
{
	const tagStrongPointDef* strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapID );
	return strongPworldDef->ClanMoney;
}
int GameClan_BaseWar::getStrongAwardBuffID( int mapID )
{
	const tagStrongPointDef* strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapID );
	return strongPworldDef->BuffID;
}
int GameClan_BaseWar::getStrongAwardBuffLV( int mapID )
{
	const tagStrongPointDef* strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapID );
	return strongPworldDef->BuffLvl;
}
int GameClan_BaseWar::getStrongAwardClanContri( int mapID )
{
	const tagStrongPointDef* strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapID );
	return strongPworldDef->ClanContri;
}
//��ȡ�Լ������Ƿ�����˾���
bool GameClan_BaseWar::IsJoinStrongSale()
{
	if ( m_CurStrongSaleInfo.ClanJoin == 1  )
	{
		return true;
	}
	return false;
}
//��ȡ�Լ����ᰵ���ʽ�
int GameClan_BaseWar::getMyClanSaleMoney()
{
	return m_CurStrongSaleInfo.ClanMoney;
}
//��ȡ���ĳɹ��������
int GameClan_BaseWar::getSaleWinerPrice()
{
	return m_CurStrongSaleInfo.AtkClanMoney;
}
//��ȡ���ĳɹ���������
const char* GameClan_BaseWar::getSaleWinerName()
{ 
	if ( m_CurStrongSaleInfo.AtkClanWid != 0 )
	{
		return GameClanManager::getSingleton().clanGetClanNameByWid( m_CurStrongSaleInfo.AtkClanWid );
	}
	return "";
}
//��ȡ��ǰ�Ƿ��ھݵ�ս��
bool GameClan_BaseWar::IsInBaseWarPworld()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	int mapID = 0;
	if ( pMain && pMain->getCurMap() )
	{
		mapID = pMain->getCurMap()->getRealId();
	}
	if ( mapID != 0 )
	{
		for( int i = 0; i < MAX_BASE_WAR_MAP; i++ )
		{
			if ( BaseWarMapList[i].nBaseWarMapID == mapID )
			{
				return true;
			}
		}
	}
	return false;
}
//��ȡ��ǰ������Դ��
int GameClan_BaseWar::getPworldAtkResNum()
{
	return m_StrongPworldInfo.AtkRes;
}
//��ȡ��ǰ�ط���Դ��
int GameClan_BaseWar::getPworldDefResNum()
{
	return m_StrongPworldInfo.DefRes;
}
//��������
void GameClan_BaseWar::SetBanner( int mapID, int npcID, int nStyle, int nBack, int nSign )
{
	tagCSPkgBody msgbody;
	msgbody.StrongClt.Cmd = STRONG_CLT_OP_SET_BANNER;
	msgbody.StrongClt.Data.SetBanner.MapID = mapID;
	msgbody.StrongClt.Data.SetBanner.NpcID = npcID;
	msgbody.StrongClt.Data.SetBanner.Style = nStyle;
	msgbody.StrongClt.Data.SetBanner.Back = nBack;
	msgbody.StrongClt.Data.SetBanner.Sign = nSign;
	SendGameServerMsg( STRONG_CLT, msgbody );
}
//������������
void GameClan_BaseWar::SetBannerXuanYan( int mapID, const char* pszXuanYan )
{
	tagCSPkgBody msgbody;
	msgbody.StrongClt.Cmd = STRONG_CLT_OP_SET_BANNER_XUANYAN;
	msgbody.StrongClt.Data.SetXuanyan.MapID = mapID;
	sprintf(msgbody.StrongClt.Data.SetXuanyan.XuanYan, "%s", pszXuanYan);
	SendGameServerMsg( STRONG_CLT, msgbody );
}
//����������ȡĳһ�������������
int GameClan_BaseWar::getBannerNumByType( int nType )
{
	int num = 0;
	for( size_t i = 0; i < m_BannerDefVec.size(); i++ )
	{
		if ( m_BannerDefVec[i]->Type == nType )
		{
			num++;
		}
	}
	return num;
}
//��ȡ���Ķ���
tagBannerDef_ToLua* GameClan_BaseWar::getBannerDef( int index )
{
	assert( index < m_BannerDefVec.size() );
	static_assert( sizeof( tagBannerDef_ToLua ) == sizeof( tagBannerDef ) );
	return (tagBannerDef_ToLua*) m_BannerDefVec[index];
}
//��ȡ��ǰ��ͼռ�칫��
tdr_ulonglong GameClan_BaseWar::getCurMapStrongOwnerWID( int mapID )
{
	for( size_t i = 0; i < m_StrongPointsVec.size(); i++ )
	{
		if ( m_StrongPointsVec[i].MapID == mapID )
		{
			return m_StrongPointsVec[i].ClanWid;
		}
	}
	return 0;
}
//��ȡ��ǰ��ͼռ�칫��Name
const char*	GameClan_BaseWar::getCurMapStrongOWnerName( int mapID )
{
	for( size_t i = 0; i < m_StrongPointsVec.size(); i++ )
	{
		if ( m_StrongPointsVec[i].MapID == mapID )
		{
			tdr_ulonglong ClanWID = m_StrongPointsVec[i].ClanWid;
			return GameClanManager::getSingleton().clanGetClanNameByWid( ClanWID );
		}
	}
	return "";
}
bool GameClan_BaseWar::IsCurMapHasStrongPoint( int mapID )
{
	for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
	{
		if ( BaseWarMapList[i].nMapID == mapID )
		{
			return true;
		}
	}
	return false;
}
//��ȡ�ݵ�ս��ͼ��Դ��memID
int	GameClan_BaseWar::getBaseWarResMonMemID( int index )
{
	return m_StrongPworldInfo.Mon[index].MemID;
}
//��ȡ�ݵ�ս��ͼ��Դ��ClanID
tdr_ulonglong GameClan_BaseWar::getBaseWarResMonClanWID( int index )
{
	return m_StrongPworldInfo.Mon[index].ClanWID;
}
//��ȡ��ǰ����WID
tdr_ulonglong GameClan_BaseWar::getAtkClanWID()
{
	return m_StrongPworldInfo.AtkClanWid;
}
//��ȡ��ǰ�ط�WID
tdr_ulonglong GameClan_BaseWar::getDefClanWID()
{
	return m_StrongPworldInfo.DefClanWid;
}
//��ȡ�ݵ�ս��ͼ��Դ��Info
tagStrongMon_ToLua GameClan_BaseWar::getBaseWarResMonInfo( int index )
{
	assert( sizeof(STRONGMON) == sizeof(tagStrongMon_ToLua) );
	tagStrongMon_ToLua strongMonInfo;
	strongMonInfo.Flag = m_StrongPworldInfo.Mon[index].Flag;
	strongMonInfo.MemID = m_StrongPworldInfo.Mon[index].MemID;
	strongMonInfo.ClanID = m_StrongPworldInfo.Mon[index].ClanID;
	strongMonInfo.ClanWID = m_StrongPworldInfo.Mon[index].ClanWID;
	strongMonInfo.X = m_StrongPworldInfo.Mon[index].X;
	strongMonInfo.Y = m_StrongPworldInfo.Mon[index].Y;
	return strongMonInfo;
}
//���þݵ�ս��Դ�ֵ���Ϣ
void GameClan_BaseWar::SetBaseWarResMonInfo()
{
	for( int i = 0; i < m_StrongPworldInfo.MonNum; i++ )
	{
		int memID = m_StrongPworldInfo.Mon[i].MemID;
		GameActor* pActor = ActorManager::getSingleton().FindActor( memID );
		if ( pActor != NULL )
		{
			pActor->setBaseWarResMon( true );
			pActor->setBaseWarClanWID( m_StrongPworldInfo.Mon[i].ClanWID );
		}
	}
}
void GameClan_BaseWar::RequestAllStrongInfo()
{
	tagCSPkgBody msgbody;
	msgbody.StrongClt.Cmd = STRONG_CLT_OP_INFO_ALL;
	SendGameServerMsg( STRONG_CLT, msgbody );
}

void GameClan_BaseWar::RequestAward( int strongID, int type )
{
	tagCSPkgBody msgbody;
	msgbody.StrongClt.Cmd = STRONG_CLT_OP_AWARD;
	msgbody.StrongClt.Data.AwardReq.StrongID = strongID;
	msgbody.StrongClt.Data.AwardReq.Type = type;
	SendGameServerMsg( STRONG_CLT, msgbody );
}

void GameClan_BaseWar::EnterStrongWar(int mapID)
{
	tagCSPkgBody msgbody;
	msgbody.StrongClt.Cmd = STRONG_CLT_OP_ENTER;
	msgbody.StrongClt.Data.EnterReq.StrongID = mapID;
	SendGameServerMsg( STRONG_CLT, msgbody );
}

int GameClan_BaseWar::getAllStrongInfoNum()
{
	return m_StrongAllInfo.Num;
}

tagStrongSaleInfo_ToLua* GameClan_BaseWar::getStrongSaleInfoByIndex( int index )
{
	assert( sizeof(tagStrongSaleInfo_ToLua) == sizeof(tagStrongSaleInfo) );
	return ( tagStrongSaleInfo_ToLua* )&m_StrongAllInfo.AllInfoOnes[index].SaleInfo;
}

tagStrongInfoOne_ToLua* GameClan_BaseWar::getStrongOccupInfoByIndex( int index )
{
	static_assert( sizeof(tagStrongInfoOne_ToLua) == sizeof(tagStrongInfoOne) );
	return ( tagStrongInfoOne_ToLua* )&m_StrongAllInfo.AllInfoOnes[index].OccupInfo;
}
unsigned int GameClan_BaseWar::getStrongOwnClanFengHaoColor( const tdr_ulonglong ClanWid )
{
	return GameClanManager::getSingleton().getClanColor((const tagLong_ToLua&)ClanWid);
}
void GameClan_BaseWar::setStrongBaseWarSaleInfoByIndex( int index )
{
	m_CurStrongSaleInfo = m_StrongAllInfo.AllInfoOnes[index].SaleInfo;
}
int GameClan_BaseWar::getCityWarFightStat()
{
	return m_StrongAllInfo.CityStat;
}
bool GameClan_BaseWar::hasStrongOpen()
{
	bool bHasStrongOpen = false;
	for( std::vector<tagStrongInfoOne>::iterator itr = m_StrongPointsVec.begin(), end = m_StrongPointsVec.end() ; itr != end; ++itr )
	{
		if ( itr->IsOpen == 1 )
		{
			bHasStrongOpen = true;
		}
	}
	return bHasStrongOpen;
}

bool GameClan_BaseWar::hasStrongPoint( GameActor *pActor )
{
	if( pActor != NULL)
	{
		for( size_t i = 0; i < m_StrongPointsVec.size(); i++ )
		{
			if ( m_StrongPointsVec[i].ClanWid != 0 && m_StrongPointsVec[i].ClanWid == pActor->getClanWID() )
			{
				return true;
			}
		}
	}
	return false;
}
tagMapAwardOreDef_ToLua* GameClan_BaseWar::getMapAwardOreDef( int nMapID )
{
	const tagMapAwardOreDef* mapAwardOreDef = DBDataManager::getSingleton().getMapAwardOreDef( nMapID );
	if ( mapAwardOreDef == NULL )
	{
		return NULL;
	}
	return (tagMapAwardOreDef_ToLua*)mapAwardOreDef;
}
int GameClan_BaseWar::getStrongWarSelfResGetNum()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	//��Ϊ������Ϣ���б仯�ŷ������Խ���ʱ��ȡ�������б��˵�ResGet
	for( int i = 0; i < m_StrongWarAll.Num; i++ )
	{
		if (strcmp( m_StrongWarAll.Ones[i].RoleName, pMain->getName()) == 0 )
		{
			return m_StrongWarAll.Ones[i].ResGet;
		}
	}
	return m_StrongWarOne.ResGet;
}
int GameClan_BaseWar::getStrongWarSelfKill()
{
	return m_StrongWarOne.CountKill;
}
int GameClan_BaseWar::getStrongWarSelfMaxContineKill()
{
	return m_StrongWarOne.MaxContinueKill;
}
int GameClan_BaseWar::getStrongWarSelfCurContineKill()
{
	return m_StrongWarOne.CurrContinueKill;
}
int GameClan_BaseWar::getStrongWarSelfDie()
{
	return m_StrongWarOne.Die;
}
void GameClan_BaseWar::clearStrongWarSelfMaxContineKill()
{
	m_StrongWarOne.CurrContinueKill = 0;
}
int GameClan_BaseWar::getStrongWarSelfAssist()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	//��Ϊ������Ϣ���б仯�ŷ������Խ���ʱ��ȡ�������б��˵�AssitNum
	for( int i = 0; i < m_StrongWarAll.Num; i++ )
	{
		if (strcmp( m_StrongWarAll.Ones[i].RoleName, pMain->getName()) == 0 )
		{
			return m_StrongWarAll.Ones[i].AssitNum;
		}
	}
	return m_StrongWarOne.AssitNum;
}
int GameClan_BaseWar::getStrongWarSelfEnterTime()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	//��Ϊ������Ϣ���б仯�ŷ������Խ���ʱ��ȡ�������б��˵Ľ���ʱ��
	for( int i = 0; i < m_StrongWarAll.Num; i++ )
	{
		if (strcmp( m_StrongWarAll.Ones[i].RoleName, pMain->getName()) == 0 )
		{
			return m_StrongWarAll.Ones[i].EnterTime;
		}
	}
	return m_StrongWarOne.EnterTime;
}

const char* GameClan_BaseWar::getStrongWarPlayerName( int index )
{
	return m_StrongWarAll.Ones[index].RoleName;
}
int GameClan_BaseWar::getStrongWarPlayerResGetNum( int index )
{
	return m_StrongWarAll.Ones[index].ResGet;
}
int GameClan_BaseWar::getStrongWarPlayerRaceId( int index )
{
	return m_StrongWarAll.Ones[index].Career;
}
int GameClan_BaseWar::getStrongWarPlayerKill( int index )
{
	return m_StrongWarAll.Ones[index].CountKill;
}
int GameClan_BaseWar::getStrongWarPlayerMaxContineKill( int index )
{
	return m_StrongWarAll.Ones[index].MaxContinueKill;
}
int GameClan_BaseWar::getStrongWarPlayerAssist( int index )
{
	return m_StrongWarAll.Ones[index].AssitNum;
}
const char* GameClan_BaseWar::getStrongWarPlayerClanName( int index )
{
	tdr_ulonglong clanWid = m_StrongWarAll.Ones[index].ClanWID;
	return ClanInfo::getSingleton().getClanName(clanWid);
}
int GameClan_BaseWar::getStrongWarPlayerEnterTime( int index )
{
	return m_StrongWarAll.Ones[index].EnterTime;
}

int GameClan_BaseWar::getStrongWarPlayerNum()
{
	return m_StrongWarAll.Num;
}

int GameClan_BaseWar::getStrongWarWinnerMoneyType()
{
	const tagStrongPointDef* strongPworldDef; 
	int mapId = 0;
	for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
	{
		if ( BaseWarMapList[i].nBaseWarMapID == m_StrongWarOne.MapID )
		{
			mapId = BaseWarMapList[i].nMapID;
		}
	}
	//��Ϊ������Ϣ���б仯�ŷ������������û�Ļ���ִ���±߷������ҵ�ͼ
	if ( mapId == 0 )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
		{
			if ( BaseWarMapList[i].nBaseWarMapID == pMain->getCurMap()->getRealId() )
			{
				mapId = BaseWarMapList[i].nMapID;
			}
		}
	}
	strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->WinAward.Money[0].Type;
}
int GameClan_BaseWar::getStrongWarWinnerMoney()
{
	const tagStrongPointDef* strongPworldDef; 
	int mapId = 0;
	for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
	{
		if ( BaseWarMapList[i].nBaseWarMapID == m_StrongWarOne.MapID )
		{
			mapId = BaseWarMapList[i].nMapID;
		}
	}
	//��Ϊ������Ϣ���б仯�ŷ������������û�Ļ���ִ���±߷������ҵ�ͼ
	if ( mapId == 0 )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
		{
			if ( BaseWarMapList[i].nBaseWarMapID == pMain->getCurMap()->getRealId() )
			{
				mapId = BaseWarMapList[i].nMapID;
			}
		}
	}
	strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->WinAward.Money[0].Num;
}
int GameClan_BaseWar::getStrongWarFailMoneyType()
{
	const tagStrongPointDef* strongPworldDef; 
	int mapId = 0;
	for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
	{
		if ( BaseWarMapList[i].nBaseWarMapID == m_StrongWarOne.MapID )
		{
			mapId = BaseWarMapList[i].nMapID;
		}
	}
	//��Ϊ������Ϣ���б仯�ŷ������������û�Ļ���ִ���±߷������ҵ�ͼ
	if ( mapId == 0 )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
		{
			if ( BaseWarMapList[i].nBaseWarMapID == pMain->getCurMap()->getRealId() )
			{
				mapId = BaseWarMapList[i].nMapID;
			}
		}
	}
	strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->FailAward.Money[0].Type;
}

int GameClan_BaseWar::getStrongWarFailMoney()
{
	const tagStrongPointDef* strongPworldDef; 
	int mapId = 0;
	for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
	{
		if ( BaseWarMapList[i].nBaseWarMapID == m_StrongWarOne.MapID )
		{
			mapId = BaseWarMapList[i].nMapID;
		}
	}
	//��Ϊ������Ϣ���б仯�ŷ������������û�Ļ���ִ���±߷������ҵ�ͼ
	if ( mapId == 0 )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
		{
			if ( BaseWarMapList[i].nBaseWarMapID == pMain->getCurMap()->getRealId() )
			{
				mapId = BaseWarMapList[i].nMapID;
			}
		}
	}
	strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->FailAward.Money[0].Num;
}
int GameClan_BaseWar::getStrongWarWinnereExp()
{
	const tagStrongPointDef* strongPworldDef; 
	int mapId = 0;
	for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
	{
		if ( BaseWarMapList[i].nBaseWarMapID == m_StrongWarOne.MapID )
		{
			mapId = BaseWarMapList[i].nMapID;
		}
	}
	//��Ϊ������Ϣ���б仯�ŷ������������û�Ļ���ִ���±߷������ҵ�ͼ
	if ( mapId == 0 )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
		{
			if ( BaseWarMapList[i].nBaseWarMapID == pMain->getCurMap()->getRealId() )
			{
				mapId = BaseWarMapList[i].nMapID;
			}
		}
	}
	strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->WinAward.Exp;
}
int GameClan_BaseWar::getStrongWarFailExp()
{
	const tagStrongPointDef* strongPworldDef; 
	int mapId = 0;
	for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
	{
		if ( BaseWarMapList[i].nBaseWarMapID == m_StrongWarOne.MapID )
		{
			mapId = BaseWarMapList[i].nMapID;
		}
	}
	//��Ϊ������Ϣ���б仯�ŷ������������û�Ļ���ִ���±߷������ҵ�ͼ
	if ( mapId == 0 )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
		{
			if ( BaseWarMapList[i].nBaseWarMapID == pMain->getCurMap()->getRealId() )
			{
				mapId = BaseWarMapList[i].nMapID;
			}
		}
	}
	strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->FailAward.Exp;
}

void GameClan_BaseWar::LeaveStrongWar()
{
	tagCSPkgBody msgbody;
	msgbody.PworldClt.CltType = PWORLD_CLT_EXIT_PWORLD;
	SendGameServerMsg(PWORLD_CLT, msgbody);
}

int GameClan_BaseWar::getStrongWarTodayEnteredID()
{
	return m_StrongWarTodayEnterID;
}

const char* GameClan_BaseWar::getStrongWarTodayEnteredStrongName()
{
	if ( m_StrongWarTodayEnterID != 0 )
	{
		const tagStrongPointDef* strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( m_StrongWarTodayEnterID );
		if ( strongPworldDef != NULL  )
		{
			return strongPworldDef->Name;
		}
	}
	return "";
}
int GameClan_BaseWar::getStrongWarWinnerItemID()
{
	const tagStrongPointDef* strongPworldDef; 
	int mapId = 0;
	for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
	{
		if ( BaseWarMapList[i].nBaseWarMapID == m_StrongWarOne.MapID )
		{
			mapId = BaseWarMapList[i].nMapID;
		}
	}
	//��Ϊ������Ϣ���б仯�ŷ������������û�Ļ���ִ���±߷������ҵ�ͼ
	if ( mapId == 0 )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
		{
			if ( BaseWarMapList[i].nBaseWarMapID == pMain->getCurMap()->getRealId() )
			{
				mapId = BaseWarMapList[i].nMapID;
			}
		}
	}
	strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->WinAward.Item[0].ItemID;
}
int GameClan_BaseWar::getStrongWarWinnerItemNum()
{
	const tagStrongPointDef* strongPworldDef; 
	int mapId = 0;
	for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
	{
		if ( BaseWarMapList[i].nBaseWarMapID == m_StrongWarOne.MapID )
		{
			mapId = BaseWarMapList[i].nMapID;
		}
	}
	//��Ϊ������Ϣ���б仯�ŷ������������û�Ļ���ִ���±߷������ҵ�ͼ
	if ( mapId == 0 )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
		{
			if ( BaseWarMapList[i].nBaseWarMapID == pMain->getCurMap()->getRealId() )
			{
				mapId = BaseWarMapList[i].nMapID;
			}
		}
	}
	strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->WinAward.Item[0].ItemNum;
}
int GameClan_BaseWar::getStrongWarFailItemID()
{
	const tagStrongPointDef* strongPworldDef; 
	int mapId = 0;
	for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
	{
		if ( BaseWarMapList[i].nBaseWarMapID == m_StrongWarOne.MapID )
		{
			mapId = BaseWarMapList[i].nMapID;
		}
	}
	//��Ϊ������Ϣ���б仯�ŷ������������û�Ļ���ִ���±߷������ҵ�ͼ
	if ( mapId == 0 )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
		{
			if ( BaseWarMapList[i].nBaseWarMapID == pMain->getCurMap()->getRealId() )
			{
				mapId = BaseWarMapList[i].nMapID;
			}
		}
	}
	strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->FailAward.Item[0].ItemID;
}
int GameClan_BaseWar::getStrongWarFailItemNum()
{
	const tagStrongPointDef* strongPworldDef; 
	int mapId = 0;
	for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
	{
		if ( BaseWarMapList[i].nBaseWarMapID == m_StrongWarOne.MapID )
		{
			mapId = BaseWarMapList[i].nMapID;
		}
	}
	//��Ϊ������Ϣ���б仯�ŷ������������û�Ļ���ִ���±߷������ҵ�ͼ
	if ( mapId == 0 )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		for ( int i = 0; i < MAX_BASE_WAR_MAP ; i++ )
		{
			if ( BaseWarMapList[i].nBaseWarMapID == pMain->getCurMap()->getRealId() )
			{
				mapId = BaseWarMapList[i].nMapID;
			}
		}
	}
	strongPworldDef = DBDataManager::getSingleton().getStrongPointDef( mapId );
	if ( strongPworldDef == NULL )
	{
		return 0;
	}
	return strongPworldDef->FailAward.Item[0].ItemNum;
}

tagStrongGlory_ToLua GameClan_BaseWar::getStrongBannerGloryInfo()
{
	return m_StrongGlory;
}