
#ifndef __GameScene_H__
#define __GameScene_H__

#include "OgreCullFrustum.h"
#include "OgreBaseObject.h"
#include "OgreColourValue.h"


namespace Ogre
{


	class ReflectEffect;
	class Shadowmap;
	class BloomEffect;
	class DistortEffect;
	class Shadowcubemap;
	class PhysicsScene;
	class PhysicsScene2;
	struct RenderPassDesc;
	class SoundNode;

	class _OgreExport CullResult
	{
	public:
		CullResult();
		~CullResult();

		void startCull(Camera *pcamera);
		void addRenderable(GameScene *pscene, RenderableObject *prenderable, int layer, void *puserdata);
		void getRenderPassRequired(RenderPassDesc &desc);

	public:
		Camera* m_pCamera;
		CullFrustum m_Frustum;

		struct Record
		{
			Record ()
			{
				pscene = NULL;
				prenderable = NULL;
				puserdata = NULL;
				layer = -1;
			}
			GameScene *pscene;
			RenderableObject *prenderable;
			void *puserdata;
			int layer;
		};
		std::vector<Record>m_Results;
	};


	struct RenderableEffectInfo
	{
		EffectObject *pobj;
		float weight;
	};
	inline bool operator<(const RenderableEffectInfo &obj1, const RenderableEffectInfo &obj2)
	{
		return obj1.weight>obj2.weight;
	}

	class _OgreExport GameScene : public BaseObject
	{
		DECLARE_RTTI_VIRTUAL(GameScene)

	public:
		virtual void update(uint dtick) = 0;

		virtual void onAttachObject(MovableObject *pobject) = 0;
		virtual void onDetachObject(MovableObject *pobject) = 0;
		virtual void onObjectPosChange(MovableObject *pobject) = 0;
		
		virtual void onRender(SceneRenderer *prenderer) = 0;	//这个函数要被取消掉
		
		virtual TerrainTile * getTerrainTile() = 0;
		virtual void getEffectObjects(std::vector<RenderableEffectInfo>&objs, RenderableObject *ptarget);

		void PreloadSound();

		//裁剪
		virtual void onCull(Camera *pcamera){}

		virtual void	caculateShadowCamera(Camera *pcamera,Camera *pdepthcamera);
		void	caculateReflectCamera(Camera *pcamera,Plane &reflectplane,Camera *preflectcamera,Plane& clipplane);

		virtual MovableObject *pickObject(IntersectType type, const WorldRay &ray, float *collide_dist, uint pickflags) = 0;
		virtual bool pickGround(const WorldRay &ray, float *collide_dist) = 0;
		virtual bool pickGround(WPOS_T x, WPOS_T z, WPOS_T *y, Vector3 *pnormal=NULL, float *water_height=NULL) = 0;

		virtual void updateFocusArea(const WorldPos center, float radius) = 0;
		virtual ReflectEffect* getReflecteffect(){return NULL;};

		bool pickGround(float x, float z, float *y)
		{
			WPOS_T wy;
			bool b = pickGround(WorldPos::Flt2Fix(x), WorldPos::Flt2Fix(z), &wy);
			if(y) *y = WorldPos::Fix2Flt(wy);

			return b;
		}

		void setViewpoint(const WorldPos &vp)
		{
			m_Viewpoint = vp;
		}

		const WorldPos &getViewpoint()
		{
			return m_Viewpoint;
		}

		PhysicsScene *getPhysicsScene()
		{
			return m_pPhysicsScene;
		}

		PhysicsScene2 *getPhysicsScene2()
		{
			return m_pPhysicsScene2;
		}
		
		std::vector<EffectObject *>m_EffectObjects;
		std::vector<SoundNode*> mSoundNodes;

		bool m_RenderDummyNode;

	protected:
		GameScene();
		~GameScene();

		PhysicsScene *m_pPhysicsScene;
		PhysicsScene2 *m_pPhysicsScene2;
		WorldPos m_Viewpoint;
	};

	class _OgreExport SimpleGameScene : public GameScene
	{
		DECLARE_RTTI(SimpleGameScene)

	public:
		SimpleGameScene();

		virtual void update(uint dtick);

		virtual void onAttachObject(MovableObject *pobject);
		virtual void onDetachObject(MovableObject *pobject);
		virtual void onObjectPosChange(MovableObject *pobject);
		virtual void onRender(SceneRenderer *prenderer);

		//裁剪
		virtual void onCull(Camera *pcamera);	

		virtual MovableObject *pickObject(IntersectType type, const WorldRay &ray, float *collide_dist, uint pickflags);
		virtual bool pickGround(const WorldRay &ray, float *collide_dist);
		virtual bool pickGround(WPOS_T x, WPOS_T z, WPOS_T *y, Vector3 *pnormal=NULL, float *water_height=NULL);

		virtual void updateFocusArea(const WorldPos center, float radius)
		{
		}
		virtual TerrainTile * getTerrainTile() { return NULL;}

	protected:
		~SimpleGameScene();

		std::vector<RenderableObject *>m_RenderObjects;
		
		std::vector<Light *>m_PointLights;
		Light*		m_GlobalDirlight[2];
		FogEffect*	m_GlobalEffect;
	};
}

#endif