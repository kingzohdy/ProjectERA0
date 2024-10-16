
#include "stdafx.h"
#include "ModelMotionInfo.h"
#include "EffectLayerInfo.h"
#include "Coord.h"
#include "EffectManager.h"
#include "MathUtility.h"
#include "EffectLayerInfo_SubClass.h"
#include "BindInfo.h"
#include "EditorManager.h"
#include "ObjectEditor.h"
#include "MainFrm.h"
#include "EffectUnit.h"

#include "OgreEntity.h"
#include <queue>

#define _CRT_SECURE_NO_WARNINGS
extern OpData gLayersOpObj;

CTimeHolder::CTimeHolder(void)
{
}

CTimeHolder::~CTimeHolder(void)
{
}


void CTimeHolder::SetEffectDynamic(CEffectUnit* pEffect)
{
}

void CTimeHolder::SetEffectStatic(CEffectUnit* pEffect)
{
}

void CTimeHolder::SetHotTime(float fHotTime)
{
	if(m_pMotionInfo->m_iSelMontion<0 || m_pMotionInfo->m_iSelMontion>=m_pMotionInfo->m_motions.size())
		return;

	CModelMotionInfo::MOTION* pMon = m_pMotionInfo->m_motions[m_pMotionInfo->m_iSelMontion];
	for(int i = 0; i < (int) pMon->objs.size(); i++)
	{
		pMon->objs[i]->RepresentAtTime(fHotTime, pMon->stMotionLodLevel);
	}
}

CModelMotionInfo::CModelMotionInfo(void) : m_pCood(NULL)
{
	m_pCood = NULL;
	m_iSelMontion = -1;
	m_iSelObj = -1;
	m_tranArr = COOD_NONE;
	m_timeHolder.m_pMotionInfo = this;
	m_timeHolder.m_fHotTime = -1;
	m_iHoverObj = -1;
	m_fHotTime = -1.0f;

	m_bShowPath = false;
	m_bLoopPlay = false;
}

CModelMotionInfo::~CModelMotionInfo(void)
{
	for(int i = 0; i < (int)m_motions.size(); i++)
	{
		for(int j = 0;  j < (int) m_motions[i]->objs.size(); j++)
		{
			delete m_motions[i]->objs[j];
		}
		delete m_motions[i];
	}
	m_motions.clear();
}



bool CModelMotionInfo::OnLButtonDown(float x, float y)
{
	CObjectLayerInfo* pObj = GetSelectedObj();
	if(pObj)
	{
		if(m_pCood)
			m_tranArr = m_pCood->SelectArrow(x, y) ;
		//if(m_tranArr != COOD_NONE)
		if(m_tranArr != COOD_NONE)
		{
			if(pObj->m_type == Ogre::MOT_MODEL_2)
			{
				CEffect2LayerInfo* pLayer = (CEffect2LayerInfo*)pObj;
				Ogre::Vector3 pos, scale, rotangle;
				Ogre::Quaternion rot;
				float trans;
				pLayer->GetSRTAtTime(m_fHotTime, pos, rot, scale, trans, rotangle);
				m_OldRot = rotangle;
				m_fMouseOffset = 0;
			}
			return true;
		}
	}

	MOTION* pMotion = GetSelectedMotion();
	if(pMotion != NULL)
	{

		if(m_iHoverObj >= 0 && m_iHoverObj < (int)pMotion->objs.size())
		{
			//SetSelect(m_iSelMontion, m_iHoverObj);
			SELECT_MOTION_STRUCT ms;
			ms.layer = m_iHoverObj;
			ms.strName = "";
			ms.bIsPublic = false;

			SendMainFrameMsg(EEM_SELECT_MOTION ,0, (LPARAM)&ms);
		}
	}
	return false;
}

bool CModelMotionInfo::OnLButtonUp(float x, float y)
{
	if(m_tranArr != COOD_NONE)
	{
		CMainFrame* pMainFrame = static_cast<CMainFrame*>( theApp.m_pMainWnd) ;
		CSRTObject* pThis = static_cast<CSRTObject*>(this);
		if(pThis == pMainFrame->m_wndSrtBar.GetSRTObject())
			pMainFrame->m_wndSrtBar.UpdateList();
		pMainFrame->PostMessage(EEM_UPDATA_LIST, UE_EFFECT_LIST);
	}
	m_tranArr = COOD_NONE;
	return false;
}

bool CModelMotionInfo::OnMouseMove(float x, float y)
{

	static float lastx = 0.0f;
	static float lasty = 0.0f;
	bool ret = false;
	CObjectLayerInfo* pObj = GetSelectedObj();
	m_iHoverObj = -1;
	if(m_pCood && pObj != NULL && pObj->m_type == Ogre::MOT_MODEL_2 )
	{

		if(m_tranArr != COOD_NONE)
		{
			if(pObj->m_pEffect != NULL)
			{
				Ogre::Vector3 offset(0.0f, 0.0f, 0.0f);
				Ogre::Quaternion rotate(0.0f, 0.0f, 0.0f, 0.0f);
				Ogre::Vector3 scale(0.0f, 0.0f, 0.0f);

				Ogre::Quaternion oldrot;
				oldrot.identity();
				rotate.identity();
				Ogre::Vector3 rotworldangle(0.0f, 0.0f, 0.0f);

				if(pObj->m_type == Ogre::MOT_MODEL)
				{
					oldrot = pObj->m_rotate;
				}
				else if(pObj->m_type == Ogre::MOT_MODEL_2 && m_iSelKey >= 0 && m_iSelKey < (int)pObj->m_posKey.m_Array.size())
				{
					//oldrot = C3DMath::XYZAngleToQuat(pObj->m_rotAngleKey.m_Array[m_iSelKey].data);
					oldrot = C3DMath::XYZAngleToQuat(m_OldRot);


				}
				else
				{
					return false;
					//assert(0 && "Find Unknown Motion Type: pEffect != NULL but not MOT_MODEL or MOT_MODEL_2");
				}


				bool bValid = true; //有时候经常会出现非法值，先做个简单的过滤，再4查问题
				// -------Position---------
				if(m_tranArr == COOD_X)
				{
					offset = oldrot.getAxisX() * m_pCood->GetXTrans(x - lastx, y - lasty);
					if(offset.length() >= 10000.0f)
						bValid = false;
				}
				if(m_tranArr == COOD_Y)
				{
					offset = oldrot.getAxisY() * m_pCood->GetYTrans(x - lastx, y - lasty);
					if(offset.length() >= 10000.0f)
						bValid = false;
				}
				if(m_tranArr == COOD_Z)
				{
					offset = oldrot.getAxisZ() * m_pCood->GetZTrans(x - lastx, y - lasty);
					if(offset.length() >= 10000.0f)
						bValid = false;
				}

				Ogre::Vector3 rotaix;
				// -------Rotation---------
				if(m_tranArr == COOD_RX)
				{
					m_fMouseOffset += m_pCood->GetTrans(x - lastx, y - lasty, COOD_RX);
					//if(m_fMouseOffset > 85.0f)
					//	m_fMouseOffset = 85;
					//if(m_fMouseOffset <= -85.0f)
					//	m_fMouseOffset = -85;
					rotate.setAxisAngle(oldrot.getAxisX(), m_fMouseOffset);
					rotaix = oldrot.getAxisX();
					//rotworldangle.x =m_pCood->GetTrans(x - lastx, y - lasty, COOD_RX);
				}
				if(m_tranArr == COOD_RY)
				{
					m_fMouseOffset += m_pCood->GetTrans(x - lastx, y - lasty, COOD_RY);
					//if(m_fMouseOffset > 85.0f )
					//	m_fMouseOffset = 85;
					//if(m_fMouseOffset <= -85.0f)
					//	m_fMouseOffset = -85;
					rotate.setAxisAngle(oldrot.getAxisY(), m_fMouseOffset);
					rotaix = oldrot.getAxisY();
					//rotworldangle.y =m_pCood->GetTrans(x - lastx, y - lasty, COOD_RY);
				}
				if(m_tranArr == COOD_RZ)
				{
					m_fMouseOffset += m_pCood->GetTrans(x - lastx, y - lasty, COOD_RZ);
					//if(m_fMouseOffset > 85.0f)
					//	m_fMouseOffset = 85;
					//if(m_fMouseOffset <= -85.0f)
					//	m_fMouseOffset = -85;
					rotate.setAxisAngle(oldrot.getAxisZ(), m_fMouseOffset);
					rotaix = oldrot.getAxisZ();
					//rotworldangle.z =m_pCood->GetTrans(x - lastx, y - lasty, COOD_RZ);
				}
				// ---Scale--------

				if(m_tranArr == COOD_SX)
				{
					scale.x = m_pCood->GetTrans(x - lastx, y - lasty, COOD_SX);
					if((GetAsyncKeyState(VK_SHIFT) & 0xff00) != 0)
					{
						scale.y = scale.z = scale.x;
					}
				}
				if(m_tranArr == COOD_SY)
				{
					scale.y = m_pCood->GetTrans(x - lastx, y - lasty, COOD_SY);
					if((GetAsyncKeyState(VK_SHIFT) & 0xff00) != 0)
					{
						scale.x = scale.z = scale.y;
					}
				}
				if(m_tranArr == COOD_SZ)
				{
					scale.z = m_pCood->GetTrans(x - lastx, y - lasty, COOD_SZ);
					if((GetAsyncKeyState(VK_SHIFT) & 0xff00) != 0)
					{
						scale.x = scale.y = scale.z;
					}
				}
				if(scale.length() >= 10000.0f)
					bValid = false;

				if(pObj->m_type == Ogre::MOT_MODEL_2 && m_iSelKey >= 0 && m_iSelKey < (int)pObj->m_posKey.m_Array.size() && bValid == true)
				{
					pObj->m_posKey.m_Array[m_iSelKey].data += offset;
					if(m_tranArr == COOD_RX || m_tranArr == COOD_RY || m_tranArr == COOD_RZ)
					{
						oldrot *= rotate;
						//Ogre::Matrix4 matold;
						//Ogre::Matrix4 matoff;
						//oldrot.GetMatrix(matold);
						//rotate.GetMatrix(matoff);

						//matold *= matoff;

						Ogre::Vector3 ra;
						C3DMath::QuaternionToEular(oldrot, &ra.x, &ra.y, &ra.z);

						//rotaix *= m_fMouseOffset;
						pObj->m_rotAngleKey.m_Array[m_iSelKey].data = ra;

					}

					if(m_tranArr == COOD_SX||m_tranArr == COOD_SY||m_tranArr == COOD_SZ)
					{
						scale.x = m_pCood->GetTrans(x - lastx, y - lasty, COOD_SX);
						if((GetAsyncKeyState(VK_SHIFT) & 0xff00) != 0)
						{
							scale.y = scale.z = scale.x;
						}


						if(pObj->m_pEffect->GetScaleType() == ST_GEOM_SCALING)
							scale.y = scale.z = scale.x;

						if(pObj->m_pEffect->GetScaleType() != ST_NO_SCALING)
						{
							Ogre::Vector3 tmpScale;
							tmpScale.x = (float) pow(2.71828182f, scale.x / 100.0f);
							tmpScale.y = (float) pow(2.71828182f, scale.y / 100.0f);
							tmpScale.z = (float) pow(2.71828182f, scale.z / 100.0f);

							Ogre::Vector3& keyScale = pObj->m_scaleKey.m_Array[m_iSelKey].data;
							//TRACE("tmpScale.x = %.3f; newScale.x = %.3f;  newScale.x * tmpScale.x = %.3f\n",
							//	tmpScale.x, newScale.x, tmpScale.x * newScale.x);
							if(keyScale.x >= 0.01f) keyScale.x *= tmpScale.x;
							else keyScale.x = 0.01f;

							if(keyScale.y >= 0.01f) keyScale.y *= tmpScale.y;
							else keyScale.y = 0.01f;

							if(keyScale.z >= 0.01f) keyScale.z *= tmpScale.z;
							else keyScale.z = 0.01f;

						}
					}
					SetCoodPlace(pObj, m_fHotTime);

					pObj->m_pEffect->GetBindInfo()->offset = pObj->m_posKey.m_Array[m_iSelKey].data;
					pObj->m_pEffect->GetBindInfo()->rotate = C3DMath::XYZAngleToQuat(pObj->m_rotAngleKey.m_Array[m_iSelKey].data);
					pObj->m_pEffect->GetBindInfo()->scale = pObj->m_scaleKey.m_Array[m_iSelKey].data;
					pObj->RepresentAtTime(m_fHotTime);

				}
			}

			ret = true;
		}
		else
		{
			m_pCood->UpdateHighLight(x, y);
		}
	}
	//else

	if(m_tranArr == COOD_NONE)
	{

		MOTION * pMotion = GetSelectedMotion();
		if(pMotion != NULL)
		{

			for(int i = 0; i < (int) pMotion->objs.size(); i++)
			{
				CObjectLayerInfo* pObj = pMotion->objs[i];
				if(pObj->m_type != Ogre::MOT_MODEL_2)
					continue;
				if(pObj->m_pEffect == NULL)
					continue;

				CEffect2LayerInfo* pEffectObj = static_cast<CEffect2LayerInfo*>(pObj);

				if(m_fHotTime < pEffectObj->m_start || m_fHotTime > pEffectObj->m_end)
					continue;

				float fRange = pEffectObj->m_pEffect->GetRange();
				Ogre::Vector3 pos, scale, rotAngle;
				Ogre::Quaternion rot;
				float trans;
				pEffectObj->GetSRTAtTime(m_fHotTime, pos, rot, scale, trans, rotAngle);
				Ogre::Matrix4 mat = CEffectMgr::GetInstance()->m_pModelActor->getMainModel()->getAnchorWorldMatrix(pEffectObj->m_anchor);
				pos = pos * mat;
				if(C3DMath::CheckTouchBox(pos, x, y, fRange * scale.x))
				{
					m_iHoverObj = i;
					//TRACE("Hover Object is %d\n", m_iHoverObj);
					break;
				}

			}
		}
	}
	lastx = x;
	lasty = y;
	return ret;
}

