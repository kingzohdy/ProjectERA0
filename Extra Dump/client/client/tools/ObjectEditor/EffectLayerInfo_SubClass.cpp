
#include "stdafx.h"
#include "EffectLayerInfo_SubClass.h"
#include "ObjectTypeHelper.h"
#include "TimeBar.h"
#include "SelectPostEffectDlg.h"
#include "ColorKeysDlg.h"
#include "ZoomKeysDlg.h"
#include "AlphaKeysDlg.h"
#include "EventKeysDlg.h"
#include "BindObjectInfoDlg.h"
#include "EffectManager.h"
#include "ModelMotionInfo.h"
#include "EffectUnitEditBar.h"
#include "BindInfo.h"
#include "KeyframeEditBar.h"
#include "MathUtility.h"
#include "EffectUnit_UnknownObject.h"
#include "MainFrm.h"
#include "ObjectEditor.h"

#include "OgreModel.h"
#include "OgreEntity.h"
#include "OgreLines.h"
#include "OgreResourceManager.h"

//-----------------------------------------------------------------------------------------------------------
CNormalPostLayerInfo::CNormalPostLayerInfo(void)
{
	m_iObjLayerLodLevel = 100;
}

CNormalPostLayerInfo::~CNormalPostLayerInfo(void)
{
}



CString	CNormalPostLayerInfo::GetDisplayName()
{
	CCvtHelper cvt;
	CString ret;
	ret.Format("后期：%s", cvt.PostEffectToStringAsPtr(m_posteffect));
	return ret;
}

void CNormalPostLayerInfo::CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix)
{
	pTimeLayer->m_blocks.clear();

	CTimeArea ta;
	ta.m_bAllowPos = true;
	ta.m_bAllowSize = true;
	ta.m_iFirst = pTimeAix->GetBoxIndex(m_start);
	ta.m_iLen = pTimeAix->GetBoxIndex(m_end) - ta.m_iFirst;

	pTimeLayer->m_blocks.push_back(ta);
	pTimeLayer->m_strName = GetDisplayName();
	pTimeLayer->m_usedOption = 1;
	pTimeLayer->m_options[0] = m_bVisible == true ? 1 : 0;
	pTimeLayer->m_iLodLevel = m_iObjLayerLodLevel;
	pTimeLayer->m_eObjType = m_type;
	pTimeLayer->m_eET = m_eEffectType;
}


void CNormalPostLayerInfo::OnActive(int iMyLayerId, int block, CTimeAixes* pTimeAix)
{
	CSelectPostEffectDlg dlg;
	CCvtHelper cvt;
	dlg.m_fStart = m_start;
	dlg.m_fEnd = m_end;
	dlg.m_bNoStart = m_hasStart ? FALSE : TRUE;
	dlg.m_bNoEnd = m_hasEnd ? FALSE : TRUE;
	dlg.m_strPostEffect = cvt.PostEffectToString(m_posteffect);
	dlg.m_fsColor = m_fs_color;
	dlg.m_fsColorKeys = m_fs_colorkeys;

	if(IDOK == dlg.DoModal())
	{
		m_start = dlg.m_fStart;
		m_end = dlg.m_fEnd;
		m_hasStart = (dlg.m_bNoStart==TRUE) ? false : true;
		m_hasEnd = (dlg.m_bNoEnd==TRUE) ? false : true;
		m_fs_color = dlg.m_fsColor;
		m_posteffect = cvt.StringToPostEffect(dlg.m_strPostEffect);
		if(m_posteffect == Ogre::PE_CUSTOM)
			m_customPE = dlg.m_customPE;
		//UpdateTimeBarList();
		//CEffectMgr::GetInstance()->GetModelMotion()->SetSelect(iMotion, pTAHDR->layer);
	}
}
//-----------------------------------------------------------------------------------------------------------
CCameraShakeLayerInfo::CCameraShakeLayerInfo()
{
	m_iObjLayerLodLevel = 100;
}

CCameraShakeLayerInfo::~CCameraShakeLayerInfo()
{
}

CString CCameraShakeLayerInfo::GetDisplayName()
{
	return "相机震动";
}

void CCameraShakeLayerInfo::CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix)
{
	pTimeLayer->m_blocks.clear();

	for(size_t i=0; i<m_transparent_keys.m_Array.size(); i++)
	{
		CTimeArea ta;
		ta.m_iLen = 1;
		ta.m_iFirst = pTimeAix->GetBoxIndex(m_transparent_keys.m_Array[i].tick/1000.0f);
		ta.m_bAllowPos = (i==0)?false:true;
		ta.m_bAllowSize = false;
		pTimeLayer->m_blocks.push_back(ta);
	}

	pTimeLayer->m_strName = GetDisplayName();
	pTimeLayer->m_usedOption = 1;
	pTimeLayer->m_options[0] = m_bVisible?1:0;
	pTimeLayer->m_iLodLevel = m_iObjLayerLodLevel;
	pTimeLayer->m_eObjType = m_type;
	pTimeLayer->m_eET = m_eEffectType;
}

void CCameraShakeLayerInfo::OnActive(int iMyLayerId, int block, CTimeAixes* pTimeAix)
{
	int area = pTimeAix->GetLayer(iMyLayerId)->IsFirstBlock(block);
	if(area>=0 || block==-1)
	{
	}
	else
	{
		float val = pTimeAix->GetBoxValue(block);
		Ogre::uint tick = TimeToTick(val);
		size_t pos = 0;
		while(m_transparent_keys.m_Array.size() > pos)
		{
			if(m_transparent_keys.m_Array[pos].tick > tick) break;
			pos++;
		}

		KeyFrameArray<float>::KEYFRAME_T newframe;
		newframe.tick = tick;

		float v1;
		float v2;
		float t;
		int prevtick = 0;
		int nexttick = 0;
		if(m_transparent_keys.m_Array.size() == 0)
			v1 = 0.0f;
		else
			v1 = m_transparent_keys.m_Array[pos-1].data;

		if(pos >= (int)m_transparent_keys.m_Array.size())
			v2 = v1;
		else
			v2 = m_transparent_keys.m_Array[pos].data;

		if(m_transparent_keys.m_Array.size() == 0 || pos >= (int)m_transparent_keys.m_Array.size())
			t = 0;
		else
		{
			prevtick = (int)m_transparent_keys.m_Array[pos - 1].tick;
			nexttick = (int)m_transparent_keys.m_Array[pos].tick;
			if(prevtick == nexttick)
				t = 0;
			else
				t = (tick - prevtick) / (float)( nexttick - prevtick );
		}

		newframe.data = v1 * (1 - t) + v2 * t;

		m_transparent_keys.m_Array.insert(m_transparent_keys.m_Array.begin() + pos, newframe);
		m_hasStart = false;
		m_hasEnd = false;	
	}
}

void CCameraShakeLayerInfo::OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix)
{
	float val = pTimeAix->GetLayerStartValue(iMyLayerId, iArea);
	Ogre::uint tick = TimeToTick(val);
	m_transparent_keys.m_Array[iArea].tick = tick;
}

//-----------------------------------------------------------------------------------------------------------
CFSEffectLayerInfo::CFSEffectLayerInfo(void)
{
	m_iObjLayerLodLevel = 100;
}

CFSEffectLayerInfo::~CFSEffectLayerInfo(void)
{
}

CString	CFSEffectLayerInfo::GetDisplayName()
{
	return "画面泛色";
}

void CFSEffectLayerInfo::CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix)
{
	pTimeLayer->m_blocks.clear();

	for(int j = 0; j < (int)m_fs_colorkeys.m_Array.size(); j++)
	{
		CTimeArea ta;
		ta.m_iLen = 1;
		ta.m_iFirst = pTimeAix->GetBoxIndex(m_fs_colorkeys.m_Array[j].tick / 1000.0f);
		ta.m_bAllowPos = (j == 0) ? false : true;
		ta.m_bAllowSize = false;
		pTimeLayer->m_blocks.push_back(ta);

	}
	pTimeLayer->m_strName = GetDisplayName();
	pTimeLayer->m_usedOption = 1;
	pTimeLayer->m_options[0] = m_bVisible == true ? 1 : 0;
	pTimeLayer->m_iLodLevel = m_iObjLayerLodLevel;
	pTimeLayer->m_eObjType = m_type;
	pTimeLayer->m_eET = m_eEffectType;
}

