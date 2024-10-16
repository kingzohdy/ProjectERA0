
#ifndef __OgreLiquid_H__
#define __OgreLiquid_H__

#include "OgreRenderableObject.h"
#include "OgreVector2.h"
#include "OgreColourValue.h"

namespace Ogre
{
	struct LIQUID_st
	{
		Vector3  pos;
		Vector3  normal;
		Vector2  uv;
	};

	class _OgreExport TLiquid :  public RenderableObject
	{
		DECLARE_RTTI(TLiquid)
	public:
		TLiquid();
		~TLiquid();

		void createReflectLiquid(const char *pnormaltex, const char *pbumptex);
		void createGeneralLiquid(const char *animtex, int num);

		bool  createVBIB(int gx,int gz,float x1,float z1,float x2,float z2, float y);
		void  setHeight(float y);
		float getHeight()
		{
			return m_WaterHeight;
		}
		void setDepthTexture(Texture *ptex);

		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);
		virtual void update(uint dtick);

		static void SetGlobalWaterTime(uint tick); //all waters use the same time以便相邻的水面能无缝连接

		enum
		{
			WATER_GENERAL = 0,
			WATER_REFLECT
		};

	public:
		int  m_WaterType;
		FixedString m_TextureName;

		int m_nGridX;
		int m_nGridZ;
		float m_NumTexRepeat;
		uint m_FrameTicks;

		bool m_bIsMagma;

		ColourValue m_ShallowWaterColor;
		ColourValue m_DeepWaterColor;
		float m_ShallowWaterDepth;
		float m_DeepWaterDepth;
		float m_MaxWaterDepth;
		float m_Speed;
		float m_Amp;

		Texture *m_pDepthTex;
		std::vector<Texture *>m_BaseTexSet;

		Texture *m_pNormalTexture;

		VertexData *m_pVB;
		IndexData *m_pIB;
		VertexDeclHandle m_VertDecl;
		Material *m_pMtlGeneral;
		Material *m_pMtlReflect;
		float m_WaterHeight;

	private:
		void releaseTexture();
		size_t m_CurTexFrame;
	};
}

#endif