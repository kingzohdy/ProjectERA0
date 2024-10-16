
#ifndef __TerrainBlock_H__
#define __TerrainBlock_H__

#include "OgreRenderableObject.h"
#include "OgreTexture.h"
#include "OgreTerrainData.h"
#include "OgreColourValue.h"
#include <vector>

namespace Ogre
{
	class _OgreExport TerrainBlock : public RenderableObject
	{
		DECLARE_RTTI(TerrainBlock)

	public:
		TerrainBlock();
		TerrainBlock(TerrainBlockSource* pBlockSource);
		~TerrainBlock();

		void update(uint dtick);
		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);
		virtual void getRenderPassRequired(RenderPassDesc &desc);

		bool getHeight(float x,float z,float* height);
		bool createRenderData(TerrainTileSource *ptile);
		void setTerrainQuality(int nQuality);
		void setSubDevie(int devie);// Lod处理过的四个方的“加点”,1表示上，2表示下，3表示左，4表示右

		virtual void BuildDecalMesh(const BoxBound& boxbound, Vector3* pVB, unsigned short* pIB, 
									int nVertexBase, int nFaceLimit, int& nVertexCount, int& nFaceCount);

		void setOnlyRenderTerrainOrWater (bool renderWater);
		bool isOnlyRenderWater ();

	private:
		bool createLiquid();
		IndexData *createIBWithHole(int nsize);
		IndexData *createIBWithHole1(int nsize, int level);
		/*IndexData *createIBWithHole2(int nsize);*/
		IndexData *createIBWithoutHole(int nsize);
		void createVBIB();
		bool createTexture(TerrainTileSource* ptile);
		void createMaterials();

	public:
		TerrainBlockSource  *m_pBlockSource;

		TerrainBlockOptionData  m_OptionData;
		TLiquid           *m_pLiquid;

		VertexData *m_pVB;
		IndexData *m_pIB[6];
		VertexDeclHandle m_VertDecl;

		Material *m_pMtl;

		int m_nTerrainQuality;//0高,1低
		int m_nTexLayerNum;
	//	BoxBound m_BoundBox;
		size_t m_nDrawStrip[6];

		bool m_bRenderWater;

		TextureData *m_pLightTexture;
		int mSub; // 1,2,3,4,5（1表示细分后，不加“连接点”的情况，2345表示剩下的四个方向的情况）
	};
}

#endif