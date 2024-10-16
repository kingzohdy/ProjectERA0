#ifndef __GameSkill_H__
#define __GameSkill_H__
#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include "OgreFixedString.h"
#include "OgreRect.h"
#include "GameSkill_Common.h"
#include "GameSkill_DefToLua.h"
#include "GameSkill_Base.h"
#include "GameSkill_Control.h"
#include <map>

class ISpellControler;
class GameSpellControler;

enum
{
	MAX_ACTION_NAME = 16,
};

const int nLeftSkill[] = {
	0,
	1010,
	2010,
	3010,
	4000
};

typedef GameSpell* (*FUN)();
typedef std::list<GameSpellPtr>			SPELL_LIST;
typedef std::list<GameSpellPtr>::iterator	SPELL_LIST_ITER;	

class SkillManager //tolua_export
	: public NetMsgHandler, public Ogre::Singleton<SkillManager>
{//tolua_export
	friend SkillHit;

public:
	//tolua_begin
	enum
	{
		MAX_ARMTYPE = MAX_SKILL_ARMTYPE
	};
	//tolua_end

public:
	SkillManager();
	~SkillManager();
	
	static void registerSpell( int nId, FUN p, tagSkillAttrib* pDes ); 
	static void registerSpellAttrib( int nId, ATTRIB_MAP& attrib_map );
	
	virtual int	HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );
	void initSkill();
	void clearSkill();

	void reset();

	ISpellControler*	getSpellControler(int id);
	//tolua_begin
	bool				isNormalSkill(int id);
	bool				isNormalSkill(int id, int lv);
	bool				isSkillPassive(int id);
	bool				isSkillPassive(int id, int lv);
	bool				isSkillLearned(int id);
	int					getNextLearnedSkill(int id);
	void				reloadSkill();
	void				reSetSkilList();
	GameSpellControler*	getSpellControler();
	ISpellUIControl*	getSpellUIControl();
	void				setSpellUIControl(ISpellUIControl* p);
	int					getSpellLv(int id);
	int					getSpellCollectNumUse(int skillid, int lv);
	int					getSpellCollectNumCur(int skillid);
	void				setSpellCollectNumCur(int id, int nCollectNum);

	tagSkillAttrib*		getSpellDes( int skillid );
	const tagSkillDef_ToLua*  getSpellStaticAtt( int skillid, int lv );
	tagDiathesisDef_ToLua*	getDiathesisDef( int id, int lv );
	void				setTmpSkill( int index, int skillId, int skillLv );
	int					getTmpSkillId( int index );
	int					getTmpSkillLv( int index );
	// 确认法术是否足够
	int					checkMagic( int skillid );
	// 是否是无双技能
	bool				isXPSkill( int skillid );
	// 获得当前职业学习等级的技能ID
	int					getLevelSkillId( int nRace, int nLv, int nIndex );
	GameSpell*			getIntentSpell();
	void				clearIntentSpell();
	GameSpell*			getReadySpell();
	void				setReadySpell(GameSpell *pSpell = NULL);
	GameSpell*			getTurnSpell();
	void				setTurnSpell(GameSpell *pSpell = NULL);
	void				setAngleSpell( GameSpell *pSpell = NULL );
	GameSpell*			getAngleSpell();
	void				sendBreakMsg(GameSpell *pSpell);
	void				sendPrepareMsg(GameSpell *pSpell);
	void				sendAttackMsg(GameSpell *pSpell, int BulletId = 0);
	void				sendAttackMsg(GameSpell *pSpell, int skillReq, int BulletId);
	void				sendHitMsg(GameSpell *pSpell, int BulletId = 0);
	void				setNextSpell(GameSpell *pSpell = NULL, int id = 0, bool immediately = true);
	void				setNextSpell(int skillId, bool immediately = true);
	void				setNextSpell(int skillId, int targetId, bool immediately = true);
	void				setNextSpell(int skillId, int x, int y, bool immediately = true);
	void				setNextSpell(int skillId, int targetId, int x, int y, bool immediately = true);
	GameSpell *			getNextSpell();
	unsigned int		getSkillReq();
	void				setSkillReq(unsigned int i);
	void				delSkillDyAttrib(int id);
	void				setSkillDyAttrib(int id, tagSkillDyAttrib *attrib);

	unsigned int		getSkillCommCoolTime(int id);
	unsigned int		getSkillSelfCoolTime(int id);
	int					getSkillDistance(int id);
	int					getSkillDistance(int id, int lv);
	unsigned int		getSkillPrepareTime(int id, int lv);
	unsigned int		getSkillPrepareTime(int id);
	unsigned int		getSkillCollectTime(int id, int lv);
	unsigned int		getSkillCollectTime(int id);
	int					getSkillCollectNum(int id, int lv);
	int					getSkillCollectNum(int id);
	unsigned int		getSkillPilotTime(int id, int lv);
	unsigned int		getSkillPilotTime(int id);
	int					getSkillPilotNum(int id, int lv);
	int					getSkillPilotNum(int id);
	int					getSkillCostMp(int id, int lv);
	int					getSkillCostMp(int id);
	int					getSkillCostCollect(int id, int lv);
	int					getSkillCostCollect(int id);
	int					getSkillCostXp(int id, int lv);
	int					getSkillCostXp(int id);
	const RESRANGE &	getSkillMainRange(int id);
	unsigned int		getSkillMaxTargets(int id);
	tagSkillDyAttrib *	getSkillDyAttrib(int id);
	void				addTmpSkill(int id);
	void				delTmpSkill(int id);
	bool				isTmpSkill(int id);
	bool				canMoveAttack(int id);
	bool				isNoPrepareSkill(int id);
	bool				canCastSelf(int id);
	void				setCommonCDTime(int id);
	void				resetSkillCDTime(int id);
	void				setSkillCDTime(int id);
	void				setSkillCDTime(int id, unsigned int left);
	void				reduceCD(int id, unsigned int reduceTime);
	void				clearCD(int id);
	void				clearAllCD();
	void				checkCD_resetTypeCD(int t, unsigned int v);
	void				checkCD_setTypeCD(int t, unsigned int v, unsigned int time);
	const tagCDTime &	getSkillCDTime(int id);
	const tagCDTime &	getCommonCDTime();
	const tagCDTime &	getCDTime(int id);
	int					castInspect(int id);
	GameSpell *			getSpellSample(int id);
	GameSpell *			createSpell(int id, ISpellControler *sc);
	void				update(unsigned int dtick);
	void				deleteActorSpell(int id);

	int					getSpellWeaponType(int id, unsigned int idx);
	int					getSpellWeaponType(int id, int lv, unsigned int idx);

	bool				castInMap(int id);
	bool				castInMap(int id, int lv);
	bool				castInPworld(int id);
	bool				castInPworld(int id, int lv);
	bool				castWeapon(int id);
	bool				castWeapon(int id, int lv);
	bool				castSkillCD(int id);
	bool				castCommonCD(int id);
	bool				castCD(int id);
	bool				castMP(int id);
	bool				castMP(int id, int lv);
	bool				castXP(int id);
	bool				castXP(int id, int lv);
	bool				castCollect(int id);
	bool				castCollect(int id, int lv);
	bool				castEnergy(int id);
	bool				castEnergy(int id, int lv);
	bool				castMachineEnergy(int id);
	bool				castMachineEnergy(int id, int lv);
	bool				castMorph(int id);
	bool				castPlayerLv(int id);
	bool				castPlayerLv(int id, int lv);
	bool				castItem(int id);
	bool				castItem(int id, int lv);

	int					useSpellOnce(int id);
	int					useSpellOnce(int id, int target);
	int					useSpellOnce(int id, int x, int y);
	int					useSpellOnce(int id, int target, int x, int y);
	int					useSpell(int id);
	int					useSpell(int id, int target);
	int					useSpell(int id, int x, int y);
	int					useSpell(int id, int target, int x, int y);
	bool				testResult(int result);
	void				unUseSpell();
	bool				normalAttack(int id);
	bool				onStart(GameSpell* pSpell);
	void				onStop();

	void				spellAttack(GameSpell *p, unsigned int delay);
	void				spellStop(GameSpell *p, unsigned int delay);
	void				spellBreak(GameSpell *p, unsigned int delay);
	void				execSpellStop();

	int					getNormalAttackSkill(int actorId);
	int					getNormalAttackSkill();

	//======================================配件相关======================================
	int					getInsteadSkillId(int id);
	void				addInsteadBind(int id, int repId);
	void				delInsteadBind(int id, int repId);

	int					getRuneSkillId(int id);
	int					getReserveSkillId(int id);

	bool				getSpellRuneEnable(int id);
	char				getSpellRuneType(int id);
	int					getSpellRuneItemId(int id);
	int					getSpellRuneItemId(int id, int type);
	const char *		getSpkillRuneDesc(int id, int level, int type);
	const char *		getSpkillRuneDesc(int id, int type);
	int					getSpkillRuneSkillId(int id, int type);
	//tolua_end

