#pragma once
#include <map>
#include "OgreMemoryDelegation.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameDBData.h"

//tolua_begin
enum
{
	CLIENT_GENDER_NONE		= 0,
	CLIENT_GENDER_MALE		= 1,
	CLIENT_GENDER_FEMALE	= 2,
	CLIENT_GENDER_MAX		= CLIENT_GENDER_FEMALE,
};

//tolua_end

class RoleAttr//tolua_export
	: public Ogre::MemoryDelegation
{//tolua_export
public:
	RoleAttr(void)
		: m_nTmpHp( 0 ),
		  m_nOwnerId( 0 ),
		  m_nPetDefID( 0 ),
		  m_nPetIconId( 0 ),
		  m_nHpRef( 0 ),
		  m_nPkValue(0),
		  m_nPkMode(0),
		  m_nClanID(0),
		  m_uClanWID( 0 ),
		  m_uTeamWID( 0 ),
		  m_uOwnerTeamWID( 0 ),
		  m_nCampIdx(0),
		  m_roleWID(0),
		  m_bIsInPKOverlay(false),
		  m_nSafeOverlayFlag(0),
// 		  m_nBootyId(0),
		  m_nItemNum(0),
		  m_nArmStar(0),
		  m_PKInfoType(MON_PK_TYPE_NONE),
		  m_DaoBaClanWid(0),
		  m_DaoBaEnd(0),
		  m_nBattePetFlag(0)
	{};
	~RoleAttr(void){};

	void				setAttriValue ( int attriId, const CSATTRIDVAL& val );
	const CSATTRIDVAL*	getAttriValule( int attriId )const;

	void				setItemNum(int num);
	
	void				setBaseAttriValue ( int attriId, const CSATTRIDVAL& val );
	//const CSATTRIDVAL*	getBaseAttriValue( int attriId )const;
	void				setKillCount( int num );
	
	void				setLostTotemExpNum( int num );

	//tolua_begin
	unsigned short		getBaseAttriValue( int attriId )const;
	tdr_ulonglong		getClanWID();
	int					getClanID();
	void				setClanWID( tdr_ulonglong clanWID );
	bool				isInSameClan( tdr_ulonglong uClanWID );
	bool				isInClan();
	virtual void		setPkValue( int nPkValue );
	tdr_ulonglong		getRoleWId();
	void				setRoleWId( tdr_ulonglong wId );
	unsigned short		getShortAttriValule( int attriId )const;
	int					getIntAttriValue( int attriId )const;
	short				getSignedShortAttriValule(int attriId) const;

	void				setName(const char *pname);
	const char*			getName();
	void				setStallName(const char* pName );
	const char*			getStallName();
	int					getHead()
	{
		return m_Head; 
	}
	int					getFace() 
	{ 
		return m_Face; 
	}
	int					getHair() 
	{
		return m_Hair; 
	}
	void				setViewFlg( int flag ) { m_ViewFlag = flag; }
	
	void				setClanName( const char* pszClanName );
	const char*			getClanName();
	void				setTeamName( const char* pname );
	const char*			getTeamName();
	tdr_ulonglong		getOwnerTeamWID();
	void				setOwnerTeamWID( tdr_ulonglong uTeamWID );
	tdr_ulonglong		getTeamWID();
	bool				isInSameTeam( tdr_ulonglong uTeamWID );
	void				setTeamWID( tdr_ulonglong uTeamWID );
	bool				isInTeam()
	{
		return m_uTeamWID != 0;
	}

	void				setCaptain( bool bFlag ) 
	{ 
		m_bCaptain = bFlag; 
	}
	bool				isCaptain ()			 
	{ 
		return m_bCaptain; 
	}

	virtual void		setType( int type );
	int					getType();

	int					getGender();
	int					getRace();

	int					getItemNum();

	void				setArmStar( int star );
	int					getArmStar();

	virtual void		setHP( int hp );			// 设置血量（伤害计算后）
	void				setTmpHp( int hp );			// 预设实际血量
	void				reSetHp( int hp );			// 重置血量
	unsigned int		getHP();
	virtual void		setMaxHP(int hp);
	unsigned int		getMaxHP();
	
	void				setMP(int mp);
	unsigned int		getMP();
	void				setMaxMP(int mp);
	unsigned int		getMaxMP();

	void				setVigor( int vigor );
	unsigned int		getVigor();
	void				setVigorMax( int vigorMax );
	unsigned int		getVigorMax();
	
	int					getXP();
	void				setXP( int xp );
	int					getMaxXP();

	int					getVIM();
	int					getMaxVIM();

	void				setLv(int lv);
	int					getLv();

	long long			getExp();
	unsigned int		getLvExp( int lv );
	int					getOfflineLvExp( int lv );

	unsigned int		getMoney();
	unsigned int		getBull();
	
	int					getStashMoney();
	int					getStashBull();

	int					getGodCoin();
	int					getBindGodCoin();
	int					getVipPoint();
	
	void				setEnerge( int nValue );
	int					getEnerge();
	void				setMaxEnerge( int nValue );
	int					getMaxEnerge();
	
	int					getSpeMove();
	int					getMaxSpeMove();

	int					getSkillPoint();
	void				setQiYue( int nQiYue ) { m_nQiYue = nQiYue; }
	int					getQiYue() { return m_nQiYue; }			

	bool				isCamp( unsigned short camp );

	void				setOwnerFlag(int flag);
	void				setOwnerName( const char* pszOwnerName );
	const char*			getOwnerName();
	void				setOwnerId( int nId );
	int					getOwnerId();
	int					getClanPosition();
	int					getPetIconId();
	int					getPetDefId();
	
	void				setDesignation( const char* pszDesignation );// 设置称号
	const char*			getDesignation();

	virtual void		setDesignationID( int nDesignationID );
	int					getDesignationID( );

	int					getCampIdx();
	bool				isSameRoleWId(RoleAttr *other);
	bool				isSameRoleWId(tdr_ulonglong wId);
	//战场 荣誉
	unsigned int		getBattleGlory();
	unsigned short		getAoShanBrand();
	unsigned short		getLiftBrand();
	unsigned int		getCharm();
	unsigned int		getSchoolVal();

	//战勋信息
	tdr_ulonglong		getDaoBaClanWid();
	unsigned int		getDaoBaEnd();
	const char*			getDaoBaName();

	int					getClanGroupID(){ return m_nClanGroup; }
	bool				isInClanGroup();

	//末世殿堂荣耀
	unsigned int		getSpartaGlory();
	//末世荣耀殿堂战斗力
	int getSpartaFightVal(){ return m_SpartaFightVal; }

	unsigned int		getZhengYiPoint();				// 正义点数
	//tolua_end
	virtual	void		setRelationType( int nRelationType );
	int					getRelationType();
	virtual void		setObjCamp( int nObjCamp );
	int					getObjCamp();


	//战勋信息
	void setDaoBaClanWid( tdr_ulonglong clanWid );
	void setDaoBaEnd( unsigned int end );
	void setDaoBaName( const char* name );

	//末世荣耀殿堂战斗力
	void setSpartaFightVal( int nVal ){ m_SpartaFightVal = nVal; }
	
	virtual void		updateUI( int nChangeValue ) {};
public:
	tdr_ulonglong	m_roleWID;							// 对象全局ID
// 	int				m_nBootyId;							// 怪物所属玩家
	int				m_Type;								
	int				m_Gender;							/* 性别 */
	int				m_Race;								/* 职业 */
	int				m_Head;
	int				m_Face;
	int				m_Hair;
	int				m_Health;			
	int				m_nItemNum;
	int				m_nArmStar;
	int				m_ViewFlag;							// 头顶标记
	bool			m_bCaptain;							// 队长标记
	unsigned short	m_ObjCamp;							// 对象阵营
	int				m_nQiYue;							// 队伍契约	
	int				m_nTeamNum;							// 队伍人数
	int				m_nRelation;						// 同主角关系
	int				m_nClanPosition;					// 公会地位
	int				m_nClanGroup;						//公会战团
	int				m_nHasOwner;                        // 是否有拥有者

	unsigned int	DayExp;                          	/* 当日已获得经验值 */
	unsigned int	SkillPoint;                      	/* 技能点 */

	std::string					m_Name;					// 对象名称
	std::string					m_TeamName;				// 队伍名称
	std::string					m_strStallName;			// 商店名称
	std::string					m_strClanName;			// 公会名称
	std::string					m_strDesignation;		// 称号名称
	int							m_nDesignationID;
	std::string					owner_name;

	int							m_nTmpHp;				// 临时血量
	int							m_nHpRef;				// 血量记数
	int							m_nMaxEnerge;
	int							m_nOwnerId;				// 所有者ID
	int							m_nPetDefID;			// 宠物定义ID
	int							m_nPetIconId;			// 宠物ICON的ID
	int							m_nPkMode;				// PK模式
	int							m_nPkValue;				// PK值
	unsigned int				m_nClanID;				// 公会ID
	int							m_nCampIdx;				// 阵营ID
	tdr_ulonglong				m_uClanWID;				// 公会WID
	tdr_ulonglong				m_uTeamWID;				// 队伍WID
	tdr_ulonglong				m_uOwnerTeamWID;		// 拥有者的队伍WID
	bool						m_bIsInPKOverlay;		// 是否在切磋区
	int							m_nSafeOverlayFlag;		// 是否在安全区
	int							m_PKInfoType;
	int							m_nKillNum;				// 熔炉怪物当前杀怪数
	int							m_nKillMax;				// 熔炉怪物最大杀怪数

	int							m_nLostTotemExpNum;		//失落图腾当前经验
	int							m_nLostTotemExpMax;		//失落图腾最大经验

	//战勋信息
	tdr_ulonglong				m_DaoBaClanWid;				//战勋击杀公会wid
	unsigned int				m_DaoBaEnd;					//战勋清除时间
	char						m_DaoBaName[NAME_LEN];				//战勋被谁击杀

	int							m_SpartaFightVal;			//末世荣耀殿堂战斗力
	int							m_nBattePetFlag;			//战宠标记

	int							m_nPatrolID;				//1-怪物机械 2-巡逻，

private:
	std::map<int, CSATTRIDVAL>	m_AttriMap;
	std::map<int, CSATTRIDVAL>	m_BaseAttriMap;
};//tolua_export