void CFSEffectLayerInfo::OnActive(int iMyLayerId, int block, CTimeAixes* pTimeAix)
{
	int area = pTimeAix->GetLayer(iMyLayerId)->IsFirstBlock(block);
	if(area >= 0 || block == -1)
	{
		CColorKeysDlg fsdlg;
		fsdlg.m_colorkeys = m_fs_colorkeys;
		fsdlg.SetCurKf(area);
		if(IDOK == fsdlg.DoModal())
		{
			m_fs_colorkeys = fsdlg.m_colorkeys;
		}
	}
	else
	{
		float value = pTimeAix->GetBoxValue(block);
		int tick =(int)( value * 1000);
		int pos = 0;
		while((int)m_fs_colorkeys.m_Array.size() > pos)
		{
			if((int)m_fs_colorkeys.m_Array[pos].tick > tick)
				break;
			pos++;
		}
		Ogre::KeyFrameArray<Ogre::Vector4>::KEYFRAME_T newframe;
		newframe.tick = tick;

		Ogre::Vector4 v1;
		Ogre::Vector4 v2;
		float t;
		int prevtick = 0;
		int nexttick = 0;
		if(m_fs_colorkeys.m_Array.size() == 0)
			v1 = Ogre::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		else
			v1 = m_fs_colorkeys.m_Array[pos-1].data;

		if(pos >= (int)m_fs_colorkeys.m_Array.size())
			v2 = v1;
		else
			v2 = m_fs_colorkeys.m_Array[pos].data;

		if(m_fs_colorkeys.m_Array.size() == 0 || pos >= (int)m_fs_colorkeys.m_Array.size())
			t = 0;
		else
		{
			prevtick = (int)m_fs_colorkeys.m_Array[pos - 1].tick;
			nexttick = (int)m_fs_colorkeys.m_Array[pos].tick;
			if(prevtick == nexttick)
				t = 0;
			else
				t = (tick - prevtick) / (float)( nexttick - prevtick );
		}

		newframe.data = v1 * (1 - t) + v2 * t;

		m_fs_colorkeys.m_Array.insert(m_fs_colorkeys.m_Array.begin() + pos, newframe);
		m_hasStart = false;
		m_hasEnd = false;
	}
}

void CFSEffectLayerInfo::OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix)
{
	Ogre::uint tick;
	float fltVal = pTimeAix->GetLayerStartValue(iMyLayerId, iArea);
	tick = (Ogre::uint)( fltVal * 1000);
	m_fs_colorkeys.m_Array[iArea].tick = tick;
}

//-----------------------------------------------------------------------------------------------------------


CZoomLayerInfo::CZoomLayerInfo(void)
{
	m_iObjLayerLodLevel = 100;
}

CZoomLayerInfo::~CZoomLayerInfo(void)
{
}


CString	CZoomLayerInfo::GetDisplayName()
{
	return "焦距拉伸";
}

void CZoomLayerInfo::CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix)
{
	pTimeLayer->m_blocks.clear();

	for(int j = 0; j < (int)m_transparent_keys.m_Array.size(); j++)
	{
		CTimeArea ta;
		ta.m_iLen = 1;
		ta.m_iFirst = pTimeAix->GetBoxIndex(m_transparent_keys.m_Array[j].tick / 1000.0f);
		ta.m_bAllowPos = (j == 0) ? false : true;
		ta.m_bAllowSize = false;
		pTimeLayer->m_blocks.push_back(ta);

	}
	pTimeLayer->m_strName = GetDisplayName();
	pTimeLayer->m_usedOption = 1;
	pTimeLayer->m_options[0] = m_bVisible == true ? 1 : 0;
	pTimeLayer->m_iLodLevel = m_iObjLayerLodLevel;
	pTimeLayer->m_eObjType = m_type;
	pTimeLayer->m_eET = m_eEffectType;
}

void CZoomLayerInfo::OnActive(int iMyLayerId, int block, CTimeAixes* pTimeAix)
{
	int area = pTimeAix->GetLayer(iMyLayerId)->IsFirstBlock(block);
	if(area >= 0 || block == -1)
	{
		CZoomKeysDlg zmdlg;
		zmdlg.m_zoomkeys  = m_transparent_keys;
		zmdlg.SetCurKf(area);
		if(IDOK == zmdlg.DoModal())
		{
			m_transparent_keys = zmdlg.m_zoomkeys;
		}
	}
	else
	{
		float value = pTimeAix->GetBoxValue(block);
		int tick =(int)( value * 1000);
		int pos = 0;
		while((int)m_transparent_keys.m_Array.size() > pos)
		{
			if((int)m_transparent_keys.m_Array[pos].tick > tick)
				break;
			pos++;
		}
		Ogre::KeyFrameArray<float>::KEYFRAME_T newframe;
		newframe.tick = tick;

		float v1;
		float v2;
		float t;
		int prevtick = 0;
		int nexttick = 0;
		if(m_transparent_keys.m_Array.size() == 0)
			v1 = 0.0f;
		else
			v1 = m_transparent_keys.m_Array[pos-1].data;

		if(pos >= (int)m_transparent_keys.m_Array.size())
			v2 = v1;
		else
			v2 = m_transparent_keys.m_Array[pos].data;

		if(m_transparent_keys.m_Array.size() == 0 || pos >= (int)m_transparent_keys.m_Array.size())
			t = 0;
		else
		{
			prevtick = (int)m_transparent_keys.m_Array[pos - 1].tick;
			nexttick = (int)m_transparent_keys.m_Array[pos].tick;
			if(prevtick == nexttick)
				t = 0;
			else
				t = (tick - prevtick) / (float)( nexttick - prevtick );
		}

		newframe.data = v1 * (1 - t) + v2 * t;

		m_transparent_keys.m_Array.insert(m_transparent_keys.m_Array.begin() + pos, newframe);
		m_hasStart = false;
		m_hasEnd = false;

	}

}

void CZoomLayerInfo::OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix)
{
	Ogre::uint tick;
	float fltVal = pTimeAix->GetLayerStartValue(iMyLayerId, iArea);
	tick = (Ogre::uint)( fltVal * 1000);
	m_transparent_keys.m_Array[iArea].tick = tick;
}

//-------------------------------------------------------------------------------------------------------------------------------
CTransLayerInfo::CTransLayerInfo(void)
{
	m_iObjLayerLodLevel = 100;
}

CTransLayerInfo::~CTransLayerInfo(void)
{
}


CString	CTransLayerInfo::GetDisplayName()
{
	return "角色透明";
}

void CTransLayerInfo::CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix)
{
	pTimeLayer->m_blocks.clear();

	for(int j = 0; j < (int)m_transparent_keys.m_Array.size(); j++)
	{
		CTimeArea ta;
		ta.m_iLen = 1;
		ta.m_iFirst = pTimeAix->GetBoxIndex(m_transparent_keys.m_Array[j].tick / 1000.0f);
		ta.m_bAllowPos = (j == 0) ? false : true;
		ta.m_bAllowSize = false;
		pTimeLayer->m_blocks.push_back(ta);

	}

	pTimeLayer->m_strName = GetDisplayName();
	pTimeLayer->m_usedOption = 1;
	pTimeLayer->m_options[0] = m_bVisible == true ? 1 : 0;
	pTimeLayer->m_iLodLevel = m_iObjLayerLodLevel;
	pTimeLayer->m_eObjType = m_type;
	pTimeLayer->m_eET = m_eEffectType;
}

void CTransLayerInfo::OnActive(int iMyLayerId, int block, CTimeAixes* pTimeAix)
{
	int area = pTimeAix->GetLayer(iMyLayerId)->IsFirstBlock(block);
	if(area >= 0 || block == -1)
	{
		CAlphaKeysDlg tpdlg;
		tpdlg.m_alphakeys  = m_transparent_keys;
		tpdlg.SetCurKf(area);
		if(IDOK == tpdlg.DoModal())
		{
			m_transparent_keys = tpdlg.m_alphakeys;
		}
	}
	else
	{
		float value = pTimeAix->GetBoxValue(block);
		int tick =(int)( value * 1000);
		int pos = 0;
		while((int)m_transparent_keys.m_Array.size() > pos)
		{
			if((int)m_transparent_keys.m_Array[pos].tick > tick)
				break;
			pos++;
		}
		Ogre::KeyFrameArray<float>::KEYFRAME_T newframe;
		newframe.tick = tick;

		float v1;
		float v2;
		float t;
		int prevtick = 0;
		int nexttick = 0;
		if(m_transparent_keys.m_Array.size() == 0)
			v1 = 0.0f;
		else
			v1 = m_transparent_keys.m_Array[pos-1].data;

		if(pos >= (int)m_transparent_keys.m_Array.size())
			v2 = v1;
		else
			v2 = m_transparent_keys.m_Array[pos].data;

		if(m_transparent_keys.m_Array.size() == 0 || pos >= (int)m_transparent_keys.m_Array.size())
			t = 0;
		else
		{
			prevtick = (int)m_transparent_keys.m_Array[pos - 1].tick;
			nexttick = (int)m_transparent_keys.m_Array[pos].tick;
			if(prevtick == nexttick)
				t = 0;
			else
				t = (tick - prevtick) / (float)( nexttick - prevtick );
		}

		newframe.data = v1 * (1 - t) + v2 * t;

		m_transparent_keys.m_Array.insert(m_transparent_keys.m_Array.begin() + pos, newframe);
		m_hasStart = false;
		m_hasEnd = false;

	}
}

