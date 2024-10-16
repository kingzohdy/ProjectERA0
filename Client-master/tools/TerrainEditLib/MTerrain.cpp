#include "mterrain.h"

#include "TerrainTile.h"
#include "EditorCamera.h"

#include "mtexmanager.h"

#include "MTerrainCull.h"
#include "MTerrainEdit.h"
#include "MPlacePlayer.h"
#include "MEventProcessFactory.h"
#include "EditorModel.h"

#include "OgreLooseOctree.h"
#include "OgreCamera.h"
#include "OgreLines.h"
#include "OgreEffectObject.h"
#include "OgreAmbientRegion.h"
#include "OgreRay.h"
#include "OgreTerrainBlock.h"
#include "OgreModel.h"
#include "OgreShadowmap.h"
#include "OgrePhysicsScene.h"
#include "OgreDecal.h"
#include "OgreLiquid.h"

#include <algorithm>

using namespace TE;
using namespace Ogre;

extern float g_fCameraNear;
extern float  g_fCameraFar;

uint g_dwCurTime = 0;

MTerrain g_Terrain;

char  g_strBackSoundFileName[128];


MTerrain::MTerrain(void)
{
	m_pSkyModel = NULL;

	g_strBackSoundFileName[0] = 0;

	m_pAmbientMgr = NULL;

	

}

MTerrain::~MTerrain(void)
{
	OGRE_DELETE(m_pAmbientMgr);
}

void MTerrain::init()
{
	GameScene::init(true, true, true, true);
	m_pAmbientMgr = new AmbientManager(this);
}

void MTerrain::update(uint dtick)
{
	g_dwCurTime += dtick;
	TLiquid::SetGlobalWaterTime(g_dwCurTime);

	float dtime = TickToTime(dtick);

	for(size_t i=0; i<m_TitleSet.size(); i++)
	{
		std::vector <TerrModel*>&modelset = m_TitleSet[i]->GetTileData()->modelData.modelSet;
		for(size_t j=0; j<modelset.size(); j++)
		{
			modelset[j]->Update(dtime);
		}

		m_TitleSet[i]->update(dtick);
	}

	if(g_TerrainEdit.GetCurEditEvent() == PLACE_PLYAER)
	{
		m_pAmbientMgr->enableTopLight(true);
		MPlacePlayer *player = static_cast<MPlacePlayer *>(g_EventProcessFactory.GetEventProcess(PLACE_PLYAER));
		m_pAmbientMgr->updatecamera(g_pCamera, player->getCurPos());
	}
	else
	{
		m_pAmbientMgr->enableTopLight(false);
		m_pAmbientMgr->updatecamera(g_pCamera, g_pCamera->getWorldPosition());
	}

	m_pAmbientMgr->update(dtick);
}

void MTerrain::onAttachObject(MovableObject *pobject)
{
	pobject->addRef();
	if(IS_KIND_OF(RenderableObject, pobject))
	{
		RenderableObject *prenderable = static_cast<RenderableObject *>(pobject);
		assert(m_RenderableObjects.find(prenderable) == m_RenderableObjects.end());

		m_RenderableObjects.insert(prenderable);
	}
	else if(IS_KIND_OF(EffectObject, pobject))
	{
		EffectObject *peffect = static_cast<EffectObject *>(pobject);
		for(size_t i=0;i<m_EffectObjects.size();i++)
		{
			if(m_EffectObjects[i] == peffect)
			{
				assert(0);
				return;
			}
		}
		m_EffectObjects.push_back(peffect);
	}
	else
	{
		assert(0);
	}
}

void MTerrain::onDetachObject(MovableObject *pobject)
{
	if(IS_KIND_OF(RenderableObject, pobject))
	{
		RenderableObject *prenderable = static_cast<RenderableObject *>(pobject);
		assert(m_RenderableObjects.find(prenderable) != m_RenderableObjects.end());
		m_RenderableObjects.erase(prenderable);
	}
	else if(IS_KIND_OF(EffectObject, pobject))
	{
		EffectObject *peffect = static_cast<EffectObject *>(pobject);
		for(size_t i=0;i<m_EffectObjects.size();i++)
		{
			if(m_EffectObjects[i] == peffect)
			{
				m_EffectObjects.erase(std::remove(m_EffectObjects.begin(), m_EffectObjects.end(), peffect), m_EffectObjects.end());
			}
		}
	}
	else
	{
		assert(0);
	}

	pobject->release();
}