inline tdr_ulonglong RoleAttr::getClanWID()
{
	return m_uClanWID;
}
inline int RoleAttr::getClanID()
{
	return m_nClanID;
}

inline void RoleAttr::setClanWID( tdr_ulonglong clanWID )
{
	m_uClanWID = clanWID;
}

inline bool	RoleAttr::isInSameClan( tdr_ulonglong uClanWID )
{
	return m_uClanWID != 0 && m_uClanWID == uClanWID;
}

inline bool RoleAttr::isInClan()
{
	return m_uClanWID != 0;
}

inline tdr_ulonglong RoleAttr::getRoleWId()
{
	return m_roleWID;
}

inline void RoleAttr::setRoleWId( tdr_ulonglong wId )
{
	m_roleWID = wId;
}

inline bool RoleAttr::isSameRoleWId(RoleAttr *other)
{
	if (other == NULL)
	{
		return false;
	}
	return getRoleWId() == other->getRoleWId();
}

inline bool RoleAttr::isSameRoleWId( tdr_ulonglong wId )
{
	if ( wId == NULL || wId == 0 )
	{
		return false;
	}
	return getRoleWId() == wId;
}

inline void RoleAttr::setBaseAttriValue ( int attriId, const CSATTRIDVAL& val )
{
	m_BaseAttriMap[attriId] = val;
}

