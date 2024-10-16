
#ifndef __Billboard_H__
#define __Billboard_H__

#include "OgreRenderableObject.h"
#include "OgreColourValue.h"
#include "OgreVector3.h"
#include "OgreVector2.h"

namespace Ogre
{
	struct BillboardVerts
	{
		Vector3 m_Pos;
		ColorQuad m_Color;
		Vector2 m_UV;
		Vector2 m_MaskUV;
	};

	class _OgreExport Billboard : public RenderableObject
	{
		DECLARE_RTTI(Billboard)
	public:
		struct UpdateData
		{
			int texrow;
			int texcol;
			ColourValue color;
			float size;
			float aspectratio;
		};

	public:
		Billboard(BillboardData *psource=NULL);

		virtual void update(uint dtick);
		virtual void resetUpdate(bool pause, uint tick=0xffffffff);
		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);
		virtual void getRenderPassRequired(RenderPassDesc &desc);

		void fillBillboardVert(BillboardVerts *pvert, unsigned short *pindex);

	protected:
		virtual ~Billboard();
		void updateBillboard(uint dtick);
		void updateBillboard2(uint dtick);
		void fillBillboardVert2(BillboardVerts *pvert, unsigned short *pindex);

	protected:
		BillboardData *m_pBillboard;

		uint m_CurTick;
		uint m_MaxTick;

		UpdateData m_UpdateData;

		Material *m_pMtl;

		Matrix4 m_WorldView;

		Vector2 m_uv0, m_uv1, m_uv2, m_uv3;
		Vector2 m_uvMask0, m_uvMask1, m_uvMask2, m_uvMask3;
	};
}

#endif