void CTransLayerInfo::OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix)
{
	Ogre::uint tick;
	float fltVal = pTimeAix->GetLayerStartValue(iMyLayerId, iArea);
	tick = (Ogre::uint)( fltVal * 1000);
	m_transparent_keys.m_Array[iArea].tick = tick;
}

//-------------------------------------------------------------------------------------------------------------------------------

CEventLayerInfo::CEventLayerInfo()
{
}

CEventLayerInfo::~CEventLayerInfo()
{
	for(int i = 0; i < (int) m_events.size(); i++)
	{
		delete m_events[i];
	}
	m_events.clear();
}


CString CEventLayerInfo::GetDisplayName()
{
	return "事件层";
}

void CEventLayerInfo::CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix) 
{
	pTimeLayer->m_blocks.clear();

	for(int j = 0; j < (int)m_events.size(); j++)
	{
		CTimeArea ta;
		ta.m_iLen = 1;
		ta.m_iFirst = pTimeAix->GetBoxIndex(m_events[j]->m_fTime);
		ta.m_bAllowPos = true ;
		ta.m_bAllowSize = false;
		pTimeLayer->m_blocks.push_back(ta);

	}
	pTimeLayer->m_strName = GetDisplayName();
	pTimeLayer->m_usedOption = 0;
	pTimeLayer->m_eObjType = m_type;
}

void CEventLayerInfo::OnActive(int iMyLayerId, int block,  CTimeAixes* pTimeAix)
{
	int area = pTimeAix->GetLayer(iMyLayerId)->IsFirstBlock(block);

	if(area >= 0 && area < m_events.size())
	{
		CEventKeysDlg dlg;
		dlg.m_EventNames = m_events[area]->m_EventNames;

		if(IDOK == dlg.DoModal())
		{
			m_events[area]->m_EventNames = dlg.m_EventNames;
		}
	}
	else
	{
		float value = pTimeAix->GetBoxValue(block);
		int pos = 0;
		while((int)m_events.size() > pos)
		{
			if(m_events[pos]->m_fTime > value)
				break;
			pos++;
		}

		CMotionEventList* pEventList = new CMotionEventList();
		pEventList->m_fTime = value;
		m_events.insert(m_events.begin() + pos, pEventList);
		m_hasStart = false;
		m_hasEnd = false;

	}
}

void CEventLayerInfo::OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix)
{
	float fltVal = pTimeAix->GetLayerStartValue(iMyLayerId, iArea);

	if(iArea >= 0 && iArea < (int)m_events.size())
	{
		m_events[iArea]->m_fTime = fltVal;
	}
}

void CEventLayerInfo::OnSizeChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix)
{
}

void CEventLayerInfo::OnSelected(int iMyLayerId,  CTimeAixes* pTimeAix)
{
}

void CEventLayerInfo::OnDeleteFrame(int iMyLayerId, int iArea, CTimeAixes* pTimeAix)
{
	if(iArea >= 0 && iArea < (int)m_events.size())
	{
		delete(m_events[iArea]);
		m_events.erase(m_events.begin() + iArea);
	}
}

void CEventLayerInfo::OnSelectFrame(int iMyLayerId, int iArea, CTimeAixes* pTimeAix)
{
}

//---------------------------------------------------------------------------------------------------------------------


CEffect2LayerInfo::CEffect2LayerInfo(void)
{
	m_bShowing = false;
	m_iObjLayerLodLevel = 100;
	m_bFolded = false;
	m_SortIndex = 0;
	m_bSelfXparent = false;
}

CEffect2LayerInfo::~CEffect2LayerInfo(void)
{
}


CString	CEffect2LayerInfo::GetDisplayName()
{
	if(m_pEffect == NULL)
		return "";
	return m_pEffect->GetName();;
}

#define MIN_KFTIME (1.0f/30.0f)

void CEffect2LayerInfo::CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix)
{
	pTimeLayer->m_blocks.clear();
	int iLastPos = 0;
	for(int j = 0; j < (int)m_posKey.m_Array.size(); j++)
	{
		CTimeArea ta;
		ta.m_iLen = 1;
		ta.m_iFirst = pTimeAix->GetBoxIndex(m_posKey.m_Array[j].tick / 1000.0f + m_start);
		ta.m_bAllowPos = true;
		ta.m_bAllowSize = false;
		ta.m_bAllowDrag = true;
		pTimeLayer->m_blocks.push_back(ta);

		iLastPos = ta.m_iFirst;

	}

	CTimeArea ta;
	ta.m_iLen = 1;
	ta.m_iFirst = pTimeAix->GetBoxIndex(m_end + MIN_KFTIME);
	ta.m_bAllowPos = true;
	ta.m_bAllowSize = false;
	ta.m_bAllowDrag = false;
	ta.m_clrFirst = RGB(230, 150, 60);

	int iCount = (int) m_posKey.m_Array.size();
	if(iCount > 0)
	{
		float fLastKeyTime = m_posKey.m_Array[iCount-1].tick * 0.001f;
		float fSub = m_end - fLastKeyTime;
		if(fSub >= 0.0f && fSub < MIN_KFTIME)
		{
			ta.m_iFirst = iLastPos + 1;
		}
	}

	//ta.m_clrEnd = RGB(200, 200, 100);
	//ta.m_clrContain = RGB(200, 200, 100);

	pTimeLayer->m_blocks.push_back(ta);
	pTimeLayer->m_usedOption = 1;
	pTimeLayer->m_options[0] = m_bVisible == true ? 1 : 0;
	if(m_eEffectType == EFFECT_VIRTUALNODE)
	{
		pTimeLayer->m_usedOption = 2; //表示虚拟体的折叠
		pTimeLayer->m_options[1] = 0;
	}
	pTimeLayer->m_strName = GetDisplayName();
	pTimeLayer->m_iLodLevel = m_iObjLayerLodLevel;
	pTimeLayer->m_bReverseSearch = true;
	pTimeLayer->m_level = GetChildLevel();
	pTimeLayer->m_eObjType = m_type;
	pTimeLayer->m_eET = m_eEffectType;
}

void CEffect2LayerInfo::OnActive(int iMyLayerId, int block, CTimeAixes* pTimeAix)
{
	CModelMotionInfo* pMotionInfo = CEffectMgr::GetInstance()->m_modelMotion;

	int area = pTimeAix->GetLayer(iMyLayerId)->IsFirstBlock(block);
	if(area >= 0 && area < (int)m_posKey.m_Array.size() || block == -1)
	{
		CBindObjectInfoDlg dlg;
		dlg.m_iAnchor = m_anchor;
		dlg.m_fStart = m_start;
		dlg.m_fEnd = m_end;
		dlg.m_fx = 0.0f;
		dlg.m_fy = 0.0f;
		dlg.m_fz = 0.0f;
		dlg.m_pCurrObj = this;
		dlg.m_SortIndex = m_SortIndex;
		int iOldAnchor = m_anchor;
		if(m_pEffect != NULL)
		{
			dlg.m_strName = m_pEffect->GetName();
			//CModelMotionInfo::MOTION* pMotion = pMotionInfo->
		}


		if(IDOK == dlg.DoModal())
		{
			m_anchor = dlg.m_iAnchor;
			if(m_anchor != iOldAnchor)
			{
				CEffectMgr::GetInstance()->RefreshEffect(m_pEffect);
			}

			m_start = dlg.m_fStart;

			if(dlg.m_fEnd > m_posKey.m_Array[m_posKey.m_Array.size() - 1].tick * 0.001f)
			{
				m_end = dlg.m_fEnd;
			}

			if(m_pEffect != NULL)
			{
				m_pEffect->SetName(dlg.m_strName);
			}

			if(dlg.m_pSelectedParent != getParent())
			{
				setParent(dlg.m_pSelectedParent);
				pMotionInfo->SetParentRelation(this, dlg.m_pSelectedParent);
			}

			m_SortIndex = dlg.m_SortIndex;

			updatePosInterpolType();
		}
	}
	else if(area == (int)m_posKey.m_Array.size())
	{
		//...
	}
	else
	{
		float ogrValue = pTimeAix->GetBoxValue(block);
		float value = ogrValue - m_start;
		if(value < 0)
			return;

		int orgTick = (int)(ogrValue * 1000);
		int tick =(int)( value * 1000);
		InsertMonKeyFrameAtPos(orgTick);

		CEffectEditBar* pBar = static_cast<CMainFrame*>(theApp.m_pMainWnd)->GetEffectBarByEffect(m_pEffect);
		CKeyFrameObject* pKfObj = NULL;
		if(pBar != NULL 
			&& (pKfObj = pBar->GetKeyFramObj()) != NULL
			&& m_pEffect->GetBindInfo()->bCommKeyFrame == true)
		{
			pKfObj->AddKeyFrame(tick);
		}

		CKeyFramePropertyObject* pKeyFrameProp = m_pEffect->GetPropertyObject();
		if(pKeyFrameProp != NULL && m_pEffect->GetBindInfo()->bCommKeyFrame == true) 
		{
			int iNewPos = pKeyFrameProp->AddKeyFrame(value);
		}

		//int pos = 0;
		//while((int)m_posKey.m_Array.size() > pos)
		//{
		//	if((int)m_posKey.m_Array[pos].tick > tick)
		//		break;
		//	pos++;
		//}
		//CModelMotionInfo* pMotion =  CEffectMgr::GetInstance()->GetModelMotion();
		//pMotion->SetHotTime(tick / 1000.0f);
		//
		//if(pos >= 0 && pos < m_posKey.m_Array.size())
		//{
		//	CEffectMgr::GetInstance()->m_modelMotion.SetSelectedKey(pos);
		//}

	}
}