void CModelMotionInfo::DoFrame(Ogre::uint time)
{
	//if(m_iSelMontion >=0 && m_iSelObj >= 0)
	//{
	//	OBJINFO* pObj = &m_motions[m_iSelMontion]->objs[m_iSelObj];
	//	m_pCood->SetMatrix
	//}
	MOTION * pMotion = GetSelectedMotion();
	if(pMotion != NULL)
	{
		if(m_iHoverObj >= 0 && m_iHoverObj < (int)pMotion->objs.size() && pMotion->objs[m_iHoverObj]->m_type == Ogre::MOT_MODEL_2)
		{
			CObjectLayerInfo* pObj = pMotion->objs[m_iHoverObj];
			if(pObj->m_pEffect != NULL && pObj->m_pEffect->GetNode() != NULL)
			{
				CEffect2LayerInfo* pEffectObj = static_cast<CEffect2LayerInfo*>(pObj);

				float fRange = pEffectObj->m_pEffect->GetRange();
				Ogre::Vector3 pos, scale, rotAngle;
				Ogre::Quaternion rot;
				float trans;
				pEffectObj->GetSRTAtTime(m_fHotTime, pos, rot, scale, trans, rotAngle);
				Ogre::Matrix4 mat = CEffectMgr::GetInstance()->m_pModelActor->getMainModel()->getAnchorWorldMatrix(pEffectObj->m_anchor);
				pos = pos * mat;

				CEnginAgent::GetInstance()->AddContainerBox(pos, fRange * scale.x, Ogre::ColorQuad(0, 255, 255, 255));
			}
		}
		Ogre::RenderLines* pDbg =	CEnginAgent::GetInstance()->m_pLineRenderer;
		for(int i = 0; i < (int)pMotion->objs.size(); i++)
		{
			if( pMotion->objs[i]->m_bVisible == false)
				continue;

			if(i == m_iHoverObj)
			{
				pMotion->objs[i]->DrawIdentifier(pDbg, m_fHotTime, IT_HOVER);
			}
			else if( i == m_iSelObj)
			{
				pMotion->objs[i]->DrawIdentifier(pDbg, m_fHotTime, IT_SELECTED);
			}
			else
			{
				pMotion->objs[i]->DrawIdentifier(pDbg, m_fHotTime, IT_DEFAULT);
			}

		}

		//if(m_bShowPath == true)
		//{
		//	CObjectLayerInfo* pObj = GetSelectedObj();
		//	if(pObj != NULL)
		//	{
		//		pObj->DrawIdentifier(;
		//	}
		//}
	}
	if(m_pCood != NULL)
		m_pCood->UpdateCoord(time / 1000.0f);
}

void CModelMotionInfo::SetCoodPlace(CObjectLayerInfo* pLayer, float fTime)
{

	if(pLayer->m_type != Ogre::MOT_MODEL_2)
		return;

	if(m_pCood == NULL)
		return;

	CEffect2LayerInfo* pEffectObj = static_cast<CEffect2LayerInfo*>(pLayer);

	Ogre::Vector3 pos, scale, rotAngle;
	Ogre::Quaternion rot;
	float trans;
	pEffectObj->GetSRTAtTime(fTime, pos, rot, scale, trans, rotAngle);

	//rot.identity();

	m_pCood->SetPosition(pos);
	m_pCood->SetRotation(C3DMath::XYZAngleToQuat(rotAngle));
	m_pCood->SetScale(scale);

	CObjectLayerInfo *parent = pEffectObj->getParent();
	if(parent!=NULL && parent->m_pEffect != NULL)
	{
		m_pCood->SetFather(parent->m_pEffect->GetNode(), pEffectObj->m_anchor);
	}
}

Ogre::Vector3	CModelMotionInfo::GetPosition()
{
	if(m_iSelMontion < 0 || m_iSelObj < 0)
	{
		return CSRTObject::GetPosition();
	}

	CObjectLayerInfo* pSel = m_motions[m_iSelMontion]->objs[m_iSelObj];
	if(pSel->m_type != Ogre::MOT_MODEL_2)
	{
		return CSRTObject::GetPosition();
	}

	if(m_iSelKey >= 0 && m_iSelKey < (int)pSel->m_posKey.m_Array.size())
	{
		return pSel->m_posKey.m_Array[m_iSelKey].data;
	}
	else
	{
		Ogre::Vector3 ret;
		int kfTime = (int)((m_fHotTime-pSel->m_start) * 1000);

		int iCount = (int)pSel->m_posKey.m_Array.size();
		if( iCount == 0)
			return CSRTObject::GetPosition();

		if(kfTime > (int)pSel->m_posKey.m_Array[iCount - 1].tick)
		{
			return  pSel->m_posKey.m_Array[iCount - 1].data;
		}
		else if(kfTime <= 0)
		{
			return pSel->m_posKey.m_Array[0].data;
		}

		pSel->m_posKey.getValue(0, (Ogre::uint)kfTime, ret);
		return ret;
	}

}

