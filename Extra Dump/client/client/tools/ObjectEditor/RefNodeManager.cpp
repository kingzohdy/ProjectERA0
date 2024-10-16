
#include "stdafx.h"
#include "RefNodeManager.h"
#include "EditorManager.h"

#include "OgreMovableObject.h"
#include "OgreDummyNode.h"


Ogre::Vector3 CRefNodeMgr::ms_defPos[] = {
	Ogre::Vector3(0.0f,	80.0f,	500.0f)
	, Ogre::Vector3(500.0f,	80.0f,	500.0f)
	, Ogre::Vector3(500.0f,	80.0f,	0.0f)
	, Ogre::Vector3(500.0f,	80.0f,	-500.0f)
	, Ogre::Vector3(0.0f,	80.0f,	-500.0f)
	, Ogre::Vector3(-500.0f,	80.0f,	-500.0f)
	, Ogre::Vector3(-500.0f,	80.0f,	0.0f)
	, Ogre::Vector3(-500.0f,	80.0f,	500.0f)
	, Ogre::Vector3(0.0f,	500.0f,	0.0f)
	, Ogre::Vector3(0.0f,	-500.0f,	0.0f)
};
int CRefNodeMgr::ms_iDefPosCount = 10;

CRefNodeMgr::CRefNodeMgr(void)
{
	m_bShowIcon = false;
}

CRefNodeMgr::~CRefNodeMgr(void)
{
	for (int i = 0; i < (int) m_RefNodes.size(); i++)
	{
		m_RefNodes[i].m_pDammyNode->detachFromScene();
		OGRE_RELEASE(m_RefNodes[i].m_pDammyNode);
	}
	m_RefNodes.clear();
}

void CRefNodeMgr::AddRefNode(Ogre::Vector3 pos, int id)
{
	CDammyNodeInfo newnode;
	newnode.m_iId = id;
	newnode.m_pDammyNode = new Ogre::DummyNode;
	newnode.m_pDammyNode->setPosition(pos);
	newnode.m_iDefault = -1;
	newnode.m_pDammyNode->attachToScene(CEnginAgent::GetInstance()->m_pRoom);
	m_RefNodes.push_back(newnode);

}

Ogre::SceneNode* CRefNodeMgr::GetNode(int id)
{
	for (int i = 0; i < (int) m_RefNodes.size(); i++)
	{
		if(m_RefNodes[i].m_iId == id)
			return m_RefNodes[i].m_pDammyNode;
	}
	return NULL;
}

CDammyNodeInfo* CRefNodeMgr::GetNodeInfo(int id)
{
	for (int i = 0; i < (int) m_RefNodes.size(); i++)
	{
		if(m_RefNodes[i].m_iId == id)
			return &m_RefNodes[i];
	}
	return NULL;
}