
#pragma once
#include "OgreVector3.h"

class CDammyNodeInfo
{
public:
	int m_iId;
	int m_iDefault;
	Ogre::SceneNode* m_pDammyNode;
	Ogre::SceneNode* m_pIconNode;

};

class CRefNodeMgr
{
public:
	CRefNodeMgr(void);
	~CRefNodeMgr(void);

	void AddRefNode(Ogre::Vector3 pos, int id);
	Ogre::SceneNode* GetNode(int id);
	CDammyNodeInfo* GetNodeInfo(int id);

	bool m_bShowIcon;
	std::vector<CDammyNodeInfo> m_RefNodes;

	static Ogre::Vector3 ms_defPos[];
	static int ms_iDefPosCount;

};