Ogre::Quaternion	CModelMotionInfo::GetRotation()
{
	if(m_iSelMontion < 0 || m_iSelObj < 0)
	{
		return CSRTObject::GetRotation();
	}

	CObjectLayerInfo* pSel = m_motions[m_iSelMontion]->objs[m_iSelObj];
	if(pSel->m_type != Ogre::MOT_MODEL_2)
	{
		return CSRTObject::GetRotation();
	}

	if(m_iSelKey >= 0 && m_iSelKey < (int)pSel->m_rotKey.m_Array.size())
	{
		return pSel->m_rotKey.m_Array[m_iSelKey].data;
	}
	else
	{
		Ogre::Quaternion ret;
		int kfTime = (int)((m_fHotTime-pSel->m_start) * 1000);

		int iCount = (int)pSel->m_rotKey.m_Array.size();
		if( iCount == 0)
			return CSRTObject::GetRotation();

		if(kfTime > (int)pSel->m_rotKey.m_Array[iCount - 1].tick)
		{
			return  pSel->m_rotKey.m_Array[iCount - 1].data;
		}
		else if(kfTime <= 0)
		{
			return pSel->m_rotKey.m_Array[0].data;
		}

		pSel->m_rotKey.getValue(0, (Ogre::uint)kfTime, ret);
		return ret;
	}
}

Ogre::Vector3	CModelMotionInfo::GetRotateAngle()
{
	if(m_iSelMontion < 0 || m_iSelObj < 0)
	{
		return CSRTObject::GetRotateAngle();
	}

	CObjectLayerInfo* pSel = m_motions[m_iSelMontion]->objs[m_iSelObj];
	if(pSel->m_type != Ogre::MOT_MODEL_2)
	{
		return CSRTObject::GetRotateAngle();
	}

	if(m_iSelKey >= 0 && m_iSelKey < (int)pSel->m_rotAngleKey.m_Array.size())
	{
		return pSel->m_rotAngleKey.m_Array[m_iSelKey].data;
	}
	else
	{
		Ogre::Vector3 ret;
		int kfTime = (int)((m_fHotTime-pSel->m_start) * 1000);

		int iCount = (int)pSel->m_rotAngleKey.m_Array.size();
		if( iCount == 0)
			return CSRTObject::GetRotateAngle();

		if(kfTime > (int)pSel->m_rotAngleKey.m_Array[iCount - 1].tick)
		{
			return  pSel->m_rotAngleKey.m_Array[iCount - 1].data;
		}
		else if(kfTime <= 0)
		{
			return pSel->m_rotAngleKey.m_Array[0].data;
		}

		pSel->m_rotAngleKey.getValue(0, (Ogre::uint)kfTime, ret);
		return ret;
	}
}


Ogre::Vector3	CModelMotionInfo::GetScale()
{
	if(m_iSelMontion < 0 || m_iSelObj < 0)
	{
		return CSRTObject::GetScale();
	}

	CObjectLayerInfo* pSel = m_motions[m_iSelMontion]->objs[m_iSelObj];
	if(pSel->m_type != Ogre::MOT_MODEL_2)
	{
		return CSRTObject::GetScale();
	}

	if(m_iSelKey >= 0 && m_iSelKey < (int)pSel->m_posKey.m_Array.size())
	{
		return pSel->m_scaleKey.m_Array[m_iSelKey].data;
	}
	else
	{
		Ogre::Vector3 ret;
		int kfTime = (int)((m_fHotTime-pSel->m_start)  * 1000);

		int iCount = (int)pSel->m_scaleKey.m_Array.size();
		if( iCount == 0)
			return CSRTObject::GetScale();

		if(kfTime > (int)pSel->m_scaleKey.m_Array[iCount - 1].tick)
		{
			return  pSel->m_scaleKey.m_Array[iCount - 1].data;
		}
		else if(kfTime <= 0)
		{
			return pSel->m_scaleKey.m_Array[0].data;
		}

		pSel->m_scaleKey.getValue(0, (Ogre::uint)kfTime, ret);
		return ret;
	}
}

float CModelMotionInfo::GetTransparent()
{
	if(m_iSelMontion < 0 || m_iSelObj < 0)
	{
		return CSRTObject::GetTransparent();
	}

	CObjectLayerInfo* pSel = m_motions[m_iSelMontion]->objs[m_iSelObj];
	if(pSel->m_type != Ogre::MOT_MODEL_2)
	{
		return CSRTObject::GetTransparent();
	}

	if(m_iSelKey >= 0 && m_iSelKey < (int)pSel->m_posKey.m_Array.size())
	{
		return pSel->m_transparent_keys.m_Array[m_iSelKey].data;
	}
	else
	{
		float ret;
		int kfTime = (int)((m_fHotTime-pSel->m_start)  * 1000);

		int iCount = (int)pSel->m_transparent_keys.m_Array.size();
		if( iCount == 0)
			return CSRTObject::GetTransparent();

		if(kfTime > (int)pSel->m_transparent_keys.m_Array[iCount - 1].tick)
		{
			return  pSel->m_transparent_keys.m_Array[iCount - 1].data;
		}
		else if(kfTime <= 0)
		{
			return pSel->m_transparent_keys.m_Array[0].data;
		}

		pSel->m_transparent_keys.getValue(0, (Ogre::uint)kfTime, ret);
		return ret;
	}
}

void CModelMotionInfo::SetTransparent(float fTransparent)
{
	if(m_iSelMontion < 0 || m_iSelObj < 0)
	{
		return;
	}

	CObjectLayerInfo* pSel = m_motions[m_iSelMontion]->objs[m_iSelObj];
	if(pSel->m_type != Ogre::MOT_MODEL_2)
	{
		return;
	}

	if(m_iSelKey >= 0 && m_iSelKey < (int)pSel->m_posKey.m_Array.size())
	{
		pSel->m_transparent_keys.m_Array[m_iSelKey].data = fTransparent;

	}

	m_timeHolder.SetHotTime(m_fHotTime);
}

void CModelMotionInfo::SetPosition(Ogre::Vector3 pos)
{
	if(m_iSelMontion < 0 || m_iSelObj < 0)
	{
		return;
	}

	CObjectLayerInfo* pSel = m_motions[m_iSelMontion]->objs[m_iSelObj];
	if(pSel->m_type != Ogre::MOT_MODEL_2)
	{
		return;
	}

	if(m_iSelKey >= 0 && m_iSelKey < (int)pSel->m_posKey.m_Array.size())
	{
		pSel->m_posKey.m_Array[m_iSelKey].data = pos;
		//m_pCood->SetPosition(pos);
		SetCoodPlace(pSel, pSel->m_posKey.m_Array[m_iSelKey].tick * 0.001f + pSel->m_start);

	}

	m_timeHolder.SetHotTime(m_fHotTime);
}

void CModelMotionInfo::SetRotation(Ogre::Quaternion rot)
{
	if(m_iSelMontion < 0 || m_iSelObj < 0)
	{
		return;
	}

	CObjectLayerInfo* pSel = m_motions[m_iSelMontion]->objs[m_iSelObj];
	if(pSel->m_type != Ogre::MOT_MODEL_2)
	{
		return;
	}

	if(m_iSelKey >= 0 && m_iSelKey < (int)pSel->m_posKey.m_Array.size())
	{
		pSel->m_rotKey.m_Array[m_iSelKey].data = rot;
		//m_pCood->SetRotation(rot);
		SetCoodPlace(pSel, pSel->m_rotKey.m_Array[m_iSelKey].tick * 0.001f + pSel->m_start);

	}
	m_timeHolder.SetHotTime(m_fHotTime);
}

void CModelMotionInfo::SetRotationAngle(Ogre::Vector3 rotAngle)
{
	if(m_iSelMontion < 0 || m_iSelObj < 0)
	{
		return;
	}

	CObjectLayerInfo* pSel = m_motions[m_iSelMontion]->objs[m_iSelObj];
	if(pSel->m_type != Ogre::MOT_MODEL_2)
	{
		return;
	}

	if(m_iSelKey >= 0 && m_iSelKey < (int)pSel->m_posKey.m_Array.size())
	{
		pSel->m_rotAngleKey.m_Array[m_iSelKey].data = rotAngle;

		SetCoodPlace(pSel, pSel->m_rotAngleKey.m_Array[m_iSelKey].tick * 0.001f + pSel->m_start);

		//Ogre::Matrix4 mx, my, mz, mat;
		//mx.MakeRotateX(rotAngle.x);
		//my.MakeRotateY(rotAngle.y);
		//mz.MakeRotateZ(rotAngle.z);
		//mat = mz * my * mx;
		//Ogre::Quaternion rot;
		//rot.SetMatrix(mat);
		//m_pCood->SetRotation(rot);

	}
	m_timeHolder.SetHotTime(m_fHotTime);
}

void CModelMotionInfo::SetScale(Ogre::Vector3 scale)
{
	if(m_iSelMontion < 0 || m_iSelObj < 0)
	{
		return;
	}

	CObjectLayerInfo* pSel = m_motions[m_iSelMontion]->objs[m_iSelObj];
	if(pSel->m_type != Ogre::MOT_MODEL_2)
	{
		return;
	}

	if(m_iSelKey >= 0 && m_iSelKey < (int)pSel->m_posKey.m_Array.size())
	{
		pSel->m_scaleKey.m_Array[m_iSelKey].data = scale;
		SetCoodPlace(pSel, pSel->m_scaleKey.m_Array[m_iSelKey].tick * 0.001f + pSel->m_start);
		//m_pCood->SetScale(scale);
	}
	m_timeHolder.SetHotTime(m_fHotTime);
}

bool CModelMotionInfo::IsGeomScale()	
{	
	if(m_iSelMontion < 0 || m_iSelObj < 0)
	{
		return false;
	}

	CObjectLayerInfo* pSel = m_motions[m_iSelMontion]->objs[m_iSelObj];
	if(pSel->m_type != Ogre::MOT_MODEL_2)
	{
		return false;
	}

	if(pSel->m_pEffect == NULL)
	{
		return false;
	}

	return (pSel->m_pEffect->GetScaleType() == ST_GEOM_SCALING) ? true : false;
};

void CModelMotionInfo::RefreshEffect(CEffectUnit* pEffect)
{
	m_pCood->SetFather(NULL);

	if(m_iSelMontion < 0)
		return;

	for(int i = 0; i < (int) m_motions[m_iSelMontion]->objs.size(); i++)
	{
		if(m_motions[m_iSelMontion]->objs[i]->m_type == Ogre::MOT_MODEL_2 
			&& m_motions[m_iSelMontion]->objs[i]->m_pEffect == pEffect)
		{
			m_motions[m_iSelMontion]->objs[i]->RefreshRepresent(m_fHotTime);
		}
	}
}