inline void RoleAttr::setKillCount( int num )
{
	m_nKillNum = num;
}

inline void RoleAttr::setLostTotemExpNum( int num )
{
	m_nLostTotemExpNum = num;
}

//inline const CSATTRIDVAL* RoleAttr::getBaseAttriValue( int attriId )const
//{
//	std::map<int, CSATTRIDVAL>::const_iterator iter = m_BaseAttriMap.find(attriId);
//	if( iter != m_BaseAttriMap.end() )
//	{
//		return &iter->second;
//	}
//
//	return NULL;
//}

inline unsigned short RoleAttr::getBaseAttriValue( int attriId )const
{
	std::map<int, CSATTRIDVAL>::const_iterator iter = m_BaseAttriMap.find(attriId);
	if( iter != m_BaseAttriMap.end() )
	{
		//return iter->second.Vim;
		return *( (unsigned short*)&iter->second );
	}

	return 0;
}

inline void RoleAttr::setAttriValue( int attriId, const CSATTRIDVAL& val )
{
	m_AttriMap[attriId] = val;
	updateUI(attriId);
}

inline const CSATTRIDVAL* RoleAttr::getAttriValule( int attriId )const
{
	std::map<int, CSATTRIDVAL>::const_iterator iter = m_AttriMap.find(attriId);
	if( iter != m_AttriMap.end() )
	{
		return &iter->second;
	}
	return NULL;
}

