
#ifndef __DirDecal_H__
#define __DirDecal_H__

#include "OgreRenderableObject.h"
#include "OgreVertexFormat.h"
#include "OgreLoadWrap.h"

namespace Ogre
{

	// ���������ʵ�����ر�켣Ч��
	class _OgreExport DirDecal : public RenderableObject, public LoadWrap
	{
		DECLARE_RTTI(DirDecal)

	public:
		DirDecal();

		void clearPoints ();
		void addPoint (Vector3 point);	// �ӽ�ɫ�Լ���ʼ��ǰ����
		std::vector<Vector3> &getRoadPoints ();

		// �󶨵�����
		virtual void attachToScene(GameScene *pscene,bool hasfather = false);
		virtual void detachFromScene();

		// ���
		void setWidth (float fWidth);
		float getWidth ();

		// �������ϸ��
		void setWidthGridNum (int iWidthGridNum);
		int getWidthGridNum ();

		// ������ͼ��ʾ��Χ��0 - 1.0f��
		void setTextureRadio (float fRadio);
		float getTextureRadio ();

		// ����������ͼ
		void setTextureRes(const std::string texName);

		// ����͸��ģʽ��0���߹⣻1��ץ��������
		void setBlendMode (int mode = 0);
		int getBlendMode ();

		//-----------------begin for �ر�ץ��-------------------

		// src �����
		// dst Ŀ���
		// fTimeAllShow ������ʾ������ʱ�䣨��ԭ������Ŀ����ʱ�䣩
		// fPointLife ÿ�����alpha������ֵ
		// fPointFadeTime ��ʧ��ʱ��(��������У���4000�뿪ʼ��ʧ����ȫ��ʧ)
		// iNumPoints �������
		void playTrace (Vector3 src, Vector3 dst, float fTimeAllShow, 
			float fPointLife=5000.0f, float fPointFadeTime=1000.0f, int iNumPoints=21);

		//-----------------end for �ر�ץ��-------------------

		// �ؽ�
		void setRebuild(bool bRebuild);
		bool shouldRebuild();
		virtual void update(uint dtick);

	public:
		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);
		virtual void updateWorldCache();

		void buildMesh(Vector3 *pvb, Vector2 *pUV, unsigned short *pib, int nVertexCount, int nFaceCount);
		void ResourceLoaded(Resource *rec, ResourceHandle h);

	protected:
		static const int max_dirdecal_size = 3000;
		Vector3 m_BuildTempVB[max_dirdecal_size * 3];
		ushort m_BuildTempIB[max_dirdecal_size * 3];

	protected:
		virtual ~DirDecal();

		int m_iBlendMode;
		float m_fWidth;
		int m_iWidthGridNum;
		float m_fRadio;

		VertexDeclHandle m_VertDecl;
		Material* m_pMtl;
		TextureData *m_pTexture;
		ResourceHandle m_hTexRes;
		VertexFormat m_VertFmt;

		int						m_nVertexCount;
		int						m_nFaceCount;
		int						m_nCurVbSize;
		int						m_nCurIbSize;

		bool					m_bIsTrace;
		Vector3					m_Src;
		Vector3					m_Dst;
		float					m_fTimeAllShow;
		int						m_iNumPoints;
		float					m_fPointLife;
		float					m_fPointFadeTime;

		Vector3*				m_pVBMem;
		Vector2*				m_pUV;
		ushort*					m_pIBMem;

		Vector3	m_LastBuildPos;
		bool m_bShouldRebuild;
		float m_fTransparent;

		std::vector<Vector3> m_RoadPoints;
		std::vector<float> m_AgeCondition;
		std::vector<float> m_AgeForCondition;
		std::vector<float> m_AgeList;
		std::vector<float> m_AlphaList;
	};

}

#endif