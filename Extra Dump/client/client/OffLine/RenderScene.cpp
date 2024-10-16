
#include "RenderScene.h"
using namespace Ogre;

int RenderUnit::unitproperty = 0;

RenderUnit::RenderUnit(void *hwnd, bool bMain) : m_pUIRenderer(NULL)
{
	RECT rect;
	GetClientRect((HWND)hwnd, &rect);

	if(bMain)
	{
		m_pTarget = RenderSystem::getSingleton().getMainWindow();
	}
	else
	{
		RenderSystem::InitDesc desc;
		memset(&desc, 0, sizeof(desc));

		desc.width = rect.right - rect.left;
		desc.height = rect.bottom - rect.top;
		desc.colorbits = 32;
		desc.alphabits = 8;
		desc.depthbits = 16;
		desc.stencilbits = 0;
		desc.hwnd = (void *)hwnd;

		if(hwnd == NULL) 
			m_pTarget = NULL;
		else
			m_pTarget = RenderSystem::getSingleton().createRenderWindow(desc);
	}

	m_pScene = new SimpleGameScene;

	m_pCamera = new Camera;
	m_pCamera->setFov(30.0f);
	m_pCamera->setLookAt(Vector3(0, 170.0f, -350.0f), Vector3(0, 70, 0), Vector3(0, 1.0f, 0));
	m_pCamera->update(0);

	LightNode *plightnode = new Light(LT_DIRECT);
	plightnode->setColor(ColourValue(0.8f, 0.8f, 0.8f, 1.0f));
	plightnode->m_Attenuation = 0.0f;
	plightnode->m_AmbientColor = ColourValue(0.2f, 0.2f, 0.2f, 1.0f);
	plightnode->enableAmbient();
	plightnode->setPosition(Vector3(-300.0f, 300.0f, -300.0f));
	plightnode->setRotation (60.0f, 60.0f, 60.0f);
	plightnode->update(0);

	plightnode->attachToScene(m_pScene);
	OGRE_RELEASE(plightnode);

	m_pRenderer = new NormalSceneRenderer();
	setClearColor(0xff2B2A23);
	m_pRenderer->setRenderTarget(m_pTarget);
	m_pRenderer->setRenderScene(m_pScene);
	m_pRenderer->setCamera(m_pCamera);

	SceneManager::getSingleton().addSceneRenderer(unitproperty, m_pRenderer);

	m_pEditorModel = 0;

	unitproperty++;
}

RenderUnit::~RenderUnit()
{
	SceneManager::getSingleton().removeSceneRenderer(m_pRenderer);

	if (m_pRenderer)
		delete m_pRenderer;

	if (m_pUIRenderer)
		delete m_pUIRenderer;
	OGRE_RELEASE(m_pCamera);
	OGRE_RELEASE(m_pScene);
}

void RenderUnit::SetModel(const std::string strFilename)
{
	if (strFilename == m_strFilename)
		return;

	// 去除旧的
	if (m_pEditorModel)
	{
		m_pEditorModel->GetShowModel()->detachFromScene();
		delete m_pEditorModel;
		m_pEditorModel = NULL;
	}

	// 加载模型
	EditorModel *pEditorModel = NULL;
	pEditorModel = new EditorModel;
	if (pEditorModel->LoadModel(strFilename.c_str()))
	{
		m_strFilename = strFilename;
		pEditorModel->GetShowModel()->attachToScene(m_pScene);
		m_pEditorModel = pEditorModel;
	}
	else
	{
		MessageBox(0, "Load model failed!", "error", 0);

		delete pEditorModel;
	}
}

void RenderUnit::update(unsigned int dtick)
{

	//m_pCamera->setLookAt(g_TerrainEdit.m_ResPreviewCamera.eyeloc, g_TerrainEdit.m_ResPreviewCamera.goal, Vector3(0, 1.0f, 0));

	m_pCamera->update(dtick);
}

void RenderUnit::setClearColor(unsigned int color)
{
	m_pRenderer->setClearParams(CLEAR_ZBUFFER|CLEAR_TARGET, color, 1.0f, 0);
}
