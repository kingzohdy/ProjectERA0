
#ifndef __OgreDecal_H__
#define __OgreDecal_H__

#include "OgreRenderableObject.h"
#include "OgreVertexFormat.h"

namespace Ogre
{
	// ��ѡȡDecal���ǵ�������ʱ����һ����һЩ�İ�Χ�У��Ա�ʹ�������һЩ���࣬ʹ֮��һ�������²������
	// ����궨���Χ����ˮƽ����������ĳߴ�
#define DECALMESH_CACHESIZE			1000.0f

	class _OgreExport DecalNode : public RenderableObject
	{
		DECLARE_RTTI(DecalNode)

	public:
		DecalNode(DecalData* pSource = NULL);

		virtual void update(uint dtick);
		virtual void resetUpdate(bool pause, uint tick=0xffffffff);
		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);
		virtual void attachToScene(GameScene *pscene,bool hasfather = false);
		virtual void updateWorldCache();

		// ����������ͼ
		void SetTextureRes(TextureData* pTexRes);

		// �������ռ��������˻��淶Χ��AABB��Χ��
		BoxBound GetWorldFatAabb();

		// ���ݻ��������ж��Ƿ���Ҫ���½���Decal����
		bool shouldRebuild();

		// ��һ��RenderableObject����ȡ���������ǵ������Σ����һ��mesh���ڻ���
		void BuildMesh(std::vector<RenderableObject*>& vecGemoNodes);
		void buildMesh(Vector3 *pvb, unsigned short *pib, int nvert, int nface);

	protected:
		~DecalNode();

		uint	m_CurTick;
		DecalData*			m_pSource;

		VertexDeclHandle m_VertDecl;
		Material*				m_pMtl;
		float					m_fRadius;
		float                   m_MaxKeyframeRadius;
		VertexFormat m_VertFmt;

		Vector3					m_vLastBuildPos;		// �ϴι���Decal����ʱ�ڵ��λ��

		int						m_nVertexCount;
		int						m_nFaceCount;
		int						m_nCurVbSize;
		int						m_nCurIbSize;

		Vector3*				m_pVBMem;
		ushort*					m_pIBMem;
		bool					m_bRadiusChange;
	};
}

#endif