void MTerrain::onObjectPosChange(MovableObject *pobject)
{
}

struct MyEffect
{
	Ogre::EffectObject *pobj;
	float weight;
};

inline bool operator<(const MyEffect &e1, const MyEffect &e2)
{
	return e1.weight > e2.weight;
}
//void MTerrain::queryShaderEnvData(ShaderEnvData &envdata, const Vector3 &center, float radius, SceneRenderer* prenderer,size_t nframecount)
//这个地方还有问题
void MTerrain::queryShaderEnvData(ShaderEnvData &envdata,const MAABBox &aabb,const Vector3 &center,
								  const Ogre::Quaternion &rotation, SceneRenderer* prenderer,size_t nframecount)
{
	memset(&envdata.flags, 0, sizeof(envdata.flags));
	envdata.ambient = ColourValue(0, 0, 0, 0);

	static std::vector<MyEffect>effects;
	effects.resize(0);

	std::vector<Ogre::EffectObject *>::iterator iter = m_EffectObjects.begin();
	for(; iter!=m_EffectObjects.end(); iter++)
	{
		EffectObject *peffect = *iter;
		if(!peffect->isShow()) continue;

		float w = peffect->getEffectWeight(aabb.getCenter(), aabb.getExtension().length());
		if(w > 0)
		{
			MyEffect tmp;
			tmp.pobj = peffect;
			tmp.weight = w;
			effects.push_back(tmp);
		}
	}

	Ogre::Matrix4 wtm;
	wtm.makeTranslateMatrix(center);

	Ogre::Matrix4 wrm;
	rotation.getMatrix(wrm);

	wtm = wrm * wtm;

	std::sort(effects.begin(), effects.end());
	for(size_t i=0; i<effects.size(); i++)
	{
		effects[i].pobj->queryShaderEnv(envdata, wtm);
		effects[i].pobj->prepare(prenderer,nframecount);
	}
}

static int s_RenderRecursive = 0;

