#pragma once
#include <map>
#include "GameEngineTypes.h"
#include "OgreMemoryDelegation.h"
#include "proto_cs.h"

struct tagCDTime;

inline void copy(tagSkillAttrib &self, const tagSkillAttrib &other);
inline void clear(tagSkillAttrib &self);
inline void init(tagSkillAttrib &self);

//tolua_begin
struct tagSkillDyAttrib
{
	int					SkillLevel;
	int					CollectNum;							// ������
	int					CollectNumMax;						// ����������
	char				RuneSlotEnabled;                 	// ���Ĳ��Ƿ����
	char				RuneSlot;                        	// ���Ĳ� Bind Macrosgroup:RUNE_TYPE
	int					RuneItemID;                      	// ������ƷID
	std::vector<int>	RuneItemIDs;						// ������ƷID
};
//tolua_end

struct tagSkillAttrib//tolua_export
{//tolua_export
	//tolua_begin
	int				m_nSkillId;					// ����Id
	int				m_nBuffId;					// buff״̬ID
	int				m_nFittingId;				// ���ID
	int				m_nBehitDelay;				// ������ʱ
	int				m_BehitId;					// ����������
	int				m_CollectActId;				// ��������Id
	int				m_ActId[3];					// ��������1-4��ʵ������
	int				m_PreActId;					// ��������
	int				m_nHitBackType;				// ��������
	int				m_nHitBackSpeed;			// �����ٶ�
	int				m_nHitBackHight;			// ���˸߶�
	int				m_nHitBackTime;				// ����ʱ��
	std::string		m_strHitBackExtraInfo;		// ������Ϣ
	int				m_nMoveTime;				// λ�ƶ�������ʱ��
	int				m_nHideWeapon;				// ��������ʱ��
	bool			m_isPreHideWeapon;			// ����ʱ�Ƿ���������
	bool			m_isNoBreak;				// ����������ʱ�Ƿ������ж�
	std::string		m_strPreEffName;			// ׼����Ч����
	std::string		m_ActEffName;				// ������Ч����
	std::string		m_HitEffName;				// ������Ч����
	std::string		m_CollectEffectName;		// ������Ч
	std::string		m_CollectBlastEffectName;	// ��������
	std::string		m_CollectPower[3];
	std::string		m_strTraceEff;
	int				m_nTraceFadeTime;
	int				m_nTraceTotalTime;
	int				m_nTraceSpeed;
	std::string		m_Name;
	std::string		m_Title;
	std::string		m_strType;
	std::string		m_strScript;

	//////////////////////////////////////////////////////////////////////////
	std::string		m_strThrowtype;
	std::string		m_strFlyActorName;
	float			m_flySpeed;
	int				m_nFlyDesAnchorId;
	int				m_nFlySrcAnchorId;
	float			m_nFlyHeightRadio;
	int				m_nFlyActorNum;
	int				m_nFlyActorDelay;
	bool			m_bFlyMulti;
	//tolua_end

	std::list<SKillObserver *>	m_lstObserver;	// �۲����б�
	tagSkillAttrib(){init(*this);}
	tagSkillAttrib(const tagSkillAttrib &other){copy(*this, other);}
	tagSkillAttrib & operator = (const tagSkillAttrib & other){clear(*this);copy(*this, other);return *this;}
	virtual ~tagSkillAttrib(){clear(*this);}
};//tolua_export

inline void clear(tagSkillAttrib &self)
{
	for (std::list<SKillObserver *>::iterator i = self.m_lstObserver.begin(); i != self.m_lstObserver.end(); ++i)
	{
		delete *i;
	}
	self.m_lstObserver.clear();
}

void init(tagSkillAttrib &self)
{
	self.m_nBuffId = 0;
	self.m_nFittingId = 0;
	self.m_nBehitDelay = 0;
	self.m_BehitId = -1;
	self.m_CollectActId = -1;
	for (size_t i = 0; i < sizeof(self.m_ActId) / sizeof(self.m_ActId[0]); ++i)
	{
		self.m_ActId[i] = -1;
	}
	self.m_PreActId = -1;
	self.m_nHitBackType = 1;
	self.m_nHitBackSpeed = 2000;
	self.m_nHitBackHight = 8000;
	self.m_nHitBackTime = 0;
	self.m_nMoveTime = -1;
	self.m_nHideWeapon = 0;
	self.m_isPreHideWeapon = false;
	self.m_isNoBreak = false;
	self.m_nTraceFadeTime = 0;
	self.m_nTraceTotalTime = 0;
	self.m_nTraceSpeed = 0;
}

