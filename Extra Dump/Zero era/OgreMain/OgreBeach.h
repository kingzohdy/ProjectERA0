
#ifndef __Beach_H__
#define __Beach_H__

#include "OgreRenderableObject.h"
#include "OgreVertexFormat.h"
#include "OgreBeachData.h"
#include <vector>

namespace Ogre
{

	class _OgreExport Beach : public RenderableObject
	{
	public:
		DECLARE_RTTI(Beach)

		Beach(BeachData *psource=NULL);
		virtual ~Beach();

		// 绑定到场景
		virtual void attachToScene(GameScene *pscene,bool hasfather = false);
		virtual void detachFromScene();

		void addPoint (Ogre::Vector3 point);
		void removeLastPoint ();
		void clearPoints ();
		int getNumPoint ();

		void reBuild ();

		// 宽度
		void setWidth (float fWidth);
		float getWidth ();

		// 超出水面高度
		void setHeight (float fHeight);
		float getHeight ();

		void setUVXRepeat (int xRepeat);
		void setUVYrepeat (int yRepeat);

		// 设置透明模式（0，高光；1，抓痕贴花）
		void setBlendMode (int mode = 1);
		int getBlendMode ();

		// 设置贴花贴图
		void setTextureRes (const std::string texName);
		void setMaskTextureRes (const std::string texName);

		void setUVSpeed (float speed);
		void setVRange (float fVRange) { m_fVRange = fVRange; }
		float getVRange () { return m_fVRange; }

		virtual void update(uint dtick);

	public:
		virtual void render (SceneRenderer* pRenderer, const ShaderEnvData &envdata);
		virtual void updateWorldCache();

	protected:
		BoxSphereBound m_WorldBounds;

		BeachData *m_pSource;

		VertexDeclHandle m_VertDecl;
		VertexFormat m_VertFmt;
		Material* m_pMtl;

		float m_fElapsedTime;
		int	m_nVertexCount;
		int	m_nFaceCount;
		int	m_nCurVbSize;
		Vector3 *m_pVBMem;
		
		// write
		float m_Width;
		float m_Height;
		int m_iBlendMode;
		float m_uvXRepeat;
		float m_uvYRepeat;
		float m_fUVSpeed;
		float m_UVTrans;
		float m_fVRange;
		std::vector<Vector3> m_RoadPoints;
		TextureData *m_pTextureRes;
		TextureData *m_pMaskTexture;
	};

}

#endif