void MTerrain::caculateShadowCamera(Ogre::Camera *pcamera,Ogre::Camera *pdepthcamera)
{
	bool ptlight = false;
	Vector3 ptpos;
	for(size_t i=0;i<m_EffectObjects.size();i++)
	{
		EffectObject* pobj = m_EffectObjects[i];
		if( IS_KIND_OF(Light,pobj) && static_cast<Light*>(pobj)->m_DoShadow)
		{
			if(static_cast<Light*>(pobj)->m_Type == LT_DIRECT)
			{

				Vector3 dir(0.0f,0.0f,1.0f);
				pobj->getRotation().rotate(m_pShadowMap->m_vLightDir,dir);								
			}
			else
			{
				m_pShadowMap->m_vLightDir = Vector3(0, -1.0f, 0);
				ptpos = pobj->getWorldPosition();
				ptlight = true;
			}
			break;
		}
	}

	Vector3 vLightDir = m_pShadowMap->m_vLightDir;
	Normalize(vLightDir);

	Matrix4 mView;
	Matrix4 mCameraView = pcamera->getViewMatrix();
	Matrix4 mCameraProj = pcamera->getProjectMatrix();
	mCameraView.inverse(mView);

	Vector3 eye	=	Vector3(mView._41,mView._42,mView._43);
	Vector3 dir	=	Vector3(mCameraView._13,mCameraView._23,mCameraView._33);

	float dis;
	WorldRay ray;
	ray.m_Origin = WorldPos(eye);
	ray.m_Dir	 = dir;
	pickGround(ray,&dis);

	Vector3 centerpos = eye + dir*dis;
	Vector3 viewcenterpos;
	mCameraView.apply4x4(viewcenterpos,centerpos );

	float nearlinearz = viewcenterpos.z - dis/2.0f;	//近处5米
	float farlinearz  = viewcenterpos.z + dis;  //
	float nearz		  = (nearlinearz*mCameraProj._33 + mCameraProj._43)/nearlinearz;
	float farz		  = (farlinearz*mCameraProj._33 + mCameraProj._43)/farlinearz;

	Matrix4 mVP		=	 mCameraView*mCameraProj;
	Matrix4 mIVP	=	 mVP;
	mIVP.inverse();

	Vector3 v[8];

	v[0] = Vector3(-1.0f, -1.0f, 0.0f);
	v[1] = Vector3( 1.0f, -1.0f, 0.0f);
	v[2] = Vector3(-1.0f,  1.0f, 0.0f);
	v[3] = Vector3( 1.0f,  1.0f, 0.0f);

	v[4] = Vector3(-1.0f, -1.0f, farz);
	v[5] = Vector3( 1.0f, -1.0f, farz);
	v[6] = Vector3(-1.0f,  1.0f, farz);
	v[7] = Vector3( 1.0f,  1.0f, farz);

	for(size_t i=0;i<8;i++)
	{
		mIVP.apply4x4(v[i],v[i]);
	}

	Vector3 vLightpos	 =  centerpos - vLightDir * 10000.0f;
	Vector3 vNormal		 =  Vector3(mView._11,0.0f,mView._13);
	Vector3	vLightUp	 =	CrossProduct(vNormal,vLightDir);

	Normalize(vLightUp);

	Matrix4 mLightView;
	Matrix4 mLightViewInv;

	mLightView.makeViewMatrix(vLightpos,eye,vLightUp);

	mLightViewInv = mLightView;
	mLightViewInv.inverse();

	for(size_t i=0;i<8;i++)
	{
		mLightView.apply4x4(v[i],v[i]);
	}

	Vector3 vMin = v[0];
	Vector3 vMax = v[0];

	for(size_t i=1;i<8;i++)
	{
		vMin = Vector3(Min(v[i].x,vMin.x),Min(v[i].y,vMin.y),Min(v[i].z,vMin.z));
		vMax = Vector3(Max(v[i].x,vMax.x),Max(v[i].y,vMax.y),Max(v[i].z,vMax.z));
	}

	size_t width	=	size_t(vMax.x - vMin.x);
	size_t Height	=	size_t(vMax.y - vMin.y);
	Vector3 vCenter = (vMin + vMax) * 0.5f;

	mLightViewInv.apply4x4(vCenter,vCenter);
	vLightpos = vCenter - vLightDir * 10000.0f;

	pdepthcamera->setLookDirect(vLightpos,vLightDir,vLightUp);
	pdepthcamera->setDepth(10.0f, 60000.0f);
	pdepthcamera->setRatio( width/float(Height) );
	if(ptlight)
	{
		pdepthcamera->setFov(150.0f);
		//pdepthcamera->setLookDirect(ptpos+Vector3(0,500.0f, 0), vLightDir, vLightUp);
		pdepthcamera->setLookDirect(ptpos, vLightDir, vLightUp);
	}
	else
	{
		pdepthcamera->setOrtho((float)width);
	}

	//临时性的代码，这个相机类还有些问题需要处理。
	pdepthcamera->update(0.0f);

	Matrix4 matV	=	pdepthcamera->getViewMatrix();	
	Matrix4 matP	=	pdepthcamera->getProjectMatrix();

	Matrix4 mDepth;
	mDepth.identity();
	mDepth._11 =  0.5f;
	mDepth._22 = -0.5f;
	mDepth._41 =  0.5f;
	mDepth._42 =  0.5f;
	m_pShadowMap->m_ProjMatrix		=	matV * matP * mDepth;
}

void MTerrain::onCull(Camera *pcamera)
{
	s_RenderRecursive++;
	MTerrainCull terrcull;
	terrcull.Cull(this);
	pcamera->m_pCullresult->startCull(pcamera);

	size_t flags = 0;
	for(size_t i=0; i<terrcull.m_ShowChunkNum; i++)
	{
		MChunk *pchunk = terrcull.m_ShowChunk[i];
		if(g_TerrainEdit.m_DrawOption.bDrawChunk)
			pcamera->m_pCullresult->addRenderable(this,pchunk,0,NULL);
		
		if(g_TerrainEdit.m_DrawOption.bDrawQuad)
		{
			MPath *ppath = pchunk->GetQuadLine();
			ppath->Render();
		}
	}

	std::set<Ogre::RenderableObject *>::iterator iter = m_RenderableObjects.begin();
	for(; iter!=m_RenderableObjects.end(); iter++)
	{
		RenderableObject *prenderable = *iter;
		if(!prenderable->isShow()) continue;

		const Ogre::BoxSphereBound &wBSB = prenderable->getWorldBounds();

		CullFrustum::CULLRET_T ret = pcamera->m_pCullresult->m_Frustum.cull(wBSB);
		if(ret == CullFrustum::CULL_OUT)
			continue;
		pcamera->m_pCullresult->addRenderable(this,prenderable,0,NULL);
	}

	for(unsigned int i=0; i<terrcull.m_ShowModelNum; i++)
	{
		terrcull.m_ShowModel[i]->onCull();
	}

	pcamera->m_pCullresult->addRenderable(this, g_TerrainEdit.m_pLinesRenderer, 0, NULL);
}

