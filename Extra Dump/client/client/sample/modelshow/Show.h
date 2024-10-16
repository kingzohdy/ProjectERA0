#pragma once


#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreRenderWindow.h"
#include "OgreTexture.h"
#include "OgreCamera.h"
#include "OgreSceneRenderer.h"
//#include "OgreHardwareBuffer.h"
#include "OgreGroundGrid.h"
#include "OgreGameScene.h"
#include "OgreMesh.h"
#include "OgreModel.h"
#include "OgreModelData.h"
#include "OgreResourceManager.h"
#include "OgreLight.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
#include "OgreRenderSystem.h"
#include "OgreEntity.h"
#include "OgreUIRenderer.h"
#include "OgreGameTerrainScene.h"
//#include "OgreTerrainData.h"
#include "OgreEntityMotionData.h"
#include "OgreEntityMotion.h"
//#include "ogrereflect.h"
#include <vector>
#include <Windows.h>

#pragma comment(lib, "winmm.lib")

using namespace Ogre;

using namespace std;

struct MOTIONINFO 
{
	uint ID;
	char name[100];
};
class CShow
{
public:
	
	CShow(void);
	~CShow(void);
	void InitRenderSys(HWND hwnd);
	void CreateData(char* fn);
	void UpdateData(uint dtick);
	void DoFrame();
	void LoadModel(const char* filename);
	void LoadBKModel(const char* filename);
	void SetAnimID(int animid);
	void SetIsChangeAnim(bool b){ m_isChangeAnim = b;}
	void SetModelSize(float size) { m_modelsize = size;}
	float GetModelSize() { return m_modelsize; }
	void SetOffsetX(float x) { m_rx += x;}
	void SetOffsetY(float y) { m_ry += y;}
	void Destory();
	void NextAnima();
	void PreAnima();
	void SetEyePos(Vector3 v);
	void SetLookAt(Vector3 v);


	
private:
	Root				*m_pRoot;
	Camera				*m_pCamera;			
	GameScene			*m_pScene;
	Light				*m_pLight;
	float				m_modelsize;
	NormalSceneRenderer *m_pScenerenderer;
	ModelData       	*m_pModeldate;
	float				m_rx;
	float				m_ry;
	bool				m_isLBDown;
	Vector3				m_eyepos,m_lookat;
	int					m_AnimID;
	bool				m_isChangeAnim;
	char				*m_OldPath;
	int					m_AnimaNum;
	Entity				*m_pBKModel;

	Entity				*m_pEntity;
	EntityData			*m_pEntityData;

	GroundGrid			*m_pGrid;
};