inline void copy(tagSkillAttrib &self, const tagSkillAttrib &other)
{
	self.m_nSkillId = other.m_nSkillId;
	self.m_nBuffId = other.m_nBuffId;
	self.m_nFittingId = other.m_nFittingId;
	self.m_nBehitDelay = other.m_nBehitDelay;
	self.m_BehitId = other.m_BehitId;
	self.m_CollectActId = other.m_CollectActId;
	for (int i = 0; i < sizeof(self.m_ActId) / sizeof(self.m_ActId[0]); i++)
	{
		self.m_ActId[i] = other.m_ActId[i];
	}
	self.m_PreActId = other.m_PreActId;
	self.m_nHitBackType = other.m_nHitBackType;
	self.m_nHitBackSpeed = other.m_nHitBackSpeed;
	self.m_nHitBackHight = other.m_nHitBackHight;
	self.m_nHitBackTime = other.m_nHitBackTime;
	self.m_strHitBackExtraInfo = other.m_strHitBackExtraInfo;
	self.m_nMoveTime = other.m_nMoveTime;
	self.m_nHideWeapon = other.m_nHideWeapon;
	self.m_isPreHideWeapon = other.m_isPreHideWeapon;
	self.m_isNoBreak = other.m_isNoBreak;
	self.m_strPreEffName = other.m_strPreEffName;
	self.m_ActEffName = other.m_ActEffName;
	self.m_HitEffName = other.m_HitEffName;
	self.m_CollectEffectName = other.m_CollectEffectName;
	self.m_CollectBlastEffectName = other.m_CollectBlastEffectName;
	for (int i = 0; i < sizeof(self.m_CollectPower) / sizeof(self.m_CollectPower[0]); i++)
	{
		self.m_CollectPower[i] = other.m_CollectPower[i];
	}
	self.m_strTraceEff = other.m_strTraceEff;
	self.m_nTraceFadeTime = other.m_nTraceFadeTime;
	self.m_nTraceTotalTime = other.m_nTraceTotalTime;
	self.m_nTraceSpeed = other.m_nTraceSpeed;
	self.m_Name = other.m_Name;
	self.m_Title = other.m_Title;
	self.m_strType = other.m_strType;
	self.m_strScript = other.m_strScript;
	for (std::list<SKillObserver *>::const_iterator i = other.m_lstObserver.begin(); i != other.m_lstObserver.end(); ++i)
	{
		self.m_lstObserver.push_back((*i)->clone());
	}
}

typedef std::map<int, tagSkillDyAttrib*>::iterator SKILLDYATTRIB_ITER;

class ISpellUIControl
{
public:
	virtual void setCoolTime( int id, int starttime, int endtime ) = 0;
	virtual void setCollect( float totleTime, float during = 0, int times = 1 ) = 0;
	virtual void setChannel( float totleTime ) = 0;
	virtual void lockUsed( bool flage = true ) = 0;
	virtual void addSpringSpell( int skillId, int nFittingIndex, int Lv, int type ) = 0;
	virtual void delSpringSpell( int skillId ) = 0;
	virtual void setTmpShortCut(  int Id, int nSkillLv, int index, int nFitting, int nType = SCT_SKILL  ) = 0;
};

class ISpellControler//tolua_export
	: public Ogre::MemoryDelegation
{//tolua_export
public:
	virtual ~ISpellControler(){}

	// ͨѶ�ӿ�
	virtual void	onPrepare(GameSpell* pSpell)						= 0;
	virtual void	onCollect(GameSpell* pSpell, unsigned int time = 0)	= 0;
	virtual void	onCollectFull(GameSpell* pSpell)					= 0;
	virtual void	onAttack(GameSpell* pSpell, bool passive)			= 0;
	virtual void	onBreak(int skillreq, int oldSkillreq, unsigned int LeftCool)	= 0;
	virtual void	onBreak(GameSpell *pSpell)		= 0;
	virtual void	onPilot(GameSpell* pSpell)							= 0;
	virtual void	onComplete(GameSpell* pSpell)						= 0;

	virtual void	onLock(int id = 0)									= 0;
	virtual void	onHitBack(GameSpell *pSpell, int during, int totalTime, int times = 0) = 0;

	// ���ܲٿ���ӵ����
	virtual int			getOwnerId()	= 0;
	virtual GameActor *	getOwner()		= 0;
	// ���������������������еļ���
	virtual GameSpell*	getCurSpell() = 0;
	virtual GameSpell*	getSpell(int skillId, int skillLv, int skillReq) = 0;
	// ��������
	virtual void		update(unsigned int dtick) = 0;

	// ����
	GameSpell*	getCurPreSpell();
	GameSpell*	getCurCloSpell();
	GameSpell*	getCurPilSpell();
};//tolua_export

inline GameSpell* ISpellControler::getCurPreSpell()
{
	GameSpell *p = getCurSpell();
	if (p == NULL)
	{
		return NULL;
	}
	if (p->getState() == GameSpell::STATE_PREPARE)
	{
		return p;
	}
	return NULL;
}

inline GameSpell* ISpellControler::getCurCloSpell()
{
	GameSpell *p = getCurSpell();
	if (p == NULL)
	{
		return NULL;
	}
	if (p->getState() == GameSpell::STATE_COLLECT)
	{
		return p;
	}
	return NULL;
}

inline GameSpell* ISpellControler::getCurPilSpell()
{
	GameSpell *p = getCurSpell();
	if (p == NULL)
	{
		return NULL;
	}
	if (p->getState() == GameSpell::STATE_PILOT)
	{
		return p;
	}
	return NULL;
}
