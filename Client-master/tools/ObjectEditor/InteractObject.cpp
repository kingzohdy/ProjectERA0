
#include "stdafx.h"
#include "InteractObject.h"
#include "Coord.h"
#include "EditorManager.h"
#include "EffectUnit.h"
#include "BindInfo.h"
#include "EffectManager.h"
#include "EffectCommand.h"
#include "MathUtility.h"
#include "ObjectEditor.h"
#include "Resource.h"

#include "OgreMovableObject.h"

INTERACT_SHOW CInteract::ms_showState = IS_HIDE;

CInteract::CInteract()
{
	m_bHovering = false;
	m_pCood = NULL;
	m_bCood = false;
	m_tran = COOD_NONE;
}
CInteract::~CInteract()
{
	Clear();
}

void CInteract::Clear()
{
	if(m_bCood && m_pCood)
	{
		m_pCood->DetachCoordFromRoom(CEnginAgent::GetInstance()->m_pRoom);

	}
	m_bHovering = false;
	OGRE_RELEASE(m_pCood);
	m_pCood = NULL; //!!!!!
	m_bCood = false;
	m_tran = COOD_NONE;
}

void CInteract::Create(CEffectUnit* pTarget)
{
	m_bHovering = false;
	m_pCood = NULL;
	m_bCood = false;
	m_tran = COOD_NONE;
	m_pTarget = pTarget;

	m_pCood = new CCoord();
	m_pCood->CreateVertex();
	if(pTarget->GetScaleType() == ST_GEOM_SCALING)
		m_pCood->SetGeomScale(true);
	else
		m_pCood->SetGeomScale(false);
}

bool CInteract::OnLBtnDown(float x, float y, bool bForceOn)
{
	if(m_pTarget->GetNode() == NULL || m_pTarget->GetBindInfo()->bDynamic == true)
		return false;


	if(m_bCood)
	{
		m_tran = m_pCood->SelectArrow(x, y);
		CEffectMgr::GetInstance()->GetCommandMgr()->ForceNewCommand();
		if(m_tran != COOD_NONE)
			return true;
	}

	bool bSelected = false;
	if(bForceOn == true)
		bSelected = true;
	else
		bSelected = C3DMath::CheckTouchBox(m_pTarget->GetNode()->getWorldPosition(),x, y, m_pTarget->GetRange());

	if(bSelected)
	{
		if(m_bCood && !bForceOn) //坐标已存在，确认移动轴
		{
			m_tran = m_pCood->SelectArrow(x, y);
			CEffectMgr::GetInstance()->GetCommandMgr()->ForceNewCommand();
		}
		if(!m_bCood) //坐标不存在，创建坐标
		{
			m_pCood->AttachCoordToRoom(CEnginAgent::GetInstance()->m_pRoom);
			OnActive();
		}
		m_bCood = true;


		return true;
	}
	else
	{
		if(m_bCood)
		{
			m_pCood->DetachCoordFromRoom(CEnginAgent::GetInstance()->m_pRoom);
			OnDeactive();
		}
		m_bCood = false;
		m_tran = COOD_NONE;


	}


	return false;

}

void CInteract::DoFrame(Ogre::uint dtick)
{
	float range= m_pTarget->GetRange();
	if(m_pTarget->GetBindInfo()->bDynamic == false)
	{

		if(m_bHovering || m_bCood)
		{
			CEnginAgent::GetInstance()->AddContainerBox(m_pTarget->GetNode()->getWorldPosition(), range, Ogre::ColorQuad(0, 255, 255, 255));
		}
		else if(ms_showState == IS_ALWAYS_SHOW)
		{
			CEnginAgent::GetInstance()->AddContainerBox(m_pTarget->GetNode()->getWorldPosition(), range, Ogre::ColorQuad(255, 255, 180, 255));
		}
	}
	m_pCood->SetFather(m_pTarget->GetNode() , 0);

	m_pCood->m_fSize = range;
	m_pCood->UpdateCoord(dtick / 1000.0f);
}


bool CInteract::OnLBtnUp(float x, float y)
{
	if(m_pTarget->GetNode() == NULL || m_pTarget->GetBindInfo()->bDynamic == true)
		return false;

	if(m_tran != COOD_NONE)
	{
		m_tran = COOD_NONE;
		PostMainFrameMsg(EEM_UPDATA_LIST, UE_EFFECT_LIST);
		return true;
	}
	return false;
}

