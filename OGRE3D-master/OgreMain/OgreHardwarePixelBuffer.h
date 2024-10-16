
#ifndef __HardwarePixelBuffer_H__
#define __HardwarePixelBuffer_H__

#include "OgreHardwareBuffer.h"
#include "OgreSingleton.h"
#include "OgrePixelFormat.h"
#include "OgreTexture.h"
#include "OgreLocker.h"

namespace Ogre
{
	class _OgreExport HardwarePixelBufferPool
	{
	public:
		HardwarePixelBufferPool(HardwareBufferUsage usage, const TextureDesc &desc);
		virtual ~HardwarePixelBufferPool();

		HardwarePixelBuffer *allocBuffer();
		void garbageCollect(uint curtime);
		PixelFormat getPixelFormat()
		{
			return m_Desc.format;
		}

		virtual HardwarePixelBuffer *newPixelBuffer() = 0;
		virtual void *lock(HardwarePixelBuffer *pbuf, uint index, HardwareBufferLockOpt lockopt, LockResult &lockresult) = 0;
		virtual void unlock(HardwarePixelBuffer *pbuf, uint index) = 0;
		virtual void loadSurfaceData(HardwarePixelBuffer *pbuf, uint index, SurfaceData *psurfacedata) = 0;
		virtual TextureRenderTarget *createRenderTarget(HardwarePixelBuffer *pbuf, uint index, int depthbits, int multisample) = 0;
		virtual TextureRenderTarget *createDepthStencil(HardwarePixelBuffer *pbuf, uint index) = 0;

	public:
		HardwareBufferUsage m_Usage;
		TextureDesc m_Desc;

	protected:
		std::vector<HardwarePixelBuffer *>m_FreeBuffers;
		std::vector<HardwarePixelBuffer *>m_UsedBuffers;

		LockSection m_FreeBufferMutex;
		LockSection m_UsedBufferMutex;
	};

	class _OgreExport HardwarePixelBuffer
	{
		friend class HardwarePixelBufferPool;
	public:
		void addRef()
		{
			m_RefCount++;
		}
		void release()
		{
			m_RefCount--;
			assert(m_RefCount >= 0);
			//if(m_RefCount == 0)	m_pPool->freeBuffer(this); //!!!不能在主线程调用这个函数
		}
		void deleteThis()
		{
			delete this;
		}

		//index = face<<16 | miplevel
		void *lock(uint index, HardwareBufferLockOpt lockopt, LockResult &lockresult)
		{
			return m_pPool->lock(this, index, lockopt, lockresult);
		}

		void unlock(uint index)
		{
			m_pPool->unlock(this, index);
			m_DataInvalid = false;
		}

		void loadSurfaceData(uint index, SurfaceData *psurfacedata)
		{
			m_pPool->loadSurfaceData(this, index, psurfacedata);
		}

		TextureRenderTarget *createRenderTarget(uint index, int depthbits, int multisample)
		{
			return m_pPool->createRenderTarget(this, index, depthbits, multisample);
		}

		TextureRenderTarget *createDepthStencil(uint index)
		{
			return m_pPool->createDepthStencil(this, index);
		}
		

		PixelFormat getPixelFormat()
		{
			return m_pPool->getPixelFormat();
		}

		virtual void getColorBit(unsigned long * pBits) = 0;

		//for debug use
		virtual void dumpToDDS(const char *path) = 0;
		virtual void *dumpToBuffer(const char *ext, size_t &buflen) = 0;

		bool m_DataInvalid;

	protected:
		HardwarePixelBuffer() : m_DataInvalid(true), m_RefCount(0), m_FreeTime(0)
		{
		}

		virtual ~HardwarePixelBuffer()
		{
		}

		int m_RefCount;
		HardwarePixelBufferPool *m_pPool;

		uint m_FreeTime; //释放的时间， 超过多长时间没用到，会被完全释放
	};

	class _OgreExport HardwarePixelBufferManager : public Singleton<HardwarePixelBufferManager>
	{
	public:
		HardwarePixelBufferManager();
		virtual ~HardwarePixelBufferManager();

		HardwarePixelBuffer *createPixelBuffer(HardwareBufferUsage usage, const TextureDesc &desc);
		uint getCurTime()
		{
			return m_CurTime;
		}
		void garbageCollect();

	private:
		virtual HardwarePixelBufferPool *newPixelBufferPool(HardwareBufferUsage usage, const TextureDesc &desc) = 0;

	protected:
		std::map<uint, HardwarePixelBufferPool *>m_Pools;
		uint m_CurTime;
	};
}

#endif //__HardwarePixelBuffer_H__