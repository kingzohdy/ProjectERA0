
#ifndef __D3D9RenderSystem_H__
#define __D3D9RenderSystem_H__

#include "OgreD3D9Prerequisites.h"
#include "OgreRenderSystem.h"
#include "OgrePixelFormat.h"
#include "OgreHardwareBuffer.h"

namespace Ogre
{
	class D3D9RenderSystem : public RenderSystem
	{
	public:
		D3D9RenderSystem(HINSTANCE hinst);
		~D3D9RenderSystem();

		virtual bool initialise(const InitDesc &desc);
		virtual void shutdown();
		virtual RenderWindow *createRenderWindow(const InitDesc &desc);
		virtual void clear( unsigned int flags, unsigned int color, float z, unsigned int stencil );
		virtual void setViewport(const Viewport &viewport);
		virtual bool beginFrame();
		virtual void endFrame();
		virtual int present();
		virtual RenderWindow *getMainWindow();
		virtual VertexDeclHandle getInputLayout(const VertexFormat &vertfmt);
		virtual int resetDevice();
		virtual int testDeviceReset();
		virtual bool restoreLostDevice();
		virtual RenderWindow *findRenderWindow(void *hwnd);
		virtual void setContextQueDesc(const ContextQueDesc &desc);
		virtual void setCursorProperty(TextureData *ptex, int hotspotx, int hotspoty, int u, int v, int width, int height);
		virtual void setCursorPosition(int x, int y);
		virtual void showCursor(bool b);

		virtual ShaderTechnique* CreateShaderTechniqueFromRawFile(const char* pfilename);
		//D3D9Shader* CreateShaderFromRawFile(const char* pfilename);
		D3DRenderWindow *findRenderWindowByHWnd(HWND hwnd);
		void removeD3DRenderWindow(D3DRenderWindow *pwnd);
		bool createD3DDevice(HWND hwnd, const D3DSETTING_T &setting, D3DPRESENT_PARAMETERS &d3dpp);
		void destroyD3DDevice();
		IDirect3DDevice9 *getD3DDevice()
		{
			return m_pd3dDevice;
		}

		D3DEnumeration *getD3DEnum()
		{
			return m_pD3DEnum;
		}

		D3DRenderWindow *getMainRenderWindow()
		{
			return m_WindowTargets[0];
		}

		bool isDeviceLost()
		{
			return m_bDeviceLost;
		}

		HINSTANCE GetInstance()
		{
			return m_hInstance;
		}

		DWORD getCreateFlags()
		{
			return m_dwCreateFlags;
		}

		D3DFORMAT fromPixelFormat(PixelFormat pfmt);
		PixelFormat toPixelFormat(D3DFORMAT d3dfmt);
		bool isFormatSupported(D3DFORMAT format, DWORD usage, D3DRESOURCETYPE restype);
		bool isTargetFormatSupported(D3DFORMAT colorformat, D3DFORMAT dsfmt);
		bool isTargetMultiSampleSupported(D3DFORMAT colorformat, D3DFORMAT dsfmt, D3DMULTISAMPLE_TYPE mstype);
		bool getProperTargetDepthFormat(D3DFORMAT &dsfmt, D3DMULTISAMPLE_TYPE mstype, D3DFORMAT colorfmt, int depthbits);
		bool getProperTargetColorFormat(D3DFORMAT &colorfmt, D3DMULTISAMPLE_TYPE mstype, D3DFORMAT dsfmt);
		void hbu2PoolUsage(DWORD &usage, D3DPOOL &pool, HardwareBufferUsage hbu);
		DWORD lockOpt2D3DFlags(HardwareBufferLockOpt opt, DWORD d3dusage);
		IDirect3DBaseTexture9 *createTexture(D3DRESOURCETYPE restype, D3DFORMAT format, DWORD usage, D3DPOOL pool, UINT width, UINT height, UINT depth, UINT miplevels);

		ID3DXBuffer *compileCode(const char *pbuffer, size_t buflen, const D3DXMACRO *pmacros, const char *funcname=NULL, const char *profile=NULL, ID3DXConstantTable **ppconst=NULL);
		ID3DXEffect *createD3DXEffect(ID3DXBuffer *pcompiledbuf);
		IDirect3DVertexShader9 *createVertexShader(ID3DXBuffer *pcompiledbuf);
		IDirect3DPixelShader9 *createPixelShader(ID3DXBuffer *pcompiledbuf);

		ID3DXEffect *createD3DXEffect (void *pcompiledbuf, UINT bufferSize);
		IDirect3DVertexShader9 *createVertexShader (void *pcompiledbuf, UINT bufferSize);
		IDirect3DPixelShader9 *createPixelShader (void *pcompiledbuf, UINT bufferSize);

		virtual void snapShotAll( const char* szSavePath );
		virtual void snapShot();
	public:
		HINSTANCE m_hInstance;
		IDirect3D9 *m_pD3D;
		IDirect3DDevice9 *m_pd3dDevice;
		D3DCAPS9 m_d3dCaps;
		DWORD    m_dwCreateFlags;
		bool     m_bDeviceRestored;
		bool     m_bDeviceLost;
		bool	 m_bInterval;

		D3DEnumeration *m_pD3DEnum;

		WNDCLASS m_WndClass;
		std::vector<D3DRenderWindow *>m_WindowTargets;

		String m_CoreMtlDir;

		bool m_bDebugVS;
		bool m_bDebugPS;
		bool m_bDebug_nv;

		std::vector<D3D9VertexDecl *>m_VertexDecls;
		D3D9HardwarePixelBufferManager *m_pPBufMgr;
		D3D9MaterialManager *m_pMtlMgr;
		D3D9HardwareBufferManager *m_pBufMgr;

		IDirect3DSurface9 *m_pCursorSurface;
		int m_CursorWidth;
		int m_CursorHeight;
	};
}

#endif