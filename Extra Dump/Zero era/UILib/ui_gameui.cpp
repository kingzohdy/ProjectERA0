#include "ui_gameui.h"
#include "ui_common.h"
#include "ui_frame.h"
#include "ui_framemgr.h"
#include "xml_uimgr.h"

extern int tolua_UITolua_open (lua_State* tolua_S);

using namespace Ogre;

extern FrameManager *g_pFrameMgr;

//-----------------------------------------------------------------------------
//  客户端可调用UI的函数接口
//-----------------------------------------------------------------------------

extern void ShowUIPanel(const char *frameName);
extern void HideUIPanel(const char *frameName);

GameUI::GameUI()
{
	m_pXmlmgr = NULL;
	g_pFrameMgr = new FrameManager;
	m_bReceiveMsg = true;
}

GameUI::~GameUI()
{
	delete(m_pXmlmgr);
	delete(g_pFrameMgr);
}

void GameUI::InitRootFrames()
{
	//初始化窗口后根窗口的子窗口显示属性继承根窗口
	for (size_t i=0; i<g_pFrameMgr->m_RootFrames.size(); i++)
	{
		Frame *pframe = g_pFrameMgr->m_RootFrames[i];
		pframe->ReplaceSpecialName();
		pframe->InitFrameStrata();

		pframe->RegisterToFrameMgr(g_pFrameMgr);
	}

	for(size_t i=0; i<g_pFrameMgr->m_RootFrames.size(); i++)
	{
		g_pFrameMgr->CreateLuaTable(g_pFrameMgr->m_RootFrames[i]);
	}
}

//int getglobal(lua_State *ls)
//{
//	if( lua_isstring(ls,1) == false )
//	{
//		lua_pushnil( ls );
//	}
//	else 
//		lua_getglobal( ls,lua_tostring( ls,1 ) );
//	return 1;
//}

bool GameUI::Create( const char *uires, int w, int h, Ogre::UIRenderer* pDisplay, Ogre::ScriptVM *pscriptvm)
{
	if(uires)
	{
		g_pDisplay = pDisplay;
		g_pUIScriptVM = pscriptvm;

		g_pFrameMgr->SetScreenWidth(w);
		g_pFrameMgr->SetScreenHeight(h);
		g_pFrameMgr->SetScreenScaleX(1.0f);
		g_pFrameMgr->SetScreenScaleY(1.0f);

		tolua_UITolua_open(g_pUIScriptVM->getLuaState());
		
		return NewXMLFile(uires);
	}
	
	return false;
}

void GameUI::Update( float deltatime )
{
	g_pFrameMgr->Update(deltatime);
}

void GameUI::Update( float deltatime, int nInterval, float fThreshold )
{

}

void GameUI::Render()
{
	g_pFrameMgr->Render();
}

int GameUI::HandleInput(const Ogre::InputEvent &event)
{
	//ui内部处理输入消息， 如果已经处理，不需要后面场景等去处理， 则返回0， 否则返回1
	//正好和游戏逻辑那边相反， 所以在这里做一下转换
	if(g_pFrameMgr && m_bReceiveMsg)
	{
		if(g_pFrameMgr->OnInputMessage(event)) 
		{
			return 0;
		}
		else 
		{
			return 1;
		}
	}
	else 
	{
		return 0;
	}
}

void GameUI::SendEvent(const char *event)
{
	if (event)
	{
		g_pFrameMgr->SendEvent(event);
	}
}

void GameUI::SetCurrentCursor(const char *state)
{
	g_pFrameMgr->setCursor(state);
}

void GameUI::ShowCursor(bool bShow)
{
	if (bShow)
	{
		g_pFrameMgr->m_pCurCursor->ShowCursor();
	}
	else
	{
		g_pFrameMgr->m_pCurCursor->HideCursor();
	}
}

bool GameUI::isCursorDrag()
{
	return g_pFrameMgr->m_pCurCursor->IsInDragState();
}

const char *GameUI::getCurrentCursor()
{
	return g_pFrameMgr->m_pCurCursor->getCursor();
}

void GameUI::ShowUIPanel(const char *frameName)
{
	if (frameName)
	{
		::ShowUIPanel(frameName);
	}
}

void GameUI::HideUIPanel(const char *frameName)
{
	if (frameName)
	{
		::HideUIPanel(frameName);
	}
}

void GameUI::UIReceiveMessage(bool bReceive)
{
	m_bReceiveMsg = bReceive;
}

void GameUI::SetEditMode(int mode)
{
	g_pFrameMgr->SetEditMode(mode);

}

int GameUI::GetEditMode()
{
	return g_pFrameMgr->GetEditMode();
}

bool GameUI::NewXMLFile(const char *uires)
{
	g_pFrameMgr->m_ObjectTable.clear();
	g_pFrameMgr->m_TemplateTable.clear();
	g_pFrameMgr->m_RootFrames.clear();

	m_pXmlmgr = new XMLManager;

	if (!m_pXmlmgr->LoadTOCFile(uires))
	{
		delete(m_pXmlmgr);
		return false;
	}

	g_pFrameMgr->m_TocFileName = uires;

	InitRootFrames();

	g_pFrameMgr->m_pCurCursor->setCursor("normal");

	return true;
}

bool GameUI::LoadXMLFile(const char *path)
{
	if (path)
	{
		if (!m_pXmlmgr->LoadUIFromXml(path))
		{
			return false;
		}

		InitRootFrames();

		return true;
	}

	return false;
}

bool GameUI::SaveXMLFile(const char *path)
{
	if (path)
	{
		if (!m_pXmlmgr->SaveUIToXml(path))
		{
			return false;
		}

		return true;
	}

	return false;
}


