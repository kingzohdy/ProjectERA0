#pragma once
//////////////////////////////////////////////////////////////////////////
// ��ݼ�����
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
	
	// ��ȡ��ͨ��������
	void	addNormalAttackSkill(int id, int lv);
	void	delNormalAttackSkill(int id);

	//tolua_begin 
	int		getNormalAttackSkill();
	const tagShortCut& getShortCut( int num );

	// �Ƿ����ק
	bool canDrag( int index );
	bool canReceiveDrag(int index);
	// �������ʩ�ż���
	bool useSkill(int skillId);
	// ʹ�ÿ�ݼ�
	void usedShortCut( int index, int nTargetIndex = 0 );
	// ��ݼ�����
	void shorCutUpKey();
	// ɾ����ݼ�
	void delShortCut( int index );
	// ������ݼ�
	void swapShortCut( int src, int des );
	// ���ÿ�ݼ���UI
	void setShortCut2UI( int nType, int Id, int index, int nFitting, int nLevel = 0 );
	// ���ÿ�ݼ�
	void setShortCut( int nType, int Id, int index, int nFitting = 0, int nLevel = 0 );
	// ��ʼ������ҿ�ݼ�
	void initNewPlayerShortCut();
	// ���ó��＼�ܿ�ݼ�
	void setPetFittingShortCut( int Id, int nSkillLv, int index, int nFitting = 0 );
	// ������ʱ��е���ܿ�ݼ�
	void setTmpShortCut(  int Id, int nSkillLv, int index, int nFitting, int nType = SCT_SKILL );
	// ��ȡ��ʱ���ܿ�ݼ�
	int getTmpShortCutId( int nIndex );
	// ��ȡ��ʱ���ܿ�ݼ�
	int getTmpShortCutLv( int nIndex );
	// ʹ�����⼼��
	void useSpecialSkill();
	// ʹ��TabѰ������Ĺ���
	void usedTab( int nType = 0 );
	// ��F12�����������
	void usedF12();
	// ��F11���������������
	void usedF11();
	// ������ϼ�λ�ļ���
	void setSPShortCut( int nType, int nValue );
	// ��ȡ��ϼ�λ����
	int  getSPShortCut( int nType );
	// ��ȡҪʹ�õĿ�ݼ�
	tagShortCut* getUseShortCut();
	// ��ȡҪʹ�õĿ�ݼ��ϵĵ���
	tagShortCut& getUseShortCut( int nIndex );
	// ���ÿ�ݼ�
	void  resetShortCut();
	// ���ÿ�ݼ����ڰ���״̬
	void  setShortCutPress(bool bPress);
	void  setShortCutKeyPress(int index, int bPress);
	//��ݼ�FULL�� ��ü��ܿ�ݼ�����
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
