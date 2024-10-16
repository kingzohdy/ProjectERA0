#pragma once
//////////////////////////////////////////////////////////////////////////
// ��ɫ��ѡ����
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
	virtual void showAdjustEffect( const char* szAllowPath, const char* szUnAllowPath, void *pExtendData );/// ��ʾ�Ƕȵ���ģ��
	virtual void showCircularEffect( const char *szPath, unsigned int nLimit, void *pExtendData);/// ��ʾ��ͷѡ��
	virtual void showRectEffect( const char *szPath, unsigned int width, void *pExtendData);/// ��ʾ��ͷѡ��
	virtual void showAngleEffect( const char *szPath, int nCumulateTime, unsigned int nDistance ); // ��Զ �Ƕȱ任 
	virtual void hideSelectEffect();
	
	void releaseEff();
	// �´�ѡ�к���
	void cancelNextSelect();
	// ����Զ������·
	void	breakAutoClick();
	// ���ѡ�е�
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
	
	// �Ƿ����ѡȡΪĿ��
	bool			canSelectTarget( MainGamePlayer* pMainPlayer, GameActor* pTarget );
	// Ŀ��ѡ��
	bool			selectTarget( MainGamePlayer* pMainPlayer, GameActor* pTarget );
	bool			tryCommunicationToTarget( GameActor* pTarget );
	// �����Ч
	void			showSelCursor( int x, int z );
	// Ŀ��ѡ����Ч
	virtual void	showSelEffect( bool bdel = false );
	// �ж��Ƿ���Ҫ����Ѱ·
	bool			needContinueMove( int nReasonCode );
	void			checkOnLButtonDown();
	// 
	void			OnExchangeLButtonDown( GameActor* pTarget );
	
private:
	Ogre::Entity*		m_pClickEntity;		
	Ogre::Entity*		m_pSelEntity;		
	int					m_PreSelId;			// �ϴ�ѡ��
	Ogre::DirDecal*		m_pDirDecal;
	Ogre::DirDecal*		m_pRectDecal;

	Ogre::Entity*		m_pSelectEff;
	Ogre::WorldPos		m_pSelectPos;		// ѡ�еĳ���λ��
	bool				m_bCancelSelect;	// �Ƿ����ѡ��
	std::pair<bool, unsigned int>	m_AutoLDownInfo;	// ����Զ��������Ϣ
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
	}					m_selectMode;	/// ѡ��ģʽ��POINTΪ�ص�ѡ���ʺ���ĳһ�ص�ΪԲ�ĵ�Ч����ANGLEΪ�Ƕȵ������ʺ����Ρ����μ���
};
