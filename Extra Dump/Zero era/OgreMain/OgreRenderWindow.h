
#ifndef __RenderWindow_H__
#define __RenderWindow_H__

#include "OgreRenderTarget.h"

namespace Ogre
{
	class _OgreExport RenderWindow : public RenderTarget
	{
		DECLARE_RTTI_VIRTUAL(RenderWindow);

	public:
		enum
		{
			PRESENT_OK = 0,  //�豸����
			PRESENT_LOSTDEVICE, //�豸��ʧ
			PRESENT_RESTORE,    //�豸���Իָ�
			PRESENT_ERROR      //�豸����
		};

	public:
		RenderWindow() : m_bMinimized(false), m_bMaximized(false), m_bWindowed(true)
		{
		}

		virtual ~RenderWindow() {}

		virtual int present() = 0;
		virtual void *getHWnd() = 0;
		virtual void onSizeOrMove() = 0;
		virtual void forceReset() = 0;
		virtual void setSyncRefresh( bool bSyncRefresh ) = 0;
		virtual void setMultiSampleLevel( int nSampleLevel ) = 0;
		void setMaximized(bool b)
		{
			m_bMaximized = b;
		}

		void setMinimized(bool b)
		{
			m_bMinimized = b;
		}

		bool getMaximized()
		{
			return m_bMaximized;
		}

		bool getMinimized()
		{
			return m_bMinimized;
		}

		bool getWindowed()
		{
			return m_bWindowed;
		}

	protected:
		bool m_bMaximized;
		bool m_bMinimized;
		bool m_bWindowed;
	};
}

#endif 