inline unsigned short RoleAttr::getShortAttriValule( int attriId )const
{
	std::map<int, CSATTRIDVAL>::const_iterator iter = m_AttriMap.find(attriId);
	if( iter != m_AttriMap.end() )
	{
		if ( attriId == ATTR_ID_HEAVY_VAL && iter->second.HeavyVal < 0 )
		{
			return 0;
		}
		return *( (unsigned short*)&iter->second );
	}
	return 0;
}

inline int RoleAttr::getIntAttriValue( int attriId )const
{
	std::map<int, CSATTRIDVAL>::const_iterator iter = m_AttriMap.find(attriId);
	if( iter != m_AttriMap.end() )
	{
		return *( (int*)&iter->second );
	}
	return 0;
}

inline void RoleAttr::setName(const char *pname)
{
	m_Name = pname;
}

inline void	RoleAttr::setClanName( const char* pszClanName )
{
	m_strClanName = pszClanName;
}

inline const char* RoleAttr::getClanName()
{
	return m_strClanName.c_str();
}

inline void RoleAttr::setTeamName( const char* pname )
{
	m_TeamName = pname;
}

inline const char* RoleAttr::getTeamName()
{
	return m_TeamName.c_str();
}

inline tdr_ulonglong RoleAttr::getOwnerTeamWID()
{
	return m_uOwnerTeamWID;	
}

inline void RoleAttr::setOwnerTeamWID( tdr_ulonglong uTeamWID )
{
	m_uOwnerTeamWID = uTeamWID;
}

inline tdr_ulonglong RoleAttr::getTeamWID()
{
	return m_uTeamWID;
}

inline bool RoleAttr::isInSameTeam( tdr_ulonglong uTeamWID )
{
	return m_uTeamWID != 0 && uTeamWID == m_uTeamWID;
}

inline void RoleAttr::setTeamWID( tdr_ulonglong uTeamWID )
{
	m_uTeamWID = uTeamWID;
}

inline const char* RoleAttr::getName()
{
	return m_Name.c_str();
}

inline void RoleAttr::setStallName(const char* pName )
{
	m_strStallName = pName;
}

inline const char* RoleAttr::getStallName()
{
	return m_strStallName.c_str();
}

inline int RoleAttr::getType()
{
	return m_Type;
}

inline int RoleAttr::getGender()
{
	return m_Gender;
}

inline int RoleAttr::getRace()
{
	return m_Race;
}

inline void RoleAttr::setTmpHp(int hp)
{
	m_nTmpHp = hp;
	m_nHpRef++;
}

inline void RoleAttr::reSetHp( int hp )
{
	m_nTmpHp = hp;
	m_nHpRef = 0;
	setHP( hp );
}

inline unsigned int RoleAttr::getHP()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_HP );
	if( p == NULL )
	{
		return 0;
	}
	return p->HP;
}

inline unsigned int RoleAttr::getMaxHP()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_HPMAX );
	if( p == NULL )
	{
		return 0;
	}
	return p->HPMax;
}

inline void RoleAttr::setMP(int mp)
{
	CSATTRIDVAL val;
	if( mp < 0 ) mp = 0;
	val.MP = mp;
	setAttriValue( ATTR_ID_MP, val);
}

inline unsigned int RoleAttr::getMP()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_MP );
	if( p == NULL )
	{
		return 0;
	} 
	return p->MP;
}

inline int RoleAttr::getCampIdx()
{
	return m_nCampIdx;
}

inline void RoleAttr::setMaxMP(int mp)
{
	CSATTRIDVAL val;
	val.MPMax = mp;
	setAttriValue( ATTR_ID_MPMAX, val);
}

inline unsigned int RoleAttr::getMaxMP()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_MPMAX );
	if( p == NULL )
	{
		return 0;
	}
	return p->MPMax;
}

