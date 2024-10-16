#pragma once
#include <map>
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameDBData.h"

class RoleAttr//tolua_export
{//tolua_export
public:
	RoleAttr(void) {};
	~RoleAttr(void){};

	void				setAttriValue ( int attriId, const CSATTRIDVAL& val );
	const CSATTRIDVAL*	getAttriValule( int attriId )const;
	
	//tolua_begin
	unsigned short		getShortAttriValule( int attriId )const;
	int					getIntAttriValue( int attriId )const;

	void				setName(const char *pname);
	const char*			getName();
	int					getHead() { return m_Head; }
	int					getFace() { return m_Face; }
	int					getHair() { return m_Hair; }
	void				setViewFlg( int flag ) { m_ViewFlag = flag; }
	
	void				setTeamName( const char* pname );
	const char*			getTeamName();
	void				setCaptain( bool bFlag ) { m_bCaptain = bFlag; }
	bool				isCaptain ()			 { return m_bCaptain;  }

	void				setType( int type );
	int					getType();

	int					getGender();
	int					getRace();

	void				setHP(int hp);
	int					getHP();
	void				setMaxHP(int hp);
	int					getMaxHP();
	
	void				setMP(int mp);
	int					getMP();
	void				setMaxMP(int mp);
	int					getMaxMP();

	void				setLv(int lv);
	int					getLv();

	void				setHealth( int nvalue );
	int					getHealth();

	int					getExp();
	int					getLvExp( int lv );

	int					getMoney();
	int					getBull();
	
	void				setEnerge( int nValue );
	int					getEnerge();
	void				setMaxEnerge( int nValue );
	int					getMaxEnerge();
	
	int					getSpeMove();
	int					getMaxSpeMove();

	int					getSkillPoint();

	bool				isCamp( unsigned short camp );
	//tolua_end
	
	virtual void		updateUI( int nChangeValue ) {};
public:
	int				m_Type;								
	int				m_Gender;							/* �Ա� */
	int				m_Race;								/* ְҵ */
	int				m_Head;
	int				m_Face;
	int				m_Hair;
	int				m_Health;			
	int				m_ViewFlag;							/* ͷ����� */
	bool			m_bCaptain;
	unsigned short	m_ObjCamp;							// ������Ӫ

	unsigned int	DayExp;                          	/* �����ѻ�þ���ֵ */
	unsigned int	SkillPoint;                      	/* ���ܵ� */
	unsigned int	Money;                           	/* ��Ǯ */
	unsigned int	Bull;                            	/* ���� */
	unsigned int	BlueJade;                        	/* ���� */
	unsigned int	GreenJade;                       	/* ���� */
	unsigned int	RedJade;                         	/* ���� */
	unsigned int	WhiteJade;                       	/* ѩ�� */
	unsigned int	BlackJade;                       	/* ī�� */
	unsigned short	BlueGem;							/* ����ʯ */
	unsigned short	GreenGem;							/* �̱�ʯ */
	unsigned short	RedGem;								/* �챦ʯ */
	unsigned short	WhiteGem;							/* �ױ�ʯ */
	unsigned short	BlackGem;							/* �ڱ�ʯ */
		
protected:
	std::string					m_Name;
	std::string					m_TeamName;

	int							m_nHealth;
	int							m_nMaxEnerge;
private:
	std::map<int, CSATTRIDVAL>	m_AttriMap;
};//tolua_export

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

inline void RoleAttr::setTeamName( const char* pname )
{
	m_TeamName = pname;
}

inline const char* RoleAttr::getTeamName()
{
	return m_TeamName.c_str();
}

inline const char* RoleAttr::getName()
{
	return m_Name.c_str();
}

inline void RoleAttr::setType( int type )
{
	m_Type = type;
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

inline void RoleAttr::setHP(int hp)
{
	CSATTRIDVAL val;
	if( hp < 0 ) hp = 0;
	val.HP = hp;
	setAttriValue( ATTR_ID_HP, val);
}

inline int RoleAttr::getHP()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_HP );
	if( p == NULL )
	{
		return 0;
	}
	return p->HP;
}

inline void RoleAttr::setMaxHP(int hp)
{
	CSATTRIDVAL val;
	val.HPMax = hp;
	setAttriValue( ATTR_ID_HPMAX, val);
}

inline int RoleAttr::getMaxHP()
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

inline int RoleAttr::getMP()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_MP );
	if( p == NULL )
	{
		return 0;
	} 
	return p->MP;
}

inline void RoleAttr::setMaxMP(int mp)
{
	CSATTRIDVAL val;
	val.MPMax = mp;
	setAttriValue( ATTR_ID_MPMAX, val);
}

inline int RoleAttr::getMaxMP()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_MPMAX );
	if( p == NULL )
	{
		return 0;
	}
	return p->MPMax;
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

inline void RoleAttr::setHealth( int nvalue )
{
	m_Health = nvalue;
}

inline int RoleAttr::getHealth()
{
	return m_Health;
}

inline int RoleAttr::getExp()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_EXP );
	if( p == NULL )
	{
		return 0;
	}
	return p->Exp;
}

inline int RoleAttr::getLvExp( int lv )
{
	const tagLevelUP LevelUpDef = DBDataManager::getSingleton().getLevelUP( lv );
	return LevelUpDef.LevelNextExp;
}

inline int RoleAttr::getMoney()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_MONEY );
	if( p == NULL )
	{
		return 0;
	}
	return p->Money;
}

inline int RoleAttr::getBull()
{
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_BULL );
	if( p == NULL )
	{
		return 0;
	}
	return p->Bull;
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
	const CSATTRIDVAL* p = getAttriValule( ATTR_ID_SPEMOVE );
	if( p == NULL )
	{
		return 0;
	}
	return p->SpeMovePower;
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