void MTerrain::onRender(Ogre::SceneRenderer *prenderer)
{
	assert(0);	
	/*
	s_RenderRecursive++;
	MTerrainCull terrcull;
	terrcull.Cull(this);

	ShaderEnvData envdata;

	Camera *pcamera = prenderer->getCamera();

	//增加一个模型的裁剪
	Ogre::CullResult result;
	result.startCull(pcamera);

	envdata.view = pcamera->getViewMatrix();
	envdata.proj = pcamera->getProjectMatrix();
	envdata.viewproj = envdata.view * envdata.proj;
	envdata.eyepos = pcamera->getEyeLoc().toVector3();


	size_t i;
	for(i=0; i<terrcull.m_ShowChunkNum; i++)
	{
		MChunk *pchunk = terrcull.m_ShowChunk[i];
		
		
		if(static_cast<NormalSceneRenderer*>(prenderer)->m_RenderUsage != RU_SHADOWMAP)
		{
			Matrix4 identity;
			identity.identity();
			queryShaderEnvData(envdata, pchunk->m_AABBox,Vector3(0.0f,0.0f,0.0f),Quaternion(0.0f,0.0f,0.0f,1.0f),prenderer,prenderer->m_NumRenderFrame);

		}
		envdata.flags.renderusage = static_cast<NormalSceneRenderer *>(prenderer)->m_RenderUsage;
		if(static_cast<NormalSceneRenderer*>(prenderer)->m_RenderUsage != RU_SHADOWMAP)
		{
			if(g_TerrainEdit.m_DrawOption.bDrawChunk) pchunk->render(prenderer, envdata);
		}

		if(s_RenderRecursive==1 && g_TerrainEdit.m_DrawOption.bDrawQuad) //first renderer
		{
			MPath *ppath = pchunk->GetQuadLine();
			ppath->Render();
		}
	}

	std::set<Ogre::RenderableObject *>::iterator iter = m_RenderableObjects.begin();
	for(; iter!=m_RenderableObjects.end(); iter++)
	{
		RenderableObject *prenderable = *iter;

		Ogre::BoxSphereBound wBSB;
		prenderable->getWorldBounds(wBSB);
		CullFrustum::CULLRET_T ret = result.m_Frustum.cull(wBSB);
		if(ret == CullFrustum::CULL_OUT)
			continue;
		envdata.flags.renderusage = static_cast<NormalSceneRenderer *>(prenderer)->m_RenderUsage;
		if(static_cast<NormalSceneRenderer*>(prenderer)->m_RenderUsage != RU_SHADOWMAP)
		{
			Matrix4 mworld = prenderable->getWorldMatrix();
			Quaternion rotation;
			rotation.setMatrix(mworld);

			queryShaderEnvData(envdata,wBSB.getBox() , prenderable->getWorldPosition(),rotation, prenderer,prenderer->m_NumRenderFrame);
			Matrix4 identity;
			identity.identity();
		}
		envdata.flags.renderusage = static_cast<NormalSceneRenderer *>(prenderer)->m_RenderUsage;
		prenderable->render(prenderer, envdata);
	}

	for(i=0; i<terrcull.m_ShowModelNum; i++)
	{
		terrcull.m_ShowModel[i]->onCull();
	}

	if(s_RenderRecursive == 1)
	{
		g_TerrainEdit.m_pLinesRenderer->render(prenderer, envdata);
	}

	s_RenderRecursive--;
	*/
}