inline void RoleAttr::setVigor( int vigor )
{
	CSATTRIDVAL val;
	val.Vigor = vigor;
	setAttriValue( ATTR_ID_VIGOR, val );
}

inline unsigned int RoleAttr::getVigor()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_VIGOR );
	if ( p == NULL )
	{
		return 0;
	}
	return p->Vigor;
}

inline void RoleAttr::setVigorMax( int vigorMax )
{
	CSATTRIDVAL val;
	val.VigorMax = vigorMax;
	setAttriValue( ATTR_ID_VIGORMAX, val );
}

inline unsigned int RoleAttr::getVigorMax()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_VIGORMAX );
	if ( p == NULL )
	{
		return 0;
	}
	return p->VigorMax;
}

inline int RoleAttr::getXP()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_XP );
	if( p == NULL )
	{
		return 0;
	}
	return p->XP;
}

inline void	RoleAttr::setXP(int xp)
{
	CSATTRIDVAL val;
	val.XP = xp;
	setAttriValue( ATTR_ID_XP, val );
}

inline int RoleAttr::getMaxXP()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_XPMAX );
	if( p == NULL )
	{
		return 0;
	}
	return p->XPMax;
}

inline int RoleAttr::getVIM()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_VIM );
	if( p == NULL )
	{
		return 0;
	}
	return p->Vim;
}

inline int RoleAttr::getMaxVIM()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_VIMMAX );
	if( p == NULL )
	{
		return 0;
	}
	return p->VimMax;
}

inline void RoleAttr::setLv( int lv )
{
	CSATTRIDVAL val;
	val.Level = lv;
	setAttriValue( ATTR_ID_LEVEL, val);
}

inline int RoleAttr::getLv()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_LEVEL );
	if( p == NULL )
	{
		return 0;
	}
	return p->Level;
}

inline long long RoleAttr::getExp()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_EXP );
	if( p == NULL )
	{
		return 0;
	}
	return p->Exp;
}

inline unsigned int RoleAttr::getLvExp( int lv )
{
	tagLevelUP* pLevelUpDef = const_cast<tagLevelUP*>( &DBDataManager::getSingleton().getLevelUP( lv ) );
	if( pLevelUpDef != NULL )
    {
        return pLevelUpDef->LevelNextExp;
	}
	return 0;
}

inline int RoleAttr::getOfflineLvExp( int lv )
{
	tagLevelUP* pLevelUpDef = const_cast<tagLevelUP*>( &DBDataManager::getSingleton().getLevelUP( lv ) );
	if( pLevelUpDef != NULL )
	{
		return pLevelUpDef->MaxOfflineExp;
	}
	return 0;
}

inline unsigned int RoleAttr::getMoney()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_MONEY );
	if( p == NULL )
	{
		return 0;
	}
	return p->Money;
}

inline unsigned int RoleAttr::getBull()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_BULL );
	if( p == NULL )
	{
		return 0;
	}
	return p->Bull;
}

inline int RoleAttr::getStashMoney()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_STASHMONEY );
	if( p == NULL )
	{
		return 0;
	}
	return p->Bull;
}

inline int RoleAttr::getStashBull()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_STASHBULL );
	if( p == NULL )
	{
		return 0;
	}
	return p->Bull;
}


inline int RoleAttr::getGodCoin()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_GODCOIN );
	if( p == NULL )
	{
		return 0;
	}
	return p->GodCoin;
}

inline int RoleAttr::getBindGodCoin()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_BINDGODCOIN );
	if( p == NULL )
	{
		return 0;
	}
	return p->BindGodCoin;
}

inline int RoleAttr::getVipPoint()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_VIPPOINT );
	if( p == NULL )
	{
		return 0;
	}
	return p->VipPoint;
}

inline void RoleAttr::setEnerge( int nValue )
{
	m_AttriMap[ATTR_ID_TMPENERGE].Energe = nValue;
}

inline int RoleAttr::getEnerge()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_TMPENERGE );
	if( p == NULL )
	{
		return 0;
	}
	return p->Energe;
}

inline void RoleAttr::setMaxEnerge( int nValue )
{
	m_nMaxEnerge = nValue;
}

inline int RoleAttr::getMaxEnerge()
{
	return m_nMaxEnerge;
}

