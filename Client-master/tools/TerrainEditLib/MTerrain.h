
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
	class MTerrain : public Ogre::GameScene
	{
	public:
		MTerrain(void);
		~MTerrain(void);

		virtual void init();

		virtual void update(Ogre::uint dtick);

		virtual void onAttachObject(Ogre::MovableObject *pobject);
		virtual void onDetachObject(Ogre::MovableObject *pobject);
		virtual void onObjectPosChange(Ogre::MovableObject *pobject);
		virtual void onRender(Ogre::SceneRenderer *prenderer);
		virtual void onCull(Ogre::Camera *pcamera);
		virtual void caculateShadowCamera(Ogre::Camera *pcamera,Ogre::Camera *pdepthcamera);

		virtual Ogre::MovableObject *pickObject(Ogre::IntersectType type, const Ogre::WorldRay &ray, float *collide_dist, Ogre::uint pickflags);
		virtual bool pickGround(const Ogre::WorldRay &ray, float *collide_dist);
		virtual bool pickGround(Ogre::WPOS_T x, Ogre::WPOS_T z, Ogre::WPOS_T *y);

		virtual void updateFocusArea(const Ogre::WorldPos center, float radius)
		{
		}

		virtual  bool LoadTitle(const char* filename) ;

		virtual  void Release();
		virtual  bool GetHeight(float x,float z,float* height, TerrPickType type=TERR_PICK_VB);

		bool GetHeightEx(float x ,float z ,float* height );
		bool GetNormal(float x, float z , MVec3* normal);
		bool IsUnderTerrain(const MVec3 &pos);
		bool TestTriangle(const Ogre::Ray &ray, int sx, int sz, int ex, int ez, float &t);
	public:
		void         SetAmbientLightColor(float r,float g,float b,float a);
		void         SetDirLightColor(float r,float g,float b,float a);
	public:
		bool                AddTitle(MTitle* title);
	public:
		MTitle*             GetTitle(int index);
		bool                TestAndRay(MVec3 &dir , MVec3 &pos , MVec3 &result,int mode = 0);
		bool                InsterectRay(MVec3 &dir , MVec3 &pos , MVec3 &result );

		bool                GetGridX( float x , int* gx);
		bool                GetGridZ( float z , int* gz);

		bool                GetGridPos(MVec3* pos,int gx,int gz);
		bool                SaveCurTile(const char* filename);
	public:
		typedef std::vector<MTitle*>    TITLE_SET;
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
		MVec3               CountPos(MVec3  &dir,MVec3  &pos,float dz,int mode);                    
		void queryShaderEnvData(Ogre::ShaderEnvData &envdata,const Ogre::BoxBound &aabb,const MVec3 &center,const Ogre::Quaternion &rotation,Ogre::SceneRenderer* prenderer,size_t nframecount);
	};

};

extern TE::MTerrain g_Terrain;