void CEffect2LayerInfo::OnCtrlDrag(int iMyLayerId, int iSrcArea, int iNewBoxPos,  CTimeAixes* pTimeAix)
{
	if(iSrcArea < 0)
	{
		return;
	}

	float ogrValue = pTimeAix->GetBoxValue(iNewBoxPos);
	float value = ogrValue - m_start;
	if(value < 0)
	{
		return;
	}

	int orgTick = (int)(ogrValue * 1000);
	int tick =(int)( value * 1000);
	int newpos = InsertMonKeyFrameAtPos(orgTick);
	if(iSrcArea >= newpos)
	{
		++iSrcArea;
	}

	m_posKey.m_Array[newpos].data = m_posKey.m_Array[iSrcArea].data;
	//m_rotKey.m_Array[newpos].data = m_rotKey.m_Array[iSrcArea].data;
	m_rotAngleKey.m_Array[newpos].data = m_rotAngleKey.m_Array[iSrcArea].data;
	m_scaleKey.m_Array[newpos].data = m_scaleKey.m_Array[iSrcArea].data;
	m_transparent_keys.m_Array[newpos].data = m_transparent_keys.m_Array[iSrcArea].data;

	CEffectEditBar* pBar = static_cast<CMainFrame*>(theApp.m_pMainWnd)->GetEffectBarByEffect(m_pEffect);
	CKeyFrameObject* pKfObj = NULL;
	if(pBar != NULL 
		&& (pKfObj = pBar->GetKeyFramObj()) != NULL
		&& m_pEffect->GetBindInfo()->bCommKeyFrame == true)
	{
		int objNewPos = pKfObj->AddKeyFrame(tick);
		assert(objNewPos == newpos);
		pKfObj->CopyKeyFrame(iSrcArea, objNewPos);
	}

	CKeyFramePropertyObject* pKeyFrameProp = m_pEffect->GetPropertyObject();
	if(pKeyFrameProp != NULL && m_pEffect->GetBindInfo()->bCommKeyFrame == true) 
	{
		int iNewPos = pKeyFrameProp->AddKeyFrame(value);
		pKeyFrameProp->CopyKeyFrame(iSrcArea, iNewPos);
	}
}

void CEffect2LayerInfo::GetSRTAtTime(float fTime, Ogre::Vector3& pos, Ogre::Quaternion& rot, Ogre::Vector3& scale, float& trans, Ogre::Vector3& rotangle)
{
	int kfpos = 0;
	int tick = int((fTime - m_start) * 1000);
	while((int)m_posKey.m_Array.size() > kfpos)
	{
		if((int)m_posKey.m_Array[kfpos].tick > tick)
			break;
		kfpos++;
	}

	Ogre::Vector3 vt1, vs1;
	Ogre::Vector3 vt2, vs2;
	//Ogre::Quaternion qr1;
	//Ogre::Quaternion qr2;
	Ogre::Vector3 vr1, vr2;
	float tr1, tr2;
	float t;
	int prevtick = 0;
	int nexttick = 0;
	if(m_posKey.m_Array.size() == 0)
	{
		vt1 = Ogre::Vector3(0.0f, 0.0f, 0.0f);
		//qr1.identity();
		vr1 = Ogre::Vector3(0.0f, 0.0f, 0.0f);
		vs1 = Ogre::Vector3(1.0f, 1.0f, 1.0f);
		tr1 = 1.0f;
	}
	else
	{
		int i = kfpos>0?kfpos-1:0;
		vt1 = m_posKey.m_Array[i].data;
		//qr1 = m_rotKey.m_Array[i].data;
		vr1 = m_rotAngleKey.m_Array[i].data;
		vs1 = m_scaleKey.m_Array[i].data;
		tr1 = m_transparent_keys.m_Array[i].data;
	}

	if(kfpos >= (int)m_posKey.m_Array.size())
	{
		vt2 = vt1;
		//qr2 = qr1;
		vr2 = vr1;
		vs2 = vs1;
		tr2 = tr1;
	}
	else
	{
		vt2 = m_posKey.m_Array[kfpos].data;
		//qr2 = m_rotKey.m_Array[kfpos].data;
		vr2 = m_rotAngleKey.m_Array[kfpos].data;
		vs2 = m_scaleKey.m_Array[kfpos].data;
		tr2 = m_transparent_keys.m_Array[kfpos].data;
	}

	if(m_posKey.m_Array.size()<=1 || kfpos >= (int)m_posKey.m_Array.size())
		t = 0;
	else
	{
		int i = kfpos>0?kfpos-1:0;
		prevtick = (int)m_posKey.m_Array[i].tick;
		nexttick = (int)m_posKey.m_Array[kfpos].tick;
		if(prevtick == nexttick)
			t = 0;
		else
			t = (tick - prevtick) / (float)( nexttick - prevtick );
	}

	pos = vt1 * (1 - t) + vt2 * t;
	m_posKey.getValue(0, Ogre::TimeToTick(fTime-m_start), &pos);

	//rot = qr1 * (1 - t) + qr2 * t;
	rotangle = vr1 * (1 - t) + vr2 * t;
	scale = vs1 * (1 - t) + vs2 * t;
	trans = tr1 * (1 - t) + tr2 * t;
}

void CEffect2LayerInfo::GetWorldSRTAtTime(float fTime, Ogre::Vector3& pos, Ogre::Quaternion& rotByAngle, Ogre::Vector3& scale)
{
	pos = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	rotByAngle.identity();
	scale = Ogre::Vector3(1.0f, 1.0f, 1.0f);

	Ogre::Vector3 vLocalPos, vLocalScale, vAngle;
	Ogre::Quaternion qLocalRotByAngle, tmp;
	float fTran;
	qLocalRotByAngle.identity();

	CEffect2LayerInfo* pCurrLevel = this;
	Ogre::Matrix4 matPos;
	Ogre::Matrix4 matRot;
	Ogre::Matrix4 matScale;

	Ogre::Matrix4 mat;
	mat.identity();

	while(pCurrLevel != NULL)
	{	
		pCurrLevel->GetSRTAtTime(fTime, vLocalPos, tmp, vLocalScale, fTran, vAngle);

		pos += vLocalPos;
		qLocalRotByAngle = C3DMath::XYZAngleToQuat(vAngle);
		rotByAngle *= qLocalRotByAngle;
		scale *= vLocalScale;	

		matPos.makeTranslateMatrix(vLocalPos);
		qLocalRotByAngle.getMatrix(matRot);
		matScale.makeScaleMatrix(vLocalScale);


		mat *= matScale * matRot * matPos;

		if(pCurrLevel->GetParentNode() != NULL && pCurrLevel->m_anchor != 0)
		{
			//mat *= pCurrLevel->GetParentNode()->GetAnchorWorldMatrix(pCurrLevel->m_anchor);
			break;

		}

		if(pCurrLevel->getParent() == NULL)
		{
			if(pCurrLevel->m_anchor != 0)
			{
				//mat *= CEffectMgr::GetInstance()->GetModelActor()->GetMainNode()->GetAnchorWorldMatrix(pCurrLevel->m_anchor);
			}
			break;

		}
		pCurrLevel = static_cast<CEffect2LayerInfo*>( pCurrLevel->getParent());
	}
	pos = mat.getTranslate();

}

