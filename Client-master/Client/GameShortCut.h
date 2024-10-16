#pragma once
//////////////////////////////////////////////////////////////////////////
// 快捷键管理
//////////////////////////////////////////////////////////////////////////
#include "OgreSingleton.h"
#include "GameUIMsg.h"
#include "GameSkill_Common.h"
#include "NetMessage.h"
#include "GameMsgPrint.h"
#include "cs_net.h"

const int MAX_SHORCUT		= 47;
const int MAX_XPSHORCUT		= 3;
const int KEY_UNUSED		= -1;
const int SHORTCUTF1		= 5;

const short SHORTCUT_VERSION = 1002;

#pragma pack(1)
struct tagShortCutSave
{
	char	nType;
	int	    Id;
	char	Fitting;
	char	nLv;
};
#pragma pack()

enum
{
	SHORTCUT_NORMAL,
	SHORTCUT_TMP,
};

//tolua_begin 
enum
{
	SELECT_CTRL_L,
	SELECT_ALT_L,
	SELECT_ALT_R,
	SELECT_CTRL_R,

	SELECT_END
};
//tolua_end

class GameShortCut //tolua_export
	: public Ogre::Singleton<GameShortCut>, public ISpellUIControl, public IGameMsgPrint
{//tolua_export
public: 
	GameShortCut(void);
	~GameShortCut(void);
	
	void setShortCut( const tagShortCut& shortcut, int num );
	
	virtual void setCoolTime(  int id, int starttime, int endtime  );
	virtual void setCollect( float totleTime, float during, int times );
	virtual void setChannel( float totleTime );
	virtual void lockUsed( bool flage = true );
	virtual void addSpringSpell( int skillId, int nFittingIndex, int Lv, int type );
	virtual void delSpringSpell( int skillId );
	virtual void clearSpringSpell();

	virtual void newSkill(int id, int lv);
	virtual void forgetSkill(int id, int lv);
	
	// 获取普通攻击技能
	void	addNormalAttackSkill(int id, int lv);
	void	delNormalAttackSkill(int id);

	//tolua_begin 
	int		getNormalAttackSkill();
	const tagShortCut& getShortCut( int num );

	// 是否可拖拽
	bool canDrag( int index );
	bool canReceiveDrag(int index);
	// 玩家主动施放技能
	bool useSkill(int skillId);
	// 使用快捷键
	void usedShortCut( int index, int nTargetIndex = 0 );
	// 快捷键弹开
	void shorCutUpKey();
	// 删除快捷键
	void delShortCut( int index );
	// 交换快捷键
	void swapShortCut( int src, int des );
	// 设置快捷键到UI
	void setShortCut2UI( int nType, int Id, int index, int nFitting, int nLevel = 0 );
	// 设置快捷键
	void setShortCut( int nType, int Id, int index, int nFitting = 0, int nLevel = 0 );
	// 初始化新玩家快捷键
	void initNewPlayerShortCut();
	// 设置宠物技能快捷键
	void setPetFittingShortCut( int Id, int nSkillLv, int index, int nFitting = 0 );
	// 设置临时机械技能快捷键
	void setTmpShortCut(  int Id, int nSkillLv, int index, int nFitting, int nType = SCT_SKILL );
	// 获取临时技能快捷键
	int getTmpShortCutId( int nIndex );
	// 获取临时技能快捷键
	int getTmpShortCutLv( int nIndex );
	// 使用特殊技能
	void useSpecialSkill();
	// 使用Tab寻找最近的怪物
	void usedTab( int nType = 0 );
	// 用F12屏蔽所有玩家
	void usedF12();
	// 用F11屏蔽所有玩家名字
	void usedF11();
	// 设置组合键位的技能
	void setSPShortCut( int nType, int nValue );
	// 获取组合键位技能
	int  getSPShortCut( int nType );
	// 获取要使用的快捷键
	tagShortCut* getUseShortCut();
	// 获取要使用的快捷键上的道具
	tagShortCut& getUseShortCut( int nIndex );
	// 重置快捷键
	void  resetShortCut();
	// 设置快捷键处于按下状态
	void  setShortCutPress(bool bPress);
	void  setShortCutKeyPress(int index, int bPress);
	//快捷键FULL后 获得技能快捷键索引
	void  setNewSkillShortcutIndex(char index);
	char  getNewSkillShortcutIndex();
	//tolua_end

	void update( unsigned int tick );
private:

	void updateUI( int num );
	void sendsetshortcutMsg();

public:
	int						m_SPShortCutSkill[SELECT_END];
	char					m_cShortcutIndex;
private:	
	std::list<tagShortCut>	m_replaySpellList;
	tagShortCut				m_ShorCut[MAX_SHORCUT];
	tagShortCut				m_TmpShorCut[MAX_SHORCUT];
	bool					m_bLock;
	int						m_nKey;
	int						m_nShortCutType;
	bool					m_isMousePress;
	int						m_isKeyPress[MAX_SHORCUT];
	
};//tolua_export
