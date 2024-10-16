#pragma once
#include "GameEngineTypes.h"
#include "OgreMemoryDelegation.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "SmartPtr.h"
#include <map>
#include <vector>
#include <list>

const int MAX_EFF			= 5;
const int ACTION_ID_NONE	= -1;
const int OUTOF_DIS			= -1;	// ���ڹ�����Χ
const int DEFAULT_DELAY		= 400;	// λ�Ƽ��ܵ�Ĭ���ӳ٣�200����
const int LOCK_ENEMY		= 5000;	// �Զ��������˵�ʱ��

typedef std::map<std::string, void*> ATTRIB_MAP;	

unsigned int getSystemTick();
const SKILLRESULT* z_skill_get_result(const SKILLDEF *pstSkillDef, int iResultID);

struct tagSkillAttrib;
struct tagSkillDef;

class SKillObserver;

class CenterPosComparer
{
	GameXYZ center;

public:
	CenterPosComparer(const GameXYZ &pos);
	bool	operator () (GameActor *a, GameActor *b);
};

class GameSpell//tolua_export
	: public AutoRelease<GameSpell>, public Ogre::MemoryDelegation
{//tolua_export
public:

	// ������ѭ����ʱ����
	enum
	{
		STATE_START = 0,		// ������ʼ
		STATE_CATCH,			// ׷��
		STATE_PREPARE,			// ����
		STATE_COLLECT,			// ����
		STATE_PILOT,			// ��������
		STATE_ATTACK,			// ��������
		STATE_BREAK,

		STATE_END,				// ��������
	};

	// ���ܼ�ⷵ��ֵ
	enum {
		RET_SUCCESS = 0,
		RET_ENV,
		RET_SAFEENV,
		RET_WEAPONSHORT,
		RET_CD,
		RET_MPSHORT,
		RET_XPSHORT,
		RET_COLLECTSHORT,
		RET_ENERGYSHORT,
		RET_MACHINEENERGYSHORT,
		RET_INFIGHT,
		RET_OUTFIGHT,
		RET_BUFSTOP,
		RET_MOVESTOP,
		RET_NOTARGET,
		RET_NOTTYPE,
		RET_NOTCAMP,
		RET_DEAD,
		RET_MASKTARGET,
		RET_MASKPOINT,
		RET_DOOTHER,
		RET_SPELLPRE,
		RET_SELFDEAD,
		RET_TETANIC,

		RET_UNKNOWN,
		RET_OUTRANGE,
		RET_NOITEM,
		RET_WORLDCITYCONTROL,
	};

public:
	GameSpell();
	virtual ~GameSpell();

public:
	static void		parserSkillAttrib( Ogre::XMLNode& child, tagSkillAttrib* p, ATTRIB_MAP& attribMap );
	virtual bool	isAutoSkill();
	//tolua_begin
	// ��ȡͬĿ�����
	virtual float			getAttackDist();
	// ������Ч����
	void			playAction(int who, int actionId, int motion_class = 0 );
	// ��������
	virtual void	update(unsigned int dtick);
	virtual void	onUpdate(unsigned int dtick);
	// ���÷�������Ŀ��
	virtual void	setSingleTarget(int targetid);

public:
	// ���ü���Ŀ��
	virtual void setTargets(int num, const int targets[MAX_SKILL_TARGET]);
	// ���ü������԰�
	virtual void setSkillMsgPackage(GameSkill_Package* Package);
	// ������ʼ
	virtual bool start();
	// �жϷ���
	virtual bool Break();
	// ��������
	virtual bool prepare();
	// ��������
	virtual bool collect();
	// ������������
	virtual void endCollect();
	// ����
	virtual bool Attack();
	// �ܻ�
	virtual void Hit();
	// ����һ�δ��
	virtual void attackTime();
	// ������������
	virtual void onPrepare();
	// ���ż�������
	virtual void onCollect();
	// ���ű�������
	virtual void onCollectTime( int time );
	// ���ű���������
	virtual void onCollectFull();
	// ���Ź�������
	virtual void onAttack(bool passive = false);
	// ��ʼ��������
	virtual void onChannel();
	// �����ܻ�����
	virtual void onHit();
	virtual void onHit(int id, GameSkill_Package *package);
	// ���ܱ����
	virtual void onBreak();
	// ����ֹͣ����
	virtual void onStop();

	void onAttack(const std::vector<int> &targets, bool passive = false);
	// ������Чת��
	const char* getAttackEffectName( const char* szEffectName );
	// �ܻ���Чת��
	const char* getHitEffectName(int id, const char* szEffectName);

public:
	// �жϼ���ʩ������
	int		check();
	int		inspect(int id, int x, int y);
	// �жϼ���ʩ����������������ʩ�ŵ��Ŀ��
	int		inspect();
	virtual int		inspect(int id){ return RET_SUCCESS; }
	virtual int		inspect(int x, int y){ return RET_SUCCESS; }
	// ���ü���Id
	void setSpellId( int id );
	// ��ȡ����Id
	int  getSpellId();
	// ���ü��ܵȼ�
	void setSpellLv(int lv);
	// ��ȡ���ܵȼ�
	int	 getSpellLv();
	// ���ü������к�
	void setSpellReq(int req);
	// ��ü������к�
	int	 getSpellReq();
	// ��ȡ������������ź�
	int  getFittingIndex();
	// ��ȡ���ܿ�����
	void setSpellController( ISpellControler* p );
	// ��÷���״̬
	int	 getState();
	// ��ȡ����Ŀ�����
	virtual int  getSingleTarget();
	// ��ȡ����ʩ����
	int	 getOwner();
	// �����������ڵ�
	void setSelfPoint(int x, int y);
	void setSelfPoint(const GameXYZ &pos);
	// ��ȡ�������ڵ�
	const GameXYZ &	getSelfPoint();
	// ���ü���Ŀ���
	void setTargetPoint(int x, int y);
	// ��ȡ����Ŀ���
	virtual const GameXYZ * getTargetPoint();
	// ���û����ӳ�
	void  setHitDelay(int nHitDelay);
	// ��ȡ�����ӳ�
	int   getHitDelay();
	// ���÷���ʩ����
	void setOwner( int id );
	// �Ƿ��Ƕ�����ʩ��
	bool canAttackSelf();
	// ��������λ��
	void setSelfPosChange( int x, int y );
	// ����λ�Ƶص�
	RESPOS circualMovePoint(const SKILLRESULT* pstSkillResult);

	int		getTarget(size_t index);
	size_t	getTargetNum();
	//tolua_end

	// ��������������
	void HitBackPrepareTimeTo(unsigned int time);
	bool isHitBackPrepareTime();
	void onHitBackPrepareTime();
	// ���㼼��Ӱ��Ŀ��
	void calSkillTargets();
	// �Լ�������
	virtual void sortTargets(std::vector<GameActor *> &targets);
	// �ж�Ŀ���Ƿ���ϼ���Ҫ��
	bool filter(GameActor *pActor);

	virtual void* getAttrib() = 0;

	// ���ü���Ŀ���
	virtual void setTargetPoint(const GameXYZ &pos) {}

	int	getCollectNum();
	unsigned int getPilotNum();
	// ��ü��ܵľ�̬����
	const tagSkillDef &			getSkillDef();
	// ��ü��ܵĶ�̬����
	const tagSkillDyAttrib *	getSkillAttrib();
	int							getSelectTarget();

protected:
	// ��ȡ���ʵ�Attrib
	void * getBestAttrib(std::vector<void *> &v);

	void	setCommonCD();
	void	setSelfCD();
	void	resetCD();

public:
	RESPOS				m_SelfPosChange;			// ����λ�øı��
	int					m_SkillState;				// ����״̬
	unsigned int		m_lastTime;
	std::list<SKillObserver *>	m_lstObserver;
	int					m_nPilotNum;				// ������������

protected:
	bool				m_selfCoolSet;
	bool				m_commonCoolSet;
	GameXYZ				m_selfPos;
	GameXYZ				m_TargetPoint;				// ʩ�ŵ�
	int					m_preSelectTarget;
	int					m_SkillID;
	int					m_SkillLv;
	int					m_SkillReq;
	int					m_OwnerID;					
	void				*m_pSkillAttrib;

	int					m_nCollectGrid;				// ��������

	ISpellControler*	m_pSpellControler;			// ����������

	const char*			m_pSpellEffName[MAX_EFF];
	int					m_pSpellEffCount;

protected:
	std::vector<GameSkill_Package *>	m_pGameSkillPackage;		// ������Ϣ��
	std::vector<std::list<SkillHit *>>	m_lstHits;
	std::vector<std::vector<int>>		m_TargetVector;				// ��������Ŀ��
	bool								m_isHitBackPrepare;
};//tolua_export