CString CModelMotionInfo::GetObjName()
{
	if(m_iSelMontion >= 0)
	{
		CString ret = m_motions[m_iSelMontion]->name.c_str();
	}
	return "";
}

// ----------float obj------------------[6/20/2008]
float CModelMotionInfo::GetFloat()
{
	CObjectLayerInfo* pObj = GetSelectedObj();
	if(pObj == NULL)
		return CFloatObject::GetFloat();

	if((pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_TRANSPARENT)
		|| (pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_ZOOM)
		|| (pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_STONE)
		|| (pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_SHAKE)
		|| (pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_DEPTHOFFIELD)
		)
	{
		if(m_iSelKey >= 0 && m_iSelKey < (int)pObj->m_transparent_keys.m_Array.size())
		{
			return pObj->m_transparent_keys.m_Array[m_iSelKey].data;;
		}
		else if(pObj->m_transparent_keys.m_Array.size() == 0)
		{
			return CFloatObject::GetFloat();
		}
		else
		{
			float retval;
			int iCount = (int)pObj->m_transparent_keys.m_Array.size();
			int tick = int((m_fHotTime - pObj->m_start) * 1000.0f);
			if(tick < 0)
				retval = pObj->m_transparent_keys.m_Array[0].data;
			else if(tick > (int)pObj->m_transparent_keys.m_Array[iCount-1].tick)
				retval = pObj->m_transparent_keys.m_Array[iCount-1].data;
			else
				pObj->m_transparent_keys.getValue(0, tick, retval);

			return retval;	
		}
	}

	return CFloatObject::GetFloat();
}

void CModelMotionInfo::SetFloat(float val)
{
	CObjectLayerInfo* pObj = GetSelectedObj();
	if(pObj == NULL)
		return;
	if((pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_TRANSPARENT)
		|| (pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_ZOOM)
		|| (pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_STONE)
		|| (pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_SHAKE)
		|| (pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_DEPTHOFFIELD)
		)
	{
		if(m_iSelKey >= 0 && m_iSelKey < (int)pObj->m_transparent_keys.m_Array.size())
		{
			pObj->m_transparent_keys.m_Array[m_iSelKey].data = val;
		}
	}
}

void CModelMotionInfo::GetFloatRange(float* pmin, float* pmax, float* ratio)
{
	CObjectLayerInfo* pObj = GetSelectedObj();
	if(pObj == NULL)
		return;
	if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_TRANSPARENT)
	{
		*pmin = 0.0f;
		*pmax = 1.0f;
		*ratio = 0.05f;
	}
	else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_ZOOM)
	{
		*pmin = -50.0f;
		*pmax = 50.0f;
		*ratio = 1.0f;
	}
	else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_STONE)
	{
		*pmin = 0.0f;
		*pmax = 1.0f;
		*ratio = 0.05f;
	}
	else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_SHAKE)
	{
		*pmin = 0.0f;
		*pmax = 1.0f;
		*ratio = 0.05f;
	}
	else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_DEPTHOFFIELD)
	{
		*pmin = 0.0f;
		*pmax = 1.0f;
		*ratio = 0.05f;
	}
}

CString	CModelMotionInfo::GetFloatName()
{
	CObjectLayerInfo* pObj = GetSelectedObj();
	if(pObj == NULL)
		return "未知";
	if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_TRANSPARENT)
	{
		return "透明度";
	}
	else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_ZOOM)
	{
		return "焦距视角相对变化";
	}
	else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_STONE)
	{
		return "石化";
	}
	else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_SHAKE)
	{
		return "震动幅度";
	}
	else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_DEPTHOFFIELD)
	{
		return "...";
	}

	return "未知";
}

// ---------color obj---------------- [6/20/2008]
Ogre::ColourValue	CModelMotionInfo::GetColor()
{
	CObjectLayerInfo* pObj = GetSelectedObj();
	if(pObj == NULL)
		return CColorObject::GetColor();

	if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_FSEFFECT)
	{
		if(m_iSelKey >= 0 && m_iSelKey < (int)pObj->m_fs_colorkeys.m_Array.size())
		{
			const Ogre::Vector4 &v = pObj->m_fs_colorkeys.m_Array[m_iSelKey].data;
			return Ogre::ColourValue(v.x, v.y, v.z, v.w);
		}
		else if(pObj->m_fs_colorkeys.m_Array.size() == 0)
		{
			return CColorObject::GetColor();
		}
		else
		{
			Ogre::Vector4 retval;
			int iCount = (int)pObj->m_fs_colorkeys.m_Array.size();
			int tick = int((m_fHotTime - pObj->m_start) * 1000.0f);
			if(tick < 0)
				retval = pObj->m_fs_colorkeys.m_Array[0].data;
			else if(tick > (int)pObj->m_fs_colorkeys.m_Array[iCount-1].tick)
				retval = pObj->m_fs_colorkeys.m_Array[iCount-1].data;
			else
				pObj->m_fs_colorkeys.getValue(0, tick, retval);

			return Ogre::ColourValue(retval.x, retval.y, retval.z, retval.w);	
		}
	}
	return CColorObject::GetColor();
}

void CModelMotionInfo::SetColor(Ogre::ColourValue val)
{
	CObjectLayerInfo* pObj = GetSelectedObj();
	if(pObj == NULL)
		return;
	if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_FSEFFECT)
	{
		if(m_iSelKey >= 0 && m_iSelKey < (int)pObj->m_fs_colorkeys.m_Array.size())
		{
			pObj->m_fs_colorkeys.m_Array[m_iSelKey].data = Ogre::Vector4(val.r, val.g, val.b, val.a);
		}
	}
}

Ogre::BlendMode CModelMotionInfo::GetIceFrozenBlend()
{
	CObjectLayerInfo* pObj = GetSelectedObj();
	if(pObj && pObj->m_type==Ogre::MOT_POSTEFFECT && pObj->m_posteffect==Ogre::PE_FROZEN)
	{
		return pObj->m_OverlayBlendMode;
	}

	return CIceFrozenObject::GetIceFrozenBlend();
}

CString CModelMotionInfo::GetOverlayTex()
{
	CObjectLayerInfo* pObj = GetSelectedObj();
	if(pObj && pObj->m_type==Ogre::MOT_POSTEFFECT && pObj->m_posteffect==Ogre::PE_FROZEN)
	{
		return pObj->m_OverlayTexPath;
	}

	return CIceFrozenObject::GetOverlayTex();
}

void CModelMotionInfo::SetIceFrozenBlend(Ogre::BlendMode blendmode, CString &texpath, float alpha)
{
	CObjectLayerInfo* pObj = GetSelectedObj();
	if(pObj && pObj->m_type==Ogre::MOT_POSTEFFECT && pObj->m_posteffect==Ogre::PE_FROZEN)
	{
		pObj->m_OverlayBlendMode = blendmode;
		pObj->m_OverlayTexPath = texpath;
		pObj->m_fOverlayAlhpa = alpha;
	}
}

float CModelMotionInfo::GetIceFrozenAlpha()
{
	CObjectLayerInfo* pObj = GetSelectedObj();
	if(pObj && pObj->m_type==Ogre::MOT_POSTEFFECT && pObj->m_posteffect==Ogre::PE_FROZEN)
	{
		return pObj->m_fOverlayAlhpa;
	}
	return CIceFrozenObject::GetIceFrozenAlpha();
}

CObjectLayerInfo* CModelMotionInfo::GetSelectedObj()
{
	if(m_iSelMontion < 0 || m_iSelMontion >= (int)m_motions.size())
		return NULL;

	if(m_iSelObj < 0 || m_iSelObj >= (int)m_motions[m_iSelMontion]->objs.size())
		return NULL;

	return m_motions[m_iSelMontion]->objs[m_iSelObj];
}

CModelMotionInfo::MOTION*	CModelMotionInfo::GetSelectedMotion()
{
	if(m_iSelMontion < 0 || m_iSelMontion >= (int)m_motions.size())
		return NULL;
	return m_motions[m_iSelMontion];
}

