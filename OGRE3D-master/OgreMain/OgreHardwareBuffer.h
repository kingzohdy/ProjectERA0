
#ifndef __HardwareBuffer_H__
#define __HardwareBuffer_H__

#include "OgreList.h"
#include "OgreSingleton.h"
#include "OgreRenderTypes.h"
#include "OgreLocker.h"
#include <map>

namespace Ogre
{
	class _OgreExport HardwareBuffer : protected ChainListNode
	{
		friend class HardwareBufferPool;
	public:
		void release();
		void *lock(HardwareBufferLockOpt lockopt=HBL_NORMAL);
		void unlock();

		HardwareBufferPool *getPool()
		{
			return m_pPool;
		}
		size_t getStart()
		{
			return m_Start;
		}
		size_t getNum()
		{
			return m_Num;
		}
		size_t getBubSize()
		{
			return m_BubSize;
		}

	protected:
		HardwareBuffer(HardwareBufferPool *pool) : m_pPool(pool), m_DataInvalid(true)
		{
		}

		virtual ~HardwareBuffer()
		{
		}

		void Set(size_t start, size_t nbytes, size_t bubsize)
		{
			m_Start = start;
			m_Num = nbytes;
			m_BubSize = bubsize;
		}

	public:
		bool m_DataInvalid;

	private:
		size_t m_Start;
		size_t m_Num;
		size_t m_BubSize;
		HardwareBufferPool *m_pPool;
	};

	class _OgreExport HardwareIndexBuffer : public HardwareBuffer
	{
	public:
		HardwareIndexBuffer(HardwareBufferPool *pool) : HardwareBuffer(pool)
		{
		}

		size_t getStartIndex()
		{
			return getStart()/2;
		}

	public:

	};

	class _OgreExport HardwareVertexBuffer : public HardwareBuffer
	{
	public:
		HardwareVertexBuffer(HardwareBufferPool *pool) : HardwareBuffer(pool), m_Stride(0)
		{
		}

		void setStride(size_t stride)
		{
			m_Stride = stride;
		}

	public:
		size_t m_Stride;
	};

	class _OgreExport HardwareBufferPool
	{
	public:
		HardwareBufferPool(bool is_vbuffer, size_t poolsize, bool bdynamic);
		virtual ~HardwareBufferPool();

		HardwareBuffer *allocBuffer(size_t num);
		void freeBuffer(HardwareBuffer *pbuf);

		void onDataInvalidate();

		virtual void *lock(size_t start, size_t nbytes, HardwareBufferLockOpt lockopt) = 0;
		virtual void unlock() = 0;

	protected:
		bool m_bDynamic;
		bool m_bVertexBuffer;
		HardwareBuffer *m_pDynBuffer;
		size_t m_PoolSize;

		ChainList<HardwareBuffer>m_FreeBuffers;
		ChainList<HardwareBuffer>m_UsedBuffers;

	private:
		HardwareBuffer *allocRandomBuffer(size_t num);
		HardwareBuffer *allocAccumBuffer(size_t num);
		HardwareBuffer *newBuffer();
		void reset();

	public:
		HardwareBufferPool *m_pNextPool;
		LockSection m_allocMutex;
	};

	class _OgreExport HardwareBufferManager : public Singleton<HardwareBufferManager>
	{
	public:
		HardwareBufferManager();
		virtual ~HardwareBufferManager();

		HardwareVertexBuffer *createVertexBuffer(HardwareBufferUsage usage, size_t nbytes, size_t stride=0);
		HardwareIndexBuffer *createIndexBuffer(HardwareBufferUsage usage, size_t nindex, bool b32bit=false);

	private:
		HardwareBuffer *createVertexOrIndexBuffer(bool is_vbuffer, HardwareBufferUsage usage, size_t nbytes, size_t stride);

		//nbytes: num bytes of pool
		virtual HardwareBufferPool *newPool(bool is_vbuffer, HardwareBufferUsage usage, size_t nbytes, size_t stride) = 0;

	protected:
		typedef std::map<unsigned int, HardwareBufferPool *> PoolTable;
		typedef std::map<unsigned int, HardwareBufferPool *>::iterator PoolTableIter;
		PoolTable m_Pools;
	};

	inline void HardwareBuffer::release()
	{
		m_pPool->freeBuffer(this);
	}

	inline void *HardwareBuffer::lock(HardwareBufferLockOpt lockopt)
	{
		return m_pPool->lock(m_Start, m_Num, lockopt);
	}

	inline void HardwareBuffer::unlock()
	{
		m_pPool->unlock();
		m_DataInvalid = false;
	}
}

#endif