extern bool   TestIfLinkMesh(const char* modelName );

MovableObject *MTerrain::pickObject(IntersectType type, const WorldRay &ray, float *collide_dist, uint pickflags)
{
	Ray rray;
	ray.getRelativeRay(rray, Vector3(0,0,0));

	for(size_t k=0; k<m_TitleSet.size(); k++)
	{		
		MTitle *pTile = m_TitleSet[k];

		for(unsigned int i=0; i<pTile->GetTileData()->modelData.modelSet.size(); i++)
		{	
			TerrModel* pModel = pTile->GetTileData()->modelData.modelSet[i];
			TerrModelData *pmodel = pModel->GetModelData();
			if(type == INTERSECT_LIGHT)
			{
				if(pmodel->cType>TYPE_PLANT_MODEL && pmodel->cType!=TYPE_LINK_MODEL)
					continue;
			}
			else
			{
				if(pmodel->cType>TYPE_PLANT_MODEL)
					continue;
				if(g_TerrainEdit.m_bLockModelEdit[pmodel->cType])//如果该类型被锁，则不被选中
					continue;

				//地形的拼接，不被选中
				if( TestIfLinkMesh(pmodel->name) )
					continue;
			}

			if(pmodel->pModel->GetShowModel()->intersectRay(type, rray, collide_dist))
			{
				return pmodel->pModel->GetShowModel();
			}

		}
	}
	return NULL;
}

bool MTerrain::pickGround(const WorldRay &worldray, float *collide_dist)
{
	Vector3 pos = worldray.m_Origin.toVector3();
	Vector3 dir = worldray.m_Dir;
	if(IsUnderTerrain(pos))
	{
		if(collide_dist) *collide_dist = 0;
		return true;
	}

	Vector3 lastpos = pos;
	float len = 0.0f;
	float step = TERRUNIT_SIZE*0.7f;
	Ray ray(pos, dir);

	while(len < TERRTILE_SIZE)
	{
		len += step;
		Vector3 curpos = pos + len*dir;

		if(IsUnderTerrain(curpos))
		{
			int gx1 = int(lastpos.x/TERRUNIT_SIZE);
			int gz1 = int(lastpos.z/TERRUNIT_SIZE);
			int gx2 = int(curpos.x/TERRUNIT_SIZE);
			int gz2 = int(curpos.z/TERRUNIT_SIZE);
			if(gx1 > gx2) Swap(gx1, gx2);
			if(gz1 > gz2) Swap(gz1, gz2);

			if(gx1 < 0) gx1 = 0;
			if(gz1 < 0) gz1 = 0;
			if(gx2 > 128) gx2 = 128;
			if(gz2 > 128) gz2 = 128;

			float t;
			if(TestTriangle(ray, gx1, gz1, gx2, gz2, t))
			{
				if(collide_dist) *collide_dist = t;
				return true;
			}
		}
		else
		{
			lastpos = curpos;
		}
	}
	return false;
}

bool MTerrain::pickGround(WPOS_T x, WPOS_T z, WPOS_T *y)
{
	assert(0);
	return false;
}

bool   MTerrain::GetGridPos(MVec3* pos,int gx,int gz)
{
    if( gx < 0 || gx >= TERRTILE_NUMBLOCK)
		return false;

	if( gz < 0 || gz >= TERRTILE_NUMBLOCK)
		return false;


	MTileData* pTile = m_TitleSet[0]->GetTileData();

	
	*pos = pTile->chunkSet [gz][gx].GetChunkNewVecData()->vVer[0].pos  ;

  return true;
};

MVec3  MTerrain::CountPos(MVec3  &dir,MVec3  &pos,float dz,int mode)
{
	MVec3  pos0 = pos + dir*dz*(float)mode;

	float hh ;	
	if(!GetHeight(pos0.x ,pos0.z,&hh))
	{   
		return pos0;
	}
	
	m_nCountNum++;
	if( m_nCountNum >=50)
		return pos0;

	float ddz = pos0.y - hh;
	
    if(ddz > 0 )
	{
		if(mode<0)
		{
			if(ddz < 0.02f)
				return pos0;

			CountPos(dir,pos0,dz/2,1);
		}
		else
		{
			if(ddz < 0.02f)
				return pos0;
			CountPos(dir,pos0,dz/2,1);
		}
	}
	else
	{
		if(mode > 0 )
		{
			if(ddz > -0.02f)
				return pos0;
			CountPos(dir,pos0,dz/2,-1);
		}
		else
		{
			if(ddz > -0.02f)
				return pos0;
			CountPos(dir,pos0,dz/2,-1);
		}
	}

	return pos0;
}	
		        