inline int RoleAttr::getSpeMove()
{	
	return 0;
}

inline int RoleAttr::getMaxSpeMove()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_SPEMOVEMAX );
	if( p == NULL )
	{
		return 0;
	}
	return p->SpeMovePowerMax;
}

inline int RoleAttr::getSkillPoint()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_SKILLPOINT );
	if( p == NULL )
	{
		return 0;
	}
	return p->SkillPoint;
}

inline bool RoleAttr::isCamp( unsigned short camp )
{
	return (m_ObjCamp&camp)!=0;
}

inline void RoleAttr::setOwnerFlag( int flag)
{
	m_nHasOwner = flag;
}

inline void RoleAttr::setOwnerName( const char* pszOwnerName )
{
	owner_name = pszOwnerName;
}

inline const char* RoleAttr::getOwnerName()
{
	return owner_name.c_str();
}

inline void RoleAttr::setOwnerId( int nId )
{
	m_nOwnerId = nId;
}

inline int RoleAttr::getOwnerId()
{
	return m_nOwnerId;
}

inline int RoleAttr::getClanPosition()
{
	return m_nClanPosition;
}

inline int RoleAttr::getPetIconId()
{
	return m_nPetIconId;
}

inline int	RoleAttr::getPetDefId()
{
	return m_nPetDefID;
}

inline int RoleAttr::getDesignationID()
{
	return m_nDesignationID;
}

inline void RoleAttr::setDesignation( const char* pszDesignation )
{
	m_strDesignation = pszDesignation;
}

inline const char* RoleAttr::getDesignation()
{
	return m_strDesignation.c_str();
}

inline void RoleAttr::setItemNum( int num )
{
	m_nItemNum = num;
}

inline int RoleAttr::getItemNum()
{
	return m_nItemNum;
}
inline void RoleAttr::setArmStar( int star )
{
	m_nArmStar = star;
}
inline int RoleAttr::getArmStar()
{
	return m_nArmStar;
}

inline int	RoleAttr::getRelationType()
{
	return m_nRelation;
}

inline int RoleAttr::getObjCamp()
{
	return m_ObjCamp;
}

inline unsigned int RoleAttr::getBattleGlory()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_BATTLE_GLORY );
	if( p == NULL )
	{
		return 0;
	}
	return p->BattleGlory;
}

inline unsigned int RoleAttr::getCharm()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_CHARM );
	if( p == NULL )
	{
		return 0;
	}
	return p->Charm;
}

inline unsigned int RoleAttr::getSchoolVal()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_SCHOOL_VAL );
	if( p == NULL )
	{
		return 0;
	}
	return p->SchoolVal;
}

inline unsigned short RoleAttr::getAoShanBrand()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_AOSHAN_BRAND );
	if( p == NULL )
	{
		return 0;
	}
	return p->AoShanBrand;
}

inline unsigned short RoleAttr::getLiftBrand()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_LIFT_BRAND );
	if( p == NULL )
	{
		return 0;
	}
	return p->LiftBrand;
}
inline unsigned int RoleAttr::getSpartaGlory()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_SPARTA_GLORY );
	if ( p == NULL )
	{
		return 0;
	}
	return p->SpartaGlory;
}

inline unsigned int RoleAttr::getZhengYiPoint()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_ZHENGYI );
	if ( p == NULL )
	{
		return 0;
	}
	return p->Zhengyi;
}

inline void RoleAttr::setDaoBaClanWid( tdr_ulonglong clanWid )
{
	m_DaoBaClanWid = clanWid;
}
inline tdr_ulonglong RoleAttr::getDaoBaClanWid()
{
	return m_DaoBaClanWid;
}
inline void RoleAttr::setDaoBaEnd( unsigned int end )
{
	m_DaoBaEnd = end;
}
inline unsigned int RoleAttr::getDaoBaEnd()
{
	return m_DaoBaEnd;
}
inline void RoleAttr::setDaoBaName( const char* name )
{
	strcpy( m_DaoBaName, name );
}
inline const char* RoleAttr::getDaoBaName()
{
	return m_DaoBaName;
}
inline bool RoleAttr::isInClanGroup()
{
	if ( m_nClanGroup > 0 )
	{
		return m_nClanGroup <= MAX_CLAN_GROUP_NUM;
	}
	return false;
}