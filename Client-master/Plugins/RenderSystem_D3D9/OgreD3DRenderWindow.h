
#ifndef __D3DRenderWindow_H__
#define __D3DRenderWindow_H__

#include "OgreD3D9Prerequisites.h"
#include "OgreRenderWindow.h"
#include "OgreRenderSystem.h"

namespace Ogre
{
	class D3DRenderWindow : public RenderWindow
	{
	public:
		D3DRenderWindow(D3D9RenderSystem *prs, const RenderSystem::InitDesc &desc, bool main_target);
		~D3DRenderWindow();

		bool onInitialise();
		void onShutdown();
		void onLostDevice();
		bool onResetDevice();
		bool filterWindowMessage(unsigned int uMsg, WPARAM wParam, LPARAM lParam, long &retvalue);

		virtual void getSize(uint &width, uint &height);
		virtual bool beginScene();
		virtual void endScene();
		virtual int present();
		virtual void *getHWnd()
		{
			return m_hWnd;
		}
		virtual void onSizeOrMove();
		virtual void forceReset();
		virtual void setSyncRefresh( bool bSyncRefresh );
		virtual void setMultiSampleLevel( int nSampleLevel );
		virtual void deleteThis();

		IDirect3DSurface9* getColorBuffer();
	public:
		bool m_bNeedResetDevice;

	private:
		void handlePossibleSizeChange();
		void buildPresentParams(D3DPRESENT_PARAMETERS &d3dpp, const D3DSETTING_T &setting);
		bool createDeviceObj();
		void destroyDeviceObj();

	private:
		D3D9RenderSystem*	m_pRS;
		IDirect3DDevice9*	m_pd3dDevice;

		HWND m_hWnd;
		RenderSystem::InitDesc m_Desc;

		IDirect3DSwapChain9*	m_pSwapChain;
		IDirect3DSurface9*		m_pColorBuffer;
		IDirect3DSurface9*		m_pDSBuffer;
		IDirect3DQuery9*		m_pEventQuery;

		DWORD m_dwWindowStyle;
		RECT  m_rcWindowBounds;
		RECT  m_rcWindowClient;
		bool m_bMainTarget;


	};
}

#endif 