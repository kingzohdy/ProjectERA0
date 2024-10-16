#pragma once
//////////////////////////////////////////////////////////////////////////
// 角色点选管理
//////////////////////////////////////////////////////////////////////////
#include "GameEngineTypes.h"
#include "OgreInputHandler.h"
#include "GameMsgPrint.h"
#include "GameManager.h"
#include "OgreDirDecal.h"
#include <utility>
#include <set>

const int MINSELECT = 80;

class IMouseEventAction
{
public:
	virtual bool onLButtonDown( GameActor* pTarget, int x, int z ){return false;}
	virtual bool onRButtonDown( GameActor* pTarget, int x, int z ){return false;}
};

class GameSelectManager : public IGameMsgPrint
{
public:
	GameSelectManager(void);
	~GameSelectManager(void);
	bool isMouseSwap();

	void onLeaveMap();
	void onLButtonDown( const Ogre::InputEvent &event );
	void onRButtonDown( const Ogre::InputEvent &event );
	void onMouseMove( const Ogre::InputEvent &event );
	
	void update( unsigned int dtick );

	static  void onRightButton(bool press);
	static	void onLeftButton(bool press);
	virtual void showSelectEffect( const char* szAllowPath, const char* szUnAllowPath, int dis, int radio, bool spin );
	virtual void showAdjustEffect( const char* szAllowPath, const char* szUnAllowPath, void *pExtendData );/// 显示角度调整模型
	virtual void showCircularEffect( const char *szPath, unsigned int nLimit, void *pExtendData);/// 显示箭头选择
	virtual void showRectEffect( const char *szPath, unsigned int width, void *pExtendData);/// 显示箭头选择
	virtual void showAngleEffect( const char *szPath, int nCumulateTime, unsigned int nDistance ); // 渐远 角度变换 
	virtual void hideSelectEffect();
	
	void releaseEff();
	// 下次选中忽略
	void cancelNextSelect();
	// 打断自动点击走路
	void	breakAutoClick();
	// 获得选中点
	Ogre::WorldPos getSelectPos()
	{
		return m_pSelectPos;
	}

	int getLastHighlightItemID();
	void HighlightNewItem( int nMemID );

	int  getLastHighStallActorID();
	void HighlightStallActor( int nActorID );

	void insertMouseEventHandler(IMouseEventAction *h);

	class PtrLess
	{
	public:
		bool operator () (const void *a, const void *b)
		{
			return (intptr_t)a < (intptr_t)b;
		}
	};

private:
	bool onCtrlLButtonDown( MainGamePlayer* pMainPlayer, GameActor* pTarget, int x, int z );
	bool onCtrlRButtonDown( MainGamePlayer* pMainPlayer, GameActor* pTarget, int x, int z );
	bool onAltLButtonDown( MainGamePlayer* pMainPlayer, GameActor* pTarget, int x, int z );
	bool onAltRButtionDown( MainGamePlayer* pMainPlayer, GameActor* pTarget, int x, int z );
	bool onShiftLButtonDown( MainGamePlayer* pMainPlayer, int x, int y );
	
	// 是否可以选取为目标
	bool			canSelectTarget( MainGamePlayer* pMainPlayer, GameActor* pTarget );
	// 目标选择
	bool			selectTarget( MainGamePlayer* pMainPlayer, GameActor* pTarget );
	bool			tryCommunicationToTarget( GameActor* pTarget );
	// 点地特效
	void			showSelCursor( int x, int z );
	// 目标选择特效
	virtual void	showSelEffect( bool bdel = false );
	// 判断是否需要继续寻路
	bool			needContinueMove( int nReasonCode );
	void			checkOnLButtonDown();
	// 
	void			OnExchangeLButtonDown( GameActor* pTarget );
	
private:
	Ogre::Entity*		m_pClickEntity;		
	Ogre::Entity*		m_pSelEntity;		
	int					m_PreSelId;			// 上次选中
	Ogre::DirDecal*		m_pDirDecal;
	Ogre::DirDecal*		m_pRectDecal;

	Ogre::Entity*		m_pSelectEff;
	Ogre::WorldPos		m_pSelectPos;		// 选中的场景位置
	bool				m_bCancelSelect;	// 是否忽略选中
	std::pair<bool, unsigned int>	m_AutoLDownInfo;	// 左键自动点击的信息
	std::string			m_szAllowEff;
	std::string			m_szUnAllowEff;
	int					m_nAllowDis;
	int					m_nRadio;
	bool				m_bSpin;
	bool				m_bAllowNow;
	int					m_pLastCmdX;
	int					m_pLastCmdY;
	bool				m_bLastCmd;
	unsigned int		m_nLimit;
	unsigned int		m_nWidth;
	int					m_nLastAngle;
	std::set<IMouseEventAction *, PtrLess>		m_lstMouseEventAction;
	int					m_nLastHighlightItemID;
	int					m_nLastStallActorID;

	int					m_nLastCumulateTime;
	int					m_nDistance;

	enum
	{
		NONE = 0,POINT, ANGLE
	}					m_selectMode;	/// 选择模式，POINT为地点选择，适合以某一地点为圆心的效果；ANGLE为角度调整，适合扇形、条形技能
};
