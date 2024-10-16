
#ifndef __D3D9HardwareBuffer_H__
#define __D3D9HardwareBuffer_H__

#include "OgreD3D9Prerequisites.h"
#include "OgreHardwareBuffer.h"

namespace Ogre
{
	class D3D9BufferPool : public HardwareBufferPool
	{
	public:
		D3D9BufferPool(bool is_vbuffer, size_t poolsize, bool bdynamic) : HardwareBufferPool(is_vbuffer, poolsize, bdynamic)
		{
		}

		virtual ~D3D9BufferPool(){}

		virtual bool create() = 0;
		virtual void onLostDevice() = 0;
		virtual bool onResetDevice() = 0;
	};

	class D3DVertexBufferPool : public D3D9BufferPool
	{
	public:
		D3DVertexBufferPool(D3D9RenderSystem *prs, HardwareBufferUsage usage, size_t nbytes);
		~D3DVertexBufferPool();

		virtual bool create();
		virtual void onLostDevice();
		virtual bool onResetDevice();

		virtual void *lock(size_t start, size_t nbytes, HardwareBufferLockOpt lockopt);
		virtual void unlock();

		IDirect3DVertexBuffer9 *getSysBuf(){ return m_pSysBuf; }

	private:
		D3D9RenderSystem *m_pRS;
		IDirect3DVertexBuffer9 *m_pSysBuf;
		D3DPOOL m_Pool;
		DWORD m_Usage;
	};

	class D3DIndexBufferPool : public D3D9BufferPool
	{
	public:
		D3DIndexBufferPool(D3D9RenderSystem *prs, HardwareBufferUsage usage, size_t nbytes, size_t stride);
		~D3DIndexBufferPool();

		virtual bool create();
		virtual void onLostDevice();
		virtual bool onResetDevice();

		virtual void *lock(size_t start, size_t nbytes, HardwareBufferLockOpt lockopt);
		virtual void unlock();

		IDirect3DIndexBuffer9 *getSysBuf(){ return m_pSysBuf; }

	private:
		D3D9RenderSystem *m_pRS;
		IDirect3DIndexBuffer9 *m_pSysBuf;
		D3DPOOL m_Pool;
		DWORD m_Usage;
		D3DFORMAT m_IndexFormat;
	};

	class D3D9HardwareBufferManager : public HardwareBufferManager
	{
	public:
		D3D9HardwareBufferManager(D3D9RenderSystem *prs) : m_pRS(prs)
		{
		}

		virtual HardwareBufferPool *newPool(bool is_vbuffer, HardwareBufferUsage usage, size_t nbytes, size_t stride);

		void onLostDevice();
		bool onResetDevice();

	private:
		D3D9RenderSystem *m_pRS;
	};
}

#endif