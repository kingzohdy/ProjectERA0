
#ifndef __SmallDecal_H__
#define __SmallDecal_H__

#include "OgreRenderableObject.h"
#include "OgreVertexFormat.h"
#include "OgreLoadWrap.h"

namespace Ogre
{

	class GameTerrainScene;

	// ���������ʵ�����ر�켣Ч��
	class _OgreExport SmallDecal : public RenderableObject, public LoadWrap
	{
		DECLARE_RTTI(SmallDecal)

	public:
		SmallDecal();

		// �󶨵�����
		virtual void attachToScene(GameScene *pscene,bool hasfather = false);
		virtual void detachFromScene();

		void setWidth (float size);
		float getWidth ();

		// ����������ͼ
		void setTextureRes(const std::string texName, bool block = true);

		// ���������
		void setWidthGridNum (int numGrid);
		int getWidthGridNum ();

		int getNumVertexCount ();

		// �ؽ�
		void setRebuild(bool bRebuild);
		bool shouldRebuild();
		virtual void update(uint dtick);

		// ����͸��ģʽ��0���߹⣻1����ϣ�
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
		int m_iWidthGridNum; // ���������Ƕ�����
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