bool CInteract::OnMouseMove(float x, float y)
{

	if(m_pTarget->GetNode() == NULL || m_pTarget->GetBindInfo()->bDynamic == true)
		return false;

	if(false) //if(m_pTarget->GetBindInfo()->boneid != 0)
	{
		if(m_bCood)
			m_pCood->DetachCoordFromRoom(CEnginAgent::GetInstance()->m_pRoom);
		m_bCood = false;
		m_tran = COOD_NONE;
		return false;
	}

	static float lastx = 0.0f;
	static float lasty = 0.0f;
	static bool init = true;
	if(!init)
	{
		if(m_tran == COOD_NONE)
		{
			if(C3DMath::CheckTouchBox(m_pTarget->GetNode()->getWorldPosition(), x, y, m_pTarget->GetRange()))
			{
				if(m_bHovering == false)
					//TRACE("Hoving\n");
					m_bHovering = true;
				//return true;
			}
			else
			{
				if(m_bHovering == true)
					//TRACE("Leaving\n");
					m_bHovering = false;
			}
			if(m_bCood)
				m_pCood->UpdateHighLight(x, y);
		}
		else
		{
			CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();
			Ogre::Vector3 offset(0.0f, 0.0f, 0.0f);
			Ogre::Quaternion rotate(0.0f, 0.0f, 0.0f, 0.0f);
			Ogre::Vector3 scale(0.0f, 0.0f, 0.0f);
			rotate.identity();
			//BIND_INFO* pBind = m_pTarget->GetBindInfo();

			Ogre::Vector3 newPosition = GetOldPosition();
			Ogre::Quaternion newRotation = GetOldRotation();
			Ogre::Vector3 newScale = GetOldScale();

			//bool bDoPos = false;
			//bool bDoRot = false;
			//bool bDoScale = false;

			// --Position-- 
			if(m_tran == COOD_X)
			{
				offset = newRotation.getAxisX() * m_pCood->GetXTrans(x - lastx, y - lasty);
			}
			if(m_tran == COOD_Y)
			{
				offset = newRotation.getAxisY() * m_pCood->GetYTrans(x - lastx, y - lasty);
			}
			if(m_tran == COOD_Z)
			{
				offset = newRotation.getAxisZ() * m_pCood->GetZTrans(x - lastx, y - lasty);
			}
			// ---Rotation--- 
			if(m_tran == COOD_RX)
			{
				rotate.setAxisAngle(Ogre::GetNormalize(newRotation.getAxisX()), m_pCood->GetTrans(x - lastx, y - lasty, COOD_RX));
			}
			if(m_tran == COOD_RY)
			{
				rotate.setAxisAngle(Ogre::GetNormalize(newRotation.getAxisY()), m_pCood->GetTrans(x - lastx, y - lasty, COOD_RY));
			}
			if(m_tran == COOD_RZ)
			{
				rotate.setAxisAngle(Ogre::GetNormalize(newRotation.getAxisZ()), m_pCood->GetTrans(x - lastx, y - lasty, COOD_RZ));
			}
			// ---Scale--------

			if(m_tran == COOD_SX)
			{
				scale.x = m_pCood->GetTrans(x - lastx, y - lasty, COOD_SX);
				if((GetAsyncKeyState(VK_SHIFT) & 0xff00) != 0)
				{
					scale.y = scale.z = scale.x;
				}
			}
			if(m_tran == COOD_SY)
			{
				scale.y = m_pCood->GetTrans(x - lastx, y - lasty, COOD_SY);
				if((GetAsyncKeyState(VK_SHIFT) & 0xff00) != 0)
				{
					scale.x = scale.z = scale.y;
				}
			}
			if(m_tran == COOD_SZ)
			{
				scale.z = m_pCood->GetTrans(x - lastx, y - lasty, COOD_SZ);
				if((GetAsyncKeyState(VK_SHIFT) & 0xff00) != 0)
				{
					scale.x = scale.y = scale.z;
				}
			}

			newPosition += offset;
			newRotation *= rotate;
			SetNewPosition(newPosition);
			SetNewRototion(newRotation);

			if(m_pTarget->GetScaleType() == ST_GEOM_SCALING)
				scale.y = scale.z = scale.x;


			if(m_pTarget->GetScaleType() != ST_NO_SCALING)
			{
				Ogre::Vector3 tmpScale;
				tmpScale.x = (float) pow(2.71828182f, scale.x / 100.0f);
				tmpScale.y = (float) pow(2.71828182f, scale.y / 100.0f);
				tmpScale.z = (float) pow(2.71828182f, scale.z / 100.0f);

				TRACE("tmpScale.x = %.3f; newScale.x = %.3f;  newScale.x * tmpScale.x = %.3f\n",
					tmpScale.x, newScale.x, tmpScale.x * newScale.x);
				if(newScale.x >= 0.01f) newScale.x *= tmpScale.x;
				else newScale.x = 0.01f;

				if(newScale.y >= 0.01f) newScale.y *= tmpScale.y;
				else newScale.y = 0.01f;

				if(newScale.z >= 0.01f) newScale.z *= tmpScale.z;
				else newScale.z = 0.01f;

				SetNewScale(newScale);

			}

			m_pTarget->EndRecordCommand(pCmd);
			m_pTarget->UpdateSource();

		}
	}

	init = false;
	lastx = x;
	lasty = y;
	return false;
}

Ogre::Quaternion CInteract::GetOldRotation()
{
	return m_pTarget->GetBindInfo()->rotate;
}

Ogre::Vector3 CInteract::GetOldPosition()
{
	return m_pTarget->GetBindInfo()->offset;
}

Ogre::Vector3 CInteract::GetOldScale()
{
	return m_pTarget->GetBindInfo()->scale;
}

void CInteract::SetNewRototion(Ogre::Quaternion rot)
{
	BIND_INFO* pBind = m_pTarget->GetBindInfo();
	pBind->rotate = rot;

	Ogre::SceneNode* pNode = m_pTarget->GetNode();
	pNode->setRotation(rot);

}
void CInteract::SetNewPosition(Ogre::Vector3 pos)
{
	BIND_INFO* pBind = m_pTarget->GetBindInfo();
	pBind->offset = pos;

	Ogre::SceneNode* pNode = m_pTarget->GetNode();
	pNode->setPosition(pos);
}

void CInteract::SetNewScale(Ogre::Vector3 scale)
{
	BIND_INFO* pBind = m_pTarget->GetBindInfo();
	pBind->scale = scale;

	Ogre::SceneNode* pNode = m_pTarget->GetNode();
	pNode->setScale(scale);
}

bool CInteract::Pick(float x, float y) 
{ 
	return false; 
}