void CModelMotionInfo::SetSelect(int mon, int obj)
{
	//if(!m_pCood)
	//return;

	CMainFrame* pMainFrame = static_cast<CMainFrame*>( theApp.m_pMainWnd) ;

	bool bSetSrt = false;
	if(mon != m_iSelMontion)
		m_timeHolder.SetHotTime(-1.0f);


	CObjectLayerInfo* pOldSel = GetSelectedObj();
	if(pOldSel != NULL)
	{
		if(pOldSel->m_type == Ogre::MOT_MODEL
			|| pOldSel->m_type == Ogre::MOT_MODEL_2)
		{
			//CEffectMgr::GetInstance()->GetModelActor()->unbindObject(m_pCood);
			m_pCood->UnbindFromModel(CEffectMgr::GetInstance()->GetModelActor());
		}
		m_iSelMontion = -1;
		m_iSelObj = -1;
	}

	if((m_iSelMontion < 0 || m_iSelObj < 0)
		&& (mon >=0 && obj >= 0))
	{
		m_iSelMontion = mon;
		m_iSelObj = obj;

		if(mon < (int)m_motions.size() && obj < (int)m_motions[mon]->objs.size())
		{	
			CObjectLayerInfo* pObj = m_motions[mon]->objs[obj];


			if(pObj->m_type == Ogre::MOT_MODEL && m_pCood)
			{
				int anchor = pObj->m_anchor;
				m_pCood->BindToModel(CEffectMgr::GetInstance()->GetModelActor(), anchor);
				SetCoodPlace(pObj, m_fHotTime);

				if(pObj->m_pEffect != NULL)
				{
					if(pObj->m_pEffect->GetScaleType() != ST_GEOM_SCALING)
					{
						m_pCood->SetGeomScale(false);
					}
					else
					{
						m_pCood->SetGeomScale(true);
					}
				}

			}
			else if(pObj->m_type == Ogre::MOT_MODEL_2 && m_pCood)
			{
				int anchor = pObj->m_anchor;
				//CEffectMgr::GetInstance()->GetModelActor()->bindObject(anchor, m_pCood);
				m_pCood->BindToModel(CEffectMgr::GetInstance()->GetModelActor(), anchor);
				m_iSelMontion = mon;
				m_iSelObj = obj;
				SetHotTime(m_fHotTime);

				pMainFrame->m_wndSrtBar.SetSRTObject(static_cast<CSRTObject*>(this));
				bSetSrt = true;
			}
			else if(pObj->m_type == Ogre::MOT_POSTEFFECT)
			{
				if(pObj->m_posteffect==Ogre::PE_TRANSPARENT || pObj->m_posteffect==Ogre::PE_ZOOM || pObj->m_posteffect==Ogre::PE_SHAKE || pObj->m_posteffect==Ogre::PE_STONE)
				{
					pMainFrame->m_wndSrtBar.SetFloatObject(static_cast<CFloatObject*>(this));
					bSetSrt = true;
				}
				else if(pObj->m_posteffect==Ogre::PE_FSEFFECT)
				{
					pMainFrame->m_wndSrtBar.SetColorObject(static_cast<CColorObject*>(this));
					bSetSrt = true;
				}
				else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_REMANETSHODOW)
				{
					pMainFrame->m_wndSrtBar.SetRemanetShadowObject(static_cast<CRemanetShadowObject*>(this));
					bSetSrt = true;
				}
				else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_OFFCOLOR)
				{
					pMainFrame->m_wndSrtBar.SetOffColorObject(static_cast<COffColorObject*>(this));
					bSetSrt = true;
				}
				else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_FROZEN)
				{
					pMainFrame->m_wndSrtBar.SetIceFrozenObject(static_cast<CIceFrozenObject*>(this));
					bSetSrt = true;
				}
				else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_DEPTHOFFIELD)
				{
					pMainFrame->m_wndSrtBar.SetFloatObject(static_cast<CFloatObject*>(this));
					bSetSrt = true;
				}
				else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_FOCUSBLUR)
				{
					pMainFrame->m_wndSrtBar.SetFocusBlurObject(static_cast<CFocusBlurObject*>(this));
					bSetSrt = true;
				}
			}
		}
	}

	m_iSelMontion = mon;
	m_iSelObj = obj;

	if(bSetSrt == false)
		pMainFrame->m_wndSrtBar.ClearTargetObject();
}

void CModelMotionInfo::SetHotTime(float time)
{
	CObjectLayerInfo* pObj = GetSelectedObj();
	if(pObj == NULL)
		return;


	m_fHotTime = time;
	if(pObj->m_type == Ogre::MOT_MODEL_2)
	{
		int anchor = pObj->m_anchor;
		//m_pCood->BindToModel(CEffectMgr::GetInstance()->GetModelActor(), anchor);

		int iFrameCount = (int)pObj->m_posKey.m_Array.size();

		int tick = (int)((time - pObj->m_start) * 1000);
		Ogre::Vector3 postion, rotAngle;
		Ogre::Quaternion rotation;
		Ogre::Vector3 scale;
		float tran;

		CEffect2LayerInfo* pLayer = (CEffect2LayerInfo*)pObj;
		pLayer->GetSRTAtTime(time, postion, rotation, scale, tran, rotAngle);

		if(time >= pLayer->m_start && time <= pLayer->m_end)
		{
			//m_pCood->SetPosition(postion);
			//m_pCood->SetRotation(C3DMath::XYZAngleToQuat(rotAngle));
			//m_pCood->SetScale(scale);
			SetCoodPlace(pLayer, time);
		}
		if(pObj->m_pEffect != NULL)
		{
			if(pObj->m_pEffect->GetScaleType() != ST_GEOM_SCALING)
			{
				m_pCood->SetGeomScale(false);
			}
			else
			{
				m_pCood->SetGeomScale(true);
			}
		}

		//CMainFrame* pMainFrame = static_cast<CMainFrame*>( theApp.m_pMainWnd) ;
		//CSRTObject* pThis = static_cast<CSRTObject*>(this);
		//if(pThis == pMainFrame->m_wndSrtBar.GetSRTObject())
		//	pMainFrame->m_wndSrtBar.UpdateList();
	}

	CMainFrame* pMainFrame = static_cast<CMainFrame*>( theApp.m_pMainWnd) ;
	pMainFrame->m_wndSrtBar.UpdateList();
	m_timeHolder.SetHotTime(time);
}

void CModelMotionInfo::SetSelectedKey(int key)
{
	CObjectLayerInfo* pObj = GetSelectedObj();
	if(pObj == NULL)
		return;



	if(pObj->m_type == Ogre::MOT_MODEL_2)
	{
		if(key >= 0 && key < (int)pObj->m_posKey.m_Array.size())
		{
			m_pCood->SetEnable(true);
			m_iSelKey = key;
		}
		else
		{
			m_pCood->SetEnable(false);
			m_iSelKey = -1;
		}
	}
	else if((pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_TRANSPARENT)
		|| (pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_ZOOM)
		|| (pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_SHAKE)
		|| (pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_DEPTHOFFIELD)
		|| (pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_FOCUSBLUR)
		)
	{
		m_pCood->SetEnable(false);
		if(key >= 0 && key < (int)pObj->m_transparent_keys.m_Array.size())
			m_iSelKey = key;
		else
			m_iSelKey = -1;
	}
	else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_FSEFFECT)
	{
		m_pCood->SetEnable(false);
		if(key >= 0 && key < (int)pObj->m_fs_colorkeys.m_Array.size())
			m_iSelKey = key;
		else
			m_iSelKey = -1;
	}
	else if(pObj->m_type == Ogre::MOT_POSTEFFECT && pObj->m_posteffect == Ogre::PE_OFFCOLOR)
	{
		m_pCood->SetEnable(false);
		if(key >= 0 && key <(int)pObj->m_fs_colorkeys.m_Array.size())
			m_iSelKey = key;
		else
			m_iSelKey = -1;
	}
	else
	{
		m_pCood->SetEnable(false);
		m_iSelKey = -1;
	}
}

int CModelMotionInfo::FindMotionByFileName(const char* pFileName)
{
	CString strNewFile = pFileName;
	strNewFile.MakeUpper();

	for(int i = 0; i < (int) m_motions.size(); i++)
	{
		MOTION* pExist = m_motions[i];
		CString strExistFile = pExist->filename.c_str();
		strExistFile.MakeUpper();

		if(strExistFile == strNewFile)
			return i;
	}

	return -1;
}

CModelMotionInfo::MOTION*	CModelMotionInfo::FindMotionOfObject(CObjectLayerInfo* pObj)
{
	int index = FindMotionIndexOfObject(pObj);
	if(index >= 0)
		return m_motions[index];
	else
		return NULL;

}

int CModelMotionInfo::FindMotionIndexOfObject(CObjectLayerInfo* pObj)
{
	for(int i = 0; i < (int) m_motions.size(); i++)
	{
		for(int k = 0; k < (int) m_motions[i]->objs.size(); k++)
		{
			if(m_motions[i]->objs[k] == pObj)
				return i;
		}
	}
	return -1;
}

int	CModelMotionInfo::FindObjectIndex(MOTION* pMotion, CObjectLayerInfo* pObj)
{
	if(pObj == NULL)
		return -1;
	for(int i = 0; i < (int) pMotion->objs.size(); i++)
	{
		if (pMotion->objs[i] == pObj) 
		{
			return i;
		}
	}
	return -1;
}

int CModelMotionInfo::AddMotion(CModelMotionInfo::MOTION* pmo)
{
	//pmo->docInfo.NewDoc();

	//if(pmo->bIsPublic == true)
	//{
	//	for(int i = 0; i < (int) m_motions.size(); i++)
	//	{
	//		MOTION* pExist = m_motions[i];
	//		CString strExistFile = pExist->filename.c_str();
	//		CString strNewFile = pmo->filename.c_str();

	//		strExistFile.MakeUpper();
	//		strNewFile.MakeUpper();

	//		if(strExistFile == strNewFile)
	//			return -1;
	//	}
	//}
	m_motions.push_back(pmo);
	return int(m_motions.size()-1);
}

void CModelMotionInfo::MergeMotion(int iFrom, int iTo, bool bDeleteFrom)
{
	if(iFrom >= 0 && iFrom < (int) m_motions.size()
		&& iTo >=0 && iTo < (int) m_motions.size()
		&& iFrom != iTo)
	{
		if(iFrom == m_iSelMontion)
		{
			SetSelect(-1, -1);
		}

		MOTION* pMotionSrc = m_motions[iFrom];
		MOTION* pMotionTo = m_motions[iTo];

		std::map<CObjectLayerInfo*, CObjectLayerInfo*> cloneAddress;

		for(int i = 0; i < (int) pMotionSrc->objs.size(); i++)
		{
			CObjectLayerInfo* pObj = NULL;
			if(bDeleteFrom)
			{
				pObj = pMotionSrc->objs[i];
				pMotionSrc->objs[i] = NULL;
			}
			else
			{
				pObj = pMotionSrc->objs[i]->CreateClone();
				if(pMotionSrc->objs[i]->getParent() != NULL)
				{
					pObj->setParent(cloneAddress[pMotionSrc->objs[i]->getParent()]);
				}
			}
			pMotionTo->objs.push_back(pObj);
			cloneAddress[pMotionSrc->objs[i]] = pObj;
		}

		if(bDeleteFrom)
		{
			pMotionSrc->objs.clear();
			delete pMotionSrc;

			m_motions.erase(m_motions.begin() + iFrom);
		}

		if(iTo == m_iSelMontion)
		{
			SetHotTime(m_fHotTime); //Refresh
		}
	}
}

