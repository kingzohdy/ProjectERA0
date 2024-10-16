
#pragma once
#include "OgreVector3.h"

class CAnchorSet
{
public:
	CAnchorSet(void);
	~CAnchorSet(void);

	struct ANCHOR_INFO
	{
		int boneid;
		//Ogre::Vector3 offset;
		int id;
	};

	enum NODE_TYPE { NT_EDITING, NT_CHUNK };//如果是编辑中的特效，
	struct ANCHOR_NODE
	{
		ANCHOR_INFO* anchor;
		NODE_TYPE type;
		Ogre::SceneNode* pNode;

		CString strResAddr;
		CEffectUnit* pEffect;

	};

	void ClearInteract();
	void CreateInteract();

	void SetAnchorInifo(Ogre::Entity* pActor);
	int  SelectAnchorByPoint(float x, float y, CSkeleton* pAnchorSkeleton);

	bool OnLButtonDown(float x, float y, CSkeleton* pAnchorSkeleton);
	bool OnLButtonUp(float x, float y, CSkeleton* pAnchorSkeleton);
	bool OnMouseMove(float x, float y, CSkeleton* pAnchorSkeleton);
	void DoFrame(Ogre::uint ldtime);

	ANCHOR_INFO* FindAnchor(int id);


	void AddBind(int anchor, CString path);

	struct ANCHOR_BIND
	{
		int itype;
		union
		{
			Ogre::MovableObject* pnode;
			Ogre::Entity* pmode;
		};
	};


	COOR_ARROW m_TransArrow;
	int m_iSelectedAnchor;
	CCoord* m_pCood;
	std::vector<ANCHOR_INFO> m_AnchorInfo;
	std::vector<ANCHOR_NODE> m_AnchoredNodes;

};
