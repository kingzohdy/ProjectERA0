#include "StdAfx.h"
#include "Show.h"

UIRenderer		*g_pUIRenderer = NULL;
HUIRES			g_hres = NULL;
//vector<SequenceDesc>  seq_array;
vector<ACTION_INFO>   actionlist;

vector<MOTIONINFO>	g_AnimaID;
int				m_CurIndex = 0;

CShow::CShow(void)
{
	m_pRoot = NULL;
	m_pCamera = NULL;
	m_pLight = NULL;
	m_pScenerenderer = NULL;
	m_pScene = NULL;

	m_modelsize = 1.0F;
	m_rx = m_ry = 0.0F;
	m_eyepos = Vector3(0,1000,-500);
	m_lookat = Vector3(0,150,0);
	m_isChangeAnim = false;

	m_pBKModel = NULL;

	m_AnimaNum = 0;

	m_pEntity = NULL;
	m_pEntityData = NULL;
	m_pGrid = NULL;

}

CShow::~CShow(void)
{
	Destory();
}

static void RenderUI()
{
	if(g_pUIRenderer == NULL)
		return;
	/*g_pUIRenderer->BeginDraw(hres);
	g_pUIRenderer->DrawRect(0,0,100,100,0xFFFFFFFF);
	g_pUIRenderer->EndDraw();
	g_pUIRenderer->DrawBox(0,0,100,100,0xFF00FF00);*/

	if (g_AnimaID.size() < 1)
	{
		return;
	}
	static Ogre::HFONT hfont = g_pUIRenderer->CreateTrueTypeFont(16, 16, "uires\\fonts\\FZHTJW.TTF", CCODING_GBK);
	g_pUIRenderer->renderText(hfont, 0, "上下键切换模型动作", 0, 0, 0xFFFF0000);
	g_pUIRenderer->renderText(hfont, 0, "左右键切换视角", 0, 20, 0xFFFF0000);

	DWORD color;
	for (int i=0; i<g_AnimaID.size(); i++)
	{
		char buf[100];

		sprintf(buf,"ID: %d   类型: %s",g_AnimaID[i].ID,g_AnimaID[i].name);

		if (g_AnimaID[i].ID == g_AnimaID[m_CurIndex].ID)
			color = 0xFFFF0000;
		else
			color = 0xFFFFFFFF;
		g_pUIRenderer->renderText(hfont, 0, buf, 0, 20*i+80, color);
	}
	
}

void CShow::InitRenderSys(HWND hwnd)
{
	m_pRoot = new Root();
	m_pRoot->initRenderSystem(hwnd);
	//m_pRoot->getSingleton().initRenderSystem(hwnd);
}

void CShow::LoadModel(const char* filename)
{
	if (m_pEntity)
	{
		m_pEntity->detachFromScene();
		m_pEntity->release();
		m_pEntity = NULL;
	}

	actionlist.clear();
	g_AnimaID.clear();
	m_modelsize = 1.0F;
	m_rx = m_ry = 0.0F;

	m_pEntity = new Entity;
	m_pEntity->load(filename,true);
	//m_pEntity->setPosition(WorldPos(0,0,0));
	m_pEntity->getActionList(actionlist);

	/*
	for (int i=0; i<actionlist.size(); i++)
	{
		if (actionlist[i].type == AT_MOTION)
		{
			m_pEntity->playMotion(actionlist[i].name);
			break;
		}
	}*/
	
	MOTIONINFO info;
	for (int i=0; i<actionlist.size(); i++)
	{
		if (actionlist[i].type == AT_BONE_ANIM)
		{
			info.ID = Entity::m_pAnimMap->getIDByName(actionlist[i].name);
			strcpy(info.name, actionlist[i].name);
			g_AnimaID.push_back(info);
		}
	}
	m_AnimaNum = g_AnimaID.size();
	if(!g_AnimaID.empty())
		m_pEntity->playAnim(g_AnimaID[0].ID);
	m_pEntity->attachToScene(m_pScene);

}


