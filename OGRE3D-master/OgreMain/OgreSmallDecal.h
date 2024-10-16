
#ifndef __SmallDecal_H__
#define __SmallDecal_H__

#include "OgreRenderableObject.h"
#include "OgreVertexFormat.h"
#include "OgreLoadWrap.h"

namespace Ogre
{

	class GameTerrainScene;

	// 此类可以现实被拉地表轨迹效果
	class _OgreExport SmallDecal : public RenderableObject, public LoadWrap
	{
		DECLARE_RTTI(SmallDecal)

	public:
		SmallDecal();

		// 绑定到场景
		virtual void attachToScene(GameScene *pscene,bool hasfather = false);
		virtual void detachFromScene();

		void setWidth (float size);
		float getWidth ();

		// 设置贴花贴图
		void setTextureRes(const std::string texName, bool block = true);

		// 横向格子数
		void setWidthGridNum (int numGrid);
		int getWidthGridNum ();

		int getNumVertexCount ();

		// 重建
		void setRebuild(bool bRebuild);
		bool shouldRebuild();
		virtual void update(uint dtick);

		// 设置透明模式（0，高光；1，混合）
		void setBlendMode (int mode = 1);
		int getBlendMode ();

		void show (bool show);
		bool isShow () { return m_bShow; }

		void BuildDecal (GameTerrainScene *scene);

	public:
		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);
		virtual void updateWorldCache();

	protected:
		static const int max_dirdecal_size = 3000;
		Vector3 m_BuildTempVB[max_dirdecal_size * 3];
		ushort m_BuildTempIB[max_dirdecal_size * 3];

	protected:
		virtual ~SmallDecal();

		void rebuildGrid ();
		void ResourceLoaded(Resource *rec, ResourceHandle h);

		Ogre::Vector3 m_Position;
		float m_fWidth;
		int m_iWidthGridNum; // 格子数，非顶点数
		VertexDeclHandle m_VertDecl;
		Material* m_pMtl;
		int m_iBlendMode;
		TextureData *m_pTexture;
		ResourceHandle m_hTexRes;
		VertexFormat m_VertFmt;
		Vector3 *m_pVBMem;
		Vector2 *m_pUV;
		ushort *m_pIBMem;
		int m_iVertexCount;
		int m_iFaceCount;
		int m_iIndexCount;

		bool m_bShow;

		int m_index;
		static int ms_index;
	};

}

#endif