bool MTerrain::InsterectRay(MVec3 &dir , MVec3 &pos , MVec3 &result)
{
	WorldRay wray;
	wray.m_Origin = pos;
	wray.m_Dir = dir;
	wray.m_Range = 100000.0f;

	float dist;
	if(pickGround(wray, &dist))
	{
						
	}
	return false;
};

#define  TEST_RAY_NUM  300

bool MTerrain::IsUnderTerrain(const MVec3 &pos)
{
	float h;
	if(!GetHeight(pos.x, pos.z, &h))
	{
		return true;
	}

	if(h >= pos.y) return true;
	return false;
}

bool MTerrain::TestTriangle(const Ogre::Ray &ray, int sx, int sz, int ex, int ez, float &t)
{
	t = MAX_FLOAT;

	Vector3 pt[4];
	//v1------v2
	//v0------v3
	for(int z=sz; z<=ez; z++)
	{
		pt[0].z = pt[3].z = z*TERRUNIT_SIZE;
		pt[1].z = pt[2].z = (z+1)*TERRUNIT_SIZE;
		for(int x=sx; x<=ex; x++)
		{
			pt[0].x = pt[1].x = x*TERRUNIT_SIZE;
			pt[2].x = pt[3].x = (x+1)*TERRUNIT_SIZE;

			for(int i=0; i<4; i++)
			{
				GetHeight(pt[i].x, pt[i].z, &pt[i].y);
			}

			float tmp;
			if(ray.intersectTriangle(pt[0], pt[1], pt[2], &tmp))
			{
				if(tmp < t) t = tmp;
			}
			if(ray.intersectTriangle(pt[0], pt[2], pt[3], &tmp))
			{
				if(tmp < t) t = tmp;
			}
		}
	}

	if(t == MAX_FLOAT) return false;
	else return true;
}

bool  MTerrain::TestAndRay(MVec3 &dir , MVec3 &pos , MVec3 &result,int mode)
{
	if(IsUnderTerrain(pos))
	{
		result = pos;
		return true;
	}

	MVec3 lastpos = pos;
	float len = 0.0f;
	float step = TERRUNIT_SIZE*0.7f;
	Ray ray(pos, dir);

	while(len < TERRTILE_SIZE)
	{
		len += step;
		MVec3 curpos = pos + len*dir;

		if(IsUnderTerrain(curpos))
		{
			int gx1 = int(lastpos.x/TERRUNIT_SIZE);
			int gz1 = int(lastpos.z/TERRUNIT_SIZE);
			int gx2 = int(curpos.x/TERRUNIT_SIZE);
			int gz2 = int(curpos.z/TERRUNIT_SIZE);
			if(gx1 > gx2) Swap(gx1, gx2);
			if(gz1 > gz2) Swap(gz1, gz2);

			if(gx1 < 0) gx1 = 0;
			if(gz1 < 0) gz1 = 0;
			if(gx2 > 128) gx2 = 128;
			if(gz2 > 128) gz2 = 128;

			float t;
			if(TestTriangle(ray, gx1, gz1, gx2, gz2, t))
			{
				result = pos + dir*t;
				return true;
			}
		}
		else
		{
			lastpos = curpos;
		}
	}
	return false;
}

bool MTerrain::LoadTitle(const char* filename) 
{	
	Release();

	MTitle*  title = new MTitle;

	m_TitleSet.push_back(title);	
	if(title->LoadFile (filename))
	{	
		FLOAT fAspect = ((FLOAT)800) / 600;
		MVec3 vFromPt   = MVec3( 0.0f, 0.0f, -5.0f );
		MVec3 vLookatPt = MVec3( 0.0f, 0.0f, 0.0f ); 
		MVec3 vUpVec    = MVec3( 0.0f, 1.0f, 0.0f );
			
		MVec3 pos;
		if(!GetGridPos(&pos, title->GetTileData()->gw/2, title->GetTileData()->gh/2))
			return false;
		pos.y += 50;
		g_Camera.reset();
		g_Camera.SetPosition(pos.x ,pos.y ,pos.z );
		return true;
	}
	else
	{
		OGRE_DELETE(title);
		return false;
	}

		
};

