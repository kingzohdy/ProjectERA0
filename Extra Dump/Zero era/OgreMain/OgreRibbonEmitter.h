
#ifndef __OgreRibbonEmitter_H__
#define __OgreRibbonEmitter_H__

#include "OgreRenderableObject.h"
#include "OgreRibbonData.h"
#include "OgreVertexFormat.h"

namespace Ogre
{
	template<typename T>
	class TinyQueue
	{
	public:
		TinyQueue() : m_Buf(NULL)
		{
		};

		~TinyQueue()
		{
			if(m_Buf != NULL) delete[] m_Buf;
		};

		void Create(int size, T def)
		{
			if(size > 0)
			{
				m_Buf = new T[size];
				for(int i = 0; i < size; i++)
					m_Buf[i] = def;
			}
			m_iHead = 0;
			m_iSize = 0;
			m_iMaxSize = size;
		};

		void Add(T val)
		{
			if(m_iMaxSize <= 0)
				return;
			m_Buf[m_iHead] = val;
			m_iHead = (m_iHead+1) % m_iMaxSize;
			if(m_iSize  < m_iMaxSize)
				m_iSize++;
		};

		T& GetHead(int offset)
		{
			int pos = (m_iHead + offset - 1) % m_iMaxSize;
			while(pos < 0) pos += m_iMaxSize;
			return m_Buf[pos];
		};

	public:
		int m_iHead;
		int m_iSize;
		int m_iMaxSize;
		T* m_Buf;
	};

	class LerpFun
	{
	public:
		void Create(float f0, float d0, float f1,  float t1);
		float GetVal(float t);
		float GetDer(float t);
	public:
		float m_a;
		float m_b;
		float m_c;
	};

	struct RibLerpData
	{
		Vector3 pos;
		Vector3 posDer;
		uint	tick;
	};

	class RibbonSegBuffer
	{
	public:
		RibbonSegBuffer();
		~RibbonSegBuffer();
		void			Create(int iSize);
		void			PopTail();
		RIBBON_T*		PushHead(RIB_SEG_TYPE type);

		int				GetCount();
		void			BeginIterate();
		RIBBON_T*		GetCurrent();
		bool			Next();
		RIBBON_T*		GetPrevRealSeg(int iPrev = 1);
		RIBBON_T*		GetPrevSeg(int iPrev = 1);
		RIBBON_T*		GetTail();
		bool			IsEndOfQueue();
		RIBBON_T*		GetHead();

		int				m_iHead;
		int				m_iTail;
		int				m_iSize;

		RIBBON_T*		m_pRibBuffer;
		int				m_iCurr;
		int				m_iCount;


	};

	struct RIBBON_VERT;
	class _OgreExport RibbonEmitter : public RenderableObject
	{
		DECLARE_RTTI(RibbonEmitter)

	public:
		size_t m_nMaxSize;
		RibbonEmitter(RibbonEmitterData *psource=NULL);

		virtual void update(uint dtick);
		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);
		virtual void getRenderPassRequired(RenderPassDesc &desc);

	protected:
		~RibbonEmitter();
		void EmitteRibbon(size_t num, const RibbonEmitterFrameData &curframedata);

		void UpdateRibbonLife(uint dtick);

		void UpdateRibbonPos();
		void UpdateRibbon(float dtime);
		int fillVertex(SceneRenderer *prenderer, VertexBuffer *&pvb, IndexBuffer *&pib);
		void RibbonEmitter::FillSingleVert(RIBBON_T& ribseg, RIBBON_VERT& outvert, const Vector3 &templateVert, const Vector2 &baseuv);


	public:
		RibbonEmitterData *m_pRibbon;
		uint m_CurTick;
	protected:
		int m_iEmitNum;

		std::vector<RIBBON_T> m_Ribbon;

		VertexDeclHandle m_VertDecl;
		Material *m_pMtl;
		VertexFormat m_VertFmt;

		uint m_AccumTick;

		int m_CurSeq;
		float m_fScale;

		Matrix4 m_View;
		TinyQueue<RibLerpData> m_LerpData;
		RibbonSegBuffer	m_RibbonBuffer;
		Vector2 m_UVRotate;
		Vector2 m_UVScale;
		Vector2 m_UVOffset;

		Vector2 m_MaskUVRotate;
		Vector2 m_MaskUVScale;
		Vector2 m_MaskUVOffset;

		RibbonEmitterFrameData m_LastFrameAnimData;
		bool m_InitFrameData;
	};
}

#endif