int CEffect2LayerInfo::InsertMonKeyFrameAtPos(Ogre::uint tick, int iCopyFrom)
{
	int pos = 0;
	int starttick =int( m_start * 1000);
	while((int)m_posKey.m_Array.size() > pos)
	{
		if((int)m_posKey.m_Array[pos].tick > tick - starttick)
			break;
		pos++;
	}
	Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T newframePos;
	Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T newframeRotAngle;
	Ogre::KeyFrameArray<Ogre::Quaternion>::KEYFRAME_T newframeRot;
	Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T newframeScale;
	Ogre::KeyFrameArray<float>::KEYFRAME_T newframeTrans;
	newframePos.tick = tick - starttick;
	newframeRotAngle.tick = tick - starttick;
	newframeScale.tick = tick - starttick;
	newframeTrans.tick = tick - starttick;

	if(iCopyFrom >= 0 && iCopyFrom < (int)m_posKey.m_Array.size())
	{
		newframePos.data = m_posKey.m_Array[iCopyFrom].data;
		//newframeRot.data = m_rotKey.m_Array[iCopyFrom].data;
		newframeRotAngle.data = m_rotAngleKey.m_Array[iCopyFrom].data;
		newframeScale.data = m_scaleKey.m_Array[iCopyFrom].data;
		newframeTrans.data = m_transparent_keys.m_Array[iCopyFrom].data;
	}
	else
	{
		GetSRTAtTime(tick / 1000.0f, newframePos.data, newframeRot.data, newframeScale.data, newframeTrans.data, newframeRotAngle.data);
	}

	//newframePos.data = vt1 * (1 - t) + vt2 * t;
	//newframeRot.data = qr1 * (1 - t) + qr2 * t;
	//newframeScale.data = vs1 * (1 - t) + vs2 * t;

	m_posKey.m_Array.insert(m_posKey.m_Array.begin() + pos, newframePos);
	updatePosInterpolType();

	//m_rotKey.m_Array.insert(m_rotKey.m_Array.begin() + pos, newframeRot);
	m_rotAngleKey.m_Array.insert(m_rotAngleKey.m_Array.begin() + pos, newframeRotAngle);
	m_scaleKey.m_Array.insert(m_scaleKey.m_Array.begin() + pos, newframeScale);
	m_transparent_keys.m_Array.insert(m_transparent_keys.m_Array.begin() + pos, newframeTrans);

	if(tick > m_end * 1000)
	{
		m_end = tick / 1000.0f;
	}



	return pos;
}


void CEffect2LayerInfo::OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix)
{
	Ogre::uint tick, oldtick;
	float fltVal = pTimeAix->GetLayerStartValue(iMyLayerId, iArea);

	if(iArea >= 0 && iArea < (int)m_posKey.m_Array.size())
	{

		oldtick = m_posKey.m_Array[iArea].tick;
		tick = (Ogre::uint)( (fltVal - m_start) * 1000);
		m_posKey.m_Array[iArea].tick = tick;
		//m_rotKey.m_Array[iArea].tick = tick;
		m_rotAngleKey.m_Array[iArea].tick = tick;
		m_scaleKey.m_Array[iArea].tick = tick;
		m_transparent_keys.m_Array[iArea].tick = tick;

		if(iArea == 0)
		{
			m_start = fltVal;
			m_posKey.m_Array[iArea].tick = 0;
			//m_rotKey.m_Array[iArea].tick = 0;
			m_rotAngleKey.m_Array[iArea].tick = 0;
			m_scaleKey.m_Array[iArea].tick = 0;
			m_transparent_keys.m_Array[iArea].tick = 0;
			for(int i = 1; i < (int)m_posKey.m_Array.size(); i++)
			{
				m_posKey.m_Array[i].tick -= tick - oldtick;
				//m_rotKey.m_Array[i].tick -= tick - oldtick;
				m_rotAngleKey.m_Array[i].tick -= tick - oldtick;
				m_scaleKey.m_Array[i].tick -= tick - oldtick;
				m_transparent_keys.m_Array[i].tick -= tick - oldtick;
			}
		}

		CEffectEditBar* pBar = static_cast<CMainFrame*>(theApp.m_pMainWnd)->GetEffectBarByEffect(m_pEffect);
		CKeyFrameObject* pKfObj = NULL;
		if(pBar != NULL 
			&& (pKfObj = pBar->GetKeyFramObj()) != NULL
			&& m_pEffect->GetBindInfo()->bCommKeyFrame == true)
		{
			for(int i = 1; i < (int)pKfObj->GetKeyFrameCount(); i++)
			{
				pKfObj->SetKeyFramePos(i, m_posKey.m_Array[i].tick);
			}

		}

		CKeyFramePropertyObject* pKeyFrameProp = m_pEffect->GetPropertyObject();
		if(pKeyFrameProp != NULL && m_pEffect->GetBindInfo()->bCommKeyFrame == true) 
		{
			for(int i = 1; i < (int)pKfObj->GetKeyFrameCount(); i++)
			{
				pKeyFrameProp->SetKeyFramePos(i, m_posKey.m_Array[i].tick * 0.001f);
			}
		}

	}
	else if(iArea == m_posKey.m_Array.size())
	{
		int iCount = (int) m_posKey.m_Array.size();
		if(iCount > 0)
		{
			float fLastKeyTime = m_posKey.m_Array[iCount-1].tick * 0.001f;
			float fSub = fltVal - fLastKeyTime;
			if(fSub >= 0.0f && fSub < MIN_KFTIME)
			{
				m_end = fLastKeyTime;
			}
			else
			{
				m_end = fltVal - MIN_KFTIME;
			}
		}
		else
		{
			m_end = fltVal - MIN_KFTIME;
		}
	}

}

void CEffect2LayerInfo::OverwriteMotionFrame()
{
	CEffectEditBar* pBar = static_cast<CMainFrame*>(theApp.m_pMainWnd)->GetEffectBarByEffect(m_pEffect);
	CKeyFrameObject* pKfObj = NULL;
	if(pBar == NULL 
		|| (pKfObj = pBar->GetKeyFramObj()) == NULL)
	{
		return;
	}

	CKeyFramePropertyObject* pKeyFrameProp = m_pEffect->GetPropertyObject();

	Ogre::Vector3 pos(0.0f, 0.0f, 0.0f), rotAngle(0.0f, 0.0f, 0.0f);
	Ogre::Quaternion rot;
	Ogre::Vector3 scale(1.0f, 1.0f, 1.0f);
	float trans = 1.0f;
	rot.identity();

	if(m_posKey.m_Array.size() > 0)
	{
		pos = m_posKey.m_Array[0].data;
		//rot = m_rotKey.m_Array[0].data;
		rotAngle = m_rotAngleKey.m_Array[0].data;
		scale = m_scaleKey.m_Array[0].data;
		trans = m_transparent_keys.m_Array[0].data;
	}

	m_posKey.m_Array.clear();
	//m_rotKey.m_Array.clear();
	m_rotAngleKey.m_Array.clear();
	m_scaleKey.m_Array.clear();
	m_transparent_keys.m_Array.clear();

	int iCount = (int)pKfObj->GetKeyFrameCount();
	if(pKeyFrameProp != NULL)
		iCount = pKeyFrameProp->GetKeyFrameCount();

	for(int i = 0; i < iCount; i++)
	{
		Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T posFrame;
		Ogre::KeyFrameArray<Ogre::Quaternion>::KEYFRAME_T rotFrame;
		Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T scaleFrame;
		Ogre::KeyFrameArray<float>::KEYFRAME_T transFrame;
		Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T rotAngleFrame;

		if(pKeyFrameProp == NULL)
		{
			posFrame.tick = pKfObj->GetKeyFrameTick(i);
			//rotFrame.tick = pKfObj->GetKeyFrameTick(i);
			rotAngleFrame.tick = pKfObj->GetKeyFrameTick(i);
			scaleFrame.tick = pKfObj->GetKeyFrameTick(i);
			transFrame.tick = pKfObj->GetKeyFrameTick(i);
		}
		else
		{
			posFrame.tick = int(pKeyFrameProp->GetKeyFrameTime(i) * 1000);
			//rotFrame.tick = pKfObj->GetKeyFrameTick(i);
			rotAngleFrame.tick = int(pKeyFrameProp->GetKeyFrameTime(i) * 1000);
			scaleFrame.tick = int(pKeyFrameProp->GetKeyFrameTime(i) * 1000);
			transFrame.tick = int(pKeyFrameProp->GetKeyFrameTime(i) * 1000);
		}

		posFrame.data = pos;
		//rotFrame.data = rot;
		rotAngleFrame.data = rotAngle;
		scaleFrame.data = scale;
		transFrame.data = trans;

		m_posKey.m_Array.push_back(posFrame);
		//m_rotKey.m_Array.push_back(rotFrame);
		m_rotAngleKey.m_Array.push_back(rotAngleFrame);
		m_scaleKey.m_Array.push_back(scaleFrame);
		m_transparent_keys.m_Array.push_back(transFrame);
	}

	m_pEffect->GetBindInfo()->bCommKeyFrame = true;

}

