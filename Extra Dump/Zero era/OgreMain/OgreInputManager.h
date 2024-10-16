
#ifndef __OgreInputManager_H__
#define __OgreInputManager_H__

#include "OgreSingleton.h"
#include "OgreInputHandler.h"
#include <deque>
#include <vector>

namespace Ogre
{
	class _OgreExport InputManager : public Ogre::Singleton<InputManager>
	{
	public:
		InputManager();
		void RegisterInputHandler(InputHandler *phandler);
		void OnWindowMessage(void *hwnd, unsigned int msgid, unsigned int wparam, unsigned int lparam);
		void Dispatch();
		bool IsKeyDown(int vkey);
		void ShowSystemCursor(bool b);
		bool IsPointIn(int x, int y);
		bool isFocus();
		void setSelfHandler(InputHandler* pHandler) { m_pSelfHandler = pHandler; }

	protected:
		std::deque<InputEvent>		m_EventQue;
		std::vector<InputHandler *>	m_Handles;
		int							m_ClientWidth;
		int							m_ClientHeight;
		bool						m_IsMouseIn;
		bool						m_bIsFocus;
		InputHandler*				m_pSelfHandler;
	};
}

#endif