void CModelMotionInfo::MergeMotion(MOTION* pFrom, int iTo)
{
	if(iTo >=0 && iTo < (int) m_motions.size() && pFrom != NULL)
	{

		MOTION* pMotionSrc = pFrom;
		MOTION* pMotionTo = m_motions[iTo];

		for(int i = 0; i < (int) pMotionSrc->objs.size(); i++)
		{
			CObjectLayerInfo * pObj = pMotionSrc->objs[i];
			pMotionSrc->objs[i] = NULL;
			pMotionTo->objs.push_back(pObj);


		}

		if(iTo == m_iSelMontion)
		{
			SetHotTime(m_fHotTime); //Refresh
		}
	}
}

void CModelMotionInfo::AddObjLayer(int mon, CObjectLayerInfo* pLayer)
{
	if(mon < 0 || mon >= (int)m_motions.size())
	{
		delete pLayer;
		return;
	}
	MOTION* pMotion = m_motions[mon];
	pMotion->objs.push_back(pLayer);
	pMotion->docInfo.Modify();
	if(pLayer->m_type == Ogre::MOT_MODEL_2)
	{
		CEffect2LayerInfo* pEffObj = static_cast<CEffect2LayerInfo*>(pLayer);
		pEffObj->MergeFrame();
	}

	m_timeHolder.SetHotTime(m_fHotTime);
}

void CModelMotionInfo::DeleteObj(int mon, int obj)
{
	if(mon >=0 && obj >= 0)
	{
		CObjectLayerInfo *pcurlayer = m_motions[mon]->objs[obj];
		if((pcurlayer->m_type == Ogre::MOT_MODEL || pcurlayer->m_type == Ogre::MOT_MODEL_2)	&& m_iSelObj == obj && m_iSelMontion == mon)
		{
			m_pCood->UnbindFromModel(CEffectMgr::GetInstance()->GetModelActor());
			m_iSelObj = -1;
		}

		if(pcurlayer->m_type == Ogre::MOT_MODEL || pcurlayer->m_type == Ogre::MOT_MODEL_2)
		{
			bool bFound = false;
			CEffectUnit* pEffect = pcurlayer->m_pEffect;
			for(int i = 0; i < (int)m_motions.size(); i++)
			{
				for(int j = 0; j < (int)m_motions[i]->objs.size(); j++)
				{
					if((m_motions[i]->objs[j]->m_type == Ogre::MOT_MODEL || m_motions[i]->objs[j]->m_type == Ogre::MOT_MODEL_2) && m_motions[i]->objs[j]->m_pEffect ==pEffect)
					{
						bFound = true;
					}
				}
			}
			if (bFound) 
			{
				pcurlayer->RepresentAtTime(-1);
				CEffectMgr::GetInstance()->ChangeEffectType(pcurlayer->m_pEffect, false);
			}
		}

		for(int i = 0; i < (int)m_motions[mon]->objs.size(); i++)
		{
			CObjectLayerInfo *player = m_motions[mon]->objs[i];
			if(player->getParent() == pcurlayer) player->setParent(pcurlayer->getParent());
		}

		delete m_motions[mon]->objs[obj];
		m_motions[mon]->docInfo.Modify();
		m_motions[mon]->objs.erase(m_motions[mon]->objs.begin() + obj);
	}



	CMainFrame* pMainFrame = static_cast<CMainFrame*>( theApp.m_pMainWnd) ;
	CSRTObject* pThis = static_cast<CSRTObject*>(this);
	if(pThis == pMainFrame->m_wndSrtBar.GetSRTObject())
	{
		pMainFrame->m_wndSrtBar.SetSRTObject(NULL);
	}
}

void CModelMotionInfo::DeleteMontion(int mon, bool bRemoveElement)
{
	std::vector<CEffectUnit*> unit;
	//for(int i = 0; i < (int)m_motions[mon]->objs.size(); i++)
	//{
	//DeleteObj(mon, i);
	//if(bRemoveElement == true && m_motions[mon]->objs[i]->m_type == Ogre::MOT_MODEL_2)
	//{
	//	unit.push_back(m_motions[mon]->objs[i]->m_pEffect);
	//}
	//}

	while(m_motions[mon]->objs.size() != 0)
	{
		if(bRemoveElement == true && m_motions[mon]->objs[0]->m_type == Ogre::MOT_MODEL_2)
		{
			unit.push_back(m_motions[mon]->objs[0]->m_pEffect);
		}
		DeleteObj(mon, 0);
	}

	m_motions.erase(m_motions.begin() + mon);

	CMainFrame* pMainFrame = static_cast<CMainFrame*>( theApp.m_pMainWnd) ;
	CSRTObject* pThis = static_cast<CSRTObject*>(this);
	if(pThis == pMainFrame->m_wndSrtBar.GetSRTObject())
	{
		pMainFrame->m_wndSrtBar.SetSRTObject(NULL);
	}

	if(bRemoveElement == true)
	{
		for(int i = 0; i < (int)unit.size(); i++)
		{
			if(unit[i] != NULL)
			{
				SendMainFrameMsg(EEM_DELETE_EFFECT, 0, (LPARAM)unit[i]);
			}
		}
	}
}


void CModelMotionInfo::CreateInteract()
{
	assert(m_pCood == NULL);
	DestroyInteract();
	m_pCood = new CCoord();
	m_pCood->CreateVertex();
	m_pCood->m_fSize = 50;

}

CModelMotionInfo::MOTION* CModelMotionInfo::GetMotionByName(CString name)
{
	name.MakeUpper();
	for(int i = 0; i < (int) m_motions.size(); i++)
	{
		CString strMonName = m_motions[i]->name.c_str();
		strMonName.MakeUpper();
		if(strMonName == name)
			return m_motions[i];
	}

	return NULL;
}

void CModelMotionInfo::DestroyInteract()
{
	//MessageBox(NULL, "DestroyInteract:  begin", "", 0);
	OGRE_RELEASE(m_pCood);
	//MessageBox(NULL, "DestroyInteract:  m_pCood cleared", "", 0);
	m_iSelMontion = -1;
	m_iSelObj = -1;
	m_tranArr = COOD_NONE;;
}

bool CModelMotionInfo::CanMerge(int mon, int obj)
{
	if(mon < 0 || mon >= (int)m_motions.size())
		return false;
	if(obj < 0 || obj < (int)m_motions[mon]->objs.size())
		return false;

	CObjectLayerInfo* pObj = m_motions[mon]->objs[obj];
	if(pObj->m_type != Ogre::MOT_MODEL_2)
		return false;

	return static_cast<CEffect2LayerInfo*>(pObj)->CanMerge();
}

void CModelMotionInfo::MergeLayerKeyFrame(int mon, int obj)
{
	if(mon < 0 || mon >= (int)m_motions.size())
		return;
	if(obj < 0 || obj >= (int)m_motions[mon]->objs.size())
		return;

	CObjectLayerInfo* pObj = m_motions[mon]->objs[obj];
	if(pObj->m_type != Ogre::MOT_MODEL_2)
		return;

	static_cast<CEffect2LayerInfo*>(pObj)->MergeFrame();
}

void CModelMotionInfo::OverwriteMotionFrame(int mon, int obj)
{
	if(mon < 0 || mon >= (int)m_motions.size())
		return;
	if(obj < 0 || obj >= (int)m_motions[mon]->objs.size())
		return;

	CObjectLayerInfo* pObj = m_motions[mon]->objs[obj];
	if(pObj->m_type != Ogre::MOT_MODEL_2)
		return;

	static_cast<CEffect2LayerInfo*>(pObj)->OverwriteMotionFrame();
}

void CModelMotionInfo::OverwriteEffectFrame(int mon, int obj)
{	
	if(mon < 0 || mon >= (int)m_motions.size())
		return;
	if(obj < 0 || obj >= (int)m_motions[mon]->objs.size())
		return;

	CObjectLayerInfo* pObj = m_motions[mon]->objs[obj];
	if(pObj->m_type != Ogre::MOT_MODEL_2)
		return;

	static_cast<CEffect2LayerInfo*>(pObj)->OverwriteEffectFrame();
}

bool CModelMotionInfo::ApartKeyFrame(int mon, int obj)
{
	if(mon < 0 || mon >= (int)m_motions.size())
		return false;
	if(obj < 0 || obj >= (int)m_motions[mon]->objs.size())
		return false;

	CObjectLayerInfo* pObj = m_motions[mon]->objs[obj];
	if(pObj->m_type != Ogre::MOT_MODEL_2)
		return false;

	return static_cast<CEffect2LayerInfo*>(pObj)->ApartKeyFrame();
}

void CModelMotionInfo::Clear()
{
	m_iSelMontion = -1;
	m_iSelObj = -1;

	m_tranArr = COOD_NONE;
	for(int i = 0; i < (int)m_motions.size(); i++)
	{
		for(int j = 0;  j < (int) m_motions[i]->objs.size(); j++)
		{
			delete m_motions[i]->objs[j];
		}
		delete m_motions[i];
	}
	m_motions.clear();
}

void CModelMotionInfo::ClearModelPrivateMotion()
{
	m_iSelMontion = -1;
	m_iSelObj = -1;

	m_tranArr = COOD_NONE;
	for(int i = 0; i < (int)m_motions.size(); i++)
	{
		if(m_motions[i]->bIsPublic == true)
			continue;

		for(int j = 0;  j < (int) m_motions[i]->objs.size(); j++)
		{
			CObjectLayerInfo* pObj = m_motions[i]->objs[j];
			//if(pObj->m_type == Ogre::MOT_MODEL_2)
			//{
			//	if(pObj->m_pEffect != NULL)
			//	{
			//		pObj->m_pEffect->GetBindInfo()->bDynamic = false;
			//	}
			//}
			delete pObj;
		}
		delete m_motions[i];
		m_motions[i] = NULL;
	}

	bool bClear = false;
	while(!bClear)
	{
		bClear = true;
		for(int i = 0; i < (int) m_motions.size(); i++)
		{
			if(m_motions[i] == NULL)
			{
				bClear = false;
				m_motions.erase(m_motions.begin() + i);
				break;
			}
		}
	}
}

