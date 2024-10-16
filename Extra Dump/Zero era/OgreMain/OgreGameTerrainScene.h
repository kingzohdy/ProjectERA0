
#ifndef __GameTerrainScene_H__
#define __GameTerrainScene_H__

#include "OgreGameScene.h"
#include "OgreTerrainData.h"
#include "OgreLoadWrap.h"
#include <vector>

namespace Ogre
{
	class LooseOctree;
	class ReflectEffect;
	class PhysicsScene;
	class PlantSetNode;

	// ��̬����
	class BHeader0
	{
	public:
		BHeader0 ()
		{
			version = 100;
			type = -1;
			number = 0;
		}
		~BHeader0 ()
		{

		}

		int version;
		int type; // 0 ���Σ�1ģ��
		int number; // ���ο�����������ģ������
	};

	class BTLightData0
	{
	public:
		BTLightData0 ()
		{
			xIndex = -1;
			zIndex = -1;
		}
		~BTLightData0 ()
		{

		}

		int xIndex;
		int zIndex;
		int number;
		std::vector<unsigned int> colors;
	};

	class BMLightData0
	{
	public:
		BMLightData0 ()
		{
			modelIndex = -1;
			meshIndex = -1;
			subMeshIndex = -1;
			number = 0;
		}
		~BMLightData0 ()
		{
		}

		std::string modelFilename;
		int modelIndex;
		int meshIndex;
		int subMeshIndex;
		int number;
		std::vector<unsigned int> colors;
	};

	class BTLightMap0
	{
	public:
		BTLightMap0 ()
		{
			x = -1;
			z = -1;
			texture = 0;
		}
		~BTLightMap0 ()
		{

		}

		int x;
		int z;
		Ogre::TextureData *texture;
	};

	class TerrainSceneNode : public LoadWrap
	{
	public:
		enum
		{
			LOAD_TERRAIN = 0, //װ�ص���, linkmesh
			LOAD_MODEL_BIG,   //��ʼװ�ش�ģ��
			LOAD_MODEL_SMALL, //��ʼװ��Сģ��
			LOAD_MODEL_DETAIL, //��ʼװ��ϸ��ģ�ͺͲݲ�
		};

	public:
		TerrainSceneNode(TerrainTile *ptile, size_t index);
		~TerrainSceneNode();

		void loadTerrain();
		void loadModels(TerrainModelType modeltype);
		void unloadModels(TerrainModelType modeltype);

		void update(uint dtick);
		void updateGrassDisturb (
			Ogre::WorldPos PlayerCurPos, 
			uint dtick);
		void setLoadLod(int loadlod);

	public:
		TerrainTile *m_pTile;
		size_t m_IndexInTile;
		TerrainBlock *m_pTerrainBlock;

		std::vector<int>m_ModelIndices;
		std::vector<int>m_PlantIndices;

		struct BackLoadInfo
		{
			ResourceHandle h;
			TileModelData *ptilemodel;
			RenderableObject *pobject;
			void*	pBSPNode;
			int modelIndex;
		};
		std::vector<BackLoadInfo>m_ModelResHandles;

		std::vector<RenderableObject *>m_Renderables;

		int m_CurLoadLod;

		ResourceHandle m_hGrass;
		PlantSetNode *m_pGrassObj;

	private:
		void mergeAttachModels(std::vector<ModelInstanceData>&instdata, TerrainModelType modeltype, int texid);
		RenderableObject * loadSingleModel(Resource *pres, TileModelData *ptilemodel, BackLoadInfo& info);
		void addModelInstances(std::vector<ModelInstanceData>&instances, TerrainModelType modeltype, int texid);
		void loadGrass();
		void unloadGrass();

		virtual void ResourceLoaded(Resource *pres, ResourceHandle h);

		float m_fDistrubingTime;

	};

	class TerrainTile
	{
	public:
		TerrainTile(GameTerrainScene *pscene, TerrainTileSource *ptiledata);
		~TerrainTile();

		void buildBlocks();
		void loadOneBlock(size_t x, size_t z);

		void buildPhysicsScene2 (PhysicsScene2 *scene);

		size_t blockIndex(size_t x, size_t z)
		{
			if (x>=m_NumBlockX || z>=m_NumBlockZ || 
				x<0 || z<0)
				return - 1;

			assert(x<m_NumBlockX && z<m_NumBlockZ);

			return z*m_NumBlockX + x;
		}

		size_t getBlockX(float x)
		{
			size_t i = (size_t)(x/m_BlockSize);
			assert(i < m_NumBlockX);
			return i;
		}

		size_t getBlockZ(float z)
		{
			size_t i = (size_t)(z/m_BlockSize);
			assert(i < m_NumBlockZ);
			return i;
		}

		int getBlockIndexX (int index)
		{			
			return (int)(index%m_NumBlockX);
		}

		int getBlockIndexZ (int index)
		{
			return (int)(index/m_NumBlockX);
		}

