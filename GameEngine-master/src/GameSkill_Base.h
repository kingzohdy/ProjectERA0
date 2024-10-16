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
const int OUTOF_DIS			= -1;	// 不在攻击范围
const int DEFAULT_DELAY		= 400;	// 位移技能的默认延迟，200毫秒
const int LOCK_ENEMY		= 5000;	// 自动锁定敌人的时间

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

	// 法术遵循以下时序发生
	enum
	{
		STATE_START = 0,		// 法术开始
		STATE_CATCH,			// 追逐
		STATE_PREPARE,			// 吟唱
		STATE_COLLECT,			// 集气
		STATE_PILOT,			// 法术引导
		STATE_ATTACK,			// 法术击出
		STATE_BREAK,

		STATE_END,				// 法术结束
	};

	// 技能检测返回值
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
	// 获取同目标距离
	virtual float			getAttackDist();
	// 动作特效播放
	void			playAction(int who, int actionId, int motion_class = 0 );
	// 法术心跳
	virtual void	update(unsigned int dtick);
	virtual void	onUpdate(unsigned int dtick);
	// 设置法术单个目标
	virtual void	setSingleTarget(int targetid);

public:
	// 设置技能目标
	virtual void setTargets(int num, const int targets[MAX_SKILL_TARGET]);
	// 设置技能属性包
	virtual void setSkillMsgPackage(GameSkill_Package* Package);
	// 法术开始
	virtual bool start();
	// 中断法术
	virtual bool Break();
	// 法术吟唱
	virtual bool prepare();
	// 法术集气
	virtual bool collect();
	// 法术结束集气
	virtual void endCollect();
	// 攻击
	virtual bool Attack();
	// 受击
	virtual void Hit();
	// 进行一次打击
	virtual void attackTime();
	// 播放吟唱动作
	virtual void onPrepare();
	// 播放集气动作
	virtual void onCollect();
	// 播放爆气动作
	virtual void onCollectTime( int time );
	// 播放爆气满动作
	virtual void onCollectFull();
	// 播放攻击动作
	virtual void onAttack(bool passive = false);
	// 开始引导动作
	virtual void onChannel();
	// 播放受击动作
	virtual void onHit();
	virtual void onHit(int id, GameSkill_Package *package);
	// 技能被打断
	virtual void onBreak();
	// 播放停止动作
	virtual void onStop();

	void onAttack(const std::vector<int> &targets, bool passive = false);
	// 攻击特效转换
	const char* getAttackEffectName( const char* szEffectName );
	// 受击特效转换
	const char* getHitEffectName(int id, const char* szEffectName);

public:
	// 判断技能施放条件
	int		check();
	int		inspect(int id, int x, int y);
	// 判断技能施放条件，给出技能施放点和目标
	int		inspect();
	virtual int		inspect(int id){ return RET_SUCCESS; }
	virtual int		inspect(int x, int y){ return RET_SUCCESS; }
	// 设置技能Id
	void setSpellId( int id );
	// 获取技能Id
	int  getSpellId();
	// 设置技能等级
	void setSpellLv(int lv);
	// 获取技能等级
	int	 getSpellLv();
	// 设置技能序列号
	void setSpellReq(int req);
	// 获得技能序列号
	int	 getSpellReq();
	// 获取技能所用配件排号
	int  getFittingIndex();
	// 获取技能控制者
	void setSpellController( ISpellControler* p );
	// 获得法术状态
	int	 getState();
	// 获取单个目标对象
	virtual int  getSingleTarget();
	// 获取法术施放者
	int	 getOwner();
	// 设置自身所在点
	void setSelfPoint(int x, int y);
	void setSelfPoint(const GameXYZ &pos);
	// 获取自身所在点
	const GameXYZ &	getSelfPoint();
	// 设置技能目标点
	void setTargetPoint(int x, int y);
	// 获取技能目标点
	virtual const GameXYZ * getTargetPoint();
	// 设置击中延迟
	void  setHitDelay(int nHitDelay);
	// 获取击中延迟
	int   getHitDelay();
	// 设置法术施放者
	void setOwner( int id );
	// 是否是对自身施放
	bool canAttackSelf();
	// 设置自身位移
	void setSelfPosChange( int x, int y );
	// 计算位移地点
	RESPOS circualMovePoint(const SKILLRESULT* pstSkillResult);

	int		getTarget(size_t index);
	size_t	getTargetNum();
	//tolua_end

	// 吟唱、集气回退
	void HitBackPrepareTimeTo(unsigned int time);
	bool isHitBackPrepareTime();
	void onHitBackPrepareTime();
	// 计算技能影响目标
	void calSkillTargets();
	// 对技能排序
	virtual void sortTargets(std::vector<GameActor *> &targets);
	// 判断目标是否符合技能要求
	bool filter(GameActor *pActor);

	virtual void* getAttrib() = 0;

	// 设置技能目标点
	virtual void setTargetPoint(const GameXYZ &pos) {}

	int	getCollectNum();
	unsigned int getPilotNum();
	// 获得技能的静态数据
	const tagSkillDef &			getSkillDef();
	// 获得技能的动态数据
	const tagSkillDyAttrib *	getSkillAttrib();
	int							getSelectTarget();

protected:
	// 获取合适的Attrib
	void * getBestAttrib(std::vector<void *> &v);

	void	setCommonCD();
	void	setSelfCD();
	void	resetCD();

public:
	RESPOS				m_SelfPosChange;			// 自身位置改变点
	int					m_SkillState;				// 法术状态
	unsigned int		m_lastTime;
	std::list<SKillObserver *>	m_lstObserver;
	int					m_nPilotNum;				// 引导攻击次数

protected:
	bool				m_selfCoolSet;
	bool				m_commonCoolSet;
	GameXYZ				m_selfPos;
	GameXYZ				m_TargetPoint;				// 施放点
	int					m_preSelectTarget;
	int					m_SkillID;
	int					m_SkillLv;
	int					m_SkillReq;
	int					m_OwnerID;					
	void				*m_pSkillAttrib;

	int					m_nCollectGrid;				// 集气几格

	ISpellControler*	m_pSpellControler;			// 法术控制者

	const char*			m_pSpellEffName[MAX_EFF];
	int					m_pSpellEffCount;

protected:
	std::vector<GameSkill_Package *>	m_pGameSkillPackage;		// 技能信息包
	std::vector<std::list<SkillHit *>>	m_lstHits;
	std::vector<std::vector<int>>		m_TargetVector;				// 法术受众目标
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