int CModelMotionInfo::ChangeLayerOrder(int iMotion, bool* pSrcPlace, int num, int iInsertAfter)
{
	if(iMotion < 0 || iMotion >= (int)m_motions.size())
		return -1;

	MOTION* pMotion = m_motions[iMotion];

	int temp = (int)pMotion->objs.size();
	if(iInsertAfter < 0 || iInsertAfter >= (int)pMotion->objs.size())
		return -1;

	CObjectLayerInfo* after = pMotion->objs[iInsertAfter];

	std::vector<CObjectLayerInfo*> srcObjs;
	std::vector<CObjectLayerInfo*> assObjs;
	int iNewPlace;

	for(UINT i=0; i<num; i++)
	{
		if(i >= pMotion->objs.size())
			break;
		if(pSrcPlace[i])
		{
			CObjectLayerInfo* src = pMotion->objs[i];
			if(after == src->getParent() || after->getParent() == src->getParent())
				srcObjs.push_back(src);
			else
			{
				assObjs.push_back(src);
				pSrcPlace[i] = false; //此item不适合插入到after后
			}
		}
		else
			assObjs.push_back(pMotion->objs[i]);
	}

	for(UINT i=0; i<assObjs.size(); i++)
	{
		if(assObjs[i] == pMotion->objs[iInsertAfter])
		{
			assObjs.insert(assObjs.begin()+i+1, srcObjs.begin(), srcObjs.end());
			iNewPlace = i + srcObjs.size();
			break;
		}
	}
	pMotion->objs = assObjs;
	ArrangeOrder(pMotion);

	return iNewPlace;
}

int CModelMotionInfo::ChangeLayerOrder(int iMotion, int iSrcPlace, int iInsertAfter)
{
	if(iMotion < 0 || iMotion >= (int)m_motions.size())
		return -1;

	MOTION* pMotion = m_motions[iMotion];

	if(iSrcPlace < 0 || iSrcPlace >= (int)pMotion->objs.size())
		return -1;
	if(iInsertAfter < 0 || iInsertAfter >= (int)pMotion->objs.size())
		return -1;
	if(iSrcPlace == iInsertAfter || iSrcPlace == iInsertAfter + 1)
		return -1;

	CObjectLayerInfo* pSrc = pMotion->objs[iSrcPlace];
	CObjectLayerInfo* pAfter = pMotion->objs[iInsertAfter];

	if(pAfter != pSrc->getParent() && pAfter->getParent() != pSrc->getParent())
		return -1;

	int iNewPlace = iInsertAfter + 1;
	pMotion->objs.insert(pMotion->objs.begin() + iNewPlace, pSrc);
	if(iNewPlace < iSrcPlace)
	{
		iSrcPlace++;
	}
	else
	{
		iNewPlace--;
	}

	pMotion->objs.erase(pMotion->objs.begin() + iSrcPlace);

	ArrangeOrder(pMotion);

	return iNewPlace;
}

int CModelMotionInfo::SetParentRelation(CObjectLayerInfo* pSrcObj, CObjectLayerInfo* pParent)
{
	for(int i = 0; i < (int)m_motions.size(); i++)
	{
		int iSrcPlace = -1;
		int iParent = -1;

		for(int k = 0; k < (int) m_motions[i]->objs.size(); k++)
		{
			if (m_motions[i]->objs[k] == pSrcObj)
				iSrcPlace = k;
			else if (m_motions[i]->objs[k] == pParent)
				iParent = k;
		}

		if(iSrcPlace != -1 && iParent != -1)
		{
			return SetParentRelation(i, iSrcPlace, iParent);
		}
	}
	return -1;
}

int CModelMotionInfo::SetParentRelation(int iMotion, bool* pSrcPlace, int num, int iParent)
{

	if(iMotion<0 || iMotion>=(int)m_motions.size())
		return -1;
	MOTION *pMotion = m_motions[iMotion];
	if(iParent<0 || iParent>=(int)pMotion->objs.size())
		return -1;

	std::vector<CObjectLayerInfo*> srcObjs1;
	std::vector<CObjectLayerInfo*> srcObjs; //需要插入的元素及其所有子元素
	std::vector<CObjectLayerInfo*> assObjs;

	int iNewPlace = -1; //记录最后一个插入元素的最新位置

	for(UINT i=0; i<num; i++)
	{
		if(i >= pMotion->objs.size())
			break;
		if(pSrcPlace[i])
		{
			if(pMotion->objs[i]->m_iObjLayerLodLevel == (int)Ogre::MOTION_LOD_NOHANDLE)
			{
				CString strName = pMotion->objs[i]->GetDisplayName();
				strName += " 不处理层不能作为别的层的子层";
				::MessageBox(NULL, strName, "警告 ！", MB_OK);
				return -1;
			}
			srcObjs.push_back(pMotion->objs[i]);
			srcObjs1.push_back(pMotion->objs[i]);
		}
		else
		{
			assObjs.push_back(pMotion->objs[i]);
		}
	}

	//获得所有的选中层及其子层
	for(UINT i=0; i<srcObjs.size(); i++)
	{
		for(UINT j=0; j<pMotion->objs.size(); j++)
		{
			if(pMotion->objs[j]->getParent() == srcObjs[i])
				srcObjs.push_back(pMotion->objs[j]);
		}
	}

	//检查是否插入到某一层的子层中，这种情况不允许
	for(UINT i=0; i<srcObjs.size(); i++)
	{
		if(pMotion->objs[iParent]->getParent() == srcObjs[i])
		{
			::MessageBox(NULL, "你正试图将一些父层插入到其子层中，这种情况不允许","警告", MB_OK);
			return -1;
		}
	}

	/*美术要求将此处的所有子层设置成与父层一个等级，那么就不用考虑其余的情况*/
	if(pMotion->objs[iParent]->m_iObjLayerLodLevel != (int)Ogre::MOTION_LOD_NOHANDLE)
	{//如果父层不是不处理层
		for(UINT i=0; i<srcObjs.size(); i++)
		{
			srcObjs[i]->m_iObjLayerLodLevel = pMotion->objs[iParent]->m_iObjLayerLodLevel;
		}
	}
	for(int i=0; i<assObjs.size(); i++)
	{
		if(assObjs[i] == pMotion->objs[iParent])
		{
			for(int j=0; j<srcObjs1.size(); j++)
				srcObjs1[j]->setParent(pMotion->objs[iParent]);
			assObjs.insert(assObjs.begin()+i+1, srcObjs1.begin(), srcObjs1.end());
			iNewPlace = i + srcObjs1.size();
			break;
		}
	}

	//如果不是不处理层，则将iParent的 LOD 赋值给子层
	if(pMotion->objs[iParent]->m_iObjLayerLodLevel != (int)Ogre::MOTION_LOD_NOHANDLE)
	{
		for(UINT i=0; i<srcObjs.size(); i++)
		{
			for(UINT j=0; j<assObjs.size(); j++)
			{
				if(srcObjs[i] == assObjs[j])
				{
					assObjs[j]->m_iObjLayerLodLevel = srcObjs[i]->m_iObjLayerLodLevel;
					break;
				}
			}
		}
	}

	pMotion->objs = assObjs; //赋值
	ArrangeOrder(pMotion);

	return iNewPlace;
}

int CModelMotionInfo::SetParentRelation(int iMotion, int iSrcPlace, int iParent)
{
	if(iMotion < 0 || iMotion >= (int)m_motions.size())
		return -1;

	MOTION* pMotion = m_motions[iMotion];

	if(iSrcPlace < 0 || iSrcPlace >= (int)pMotion->objs.size())
		return -1;
	if(iParent < 0 || iParent >= (int)pMotion->objs.size())
		return -1;

	CObjectLayerInfo* pSrc = pMotion->objs[iSrcPlace];

	CObjectLayerInfo* pParent = pMotion->objs[iParent];
	if(pParent->ReceiveChild(pSrc) == false)
		return -1;

	CObjectLayerInfo* pTestParent = pParent;
	while(pTestParent != NULL)
	{
		if(pTestParent == pSrc)
			return -1;
		pTestParent = pTestParent->getParent();
	}

	//调整位置到父物体之后
	int iNewPlace = iParent + 1;
	pMotion->objs.insert(pMotion->objs.begin() + iNewPlace, pSrc);
	if(iNewPlace < iSrcPlace)
	{
		iSrcPlace++;
	}
	else
	{
		iNewPlace--;
	}
	pMotion->objs.erase(pMotion->objs.begin() + iSrcPlace);

	pSrc->setParent(pParent);

	ArrangeOrder(pMotion);

	return iNewPlace;

}

void CModelMotionInfo::ArrangeOrder(MOTION* pmo)
{
	std::vector<CObjectLayerInfo*> targetList;
	targetList.push_back(NULL);
	std::vector<CObjectLayerInfo*> tempList;
	std::queue<CObjectLayerInfo*> pending;
	CObjectLayerInfo* pCurrParent = NULL;

	bool bFinished = false;

	while(!bFinished)
	{
		tempList.clear();
		for(int i = 0; i < (int) pmo->objs.size(); i++)
		{
			if(pmo->objs[i]->getParent() == pCurrParent)
			{
				tempList.push_back(pmo->objs[i]);
				pending.push(pmo->objs[i]);
			}
		}

		std::vector<CObjectLayerInfo*>::iterator it = targetList.begin();
		while(*it != pCurrParent && it != targetList.end())
			it++;
		if(it != targetList.end())
			it++;

		targetList.insert(it, tempList.begin(), tempList.end());

		if(pending.size() == 0)
		{
			bFinished = true;
		}
		else
		{
			pCurrParent = pending.front();
			pending.pop();
		}
	}
	targetList.erase(targetList.begin());
	pmo->objs = targetList;

}