typedef SmartPtr<GameSpell> GameSpellPtr;

inline int GameSpell::getSpellId()
{
	return m_SkillID;
}

inline int	GameSpell::getCollectNum()
{
	return m_nCollectGrid;
}

inline int GameSpell::getFittingIndex()
{
	return 0;
}

inline void GameSpell::setSpellController( ISpellControler* p )	
{ 
	m_pSpellControler = p;  
}

inline int GameSpell::getSelectTarget()
{
	return m_preSelectTarget;
}

inline int GameSpell::getState()
{
	return m_SkillState;
}

inline int GameSpell::getOwner()
{
	return m_OwnerID;
}

inline void GameSpell::setHitDelay(int nHitDelay)
{
}

class SKillObserver : public Ogre::MemoryDelegation
{
public:
	virtual ~SKillObserver(){}
	virtual SKillObserver *clone() = 0;
	virtual void	onPrepare(GameSpell *pSpell){}
	virtual void	onCollect(GameSpell *pSpell){}
	virtual void	onCollectTime(GameSpell *pSpell, int count){}
	virtual void	onCollectFull(GameSpell *pSpell){}
	virtual void	onAttack(GameSpell *pSpell){}
	virtual void	onHit(GameSpell *pSpell){}
	virtual void	onHited(GameSpell *pSpell){}
	virtual void	onHited(GameSpell *pSpell, int id){}
	virtual void	onUpdate(GameSpell *pSpell, unsigned int time){}
	virtual void	onStop(GameSpell *pSpell){}
};

#define REGIST_OBSERVER(classname) GameSpell::registObserver(classname::tag, classname::create)
#define DECLARE_OBSERVER(classname) \
	public:\
	static SKillObserver* create(Ogre::XMLNode& node)\
	{\
		return new classname(node);\
	}\
	static const char *tag;\
	virtual SKillObserver *clone()\
	{\
		return new classname(*this);\
	}