
#ifndef __RenderableObject_H__
#define __RenderableObject_H__

#include "OgreMovableObject.h"
#include "OgreRenderTypes.h"


namespace Ogre
{
	struct RenderPassDesc
	{
		uint pass_usages; //��Ҫ��renderusage�� ����ˮ����ҪRU_REFLECT

		float reflect_height;
		float reflect_distance; //�������ˮ�棬 ȡ��߶�

		Vector3 scene_center;
	};

	class _OgreExport RenderableObject : public MovableObject
	{
		DECLARE_RTTI_VIRTUAL(RenderableObject)

	public:
		virtual void enableUVMask (bool enable, bool stdmtlUse)
		{

		}

		virtual void setLiuGuangTexture (TextureData *texture)
		{

		}

		virtual void setLiuGuangTexture (const char *filename)
		{

		}

		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata) = 0;

		virtual void setCanSel(bool b = true);
		bool getCanSel();
		
		virtual void setLayer(RenderLayer layer)
		{
			m_RenderLayer = layer;
		}

		// ���������Ⱦ�ڵ��decal mesh
		virtual void BuildDecalMesh(const BoxBound& boxbound, Vector3* pVB, unsigned short* pIB, 
									int nVertexBase, int nFaceLimit, int& nVertexCount, int& nFaceCount)
		{
			nVertexCount = 0;
			nFaceCount = 0;
		}

		uint getRenderUsageBits()
		{
			return m_RenderUsageBits;
		}

		virtual void addRenderUsageBits(RenderUsage usage)
		{
			m_RenderUsageBits |= 1<<usage;
		}

		virtual void clearRenderUsageBits(RenderUsage usage)
		{
			m_RenderUsageBits &= ~(1<<usage);
		}

		virtual void getRenderPassRequired(RenderPassDesc &desc)
		{
		}

		uint getUserType()
		{
			return m_UserType;
		}

		void setUserType(uint type)
		{
			m_UserType = type;
		}

		void setDrawbackScene (bool useDrawback)
		{
			m_bUseDrawback = useDrawback;
		}

		bool isDrawbackScene ()
		{
			return m_bUseDrawback;
		}

		BoxBound* GetBoxBound(){return &m_renderBoxBound;}

	public:
		int		m_numpointlight;
		Light*	m_plights[4];	
		//����ط���д���Ĵ���,�ǵ�Ҫ�޸�
		//����Ҫע��Ƶ����ü������⡣

		bool m_bBakeStaticLight;
		bool m_bUseDrawback;
		BoxBound m_renderBoxBound;
		
	protected:
		RenderableObject() : m_RenderLayer(RL_SCENE), m_UserType(0), m_canSel(false)
		{
			m_numpointlight	= 0;
			m_RenderUsageBits = (1<<RU_UI)|(1<<RU_GENERAL);

			m_bBakeStaticLight = false;
			m_bUseDrawback = false;
		}

		~RenderableObject()
		{
		}

		RenderLayer m_RenderLayer;
		uint m_UserType;
		uint m_RenderUsageBits;
		bool m_canSel;
	};

	typedef RenderableObject GeomNode;
}

#endif
