
#include "stdafx.h"
#include "AnchorSet.h"
#include "Coord.h"
#include "EditorManager.h"
#include "OgreModel.h"
#include "OgreModelData.h"
#include "OgreEntity.h"

CAnchorSet::CAnchorSet(void) : m_iSelectedAnchor(-1), m_pCood(NULL)
{
}

CAnchorSet::~CAnchorSet(void)
{
	ClearInteract();
}




void CAnchorSet::ClearInteract()
{
	if(m_iSelectedAnchor != -1 && m_pCood)
	{
		m_pCood->DetachCoordFromRoom(CEnginAgent::GetInstance()->m_pRoom);

	}
	OGRE_RELEASE(m_pCood);
	m_iSelectedAnchor = -1;
	m_TransArrow = COOD_NONE;
}

void CAnchorSet::CreateInteract()
{	
	m_pCood = NULL;
	m_iSelectedAnchor = -1;
	m_TransArrow = COOD_NONE;

	m_pCood = new CCoord();
	m_pCood->CreateVertex();
	m_pCood->m_fSize = 10.0f;
}

void CAnchorSet::SetAnchorInifo(Ogre::Entity* pActor)
{
	m_AnchorInfo.clear();
	ClearInteract();
	if(pActor != NULL)
	{
		std::vector<int>anchors;
		pActor->getAnchors(anchors);
		for(size_t i=0; i<anchors.size(); i++)
		{
			CAnchorSet::ANCHOR_INFO a;
			a.id = anchors[i];
			a.boneid = 0;
			m_AnchorInfo.push_back(a);
		}
		CreateInteract();
	}
}

bool PtInRect(float x, float y, float centerx, float centery, float halfheight, float halfwidth)
{
	if( ((x - centerx) * (x - centerx) < halfwidth * halfwidth )
		&& ((y - centery) * (y - centery) < halfheight * halfheight ))
	{
		return true;
	}
	return false;
}

int CAnchorSet::SelectAnchorByPoint(float x, float y, CSkeleton* pAnchorSkeleton)
{
	//for(int i = 0; i <(int) m_AnchorInfo.size(); i++)
	//{
	//	Ogre::Vector3 pt = m_AnchorInfo[i].offset;

	//	Ogre::Matrix4 local ;
	//	pAnchorSkeleton->m_pSkeleton->GetBoneTM(m_AnchorInfo[i].boneid, local );
	//	Ogre::Matrix4 matViewProj = 
	//		CEnginAgent::GetInstance()->m_pCamera->GetViewMatrix() * CEnginAgent::GetInstance()->m_pCamera->GetProjectMatrix();

	//	pt = pt * local * matViewProj;
	//	if(PtInRect(x, y, pt.x / pt.z, pt.y / pt.z, 0.2f, 0.2f))
	//		return i;
	//}
	return -1;
}


bool CAnchorSet::OnLButtonDown(float x, float y, CSkeleton* pAnchorSkeleton)
{
	//if(pAnchorSkeleton->m_pSkeleton == NULL)
	//	return false;

	//int iSel = SelectAnchorByPoint(x, y, pAnchorSkeleton);
	//
	////对原来的选择，不处理
	////if(iSel == m_iSelectedAnchor)
	//	//return false;
	//
	////已经有选择，判断是取消选择还是按下移动轴
	//if(m_iSelectedAnchor != -1 )
	//{
	//	COOR_ARROW arr = m_pCood->SelectArrow(x, y);
	//	m_TransArrow = arr;
	//	if(m_TransArrow != COOD_NONE)
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		m_iSelectedAnchor = -1;
	//		m_pCood->DetachCoordFromRoom(CEnginAgent::GetInstance()->m_pRoom);
	//	}
	//}

	//if(iSel != -1 && m_AnchorInfo[iSel].boneid == 0 && m_AnchorInfo[iSel].id != 0)
	//{
	//	if(m_iSelectedAnchor == -1)
	//		m_pCood->AttachCoordToRoom(CEnginAgent::GetInstance()->m_pRoom);
	//	m_iSelectedAnchor = iSel;
	//	return true;
	//}

	return false;
}

bool CAnchorSet::OnLButtonUp(float x, float y, CSkeleton* pAnchorSkeleton)
{
	//if(m_TransArrow != COOD_NONE)
	//{
	//	PostMainFrameMsg(EEM_UPDATE_ANCHOR_ITEM);
	//}
	//m_TransArrow = COOD_NONE;
	return false;
}

bool CAnchorSet::OnMouseMove(float x, float y, CSkeleton* pAnchorSkeleton)
{
	//static float lastx = 0.0f;
	//static float lasty = 0.0f;
	//static bool init = true;
	//bool ret = false;
	//if(!init)
	//{
	//	if(m_TransArrow != COOD_NONE && m_iSelectedAnchor != -1)
	//	{
	//		if(m_TransArrow == COOD_X)
	//		{
	//			m_AnchorInfo[m_iSelectedAnchor].offset.x += m_pCood->GetXTrans(x - lastx, y - lasty) ;
	//		}
	//		if(m_TransArrow == COOD_Y)
	//		{
	//			m_AnchorInfo[m_iSelectedAnchor].offset.y += m_pCood->GetYTrans(x - lastx, y - lasty) ;
	//		}
	//		if(m_TransArrow == COOD_Z)
	//		{
	//			m_AnchorInfo[m_iSelectedAnchor].offset.z += m_pCood->GetZTrans(x - lastx, y - lasty) ;
	//		}
	//		ret = true;
	//	}
	//	else
	//	{
	//		if(m_iSelectedAnchor != -1)
	//			m_pCood->UpdateHighLight(x, y);
	//	}
	//}
	//lastx = x;
	//lasty = y;
	//init = false;
	return false;
}

void CAnchorSet::DoFrame(Ogre::uint ldtime)
{
	//if(m_iSelectedAnchor != -1)
	//m_pCood->SetPosition(m_AnchorInfo[m_iSelectedAnchor].offset);
}

CAnchorSet::ANCHOR_INFO* CAnchorSet::FindAnchor(int id)
{
	for(int i = 0; i < (int) m_AnchorInfo.size(); i++)
	{
		if(m_AnchorInfo[i].id == id)
			return &m_AnchorInfo[i];
	}
	return NULL;
}