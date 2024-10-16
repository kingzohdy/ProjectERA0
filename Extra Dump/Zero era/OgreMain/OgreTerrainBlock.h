
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
		void setSubDevie(int devie);// Lod��������ĸ����ġ��ӵ㡱,1��ʾ�ϣ�2��ʾ�£�3��ʾ��4��ʾ��

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

		int m_nTerrainQuality;//0��,1��
		int m_nTexLayerNum;
	//	BoxBound m_BoundBox;
		size_t m_nDrawStrip[6];

		bool m_bRenderWater;

		TextureData *m_pLightTexture;
		int mSub; // 1,2,3,4,5��1��ʾϸ�ֺ󣬲��ӡ����ӵ㡱�������2345��ʾʣ�µ��ĸ�����������
	};
}

#endif