void CShow::CreateData(char *fn)
{
	AnimMap *panimmap = new AnimMap;
	panimmap->loadAnimNamesFromFile("db\\client\\AnimMap.csv");
	Entity::m_pAnimMap = panimmap;
	
	g_pUIRenderer = new UIRenderer;
	g_pUIRenderer->setPreRenderCallback( RenderUI );
	g_hres = g_pUIRenderer->CreateTexture("uires\\texture\\hammer_premul.dds", BLEND_ALPHABLEND);

	m_pScene = new SimpleGameScene;

	

	LoadModel(fn);

	Light *m_pLight = new Light(LT_DIRECT);			//光照模型
	m_pLight->setRotation(0,45,0);
	m_pLight->setColor(ColourValue(0.5f, 0.5f, 0.5f));
	m_pLight->enableAmbient();
	m_pLight->m_AmbientColor = ColourValue(1.0f, 1.0f, 1.0f);
	m_pLight->setGlobal(true);
	m_pLight->attachToScene(m_pScene);

	m_pCamera = new Camera;											//摄像机
	m_pCamera->setRatio(1024.0f/768.0f);							//宽高比
	m_pCamera->setLookAt(m_eyepos, m_lookat, Vector3(0, 1.0f, 0));	//设置摄像机位置 视点和正方向
	m_pCamera->setDepth(1.0f,60000.0f);								//设置近截面和远截面

	/*m_pGrid = new GroundGrid;
	m_pGrid->setGridGround(3000.0F, 3000.0F, 100, 100);
	m_pGrid->setPosition(WorldPos(0, 0, 0));
	m_pGrid->setBorderColor(0xFF00FF00);
	m_pGrid->setLineColor(0xFF000000);
	m_pGrid->attachToScene(m_pScene);*/

	

	m_pScenerenderer = new NormalSceneRenderer(RU_GENERAL);

	m_pScenerenderer->setClearParams(CLEAR_TARGET|CLEAR_ZBUFFER, 0xff123456, 1.0f, 0);  //清空缓冲
	//m_pScenerenderer->setRenderTarget(RenderSystem::getSingleton().getMainWindow());	//设置渲染窗口	
	m_pScenerenderer->setCamera(m_pCamera);											    //设置摄像机
	m_pScenerenderer->setRenderScene(m_pScene);										    //设置渲染的场景

	SceneManager::getSingleton().addSceneRenderer(1, m_pScenerenderer);				    //把场景对象添加到场景管理器			
	SceneManager::getSingleton().addSceneRenderer(0, g_pUIRenderer);				    //把UI渲染对象添加到场景管理器
}



void CShow::UpdateData(uint dtick)
{


	if (m_pScene)
	{
		m_pScene->update(dtick);
	}
	if (m_pEntity)
	{
		m_pEntity->update(dtick);
		m_pEntity->setRotation(m_rx, m_ry, 0);
		m_pEntity->setScale(m_modelsize);
		if(m_isChangeAnim)
		{
			m_pEntity->playAnim(m_AnimID);
			SetIsChangeAnim(false);
		}	
	}
	if (m_pBKModel)
	{
		m_pBKModel->update(dtick);
	}

	if (m_pGrid)
	{
		m_pGrid->update(dtick);
	}

	if(m_pCamera)
	{
		m_pCamera->setLookAt(m_eyepos, m_lookat, Vector3(0, 1.0f, 0));
		m_pCamera->update(dtick);
	}
}

void CShow::DoFrame()
{
	if (SceneManager::getSingletonPtr())
	{
		SceneManager::getSingleton().doFrame();
	}
}

void CShow::SetAnimID(int animid)
{
	m_AnimID = animid;
	for (int i=0; i<g_AnimaID.size(); i++)
	{
		if (g_AnimaID[i].ID == animid)
		{
			m_CurIndex = i;
			return;
		}
	}
}

void CShow::Destory()
{
	
	OGRE_RELEASE(m_pCamera);
	OGRE_RELEASE(m_pLight);
	if (m_pBKModel)
	{
		m_pBKModel->detachFromScene();
		OGRE_DELETE(m_pBKModel);
	}
	if (m_pEntity)
	{
		m_pEntity->detachFromScene();
		OGRE_DELETE(m_pEntity);
	}
	OGRE_DELETE(g_hres);
	OGRE_RELEASE(m_pScene);
	OGRE_DELETE(m_pScenerenderer);	
	/*if (g_pUIRenderer)
	{
		SceneManager::getSingleton().removeSceneRenderer(g_pUIRenderer);
		delete g_pUIRenderer;
		g_pUIRenderer = NULL;
	}*/

	actionlist.clear();
	delete m_pRoot;
}

void CShow::NextAnima()
{
	if(g_AnimaID.size() <= 1)
		return;
	m_CurIndex++;
	if (m_CurIndex >= m_AnimaNum)
	{
		m_CurIndex = 0;
	}
	if(m_pEntity)
		m_pEntity->playAnim(g_AnimaID[m_CurIndex].ID);
}

void CShow::PreAnima()
{
	if(g_AnimaID.size() <= 1)
		return;
	m_CurIndex--;
	if (m_CurIndex < 0)
	{
		m_CurIndex = m_AnimaNum - 1;
	}
	if(m_pEntity)
		m_pEntity->playAnim(g_AnimaID[m_CurIndex].ID);
}


void CShow::SetEyePos(Vector3 v)
{
	m_eyepos = v;
}

void CShow::SetLookAt(Vector3 v)
{
	m_lookat = v;
}

void CShow::LoadBKModel(const char* filename)
{
	if (m_pBKModel)
	{
		m_pBKModel->detachFromScene();
		m_pBKModel->release();
		m_pBKModel = NULL;
	}
	m_pBKModel = new Entity;
	m_pBKModel->load(filename,true);
	if (m_pBKModel != NULL)
	{
		WorldPos bkpos(0,0,0);
		m_pBKModel->setPosition(bkpos);
		m_pBKModel->attachToScene(m_pScene);
	}
}

