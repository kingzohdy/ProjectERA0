
#ifndef __DirDecal_H__
#define __DirDecal_H__

#include "OgreRenderableObject.h"
#include "OgreVertexFormat.h"
#include "OgreLoadWrap.h"

namespace Ogre
{

	// 此类可以现实被拉地表轨迹效果
	class _OgreExport DirDecal : public RenderableObject, public LoadWrap
	{
		DECLARE_RTTI(DirDecal)

	public:
		DirDecal();

		void clearPoints ();
		void addPoint (Vector3 point);	// 从角色自己开始向前延伸
		std::vector<Vector3> &getRoadPoints ();

		// 绑定到场景
		virtual void attachToScene(GameScene *pscene,bool hasfather = false);
		virtual void detachFromScene();

		// 宽度
		void setWidth (float fWidth);
		float getWidth ();

		// 宽度网格细分
		void setWidthGridNum (int iWidthGridNum);
		int getWidthGridNum ();

		// 设置贴图显示范围（0 - 1.0f）
		void setTextureRadio (float fRadio);
		float getTextureRadio ();

		// 设置贴花贴图
		void setTextureRes(const std::string texName);

		// 设置透明模式（0，高光；1，抓痕贴花）
		void setBlendMode (int mode = 0);
		int getBlendMode ();

		//-----------------begin for 地表抓痕-------------------

		// src 发起点
		// dst 目标点
		// fTimeAllShow 拉痕显示完整的时间（从原点拉到目标点的时间）
		// fPointLife 每个点的alpha的生命值
		// fPointFadeTime 消失的时间(例如参数中，从4000秒开始消失到完全消失)
		// iNumPoints 点的数量
		void playTrace (Vector3 src, Vector3 dst, float fTimeAllShow, 
			float fPointLife=5000.0f, float fPointFadeTime=1000.0f, int iNumPoints=21);

		//-----------------end for 地表抓痕-------------------

		// 重建
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