
#ifndef __OgreDecal_H__
#define __OgreDecal_H__

#include "OgreRenderableObject.h"
#include "OgreVertexFormat.h"

namespace Ogre
{
	// 在选取Decal覆盖的三角形时采用一个大一些的包围盒，以便使结果集有一些冗余，使之在一定条件下不需更新
	// 这个宏定义包围盒在水平方向上扩大的尺寸
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

		// 设置贴花贴图
		void SetTextureRes(TextureData* pTexRes);

		// 获得世界空间中增加了缓存范围的AABB包围盒
		BoxBound GetWorldFatAabb();

		// 根据缓存数据判断是否需要重新建立Decal网格
		bool shouldRebuild();

		// 从一组RenderableObject中提取被贴花覆盖的三角形，组成一个mesh用于绘制
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

		Vector3					m_vLastBuildPos;		// 上次构建Decal网格时节点的位置

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