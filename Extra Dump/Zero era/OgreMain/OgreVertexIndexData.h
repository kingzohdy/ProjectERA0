
#ifndef __VertexIndexData_H__
#define __VertexIndexData_H__

#include "OgreVertexFormat.h"
#include "OgreRenderTypes.h"
#include "OgreVector3.h"
#include "OgreResource.h"
#include "OgreSingleton.h"

namespace Ogre
{
	class _OgreExport VertexData : public VertexBuffer
	{
		DECLARE_RTTI(VertexData);

	public:
		VertexData();
		VertexData(const VertexFormat &vertfmt, size_t nvertex);
		VertexData(const VertexData &rhs);

		void init(const VertexFormat &vertfmt, size_t nvertex);

		HardwareVertexBuffer *getHBuf();
		VertexDeclHandle getVertexDecl();

		size_t getNumVertex()
		{
			return m_nVertex;
		}

		char *getVertexElement(size_t ivert, VertexElementSemantic ves)
		{
			const VertexElement *pelement = m_VertFormat.getElementBySemantic(ves);
			if(pelement == NULL) return NULL;

			uint offset = (uint)ivert*m_Stride + pelement->m_Offset;
			return &m_VertData[0] + offset;
		}

		char *getPosition(size_t ivert)
		{
			return getVertexElement(ivert, VES_POSITION);
		}

		char *getNormal(size_t ivert)
		{
			return getVertexElement(ivert, VES_NORMAL);
		}

		char *getColor(size_t ivert)
		{
			return getVertexElement(ivert, VES_COLOR);
		}

		template<typename VisitFunc>
		size_t visitVertices(VisitFunc func, void *pparam)
		{
			size_t nvert = m_VertData.size() / m_Stride;
			for(size_t i=0; i<nvert; i++)
			{
				func(&m_VertData[i*m_Stride], i, pparam);
			}

			return nvert;
		}

		void *lock();

		void unlock();

		virtual void _serialize(Archive &ar, int version);

	public:
		VertexFormat m_VertFormat;
		Vector3 m_MinPos;
		Vector3 m_MaxPos;

		size_t m_nVertex;
		uint m_Stride;
		std::vector<char>m_VertData;
		HardwareVertexBuffer *m_pVB;

	private:
		VertexDeclHandle m_VertDecl;
		virtual ~VertexData();
	};

	class _OgreExport IndexData : public IndexBuffer
	{
		DECLARE_RTTI(IndexData)

	public:
		IndexData();
		IndexData(size_t nindex);
		IndexData(size_t num_inst, IndexData **psrcdata, size_t *num_meshvert, PrimitiveType primtype);
		IndexData(const IndexData &rhs);

		HardwareIndexBuffer *getHBuf();
		bool isUsed()
		{
			return m_IndexData.size() > 0;
		}

		size_t getNumIndex()
		{
			return m_IndexData.size();
		}

		void *lock();
		void unlock();

		virtual void _serialize(Archive &ar, int version);

	public:
		std::vector<ushort>m_IndexData;
		HardwareIndexBuffer *m_pIB;

	private:
		virtual ~IndexData();
	};

	class DynamicBufferPool;
	class _OgreExport DynamicVertexBuffer : public VertexBuffer
	{
		DECLARE_RTTI(DynamicVertexBuffer)
	public:
		DynamicVertexBuffer()
		{
		}

		virtual  HardwareVertexBuffer *getHBuf();

		void *lock();

		void unlock()
		{
		}

		void reset(DynamicBufferPool *pool, size_t offset, const VertexFormat &vfmt, size_t nvertex);

	private:
		~DynamicVertexBuffer()
		{
		}

		VertexFormat m_VertFormat;
		size_t m_nVertex;
		size_t m_Stride;

		DynamicBufferPool *m_pPool;
		size_t m_PoolOffset;
	};

	class _OgreExport DynamicIndexBuffer : public IndexBuffer
	{
		DECLARE_RTTI(DynamicIndexBuffer)
	public:
		DynamicIndexBuffer()
		{
		}

		virtual  HardwareIndexBuffer *getHBuf();

		void *lock();
		
		void unlock()
		{
		}

		void reset(DynamicBufferPool *pool, size_t offset, size_t nindex)
		{
			m_pPool = pool;
			m_PoolOffset = offset;
			m_nIndex = nindex;
		}

	private:
		~DynamicIndexBuffer()
		{
		}

		DynamicBufferPool *m_pPool;
		size_t m_PoolOffset;
		size_t m_nIndex;
	};

	class DynamicBufferPool
	{
	public:
		DynamicBufferPool(size_t poolsize);
		~DynamicBufferPool();

		void reset();
		DynamicIndexBuffer *allocIndexBuffer(size_t nindex);
		DynamicVertexBuffer *allocVertexBuffer(const VertexFormat &vfmt, size_t nvertex);
		uchar *getBufferPtr(size_t offset)
		{
			return &m_Buffer[offset];
		}

	public:
		//uint m_DataFrame;

	private:
		std::vector<uchar>m_Buffer;
		size_t m_CurAllocPos;
		uint m_AllocFrame;

		std::vector<DynamicVertexBuffer *>m_FreeVBs;
		std::vector<DynamicVertexBuffer *>m_UsedVBs;
		std::vector<DynamicIndexBuffer *>m_FreeIBs;
		std::vector<DynamicIndexBuffer *>m_UsedIBs;

	private:
		size_t allocBuffer(size_t len);
	};
}

#endif 