		float getTileSizeX()
		{
			return float(m_BlockSize*m_NumBlockX);
		}

		float getTileSizeZ()
		{
			return float(m_BlockSize*m_NumBlockZ);
		}

		void getBlockRange(int &sx, int &sz, int &ex, int &ez, const BoxSphereBound &bound);

		TerrainSceneNode *getBlock(size_t x, size_t z)
		{
			return m_Nodes[blockIndex(x, z)];
		}

		void update(uint dtick);
		void updateGrassDisturb (
			Ogre::WorldPos PlayerCurPos, 
			uint dtick);

	public:
		GameTerrainScene*				m_pScene;
		TerrainTileSource*				m_pTileData;
		std::vector<TerrainSceneNode *>	m_Nodes;
		float							m_BlockSize;
		size_t							m_NumBlockX;
		size_t							m_NumBlockZ;

		LooseOctree*					m_pSpatialTree;
	};

	enum SceneObjectType
	{
		SOT_TERRAINOBJ = 1,
		SOT_RENDEROBJ,
		SOT_EFFECTOBJ,
		SOT_ACTOR,
	};
	class _OgreExport GameTerrainScene : public GameScene
	{
		DECLARE_RTTI(GameTerrainScene)

	public:
		GameTerrainScene();
		GameTerrainScene(const FixedString &scene_pathName, size_t ntilex, size_t ntilez);
		~GameTerrainScene();

		void switchUseStaticLight (bool useStaticLight);
		void processEdgeLight ();
		bool isUseStaticLight ();

		virtual void update(uint dtick);
		void updateGrassDisturb (
			Ogre::WorldPos PlayerCurPos, 
			uint dtick);

		virtual void onAttachObject(MovableObject *pobject);
		virtual void onDetachObject(MovableObject *pobject);
		virtual void onObjectPosChange(MovableObject *pobject);
		virtual void onRender(SceneRenderer *prenderer);
		virtual void onCull(Camera *pcamera);
		virtual void	caculateShadowCamera(Camera *pcamera,Camera *pdepthcamera);
		void caculateShadowCamera2(Camera *pcamera, Camera *pdepthcamera);

		virtual MovableObject *pickObject(IntersectType type, const WorldRay &ray, float *collide_dist, uint pickflags);
		virtual bool pickGround(const WorldRay &ray, float *collide_dist);
		virtual bool pickGround(WPOS_T x, WPOS_T z, WPOS_T *y, Vector3 *pnormal=NULL, float *water_height=NULL);

		virtual void updateFocusArea(const WorldPos center, float radius);

		virtual void getEffectObjects(std::vector<RenderableEffectInfo>&objs, RenderableObject *ptarget);
		virtual TerrainTile * getTerrainTile();

		void addTerrainTile(size_t x, size_t z, TerrainTileSource *ptiledata);
		void loadPhysicsScene2 ();
		void buildPhysicsScene2 ();// �������û�У��ʹ���
		void savePhysicsScene2 ();
		size_t TileIndex(size_t x, size_t z)
		{
			return z*m_nTileX + x;
		}
		std::string getPlantResName(TerrainTileSource *ptiledata, size_t nBlock);

	public:
		bool m_bLoadGrass;  // �Ƿ���ز�


		// ��̬����
		BHeader0 mTerrainHead;
		BHeader0 mModelHead;
		std::vector<BTLightData0> mTLightDatas;
		std::vector<BMLightData0> mMLightDatas;
		std::vector<BTLightMap0> mTLightMaps;

	protected:
		size_t m_nTileX;
		size_t m_nTileZ;
		std::vector<TerrainTile *>m_Tiles;

		//std::vector<EffectObject *>m_EffectObjects;//global effect objects

		uint m_SceneTick;

	public:
		bool getGroundHeight(float x, float z, float *h, Vector3 *pnormal=NULL, float *waterheight=NULL);

	protected:
		bool isUnderTerrain(const Vector3 &pos);
		bool testTriangle(const Ogre::Ray &ray, int sx, int sz, int ex, int ez, float &t);
		bool pickTerrain(const WorldRay &ray, float *collide_dist);

		void buildDecalMesh(const BoxSphereBound &bound, Vector3 *pVB, unsigned short* pIB, int nVertexBase, int nFaceLimit, int& nVertexCount, int& nFaceCount);
		void buildDecalMesh(const BoxSphereBound &bound, 
			std::vector<Vector3> &roadPointList, float fWidth, int iWidthGridNum, 
			Vector3 *pVB, Vector2 *pUV, unsigned short *pIB,  
			int nVertexBase, int nFaceLimit, int &nVertexCount, int &nFaceCount);
	private:
		const FixedString m_ScencePath;		//��ǰ��ͼ�ļ�·��
		int m_lightMode; // 0ʵʱ�� 1���㣬 2��ͼ

	};
}

#endif




