public:
	static std::map<int, ATTRIB_MAP>	m_SkillAttribMap;		// 技能属性表

private:
	struct tagtmpSkillInfo
	{
		int m_nSkillId;
		int m_nLv;
		tagtmpSkillInfo()
		{
			m_nSkillId = 0;
			m_nLv = 0;
		}
	};
	int										m_isInAttack;
	std::map<int, tagSkillDyAttrib*>		m_SkillDyAttrib;
	std::map<int, ISpellControler *>		m_ActorSpellControl;
	std::map<int, GameSpellPtr>				m_SpellSample;
	std::map<int, tagCDTime>				m_SkillCD;
	tagCDTime								m_PublicCD;
	ISpellUIControl							*m_SpellUIControl;
	unsigned int							m_skillReq;
	GameSpellPtr							m_pIntentSpell;
	GameSpellPtr							m_pReadySpell;
	GameSpellPtr							m_pTurnSpell;
	GameSpellPtr							m_pNextSpell;

	GameSpellPtr							m_pAngleSpell;			// 类角度技能
	int										m_pNextSpellId;
	unsigned int							m_nextTime;
	int										m_lockSkill;
	std::set<int>							m_TmpSkills;

	tagtmpSkillInfo							m_nTmpSkillId[4];		// 临时技能
	static std::map<int, FUN>				m_SkillCreateFun;		// 技能构造函数
	static std::map<int, tagSkillAttrib*>	m_SkillDes;				// 技能描述信息

	std::map<int, std::pair<int, std::vector<int>>>	m_InsteadSkills;
	std::map<int, int>						m_RuneSkill;
	std::map<int, int>						m_ReserveSkill;

	std::set<SkillHit *>									hitArray;
	std::vector<std::pair<unsigned int, GameSpellPtr>>		attackArray;
	std::vector<std::pair<unsigned int, GameSpellPtr>>		stopArray;
	std::vector<std::pair<unsigned int, GameSpellPtr>>		breakArray;
};//tolua_export

inline unsigned int SkillManager::getSkillReq()
{
	return m_skillReq;
}

inline void SkillManager::setSkillReq(unsigned int i)
{
	if (i > m_skillReq)
	{
		m_skillReq = i;
	}
}

#endif