void CEffect2LayerInfo::OverwriteEffectFrame()
{
	CEffectEditBar* pBar = static_cast<CMainFrame*>(theApp.m_pMainWnd)->GetEffectBarByEffect(m_pEffect);
	CKeyFrameObject* pKfObj = NULL;
	if(pBar == NULL 
		|| (pKfObj = pBar->GetKeyFramObj()) == NULL)
	{
		return;
	}

	CKeyFramePropertyObject* pKeyFrameProp = m_pEffect->GetPropertyObject();
	if(pKeyFrameProp != NULL)
	{
		for(int i = 1; i < pKeyFrameProp->GetKeyFrameCount(); i++)
		{
			pKeyFrameProp->DeleteKeyFrame(1);
		}

		for(int i = 1; i < (int)m_posKey.m_Array.size(); i++)
		{
			int index = pKeyFrameProp->AddKeyFrame(m_posKey.m_Array[i].tick * 0.001f);
			pKeyFrameProp->CopyKeyFrame(0, index);
		}
	}
	else
	{

		for(int i = 1; i < pKfObj->GetKeyFrameCount(); i++)
		{
			pKfObj->DeleteKeyFrame(1);
		}

		for(int i = 1; i < (int)m_posKey.m_Array.size(); i++)
		{
			int index = pKfObj->AddKeyFrame(m_posKey.m_Array[i].tick);
			pKfObj->CopyKeyFrame(0, index);
		}
	}

	m_pEffect->GetBindInfo()->bCommKeyFrame = true;
}

void CEffect2LayerInfo::MergeFrame()
{
	CEffectEditBar* pBar = static_cast<CMainFrame*>(theApp.m_pMainWnd)->GetEffectBarByEffect(m_pEffect);
	CKeyFrameObject* pKfObj = NULL;
	if(pBar == NULL 
		|| (pKfObj = pBar->GetKeyFramObj()) == NULL)
	{
		return;
	}
	CKeyFramePropertyObject* pKeyFrameProp = m_pEffect->GetPropertyObject();

	int iCurrMonKf = 0;
	int iCurrEffKf = 0;

	int iMaxTick = 0;

	int starttick = (int)(m_start * 1000);

	if(pKeyFrameProp != NULL)
	{
		while(iCurrMonKf < (int)m_posKey.m_Array.size() || iCurrEffKf < pKeyFrameProp->GetKeyFrameCount())
		{
			if(iCurrMonKf >= (int)m_posKey.m_Array.size())
			{
				Ogre::uint tick;
				tick = (Ogre::uint)(pKeyFrameProp->GetKeyFrameTime(iCurrEffKf) * 1000);
				InsertMonKeyFrameAtPos(tick + starttick);
				iCurrMonKf++;
				iCurrEffKf++;
				iMaxTick = tick;
			}
			else if(iCurrEffKf >= pKeyFrameProp->GetKeyFrameCount())
			{
				Ogre::uint tick;
				tick = m_posKey.m_Array[iCurrMonKf].tick;
				pKeyFrameProp->AddKeyFrame(tick * 0.001f);
				iCurrMonKf++;
				iCurrEffKf++;
				iMaxTick = tick;
			}
			else 
			{
				Ogre::uint effTick = (Ogre::uint)(pKeyFrameProp->GetKeyFrameTime(iCurrEffKf) * 1000);
				Ogre::uint monTick = m_posKey.m_Array[iCurrMonKf].tick;

				if(effTick + 50 < monTick)
				{
					InsertMonKeyFrameAtPos(effTick + starttick);
					iCurrMonKf++;
					iCurrEffKf++;
					iMaxTick = monTick;
				}
				else if(monTick + 50 < effTick)
				{
					pKeyFrameProp->AddKeyFrame(monTick * 0.001f);
					iCurrMonKf++;
					iCurrEffKf++;
					iMaxTick = effTick;
				}
				else
				{	//若}边当前KF差别很小，就合并为一帧，把时间设为相同
					pKfObj->SetKeyFramePos(iCurrEffKf, monTick);
					iCurrMonKf++;
					iCurrEffKf++;

					iMaxTick = effTick > monTick ? effTick : monTick;
				}

			}
		}
	}
	else
	{
		while(iCurrMonKf < (int)m_posKey.m_Array.size() || iCurrEffKf < pKfObj->GetKeyFrameCount())
		{
			if(iCurrMonKf >= (int)m_posKey.m_Array.size())
			{
				Ogre::uint tick;
				tick = pKfObj->GetKeyFrameTick(iCurrEffKf);
				InsertMonKeyFrameAtPos(tick + starttick);
				iCurrMonKf++;
				iCurrEffKf++;
				iMaxTick = tick;
			}
			else if(iCurrEffKf >= pKfObj->GetKeyFrameCount())
			{
				Ogre::uint tick;
				tick = m_posKey.m_Array[iCurrMonKf].tick;
				pKfObj->AddKeyFrame(tick);
				iCurrMonKf++;
				iCurrEffKf++;
				iMaxTick = tick;
			}
			else 
			{
				Ogre::uint effTick = pKfObj->GetKeyFrameTick(iCurrEffKf);
				Ogre::uint monTick = m_posKey.m_Array[iCurrMonKf].tick;

				if(effTick + 50 < monTick)
				{
					InsertMonKeyFrameAtPos(effTick + starttick);
					iCurrMonKf++;
					iCurrEffKf++;
					iMaxTick = monTick;
				}
				else if(monTick + 50 < effTick)
				{
					pKfObj->AddKeyFrame(monTick);
					iCurrMonKf++;
					iCurrEffKf++;
					iMaxTick = effTick;
				}
				else
				{	//若}边当前KF差别很小，就合并为一帧，把时间设为相同
					pKfObj->SetKeyFramePos(iCurrEffKf, monTick);
					iCurrMonKf++;
					iCurrEffKf++;

					iMaxTick = effTick > monTick ? effTick : monTick;
				}

			}
		}

	}



	m_pEffect->GetBindInfo()->bCommKeyFrame = true;
	m_end = m_start + (float)iMaxTick * 0.001f + 0.5f;

}

bool CEffect2LayerInfo::ApartKeyFrame()
{
	if(m_pEffect != NULL)
	{
		if(m_pEffect->GetBindInfo()->bCommKeyFrame == true)
		{
			m_pEffect->GetBindInfo()->bCommKeyFrame = false;
			return true;
		}
	}
	return false;
}

bool CEffect2LayerInfo::CanMerge()
{
	if(m_pEffect != NULL &&
		(m_pEffect->GetType() == EFFECT_PEMITTER
		|| m_pEffect->GetType() == EFFECT_RIBBON
		|| m_pEffect->GetType() == EFFECT_DECAL))
	{
		return true;
	}
	return false;
}

void CEffect2LayerInfo::OnDeleteFrame(int iMyLayerId, int iArea, CTimeAixes* pTimeAix)
{
	if(iArea < 0 || iArea >= (int)m_posKey.m_Array.size())
		return;

	bool bHasMerged = false;
	CEffectEditBar* pBar = static_cast<CMainFrame*>(theApp.m_pMainWnd)->GetEffectBarByEffect(m_pEffect);
	CKeyFrameObject* pKfObj = NULL;
	if(pBar != NULL 
		&& (pKfObj = pBar->GetKeyFramObj()) != NULL
		&& m_pEffect->GetBindInfo()->bCommKeyFrame == true)
	{
		bHasMerged = true;
	}

	CKeyFramePropertyObject* pKeyFrameProp = m_pEffect->GetPropertyObject();
	if(m_pEffect->GetBindInfo()->bCommKeyFrame == true && pKeyFrameProp != NULL)
	{
		bHasMerged = true;
	}

	m_posKey.m_Array.erase(m_posKey.m_Array.begin() + iArea);
	updatePosInterpolType();

	//m_rotKey.m_Array.erase(m_rotKey.m_Array.begin() + iArea);
	m_rotAngleKey.m_Array.erase(m_rotAngleKey.m_Array.begin() + iArea);
	m_scaleKey.m_Array.erase(m_scaleKey.m_Array.begin() + iArea);
	m_transparent_keys.m_Array.erase(m_transparent_keys.m_Array.begin() + iArea);

	if(bHasMerged == true && pKeyFrameProp != NULL)
		pKeyFrameProp->DeleteKeyFrame(iArea);
	else if (bHasMerged == true)
		pKfObj->DeleteKeyFrame(iArea);

	if(iArea == 0 && m_posKey.m_Array.size() > 0)
	{
		Ogre::uint firstTick = m_posKey.m_Array[0].tick;
		m_start += firstTick / 1000.0f;
		for(int i = 0;  i < (int)m_posKey.m_Array.size(); i++)
		{
			m_posKey.m_Array[i].tick -= firstTick;
			//m_rotKey.m_Array[i].tick -= firstTick;
			m_rotAngleKey.m_Array[i].tick -= firstTick;
			m_scaleKey.m_Array[i].tick -= firstTick;
			m_transparent_keys.m_Array[i].tick -= firstTick;

			if(bHasMerged == true)
			{
				if(pKeyFrameProp != NULL)
				{
					float fOldTime = pKeyFrameProp->GetKeyFrameTime(i);
					pKeyFrameProp->SetKeyFramePos(i, fOldTime - (firstTick * 0.001f));
				}
				else
				{
					Ogre::uint oldtick = pKfObj->GetKeyFrameTick(i);
					pKfObj->SetKeyFramePos(i, oldtick - firstTick);
				}
			}
		}
	}

	CreateTimeLayer(pTimeAix->GetLayer(iMyLayerId), pTimeAix);
	pTimeAix->Invalidate(FALSE);
}

