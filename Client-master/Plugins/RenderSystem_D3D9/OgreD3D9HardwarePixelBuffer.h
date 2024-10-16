
#ifndef __D3D9HardwarePixelBuffer_H__
#define __D3D9HardwarePixelBuffer_H__

#include "OgreD3D9Prerequisites.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreRenderTarget.h"

namespace Ogre
{
	class D3D9TextureRenderTarget;
	class D3D9HardwarePixelBuffer;

	class D3D9PixelBufferPool : public HardwarePixelBufferPool
	{
	public:
		D3D9PixelBufferPool(D3D9RenderSystem *prs, HardwareBufferUsage usage, const TextureDesc &desc);
		~D3D9PixelBufferPool();

		virtual HardwarePixelBuffer *newPixelBuffer();
		virtual void *lock(HardwarePixelBuffer *pbuf, uint index, HardwareBufferLockOpt lockopt, LockResult &lockresult);
		virtual void unlock(HardwarePixelBuffer *pbuf, uint index);
		virtual void loadSurfaceData(HardwarePixelBuffer *pbuf, uint index, SurfaceData *psurfacedata);
		virtual TextureRenderTarget *createRenderTarget(HardwarePixelBuffer *pbuf, uint index, 
															int depthbits, int multisample );
		virtual TextureRenderTarget *createDepthStencil(HardwarePixelBuffer *pbuf, uint index);
		void getSurfaceSize(uint index, uint &width, uint &height);
		IDirect3DSurface9 *getSurface(HardwarePixelBuffer *pbuf, uint index);
		void releaseRenderTarget(D3D9TextureRenderTarget *ptarget);

		void onLostDevice();
		bool onResetDevice();

		void createPixelBufferSysTexture(D3D9HardwarePixelBuffer *pbuffer);
		void releasePixelBufferSysTexture(D3D9HardwarePixelBuffer *pbuffer);

		bool getSupportedParam(HardwareBufferUsage usage, const TextureDesc &desc);
		D3D9RenderSystem *m_pRS;
	private:
		
		D3DRESOURCETYPE					m_Type;
		D3DFORMAT						m_Format;
		DWORD							m_Usage;
		D3DPOOL							m_Pool;
		UINT							m_Width;
		UINT							m_Height;
		UINT							m_Depth;
		UINT							m_MipLevels;
		UINT							m_nMemSize;
		bool						    m_bAutoMipmap;

		std::vector<IDirect3DBaseTexture9 *>m_SysTextures;
		std::vector<D3D9TextureRenderTarget *>m_Targets;
	};

	class D3D9HardwarePixelBuffer : public HardwarePixelBuffer
	{
	public:
		D3D9HardwarePixelBuffer() : m_pSysTex(NULL)
		{
		}

		virtual void dumpToDDS(const char *path);
		virtual void *dumpToBuffer(const char *ext, size_t &buflen);

		IDirect3DSurface9 *getSurface(uint index)
		{
			return static_cast<D3D9PixelBufferPool *>(m_pPool)->getSurface(this, index);
		}

		void getSurfaceSize(uint index, uint &width, uint &height)
		{
			static_cast<D3D9PixelBufferPool *>(m_pPool)->getSurfaceSize(index, width, height);
		}

		IDirect3DBaseTexture9 *getSysTexture(){ return m_pSysTex; }
		virtual void	getColorBit(unsigned long * pBits);

	private:
		virtual ~D3D9HardwarePixelBuffer();

	public:
		IDirect3DBaseTexture9*			m_pSysTex;
	};

	class D3D9TextureRenderTarget : public TextureRenderTarget
	{
	public:
		enum RenderTargetType
		{
			Normal_RenderTarget = 0,
			DepthStencil_RenderTarget,
		};

		D3D9TextureRenderTarget(D3D9RenderSystem *prs, D3D9HardwarePixelBuffer *pbuf, uint index, int depthbits, 
								RenderTargetType rttype, int multisample=0 );
		~D3D9TextureRenderTarget();

		virtual void getSize(uint &width, uint &height)
		{
			width = m_Width;
			height = m_Height;
		}

		virtual HardwareTexture *getColorTexture()
		{
			return m_pColorBuf;
		}

		virtual void deleteThis()
		{
			m_pPool->releaseRenderTarget(this);
		}

		virtual void setStretchRect( int left, int top, int right, int bottom, int dLeft, int dTop, int dRight, int dBottom );

		bool beginScene();
		void endScene();
		
		virtual bool isSuccessCreateRenderTarget();
		virtual bool isSuccessCreateDSBuffer();
		void requireOrReleaseDSBuffer(bool b);
		void requireOrReleaseRenderTarget(bool b);

	public:
		RenderTargetType  m_RenderTargetType;
		D3D9PixelBufferPool *m_pPool;
		D3D9RenderSystem *m_pRS;
		D3D9HardwarePixelBuffer *m_pColorBuf;
		IDirect3DSurface9 *m_pDSBuf;
		IDirect3DSurface9 *m_pRenderTarget;

		uint m_SurfaceIndex;
		uint m_Width;
		uint m_Height;
		D3DFORMAT m_ColorFormat;
		D3DFORMAT m_DSFormat;
		D3DMULTISAMPLE_TYPE m_MSType;
		bool m_HasZBuffer;
		bool m_bStretch;
		RECT m_StretchSrcRect;
		RECT m_StretchDestRect;
	};

	class D3D9HardwarePixelBufferManager : public HardwarePixelBufferManager
	{
	public:
		D3D9HardwarePixelBufferManager(D3D9RenderSystem *prs);
		~D3D9HardwarePixelBufferManager();

		virtual HardwarePixelBufferPool *newPixelBufferPool(HardwareBufferUsage usage, const TextureDesc &desc);
		
		bool hasRenderTarget(uint width, uint height, D3DFORMAT format, D3DMULTISAMPLE_TYPE mstype=D3DMULTISAMPLE_NONE);
		bool hasDSBuffer(uint width, uint height, D3DFORMAT format, D3DMULTISAMPLE_TYPE mstype=D3DMULTISAMPLE_NONE);

		IDirect3DSurface9 *requireDSBuffer(uint width, uint height, D3DFORMAT format, D3DMULTISAMPLE_TYPE mstype=D3DMULTISAMPLE_NONE);
		IDirect3DSurface9 *requireRenderTarget(uint width, uint height, D3DFORMAT format, D3DMULTISAMPLE_TYPE mstype=D3DMULTISAMPLE_NONE);
		void releaseDSBuffer(IDirect3DSurface9 *pbuf);
		void releaseRenderTarget(IDirect3DSurface9 *pbuf);

		void onLostDevice();
		bool onResetDevice();

	private:
		D3D9RenderSystem *m_pRS;

		struct DSBuffer
		{
			IDirect3DSurface9 *pbuf;
			uint width;
			uint height;
			D3DFORMAT format;
			D3DMULTISAMPLE_TYPE mstype;
			int refcount;
		};

		struct RTBuffer
		{
			IDirect3DSurface9 *pbuf;
			uint width;
			uint height;
			D3DFORMAT format;
			D3DMULTISAMPLE_TYPE mstype;
			int refcount;
		};

		std::vector<DSBuffer>m_DSBuffers;
		std::vector<RTBuffer>m_RenderTargets;
	};
}

#endif
