
#pragma once


#include "TerrainBlock.h"
#include "TerrainTile.h"

#include "OgreGameScene.h"
#include "OgreBounding.h"

#include <vector>
#include <set>
using namespace std;



namespace TE
{
	enum TerrPickType
	{
		TERR_PICK_VB = 0,  //使用vb里面的高度信息pick
		TERR_PICK_VB_BSP,  //使用vb里面的高度信息和bsp数据pick
		TERR_PICK_HEIGHTMAP,//使用heightmap的高度信息pick
	};
	class Terrain : public Ogre::GameScene
	{
	public:
		Terrain(void);
		~Terrain(void);

		virtual void init();

		virtual void update(Ogre::uint dtick);

		virtual void onAttachObject(Ogre::MovableObject *pobject);
		virtual void onDetachObject(Ogre::MovableObject *pobject);
		virtual void onObjectPosChange(Ogre::MovableObject *pobject);
		virtual void onRender(Ogre::SceneRenderer *prenderer);
		virtual void onCull(Ogre::Camera *pcamera);
		virtual void caculateShadowCamera2(Ogre::Camera *pcamera, Ogre::Camera *pdepthcamera);
		virtual void caculateShadowCamera(Ogre::Camera *pcamera,Ogre::Camera *pdepthcamera);

		virtual Ogre::MovableObject *pickObject(Ogre::IntersectType type, const Ogre::WorldRay &ray, float *collide_dist, Ogre::uint pickflags);
		virtual bool pickGround(const Ogre::WorldRay &ray, float *collide_dist);
		virtual bool pickGround(Ogre::WPOS_T x, Ogre::WPOS_T z, Ogre::WPOS_T *y, Ogre::Vector3 *pnormal=NULL, float *water_height=NULL);

		virtual void updateFocusArea(const Ogre::WorldPos center, float radius)
		{
		}
		virtual Ogre::TerrainTile *Ogre::GameScene::getTerrainTile()
		{
			return NULL;
		}
		virtual  bool LoadTitle(const char* filename) ;

		virtual  void Release();
		virtual  bool GetHeight(float x,float z,float* height, TerrPickType type=TERR_PICK_VB);
		virtual  bool GetWaterHeight (float x,float z,float* height);

		bool GetHeightEx(float x ,float z ,float* height );
		bool GetNormal(float x, float z , Ogre::Vector3* normal);
		bool IsUnderTerrain(const Ogre::Vector3 &pos);
		bool TestTriangle(const Ogre::Ray &ray, int sx, int sz, int ex, int ez, float &t);
	public:
		void         SetAmbientLightColor(float r,float g,float b,float a);
		void         SetDirLightColor(float r,float g,float b,float a);
	public:
		bool                AddTitle(EditorTerrainTile* title);
	public:
		EditorTerrainTile*             GetTitle(int index);
		bool                TestAndRay (Ogre::Vector3 &dir , Ogre::Vector3 &pos , Ogre::Vector3 &result,int mode = 0);
		bool                InsterectRay(Ogre::Vector3 &dir , Ogre::Vector3 &pos , Ogre::Vector3 &result );

		bool				TestTerrainAndWater (Ogre::Vector3 &dir , Ogre::Vector3 &pos , Ogre::Vector3 &result,int mode=0);

		bool                GetGridX( float x , int* gx);
		bool                GetGridZ( float z , int* gz);

		bool                GetGridPos(Ogre::Vector3* pos,int gx,int gz);
		bool                SaveCurTile(const char* filename);
	public:
		typedef std::vector<EditorTerrainTile*>    TITLE_SET;
		TITLE_SET                       m_TitleSet;
	private:
		int                                 m_nCountNum;

	public:
		//增加天空模型
		Ogre::SkyModel                      *m_pSkyModel;
		Ogre::AmbientManager *m_pAmbientMgr;

		std::set<Ogre::RenderableObject *>m_RenderableObjects;
		//std::set<Ogre::EffectObject *>m_EffectObjects;

	private:
		Ogre::Vector3               CountPos(Ogre::Vector3  &dir,Ogre::Vector3  &pos,float dz,int mode);                    
		void queryShaderEnvData(Ogre::ShaderEnvData &envdata,const Ogre::BoxBound &aabb,const Ogre::Vector3 &center,const Ogre::Quaternion &rotation,Ogre::SceneRenderer* prenderer,size_t nframecount);
	};

	extern Terrain g_Terrain;
}