bool   MTerrain::GetGridX( float posx , int* gx)
{
	for(unsigned int i = 0 ; i < m_TitleSet.size () ; i++)
	{
		MTileData *ptiledata = ((MTitle*)m_TitleSet[i])->GetTitleData();
		if(posx<ptiledata->box.minpos.x || posx>ptiledata->box.maxpos.x)
			continue;
		 
		*gx =int((posx - ptiledata->box.minpos.x)  / TERRBLOCK_SIZE +0.002);
		if(*gx < 0) *gx = 0;
		if(*gx >= ptiledata->gw) *gx = ptiledata->gw - 1;
		return true;  
	}
	return false;
};

bool   MTerrain::AddTitle(MTitle* title)
{
   if(title == NULL)
	   return false;
		
   m_TitleSet.push_back(title);
   
	return true;

};

bool   MTerrain::GetGridZ( float posz , int* gz)
{
	for(unsigned int i = 0 ; i < m_TitleSet.size () ; i++)
	{
		MTileData *ptiledata = ((MTitle*)m_TitleSet[i])->GetTitleData();
		if(posz<ptiledata->box.minpos.z || posz>ptiledata->box.maxpos.z) 
			continue;
		 
		*gz =int((posz - ptiledata->box.minpos.z )  / TERRBLOCK_SIZE +0.002);
		if(*gz < 0) *gz = 0;
		if(*gz >= ptiledata->gh) *gz = ptiledata->gh - 1;
		return true;  
	}

	return false;
};

MTitle*  MTerrain::GetTitle(int index)
{
    if(index <0 || index >= (int)m_TitleSet.size () )
		return NULL;

	return m_TitleSet[index];
};

void MTerrain::Release()
{	
	g_TexManager.Release();

	for(unsigned int i = 0 ; i < m_TitleSet.size () ; i++)
	{
		OGRE_DELETE( m_TitleSet[i]) ;
	}
	m_TitleSet.clear ();

	//OGRE_RELEASE( m_pSkyModel );

};
bool    MTerrain::SaveCurTile(const char* filename)
{
    if(m_TitleSet.empty ())
		return false;
   
	((MTitle*)m_TitleSet[0])->SaveFile ( filename );
    return true;
}

			 
bool MTerrain::GetNormal(float x, float z , MVec3* normal)
{
	(*normal) = MVec3(0,1,0);

    for(unsigned int i = 0 ; i < m_TitleSet.size() ; i++)
	{
		if(m_TitleSet[i]->GetNormal (x,z,normal))
		{
			return true;
		}
	}

	return false;
};

bool MTerrain::GetHeightEx(float x,float z,float* height)
{
	(*height) = 0;
    for(unsigned int i = 0 ; i < m_TitleSet.size() ; i++)
	{
		if(m_TitleSet[i]->GetHeightEx (x,z,height))
		{
			return true;
		}
	}

	return false;
}

bool MTerrain::GetHeight(float x,float z,float* height, TerrPickType type)
{
	(*height) = 0;

	float maxh = -MAX_FLOAT;
	float h;
    for(unsigned int i = 0 ; i < m_TitleSet.size() ; i++)
	{
		bool use_vb = type!=TERR_PICK_HEIGHTMAP;
		if(m_TitleSet[i]->GetHeight(x,z,&h,use_vb) && h>maxh)
		{
			maxh = h;
		}
	}

	if(type==TERR_PICK_VB_BSP)
	{
		Ray ray(Vector3(x, 100000.0f, z), Vector3(0, -1.0f, 0));
		float dist;
		if(getPhysicsScene()->pick(ray, &dist))
		{
			Vector3 pos = ray.m_Origin + ray.m_Dir*dist;
			if(pos.y > maxh) maxh = pos.y;
		}
	}

	if(maxh > -MAX_FLOAT)
	{
		*height = maxh;
		return true;
	}
	else return false;
};