bool CModelMotionInfo::LoadSingleMotionFromFile(MOTION *pmo, const char* pFileName)
{
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(pFileName);
	if(pChunk != NULL)
	{
		if(IS_KIND_OF(Ogre::EntityMotionData, pChunk))
		{
			LoadSingleMotion(static_cast<Ogre::EntityMotionData*>(pChunk), pmo, pFileName);
			pmo->filename = pFileName;
			pmo->docInfo.Open();

			OGRE_RELEASE(pChunk);
			return true;
		}
		else
		{
			OGRE_RELEASE(pChunk);

		}
	}
	return false;
}

void CModelMotionInfo::LoadSingleMotion(Ogre::EntityMotionData* pmotionchunk, MOTION *pmo, const char* pFileName)
{
	std::vector<Ogre::Resource *>modelchunks;
	pmo->fTime = pmotionchunk->m_fTime;
	pmo->stMotionLodLevel = pmotionchunk->m_iModLodLevel;
	pmo->name = pmotionchunk->m_strName;
	if(pFileName == NULL) pmo->filename = "";
	else pmo->filename = pFileName;
	pmo->savePos = SP_ON_MODEL;
	pmo->loop = pmotionchunk->m_loop;

	size_t i;
	for(i=0; i<pmotionchunk->m_Elements.size(); i++)
	{
		Ogre::MotionElementData *pelementdata = pmotionchunk->m_Elements[i];

		Ogre::POSTEFFECT_T posteffect = Ogre::PE_MOTION_BLUR;
		if(IS_KIND_OF(Ogre::MotionPostElementData, pelementdata))
		{
			posteffect = static_cast<Ogre::MotionPostElementData *>(pelementdata)->posteffect;
		}
		CObjectLayerInfo* poi = CObjectLayerInfo::CreateObjectLayer(pelementdata->m_type, posteffect);
		if(!poi->loadFromElementData(pelementdata))
		{
			delete poi;
			continue;
		}

		pmo->objs.push_back(poi);
	}

	for(i=0; i<pmotionchunk->m_Elements.size(); i++)
	{
		Ogre::MotionElementData *pelementdata = pmotionchunk->m_Elements[i];
		if(pelementdata->m_iParent >= 0)
		{
			assert(pelementdata->m_iParent < (int)pmo->objs.size());
			pmo->objs[i]->setParent(pmo->objs[pelementdata->m_iParent]);
		}
	}
}

bool CModelMotionInfo::SaveSingleMotionToFile(MOTION* pmo, const char* pFileName, bool bAsBackup)
{
	Ogre::EntityMotionData* pChunk = new Ogre::EntityMotionData();
	SaveSingleMotion(pmo, pChunk);
	pChunk->save(pFileName);
	pChunk->release();

	if(bAsBackup == false)
	{
		pmo->filename = pFileName;
		pmo->docInfo.Save();
	}
	return true;
}

void CModelMotionInfo::SaveSingleMotion(MOTION* pmo, Ogre::EntityMotionData* pMotionChunk, const char* pFileName)
{
	pMotionChunk->m_fTime = pmo->fTime;
	pMotionChunk->m_iModLodLevel = pmo->stMotionLodLevel;
	pMotionChunk->m_strName = pmo->name.c_str();
	pMotionChunk->m_loop = pmo->loop;

	std::map<int, int> indexToEvent;
	for(size_t i=0; i<pmo->objs.size(); i++)
	{
		CObjectLayerInfo& src = *(pmo->objs[i]);

		Ogre::MotionElementData *pelementdata = CObjectLayerInfo::createMotionElementData(src.m_type);
		int parent_index = FindObjectIndex(pmo, src.getParent());
		assert(parent_index != i);
		src.saveToElementData(pelementdata, (int)i, parent_index);

		pMotionChunk->m_Elements.push_back(pelementdata);
	}

	if (pFileName != NULL) 
	{
		pmo->filename = pFileName;
	}

	if(pmo->bIsPublic) pMotionChunk->setResPath(pmo->filename.c_str());
}

bool CModelMotionInfo::SaveMotionObjectAsMotion(MOTION* pSrcMo, int index, const char* pFileName, int nNum, bool* bSave)
{
	MOTION* pMotion = new MOTION;
	pMotion->loop = pSrcMo->loop;

	if( nNum != 0 )
	{
		int nTotal = (int)pSrcMo->objs.size();
		for( int i=0; i<nNum && i<nTotal; i++ )
			if( bSave[i] )
			{
				CObjectLayerInfo* pObj = pSrcMo->objs[i];
				pMotion->objs.push_back( pObj );
			}
	}
	else 
	{
		CObjectLayerInfo* pObj = pSrcMo->objs[index];
		pMotion->objs.push_back(pObj);
	}

	pMotion->name = pSrcMo->name;
	pMotion->fTime = pSrcMo->fTime;
	bool ret = SaveSingleMotionToFile(pMotion, pFileName);

	delete pMotion;

	return ret;
}

void CModelMotionInfo::LoadFromChunk(Ogre::EntityData *pentitydata)
{
	size_t i;
	for(i=0; i<pentitydata->m_Motions.size(); i++)
	{
		Ogre::EntityMotionData *pmotiondata = pentitydata->m_Motions[i];
		MOTION *pmo = new MOTION;
		LoadSingleMotion(pmotiondata, pmo);
		m_motions.push_back(pmo);
	}

	for(i=0; i<pentitydata->m_AddtionAnims.size(); i++)
	{
		Ogre::EntityMotionData *pmotiondata = pentitydata->m_AddtionAnims[i];
		MOTION *pmo = new MOTION;
		LoadSingleMotion(pmotiondata, pmo);
		pmo->bIsPublic = true;
		m_motions.push_back(pmo);
	}

	SendMainFrameMsg(EEM_UPDATA_LIST, UE_EFFECT_LIST, 0);
}

void CModelMotionInfo::CreateMotionChunk(Ogre::EntityData *pentitydata)
{
	if(m_motions.size() == 0)
		return;

	for(int i = 0; i < (int)m_motions.size(); i++)
	{
		Ogre::EntityMotionData* pMotionChunk = new Ogre::EntityMotionData();
		SaveSingleMotion(m_motions[i], pMotionChunk);

		if(m_motions[i]->bIsPublic == false)
		{
			pentitydata->m_Motions.push_back(pMotionChunk);
		}
		else
		{
			pentitydata->m_AddtionAnims.push_back(pMotionChunk);
		}
	}
}

void CModelMotionInfo::CreateSingleMotionChunk(int iMon, Ogre::EntityData *pentitydata)
{
	if(iMon < 0 || iMon >= m_motions.size())
		return;

	Ogre::EntityMotionData* pMotionChunk = new Ogre::EntityMotionData();

	CString strOldName = m_motions[iMon]->name.c_str();
	m_motions[iMon]->name = "default";

	SaveSingleMotion(m_motions[iMon], pMotionChunk);
	pentitydata->m_Motions.push_back(pMotionChunk);

	m_motions[iMon]->name = strOldName.GetBuffer();
}

void CModelMotionInfo::PasteLayers()
{
	assert(m_iSelMontion>=0 && m_iSelMontion<(int)m_motions.size());
	assert(m_iSelObj>=0 && m_iSelObj<(int)m_motions[m_iSelMontion]->objs.size());
	int nSelLayer = m_iSelObj; //刚开始选定的对象层
	int nSrcMotion = gLayersOpObj.stMotion;
	if(gLayersOpObj.stOpType == COPY)
	{
		for(int i=0; i<(int)gLayersOpObj.vLayers.size(); i++)
		{
			int nSrcLayer = gLayersOpObj.vLayers[i];
			CObjectLayerInfo* pCloneObjLayer = m_motions[nSrcMotion]->objs[nSrcLayer]->CreateClone();
			if(pCloneObjLayer == NULL)
			{
				AfxMessageBox("对象层复制出错");
				return;
			}
			pCloneObjLayer->setParent(NULL); //删除它的父对象层
			m_motions[m_iSelMontion]->objs.insert(m_motions[m_iSelMontion]->objs.begin()+(++nSelLayer), pCloneObjLayer);
		}
	}
	else if(gLayersOpObj.stOpType == CUT)
	{
		for(int i=0; i<(int)gLayersOpObj.vLayers.size(); i++)
		{
			int nSrcLayer = gLayersOpObj.vLayers[i];
			CObjectLayerInfo* pObjLayer = m_motions[nSrcMotion]->objs[nSrcLayer];
			if(pObjLayer == NULL)
			{
				AfxMessageBox("对象层剪切出错");
				return;
			}
			//处理其子对象层
			for(int j=0; j<(int)m_motions[nSrcMotion]->objs.size(); j++)
			{
				if(pObjLayer == m_motions[nSrcMotion]->objs[j]->getParent())
					m_motions[nSrcMotion]->objs[j]->setParent(pObjLayer->getParent());
			}
			m_motions[m_iSelMontion]->objs.insert(m_motions[m_iSelMontion]->objs.begin()+(++nSelLayer), pObjLayer);
		}
		for(int i=gLayersOpObj.vLayers.size()-1; i>=0; i--)
		{
			int nSrcLayer = gLayersOpObj.vLayers[i];
			CObjectLayerInfo* pObjLayer = m_motions[nSrcMotion]->objs[nSrcLayer];
			pObjLayer->setParent(NULL); //删除它的父对象层
			m_motions[nSrcMotion]->objs.erase(m_motions[nSrcMotion]->objs.begin()+nSrcLayer);
		}
	}
}