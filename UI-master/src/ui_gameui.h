#pragma once
#include "ui_common.h"
#include "OgreInputHandler.h"

enum eCursorType
{
	eCursorMoveChange = 0,	//在鼠标移动的时候会改变。
	eCursorOnlyOnUi,		//只会在UI上
	eCursorNoMoveChange,			//在鼠标移动的时候不会改变
};

const int MAX_LEVEL_CAN_MOVE = eCursorNoMoveChange;
class XMLManager;


static int s_CursorLevel[] =
{
	eCursorMoveChange,		//普通
	eCursorMoveChange,		//无法点击
	eCursorMoveChange,			//攻击
	eCursorMoveChange,		//拾取
	eCursorMoveChange,		//NPC购买
	eCursorNoMoveChange,		//交易
	eCursorNoMoveChange,		//禁止交易的状态
	eCursorMoveChange,			//谈话
	eCursorNoMoveChange,		//修理
	eCursorMoveChange,		//挖矿
	eCursorMoveChange,		//学习技能
	eCursorMoveChange,		//抓宠物
	eCursorOnlyOnUi,		//超链接
	eCursorMoveChange,		//drag
	eCursorOnlyOnUi,		//拾取按钮指针

	eCursorMoveChange,		//无法攻击
	eCursorMoveChange,		//无法购买
	eCursorMoveChange,		//无法拾取
	eCursorMoveChange,		//无法邮件
	eCursorMoveChange,		//无法挖矿
	eCursorMoveChange,		//无法修理
	eCursorMoveChange,		//无法谈话
	eCursorMoveChange,		//无法训练
};

//-----------------------------------------------------------------------------
//  客户端可调用UI的函数接口
//-----------------------------------------------------------------------------
class GameUI : public Ogre::InputHandler
{
//对客户端的接口函数
public:
	GameUI();
	~GameUI();

	virtual int HandleInput(const Ogre::InputEvent &event);

	bool Create( const char *uires, int w, int h, Ogre::UIRenderer* pDisplay, Ogre::ScriptVM *pscriptvm);

	//void Destroy();
	void Update( float deltatime );
	void Update( float deltatime, int nInterval, float fThreshold );
	void Render();
	void SendEvent(const char *event);
	void SetCurrentCursor(const char *state);
	void ShowCursor(bool bShow);
	bool isCursorDrag();
	const char *getCurrentCursor();

	void ShowUIPanel(const char *frameName);
	void HideUIPanel(const char *frameName);

	void UIReceiveMessage(bool bReceive);
//供UI编辑器调用的接口函数
public:
	void SetEditMode(int mode);
	int GetEditMode();

	bool NewXMLFile(const char *uires);
	bool LoadXMLFile(const char *path);
	bool SaveXMLFile(const char *path);

//供UI内部调用的函数
public:
	static void InitRootFrames();

private:
	XMLManager *m_pXmlmgr;
	bool m_bReceiveMsg;
};