void CEffect2LayerInfo::OnSelectFrame(int iMyLayerId, int iArea, CTimeAixes* pTimeAix)
{
	CEffectEditBar* pBar = static_cast<CMainFrame*>(theApp.m_pMainWnd)->GetEffectBarByEffect(m_pEffect);
	CKeyFrameObject* pKfObj = NULL;
	if(pBar != NULL 
		&& (pKfObj = pBar->GetKeyFramObj()) != NULL
		&& m_pEffect->GetBindInfo()->bCommKeyFrame == true)
	{
		pKfObj->ActiveKeyFrame(iArea);
	}
}

void CEffect2LayerInfo::OnSelected(int iMyLayerId,  CTimeAixes* pTimeAix)
{
	CEffectUnit* pEffect = m_pEffect;
	CEffectMgr* pEffectMgr = CEffectMgr::GetInstance();
	CMainFrame* pMainFrame = static_cast<CMainFrame*>( theApp.m_pMainWnd) ;
	//pMainFrame->OnGotoPage(0, (LPARAM)pEffect);
	PostMainFrameMsg(EEM_GOTO_PAGE, 0, (LPARAM)pEffect);

	//for(int i = 0; i < (int)pEffectMgr->m_Effects.size(); i++)
	//{
	//	if(pEffectMgr->m_Effects[i] == pEffect)
	//	{
	//		PostMainFrameMsg(EEM_GOTO_PAGE, (WPARAM)i, 0);
	//		break;
	//	}
	//}
}

bool CEffect2LayerInfo::CanShow(float fTime)
{
	CObjectLayerInfo* pParent = this;
	while(pParent)
	{
		if(pParent->m_iObjLayerLodLevel == (int)Ogre::MOTION_LOD_NOHANDLE)
		{
			return false;
		}
		pParent = pParent->getParent();
	}

	if(fTime < 0 || fTime < m_start-0.015f || fTime > m_end+0.015f || m_bVisible == false || m_iObjLayerLodLevel > m_iMonLod)
	{
		return false;
	}

	if(getParent() != NULL && getParent()->m_type == Ogre::MOT_MODEL_2)
	{
		bool bParentVisible = true;
		CObjectLayerInfo* pParent = this;
		while(pParent != NULL)
		{
			if(pParent->m_bVisible == false)
			{
				bParentVisible = false;
			}
			pParent = pParent->getParent();
		}

		if(fTime < 0 || fTime < getParent()->m_start-0.015f || fTime > getParent()->m_end+0.015f || bParentVisible == false)
		{
			return false;
		}
	}
	return true;
}

Ogre::SceneNode *CEffect2LayerInfo::GetParentNode()
{
	if(getParent() == NULL)
		return NULL;

	if(getParent()->m_type != Ogre::MOT_MODEL_2)
	{
		return NULL;
	}

	if(getParent()->m_pEffect == NULL)
	{
		return NULL;
	}

	return getParent()->m_pEffect->GetNode();
}

void CEffect2LayerInfo::RepresentAtTime(float fTime, int lod)
{
	m_iMonLod = lod;
	RepresentAtTime(fTime);
}

void CEffect2LayerInfo::RepresentAtTime(float fTime)
{
	if(m_pEffect == NULL)
		return;

	if(CanShow(fTime) == false)
	{
		if(m_bShowing == true)
		{
			m_pEffect->UnbindFromModel(CEffectMgr::GetInstance()->GetModelActor());
			m_pEffect->ReleaseNode();
			m_bShowing = false;
		}
	}
	else
	{
		Ogre::Vector3 pos, scale, rotAngle;
		Ogre::Quaternion rot;
		float trans;
		GetSRTAtTime(fTime, pos, rot, scale, trans, rotAngle);

		if(m_bShowing == false)
		{
			m_pEffect->CreateNode();

			if(m_pEffect->GetNode() != NULL)
			{
				m_pEffect->BindToModel(CEffectMgr::GetInstance()->GetModelActor(), m_anchor);
				m_bShowing = true;
			}
		}

		if(m_pEffect->GetNode())
		{
			if(GetParentNode() != NULL)
			{
				m_pEffect->GetNode()->setSRTFather(GetParentNode(), m_anchor);
			}

			m_pEffect->GetNode()->setPosition(pos);
			m_pEffect->GetNode()->setRotation(C3DMath::XYZAngleToQuat(rotAngle));
			m_pEffect->GetNode()->setScale(scale);

			if(IS_KIND_OF(Ogre::RenderableObject, m_pEffect->GetNode()))
			{
				static_cast<Ogre::RenderableObject*>(m_pEffect->GetNode())->setTransparent(trans);
			}
		}


	}

	RefreshObjInfo(fTime);

}

void CEffect2LayerInfo::RefreshRepresent(float fTime)
{
	for(size_t i=0; i<getNumChild(); i++)
	{
		getIthChild(i)->RefreshRepresent(fTime);
	}

	if(m_pEffect == NULL)
		return;

	if(CanShow(fTime) == false)
	{
		if(m_bShowing == true)
		{
			m_pEffect->UnbindFromModel(CEffectMgr::GetInstance()->GetModelActor());
			m_pEffect->ReleaseNode();
			m_bShowing = false;
		}
	}
	else
	{
		Ogre::Vector3 pos, scale, rotAngle;
		Ogre::Quaternion rot;
		float trans;
		GetSRTAtTime(fTime, pos, rot, scale, trans, rotAngle);
				
		if(m_bShowing == true)
		{
			m_pEffect->UnbindFromModel(CEffectMgr::GetInstance()->GetModelActor());
			m_pEffect->ReleaseNode();
		}

		m_pEffect->CreateNode();

		if(m_pEffect->GetNode())
		{		
			m_pEffect->BindToModel(CEffectMgr::GetInstance()->GetModelActor(), m_anchor);

			m_bShowing = true;
			if(GetParentNode() != NULL)
			{
				m_pEffect->GetNode()->setSRTFather(GetParentNode(), m_anchor);
			}

			m_pEffect->GetNode()->setPosition(pos);
			m_pEffect->GetNode()->setRotation(C3DMath::XYZAngleToQuat(rotAngle));
			m_pEffect->GetNode()->setScale(scale);
			if(IS_KIND_OF(Ogre::RenderableObject, m_pEffect->GetNode()))
			{
				static_cast<Ogre::RenderableObject *>(m_pEffect->GetNode())->setTransparent(trans);
			}
		}

	}

	RefreshObjInfo(fTime);

}

void CEffect2LayerInfo::RefreshObjInfo(float fTime)
{	
	int tick = int((fTime - m_start) * 1000);
	if(tick < 0)
		tick = -1;
	if(m_pEffect == NULL)
		return;
	Ogre::Resource* pRes = m_pEffect->GetSource();
	if(pRes == NULL)
		return;

	CKeyFramePropertyObject* pKeyFrameProperty = m_pEffect->GetPropertyObject();
	if(pKeyFrameProperty != NULL)
	{
		if(tick >= 0)
			pKeyFrameProperty->SetHotTime(tick * 1000.0f);
		else
			pKeyFrameProperty->SetHotTime(-1.0f);
	}

	if(m_pEffect->GetNode())
	{
		m_pEffect->GetNode()->resetUpdate(true, tick);
	}

	if(m_pEffect->GetType() == EFFECT_BILLBOARD)
	{

	}
	/*
	if(m_pEffect->GetType() == EFFECT_PEMITTER)
	{
	static_cast<Ogre::ParticleEmitterData*>(pRes)->SetFixTick(tick);
	}
	else if(m_pEffect->GetType() == EFFECT_DECAL)
	{
	static_cast<Ogre::DecalData*>(pRes)->SetFixTick(tick);
	}
	else if(m_pEffect->GetType() == EFFECT_RIBBON)
	{
	static_cast<Ogre::RibbonEmitterData*>(pRes)->SetFixTick(tick);
	}
	else if(m_pEffect->GetType() == EFFECT_GEOM)
	{
	static_cast<Ogre::ParamShapeData*>(pRes)->SetFixTick(tick);
	}
	else if(m_pEffect->GetType() == EFFECT_BILLBOARD)
	{
	static_cast<Ogre::BillboardData*>(pRes)->SetFixTick(tick);
	}
	else if(m_pEffect->GetType() == EFFECT_UNKNOWN)
	{
	PlayUnknownObjAnim();
	CUnknownObject* pUnknownObj = static_cast<CUnknownObject*>(m_pEffect);
	if(pUnknownObj->IsModel())
	{
	Ogre::Entity* pModel = m_pEffect->GetModel();
	if(pModel != NULL)
	{
	if(tick >= 0)
	{
	pModel->Pause(false);
	//pModel->StopMotion(NULL);
	pModel->Update(tick * 0.001f);
	pModel->GetMainNode()->Update(tick);
	pModel->Pause(true);
	}
	else
	{
	pModel->Pause(false);
	}
	}

	}
	else
	{
	Ogre::SceneNode* pNode = m_pEffect->GetNode();
	if(pNode != NULL && pNode->IsKindOf(&Ogre::Model::m_RTTI) == true)
	{
	Ogre::Model* pActor = static_cast<Ogre::Model*>(pNode);
	if(tick >= 0)
	{
	pActor->Update(tick);
	pActor->Pause(true);
	}
	else
	{
	pActor->Pause(false);
	}
	}
	}
	}
	*/
}

