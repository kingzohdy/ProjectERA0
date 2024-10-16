
#ifndef __OgreParametricShape_H__
#define __OgreParametricShape_H__

#include "OgreRenderableObject.h"
#include "OgreParametricShapeData.h"
#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreVertexFormat.h"

namespace Ogre
{
	struct GEOMETRY_VERT
	{
		Vector3 pos;
		ColorQuad color;
		Vector2 uv;
		Vector2 uvMask;
	};

	struct ParamShapeFrameData
	{
		float m_fSphereRadius;
		float m_fSphereAngle;

		float m_fTorusNRadius;
		float m_fTorusLRadius;

		float m_fTubeNRadius;
		float m_fTubeLRadius;
		float m_fTubeHeight;

		float m_fColumnDownRadius;
		float m_fColumnUpRadius;
		float m_fColumnHeight;
		ColourValue m_Color;

		float m_UOffset;
		float m_VOffset;
		float m_fRound;
		float m_UScale;
		float m_VScale;
		float m_UVRotate;

		float m_MaskUOffset;
		float m_MaskVOffset;
		float m_MaskUScale;
		float m_MaskVScale;
		float m_MaskUVRotate;

		int m_iCurrTile;
		int m_iCurrMaskTile;
	};

	class _OgreExport ParametricShape : public RenderableObject
	{
		DECLARE_RTTI(ParametricShape)

	public:
		ParametricShape(ParamShapeData* pSource = NULL);
		void SetSeg(size_t nSegu,size_t nSegv);

		virtual void update(uint dtick);
		virtual void resetUpdate(bool pause, uint tick=0xffffffff);
		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);
		virtual void getRenderPassRequired(RenderPassDesc &desc);

		void fillContext(SceneRenderer *prenderer, ShaderContext *pcontext, PARAMSHAPE_TYPE GeoType,size_t nSegu, size_t nSegv);
		void	SetAABB();

		void	SetSphereRadius(float fRadius);
		void	SetTubeRadius(float fTubeNRadius,float fTubeLRadius,float fTubeHeight);
		void	SetTorusRadius(float fTorusNRadius,float fTorusLRadius);
		void	SetTextureRes(TextureData *pTexRes);
		void	SetBlendMode(int BlendMode);

		void ResetTime(uint tick)			{ m_CurTick = tick; };
	protected:
		~ParametricShape();

	public:
		ParamShapeData* m_pSource;
		uint m_CurTick;

	protected:
		int	m_nIndex;
		int	m_nVerts;
		VertexFormat        m_VertFmt;
		VertexDeclHandle   m_VertDecl;
		TextureData *m_pCurTex;
		TextureData *m_pCurMaskTex;
		Material *m_pMtl;
		ParamShapeFrameData m_FrameData;
	};
}

#endif