void CEffect2LayerInfo::PlayUnknownObjAnim()
{
	CUnknownObject* pUnknownObj = static_cast<CUnknownObject*>(m_pEffect);
	CModelMotionInfo::MOTION* pCurMotion = CEffectMgr::GetInstance()->m_modelMotion->GetSelectedMotion();
	Ogre::AnimPlayInfo animInfo;
	animInfo.nNumSequences = 0;
	for(int i = 0; i < (int)pCurMotion->objs.size(); i++)
	{
		if(pCurMotion->objs[i]->m_type == Ogre::MOT_ANIM || pCurMotion->objs[i]->m_type == Ogre::MOT_ANIM_ADD)
		{
			animInfo.nSequence[animInfo.nNumSequences] = pCurMotion->objs[i]->m_anim;
			animInfo.nPriority[animInfo.nNumSequences] = pCurMotion->objs[i]->m_priority;
			animInfo.nNumSequences++;
		}
	}

	if(pUnknownObj->GetBindInfo()->animmode == Ogre::BAM_PARENT)
	{
		if(pUnknownObj->IsModel() == true)
		{
			Ogre::Entity* pModel = m_pEffect->GetModel();
			if(pModel != NULL)
			{
				pModel->playMotion(pUnknownObj->GetBindInfo()->animname.GetBuffer());
			}
		}
		else
		{
			Ogre::SceneNode* pNode = m_pEffect->GetNode();
			if(pNode != NULL && IS_KIND_OF(Ogre::Model, pNode))
			{
				int id = atoi(pCurMotion->name.c_str());
				if(id >= 0)
				{
					static_cast<Ogre::Model*>(pNode)->playAnim(id);
				}
			}
		}
	}
	else if(pUnknownObj->GetBindInfo()->animmode == Ogre::BAM_SELF && pUnknownObj->GetBindInfo()->animname != "")
	{
		if(pUnknownObj->IsModel() == true)
		{
			Ogre::Entity* pModel = m_pEffect->GetModel();
			if(pModel != NULL)
			{
				pModel->playMotion(pCurMotion->name.c_str());
			}
		}
		else
		{
			Ogre::SceneNode* pNode = m_pEffect->GetNode();
			if( pNode != NULL && IS_KIND_OF(Ogre::Model, pNode))
			{
				int id = atoi(pCurMotion->name.c_str());
				if(id >= 0)
				{
					static_cast<Ogre::Model*>(pNode)->playAnim(id);
				}
			}
		}
	}
}

CObjectLayerInfo*	CEffect2LayerInfo::CreateClone()
{
	CObjectLayerInfo* pClone = CObjectLayerInfo::CreateClone();
	assert(pClone->m_type == Ogre::MOT_MODEL_2);

	if(m_pEffect != NULL)
	{
		CString name = m_pEffect->GetName() + " 复制";
		CString path = m_pEffect->GetResPath();

		Ogre::Resource* pChunk = NULL;
		if(m_pEffect->GetBindInfo()->bIntegrated == true)
		{
			pChunk = m_pEffect->GetSource();

		}
		else
		{
			pChunk = Ogre::ResourceManager::getSingleton().blockLoad(m_pEffect->GetResPath().GetBuffer());
		}

		if(pChunk == NULL)
		{
			delete pClone;
			return NULL;
		}


		char* tmp = new char[1024];
		NEWEFFECT_INFO ni;
		ni.anchor = 0;

		strcpy(tmp, (const char*)(name.GetBuffer()));
		ni.path = tmp;
		ni.pChunk = pChunk;
		ni.bIsPath = false;
		ni.realpath = path.GetBuffer();

		ni.offset = m_pEffect->GetBindInfo()->offset;
		ni.rotate = m_pEffect->GetBindInfo()->rotate;
		ni.bDynamic = true;
		ni.bShowPane = false;
		ni.scale = m_pEffect->GetBindInfo()->scale;

		SendMainFrameMsg(EEM_REQ_LOAD_EFFECT, 0 , (LPARAM)&ni);

		pClone->m_pEffect = (CEffectMgr::GetInstance()->m_Effects[CEffectMgr::GetInstance()->m_Effects.size() - 1]);

		pClone->m_pEffect->GetBindInfo()->bCommKeyFrame = m_pEffect->GetBindInfo()->bCommKeyFrame;

	}
	return pClone;

}

bool CEffect2LayerInfo::ReceiveChild(CObjectLayerInfo* pChildLayer)
{
	if(pChildLayer->m_type != Ogre::MOT_MODEL_2)
		return false;

	CObjectLayerInfo* pParent = this;
	while(pParent != NULL)
	{
		if(pChildLayer == pParent)
			return false;
		pParent = pParent->getParent();

	}

	return true;
}

bool CEffect2LayerInfo::GetAnchorList(std::vector<int>& outAnchor)
{
	outAnchor.clear();
	outAnchor.push_back(0);
	return true;
}

void CEffect2LayerInfo::DrawIdentifier(Ogre::RenderLines* pDbg, float fTime, IDNTYPE type)
{
	bool bDrawTrack = CEffectMgr::GetInstance()->m_modelMotion->m_bShowPath;
	Ogre::ColorQuad virtualboxColor;
	switch(type)
	{
	case IT_DEFAULT:
		{
			//virtualboxColor = Ogre::ColorQuad(
		}
		break;
	case IT_SELECTED:
		{
			if(bDrawTrack)
				DrawTrack(pDbg, Ogre::ColorQuad(255, 255, 0));
		}
		break;
	case IT_HOVER:
		{
			if(bDrawTrack)
				DrawTrack(pDbg, Ogre::ColorQuad(100, 200, 100));
		}
		break;
	default:
		{
		}
		break;
	}


	//if(m_pEffect != NULL && m_pEffect->GetType() == EFFECT_VIRTUALNODE)
	//{
	//	Ogre::Vector3 pos, scale;
	//	Ogre::Quaternion rotByAngle;
	//	GetWorldSRTAtTime(fTime, pos, rotByAngle, scale);
	//	Ogre::Matrix4 matPos, matScale, matRot;
	//	matPos.makeTranslateMatrix(pos);
	//	matScale.makeScaleMatrix(scale);
	//	rotByAngle.getMatrix(matRot);

	//	Ogre::Matrix4 mat;
	//	mat = matScale * matRot * matPos;
	//	DrawBox(pDbg, mat, 30.0f, Ogre::ColorQuad(0, 255, 0));
	//}

}

void CEffect2LayerInfo::DrawTrack(Ogre::RenderLines* pDbg, Ogre::ColorQuad color)
{
	float fScanTime = m_start;
	float fTimeStep = 0.01f;

	Ogre::Vector3 pos, scale;
	Ogre::Quaternion rotByAngle;

	Ogre::Vector3 lastpos;
	bool bInit = true;
	while(fScanTime < m_end)
	{
		GetWorldSRTAtTime(fScanTime, pos, rotByAngle, scale);

		if(!bInit)
		{
			pDbg->addLine(lastpos, pos, color.c);
		}
		bInit = false;
		lastpos = pos;
		fScanTime += fTimeStep;
	}
}

void CEffect2LayerInfo::DrawBox(Ogre::RenderLines* pDbg, Ogre::Matrix4 tm, float fSize, Ogre::ColorQuad color)
{
	Ogre::Vector3 rawPos[] ={
		Ogre::Vector3(-1,-1,-1), Ogre::Vector3(1,-1,-1), Ogre::Vector3(1,-1,1), Ogre::Vector3(-1,-1,1),
		Ogre::Vector3(-1, 1,-1), Ogre::Vector3(1, 1,-1), Ogre::Vector3(1, 1,1), Ogre::Vector3(-1, 1,1)};

		int indexes[] = {0,1,  1,2,  2,3,  3,0,  4,5,  5,6,  6,7,  7,4,  0,4,  1,5,  2,6,  3,7};

		for(int i = 0; i < 8; i++)
		{
			rawPos[i] *= fSize;
			//tm.TransformCoord(rawPos[i], rawPos[i]);
			rawPos[i] = rawPos[i] * tm;
		}

		for(int i = 0; i < 24; i += 2)
		{
			pDbg->addLine(rawPos[indexes[i]], rawPos[indexes[i + 1]], color.c);
		}
}