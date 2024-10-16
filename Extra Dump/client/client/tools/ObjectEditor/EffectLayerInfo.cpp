
#include "stdafx.h"
#include "EffectLayerInfo_SubClass.h"
#include "TimeBar.h"
#include "ObjectTypeHelper.h"
#include "EffectManager.h"
#include "SkeletonInfo.h"
#include "SelectAnimDlg.h"
#include "MathUtility.h"
#include "ObjectEditor.h"
#include "MainFrm.h"
#include "BindInfo.h"

#include "OgreModel.h"
#include "OgreAnimationData.h"
#include "OgreEntity.h"
#include "OgreAnimationPlayer.h"
#include "OgreEntityMotionData.h"
#include "OgreResourceManager.h"

CObjectLayerInfo::CObjectLayerInfo(void)
{
	m_bVisible = true;
	m_pParent = NULL;

	m_iObjLayerLodLevel = 100;
	m_PosInterpolType = Ogre::INTERPOL_LINEAR;

	m_OverlayBlendMode = BLEND_OPAQUE;
	m_OverlayTexPath = "";
	m_fOverlayAlhpa = 1.0f;
}

CObjectLayerInfo::~CObjectLayerInfo(void)
{
}

CObjectLayerInfo* CObjectLayerInfo::CreateObjectLayer(Ogre::MOTIONOBJTYPE_T type, Ogre::POSTEFFECT_T posteffect)
{
	if(type == Ogre::MOT_ANIM || type == Ogre::MOT_ANIM_ADD)
	{
		return new CAnimLayerInfo();
	}
	else if(type==Ogre::MOT_POSTEFFECT && posteffect==Ogre::PE_SHAKE)
	{
		return new CCameraShakeLayerInfo();
	}
	else if(type == Ogre::MOT_POSTEFFECT && (
		posteffect == Ogre::PE_MOTION_BLUR 
		|| posteffect == Ogre::PE_GRAY
		|| posteffect == Ogre::PE_GLOW
		|| posteffect == Ogre::PE_BLOOM
		|| posteffect == Ogre::PE_FROZEN
		|| posteffect == Ogre::PE_CUSTOM)
		)
	{
		return new CNormalPostLayerInfo();
	}
	else if(type == Ogre::MOT_POSTEFFECT && 	posteffect == Ogre::PE_FSEFFECT)
	{
		return new CFSEffectLayerInfo();
	}
	else if(type == Ogre::MOT_POSTEFFECT && 	posteffect == Ogre::PE_ZOOM)
	{
		return new CZoomLayerInfo();
	}
	else if(type == Ogre::MOT_POSTEFFECT && 	posteffect == Ogre::PE_TRANSPARENT)
	{
		return new CTransLayerInfo();
	}
	else if(type == Ogre::MOT_EVENTS)
	{
		return new CEventLayerInfo();
	}
	else if(type ==  Ogre::MOT_MODEL_2 || type == Ogre::MOT_MODEL)
	{
		return new CEffect2LayerInfo();
	}
	return new CObjectLayerInfo();
}

Ogre::MotionElementData *CObjectLayerInfo::createMotionElementData(Ogre::MOTIONOBJTYPE_T type)
{
	if(type==Ogre::MOT_ANIM || type==Ogre::MOT_ANIM_ADD)
	{
		return new Ogre::MotionAnimElementData;
	}
	else if(type == Ogre::MOT_POSTEFFECT)
	{
		return new Ogre::MotionPostElementData;
	}
	else if(type == Ogre::MOT_EVENTS)
	{
		return new Ogre::MotionEventElementData;
	}
	else if(type ==  Ogre::MOT_MODEL_2 || type == Ogre::MOT_MODEL)
	{
		return new Ogre::MotionBindElementData;
	}
	else
	{
		assert(0);
		return NULL;
	}
}

bool CObjectLayerInfo::loadFromElementData(Ogre::MotionElementData *pelementdata)
{
	m_start = pelementdata->m_fStart;
	m_end = pelementdata->m_fEnd;
	m_offset = Ogre::Vector3(0,0,0);
	m_rotate.identity();
	m_type = pelementdata->m_type;
	m_iObjLayerLodLevel = pelementdata->m_iObjLodLevel;
	m_pEffect = NULL;
	m_pParent = NULL;

	/*
	m_Interval			= src.stIntervalTime;
	m_Images			= src.stImages;

	m_iceblend			= src.stIceBlend;
	m_bFade			= src.stFade;
	*/

	if(IS_KIND_OF(Ogre::MotionBindElementData, pelementdata))
	{
		Ogre::MotionBindElementData *pbinddata = static_cast<Ogre::MotionBindElementData *>(pelementdata);
		m_anchor = pbinddata->anchorid;
		m_posKey = pbinddata->pos_key;
		m_PosInterpolType = m_posKey.m_Type;

		m_rotKey = pbinddata->rot_key;
		m_scaleKey = pbinddata->scale_key;
		m_rotAngleKey = pbinddata->rotangle_key;
		m_transparent_keys = pbinddata->transparent_keyframe;

		m_bVisible = pbinddata->obj.isvisible;
		m_eEffectType = (EFFECT_TYPE)pbinddata->obj.effect_type;
		m_SortIndex = pbinddata->obj.sort_index;
		m_bFolded = pbinddata->obj.isfolded;
		
		if(pbinddata->rotdatatype == Ogre::BRD_QUAT)
		{
			for(size_t i=0; i<pbinddata->rot_key.m_Array.size(); i++)
			{
				Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T kf;
				Ogre::KeyFrameArray<Ogre::Quaternion>::KEYFRAME_T srckf = pbinddata->rot_key.m_Array[i];
				kf.tick = srckf.tick;
				C3DMath::QuaternionToEular(srckf.data, &kf.data.x, &kf.data.y, &kf.data.z);
				m_rotAngleKey.m_Array.push_back(kf);
			}
		}

		NEWEFFECT_INFO ni;
		ni.anchor = 0;
		if(pbinddata->obj.presource == NULL)
		{
			//return false;
			pbinddata->obj.presource = ResourceManager::getSingleton().blockLoad("toolres\\fxeditor\\lighticon.omod");

			//pbinddata->obj.presource = ResourceManager::getSingleton().blockLoad(pbinddata->obj.name);
			//if(pbinddata->obj.presource == NULL) return false;
		}

		if(pbinddata->obj.presource->getResPath().length()>0)
		{
			ni.realpath = (const char *)pbinddata->obj.presource->getResPath();
		}
		else ni.realpath = "";
		ni.pChunk = pbinddata->obj.presource;
		ni.bIsPath = false;
		ni.bIntegrated = true;
		ni.path = pbinddata->obj.name;
		ni.offset = Ogre::Vector3(0.0f, 0.0f, 0.0f);
		ni.rotate.identity();
		ni.bDynamic = true;
		ni.bShowPane = false;
		ni.scale = Ogre::Vector3(1.0f, 1.0f, 1.0f);
		ni.animmode = pbinddata->obj.animmode;
		ni.animname = pbinddata->obj.animname;
		static_cast<CMainFrame*>(theApp.m_pMainWnd)->OnReqestLoadEffect(0, (LPARAM)&ni);

		m_pEffect = (CEffectMgr::GetInstance()->m_Effects[CEffectMgr::GetInstance()->m_Effects.size() - 1]);
		m_pEffect->GetBindInfo()->bCommKeyFrame = pbinddata->obj.iscommonframe;
	}
	else if(IS_KIND_OF(Ogre::MotionAnimElementData, pelementdata))
	{
		Ogre::MotionAnimElementData *panimdata = static_cast<Ogre::MotionAnimElementData *>(pelementdata);
		m_anim = panimdata->anim;
		m_priority = panimdata->priority;
	}
	else if(IS_KIND_OF(Ogre::MotionPostElementData, pelementdata))
	{
		Ogre::MotionPostElementData *ppostdata = static_cast<Ogre::MotionPostElementData *>(pelementdata);
		m_posteffect = ppostdata->posteffect;
		/*
		if(src.posteffect == MAY::PE_OFFCOLOR)
		m_offcolor = src.fs_keyframe; //偏色与泛光共享fs_keyframe变量用于存储
		else
		m_fs_colorkeys		= src.fs_keyframe;
		*/
		m_fs_colorkeys = ppostdata->fs_keyframe;

		m_OverlayBlendMode = ppostdata->m_OverlayBlendMode;
		m_OverlayTexPath = ppostdata->m_OverlayTexPath;
		m_fOverlayAlhpa = ppostdata->m_OverlayAlpha;

		/*
		if(src.posteffect == MAY::PE_SHAKE) //镜头抖动和透明共享transparent_keyframe变量用于存储
		m_camerashake_keys = src.transparent_keyframe;
		else
		m_transparent_keys = src.transparent_keyframe;
		*/
		m_transparent_keys = ppostdata->transparent_keyframe;
	}
	else if(IS_KIND_OF(Ogre::MotionEventElementData, pelementdata))
	{
		Ogre::MotionEventElementData *peventdata = static_cast<Ogre::MotionEventElementData *>(pelementdata);
		for(size_t i=0; i<peventdata->m_Events.size(); i++)
		{
			CMotionEventList* pList = new CMotionEventList();
			Ogre::EVENT_ITEM *peventitem = peventdata->m_Events[i];
			for(size_t j=0; j<peventitem->eventNames.size(); j++)
			{
				pList->m_EventNames.push_back((const char *)peventitem->eventNames[j]);
			}
			pList->m_fTime = peventitem->fEventTime;

			m_events.push_back(pList);
		}
	}

	if(m_transparent_keys.m_Array.size() < m_posKey.m_Array.size())
	{
		m_transparent_keys.m_Array.clear();
		for(int k = 0; k < (int)m_posKey.m_Array.size(); k++)
		{
			Ogre::KeyFrameArray<float>::KEYFRAME_T frame;
			frame.tick = m_posKey.m_Array[k].tick;
			frame.data = 1.0f;
			m_transparent_keys.m_Array.push_back(frame);
		}
	}

	if(m_rotAngleKey.m_Array.size() < m_posKey.m_Array.size())
	{
		m_rotAngleKey.m_Array.clear();
		for(int k = 0; k < (int)m_posKey.m_Array.size(); k++)
		{
			Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T frame;
			frame.tick = m_posKey.m_Array[k].tick;
			frame.data = Ogre::Vector3(1.0f, 1.0f, 1.0f);
			m_rotAngleKey.m_Array.push_back(frame);
		}

	}

	m_hasStart = (pelementdata->m_lifetype == Ogre::MOL_ALL_TIME || pelementdata->m_lifetype == Ogre::MOL_INIT_TO_END) ? false : true;
	m_hasEnd = (pelementdata->m_lifetype == Ogre::MOL_ALL_TIME || pelementdata->m_lifetype == Ogre::MOL_START_WITHOUT_END) ? false : true;

	return true;
}
/*
static void GetFirstControlPoints(std::vector<Ogre::Vector3>&ret, std::vector<Ogre::Vector3>&rhs)
{
	int n = (int)rhs.length();
	std::vector<float>tmp(n);
	ret.resize(n);

	float b = 2.0f;
	ret[0] = rhs[0]/b;
	for(int i=1; i<n; i++)
	{
		tmp[i] = 1.0f/b;
		b = (i<n-1? 4.0f:3.5f) - tmp[i];
		ret[i] = (rhs[i] - ret[i-1]) / b;
	}

	for(int i=1; i<n; i++)
	{
		ret[n-i-1] -= ret[n-1]*tmp[n-i];
	}
}

static void ChangeKeyFramesCurve(Ogre::KeyFrameArray<Ogre::Vector3>&frames)
{
	int nkey = (int)frames.getNumKey();
	if(nkey < 3) return;

	frames.m_Type = INTERPOL_HERMITE;
	frames.m_CtrlPt.resize(nkey);

	int n = nkey-1;
	std::vector<Ogre::Vector3>rhs(n);
	for(int i=1; i<n-1; i++) rhs[i] = frames.m_Array[i].data*4.0f + frames.m_Array[i+1].data*2.0f;
	rhs[0] = frames.m_Array[0].data + frames.m_Array[1].data*2.0f;
	rhs[n-1] = (frames.m_Array[n-1].data*8.0f + frames.m_Array[n].data)*0.5f;

	std::vector<Ogre::Vector3>firstpoints(n);
	std::vector<Ogre::Vector3>secondpoints(n);
	GetFirstControlPoints(firstpoints, rhs);

	for(int i=0; i<n; i++)
	{
		if(i < n-1) secondpoints[i] = frames.m_Array[i+1].data*2.0f - firstpoints[i+1];
		else secondpoints[i] = (frames.m_Array[n].data + firstpoints[n-1]) /2.0f;
	}

	for(int i=0; i<nkey; i++)
	{
		if(i<nkey-1) frames.m_CtrlPt[i].out = firstpoints[i];
		else frames.m_CtrlPt[i].out = Ogre::Vector3(0,0,0);

		if(i>0) frames.m_CtrlPt[i].in = secondpoints[i];
	}
}*/

static void ChangeKeyFramesCurve(Ogre::KeyFrameArray<Ogre::Vector3>&frames)
{
	int nkey = (int)frames.getNumKey();
	if(nkey < 3) return;

	frames.m_Type = INTERPOL_HERMITE;
	frames.m_CtrlPt.resize(nkey);

	//for(int i=1; i<nkey-1; i++)
	for(int i=0; i<nkey; i++)
	{
		int next = i+1>=nkey?nkey-1:i+1;
		int prev = i-1<0?0:i-1;
		Vector3 dt = (frames.m_Array[next].data - frames.m_Array[prev].data)*0.5f;

		//Vector3 dt = (frames.m_Array[i+1].data - frames.m_Array[i-1].data)*0.5f;
		frames.m_CtrlPt[i].out = dt;
		frames.m_CtrlPt[i].in = dt;
	}
/*
	frames.m_CtrlPt[0].out = frames.m_Array[1].data - frames.m_CtrlPt[1].in - frames.m_Array[0].data;
	frames.m_CtrlPt[nkey-1].in = frames.m_Array[nkey-1].data - (frames.m_Array[nkey-2].data + frames.m_CtrlPt[nkey-2].out);

	frames.m_CtrlPt[0].in = frames.m_CtrlPt[nkey-1].out = Ogre::Vector3(-1000000.0f, -10000000.0f, -100000000.0f);
	*/
}

void CObjectLayerInfo::updatePosInterpolType()
{
	if(m_PosInterpolType == Ogre::INTERPOL_LINEAR)
	{
		m_posKey.m_Type = Ogre::INTERPOL_LINEAR;
		m_posKey.m_CtrlPt.resize(0);
	}
	else ChangeKeyFramesCurve(m_posKey);
}

void CObjectLayerInfo::saveToElementData(Ogre::MotionElementData *pelementdata, int layer_index, int parent_index)
{
	pelementdata->m_fStart = m_start;
	pelementdata->m_fEnd = m_end;
	pelementdata->m_type = m_type;
	pelementdata->m_iParent = parent_index;
	pelementdata->m_iObjLodLevel = m_iObjLayerLodLevel;

	if(m_hasStart && m_hasEnd) pelementdata->m_lifetype	= Ogre::MOL_START_TO_END;
	else if(!m_hasStart && m_hasEnd) pelementdata->m_lifetype = Ogre::MOL_INIT_TO_END;
	else if(m_hasStart && !m_hasEnd) pelementdata->m_lifetype = Ogre::MOL_START_WITHOUT_END;
	else if(!m_hasStart && !m_hasEnd) pelementdata->m_lifetype = Ogre::MOL_ALL_TIME;

	if(IS_KIND_OF(Ogre::MotionBindElementData, pelementdata))
	{
		Ogre::MotionBindElementData *pbinddata = static_cast<Ogre::MotionBindElementData *>(pelementdata);
		pbinddata->anchorid = m_anchor;
		pbinddata->pos_key = m_posKey;
		pbinddata->rot_key = m_rotKey;
		pbinddata->scale_key = m_scaleKey;
		pbinddata->rotdatatype = Ogre::BRD_EULAR;
		pbinddata->rotangle_key = m_rotAngleKey;
		pbinddata->transparent_keyframe = m_transparent_keys;

		if(m_pEffect != NULL)
		{
			if(m_pEffect->GetBindInfo()->bIntegrated == true)
			{
				pbinddata->obj.presource = m_pEffect->GetSource();
				pbinddata->obj.presource->addRef();
			}
			else
			{
				pbinddata->obj.presource = Ogre::ResourceManager::getSingleton().blockLoad(m_pEffect->GetResPath().GetBuffer());
				pbinddata->obj.extern_res = true;
			}
			pbinddata->obj.layer_index = layer_index;
			pbinddata->obj.iscommonframe = m_pEffect->GetBindInfo()->bCommKeyFrame;
			pbinddata->obj.animmode = m_pEffect->GetBindInfo()->animmode;
			pbinddata->obj.animname = m_pEffect->GetBindInfo()->animname.GetBuffer();
			pbinddata->obj.isvisible = m_bVisible;
			pbinddata->obj.mottype = Ogre::MOT_MODEL_2;
			pbinddata->obj.name = m_pEffect->GetName().GetBuffer();
			pbinddata->obj.sort_index = m_SortIndex;

			//虚拟体的折叠功能
			if(m_eEffectType == EFFECT_VIRTUALNODE)
			{
				pbinddata->obj.isfolded = m_bFolded;
				pbinddata->obj.effect_type = m_eEffectType;
			}

			size_t nkey = pbinddata->pos_key.getNumKey();
			if(pbinddata->transparent_keyframe.getNumKey() > nkey)
			{
				pbinddata->transparent_keyframe.setNumKey(nkey);
			}
			if(pbinddata->rot_key.getNumKey() > nkey)
			{
				pbinddata->rot_key.setNumKey(nkey);
			}
			if(pbinddata->scale_key.getNumKey() > nkey)
			{
				pbinddata->scale_key.setNumKey(nkey);
			}
		}
	}
	else if(IS_KIND_OF(Ogre::MotionAnimElementData, pelementdata))
	{
		Ogre::MotionAnimElementData *panimdata = static_cast<Ogre::MotionAnimElementData *>(pelementdata);
		panimdata->anim = m_anim;
		panimdata->priority = m_priority;
	}
	else if(IS_KIND_OF(Ogre::MotionPostElementData, pelementdata))
	{
		Ogre::MotionPostElementData *ppostdata = static_cast<Ogre::MotionPostElementData *>(pelementdata);
		ppostdata->posteffect = m_posteffect;
		ppostdata->fs_keyframe = m_fs_colorkeys;
		ppostdata->transparent_keyframe = m_transparent_keys;
		ppostdata->m_OverlayBlendMode = m_OverlayBlendMode;
		ppostdata->m_OverlayTexPath = (const char *)m_OverlayTexPath;
		ppostdata->m_OverlayAlpha = m_fOverlayAlhpa;
	}
	else if(IS_KIND_OF(Ogre::MotionEventElementData, pelementdata))
	{
		Ogre::MotionEventElementData *peventdata = static_cast<Ogre::MotionEventElementData *>(pelementdata);

		for(size_t i=0; i<m_events.size(); i++)
		{
			CMotionEventList *plist = m_events[i];
			Ogre::EVENT_ITEM *pitem = new Ogre::EVENT_ITEM;
			pitem->fEventTime = plist->m_fTime;
			for(size_t j=0; j<plist->m_EventNames.size(); j++)
			{
				pitem->eventNames.push_back(plist->m_EventNames[j].GetBuffer());
			}

			peventdata->m_Events.push_back(pitem);
		}
	}
}

void CObjectLayerInfo::CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix)
{
	pTimeLayer->m_strName = GetDisplayName();
}

void CObjectLayerInfo::OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix)
{
	float len = pTimeAix->GetLayerLenValue(iMyLayerId);
	float headlen = pTimeAix->GetLayerStartValue(iMyLayerId);

	m_start = headlen;
	m_end = headlen + len;
}

void CObjectLayerInfo::OnSizeChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix)
{
	float len = pTimeAix->GetLayerLenValue(iMyLayerId); 
	float headlen = pTimeAix->GetLayerStartValue(iMyLayerId);

	m_start = headlen;
	m_end = headlen + len;
}

CObjectLayerInfo* CObjectLayerInfo::CreateClone()
{
	CObjectLayerInfo* pClone = CreateObjectLayer(m_type, m_posteffect);
	if(pClone == NULL)
	{
		return NULL;
	}

	pClone->m_pEffect				= m_pEffect;
	pClone->m_PosInterpolType       = m_PosInterpolType;
	pClone->m_start					= m_start;
	pClone->m_end					= m_end;
	pClone->m_hasStart				= m_hasStart;
	pClone->m_hasEnd				= m_hasEnd;
	pClone->m_anchor				= m_anchor;
	pClone->m_offset				= m_offset;
	pClone->m_rotate				= m_rotate;
	pClone->m_scale					= m_scale;
	pClone->m_type					= m_type;
	pClone->m_anim					= m_anim;
	pClone->m_priority				= m_priority;
	pClone->m_posteffect			= m_posteffect;
	pClone->m_fs_color				= m_fs_color;
	pClone->m_fs_colorkeys			= m_fs_colorkeys;
	pClone->m_transparent_keys		= m_transparent_keys;
	pClone->m_customPE				= m_customPE;
	pClone->m_OverlayBlendMode      = m_OverlayBlendMode;
	pClone->m_OverlayTexPath        = m_OverlayTexPath;
	pClone->m_fOverlayAlhpa			= m_fOverlayAlhpa;
	pClone->m_posKey				= m_posKey;
	pClone->m_rotKey				= m_rotKey;
	pClone->m_scaleKey				= m_scaleKey;
	pClone->m_rotAngleKey			= m_rotAngleKey;

	pClone->setParent(m_pParent);

	pClone->m_events.clear();
	for (int i = 0; i < (int)m_events.size(); i++)
	{
		CMotionEventList* pList = new CMotionEventList();
		pList->m_fTime = m_events[i]->m_fTime;
		pList->m_EventNames = m_events[i]->m_EventNames;
		pClone->m_events.push_back(pList);
	}

	return pClone;
}

int CObjectLayerInfo::GetChildLevel()
{
	CObjectLayerInfo* pLayer = m_pParent;
	int iLevel = 0;
	while(pLayer != NULL)
	{
		iLevel++;
		pLayer = pLayer->m_pParent;
	}
	return  iLevel;
}

CObjectLayerInfo *CObjectLayerInfo::getParent()
{
	return m_pParent;
}
void CObjectLayerInfo::setParent(CObjectLayerInfo *p)
{
	if(m_pParent)
	{
		std::vector<CObjectLayerInfo *>::iterator iter = m_pParent->m_Children.begin();
		for(; iter!=m_pParent->m_Children.end(); iter++)
		{
			if(*iter == this)
			{
				m_pParent->m_Children.erase(iter);
				break;
			}
		}
	}

	m_pParent = p;
	if(p == NULL) return;

#ifdef _DEBUG
	for(size_t i=0; i<m_pParent->m_Children.size(); i++)
	{
		assert(m_pParent->m_Children[i] != p);
	}
#endif

	m_pParent->m_Children.push_back(this);
}

size_t CObjectLayerInfo::getNumChild()
{
	return m_Children.size();
}

CObjectLayerInfo *CObjectLayerInfo::getIthChild(size_t i)
{
	return m_Children[i];
}

// --------------------CAnimLayerInfo---------------------------------- [6/11/2008]

CString CAnimLayerInfo::GetDisplayName()
{
	CCvtHelper cvt;
	CString ret;
	ret.Format("动画：(%s)", cvt.AnimIDToDescAsPtr(m_anim));
	return ret;
}

void CAnimLayerInfo::CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix)
{
	pTimeLayer->m_blocks.clear();

	CTimeArea ta;
	ta.m_bAllowPos = true;
	ta.m_bAllowSize = true;
	ta.m_iFirst = pTimeAix->GetBoxIndex(m_start);
	ta.m_iLen = pTimeAix->GetBoxIndex(m_end) - ta.m_iFirst;

	CSkeleton* pSkeleton = CEffectMgr::GetInstance()->m_skeleton;
	for(int i = 0; i < (int)pSkeleton->m_animOrder.size(); i++)
	{
		int seqid = pSkeleton->m_animOrder[i].iSeqId;
		int seqindex = pSkeleton->m_animOrder[i].iSeqIndex;
		Ogre::SequenceDesc *pseq = pSkeleton->GetSequence(seqid);

		if(seqid == m_anim)
		{
			int ticklen = pseq->time_end - pseq->time_start;
			float fTimeLen = ticklen / 1000.0f;
			ta.m_bAllowSize = false;
			ta.m_iLen = pTimeAix->GetBoxIndex(fTimeLen + m_start);
			break;

		}
	}

	pTimeLayer->m_blocks.push_back(ta);
	pTimeLayer->m_strName = GetDisplayName();
	pTimeLayer->m_usedOption = 1;
	pTimeLayer->m_options[0] = m_bVisible == true ? 1 : 0;
	pTimeLayer->m_iLodLevel = m_iObjLayerLodLevel;
	pTimeLayer->m_eObjType = m_type;
	pTimeLayer->m_eET = m_eEffectType;
}

void CAnimLayerInfo::OnActive(int iMyLayerId, int block,  CTimeAixes* pTimeAix)
{
	CCvtHelper cvt;
	CSelectAnimDlg dlg;
	dlg.m_iSel = m_anim;
	dlg.m_iPriority = m_priority;
	dlg.m_fStart = m_start;
	dlg.m_fEnd = m_end;
	dlg.m_bNoStart = m_hasStart ? FALSE : TRUE;
	dlg.m_bNoEnd = m_hasEnd ? FALSE : TRUE;
	if(m_type == Ogre::MOT_ANIM)
		dlg.m_bOverwrite = TRUE;
	else
		dlg.m_bOverwrite = FALSE;

	if(IDOK == dlg.DoModal())
	{
		m_anim = dlg.m_iSel;
		m_start = dlg.m_fStart;
		m_end = dlg.m_fEnd;
		m_hasStart = (dlg.m_bNoStart==TRUE) ? false : true;
		m_hasEnd = (dlg.m_bNoEnd==TRUE) ? false : true;
		m_type = (dlg.m_bOverwrite==TRUE) ? Ogre::MOT_ANIM : Ogre::MOT_ANIM_ADD;
		m_priority = dlg.m_iPriority;

		pTimeAix->SetLayerStartValue(iMyLayerId, m_start);
		pTimeAix->SetLayerLenValue(iMyLayerId, m_end - m_start);

		pTimeAix->GetLayer(iMyLayerId)->m_strName = GetDisplayName();;

		//CEffectMgr::GetInstance()->GetModelMotion()->SetSelect(iMotion, pTAHDR->layer);
	}
}

void CAnimLayerInfo::RepresentAtTime(float fTime)
{

	RefreshRepresent(fTime);
}

void CAnimLayerInfo::RepresentAtTime(float fTime, int lod)
{
	RefreshRepresent(fTime);
}

void CAnimLayerInfo::RefreshRepresent(float fTime)
{
	Ogre::Model* pNode = CEffectMgr::GetInstance()->GetModelActor()->getMainModel();
	if(pNode)
	{
		Ogre::Model* pActorNode = static_cast<Ogre::Model*>(pNode);
		int tick = int((fTime - m_start)*1000);
		if(fTime < 0)
		{
			pActorNode->resetUpdate(false);
		}
		else if(tick >= 0)
		{
			if(pActorNode->getAnimPlayer()) pActorNode->getAnimPlayer()->setAnimDelayInOut(0, 0);
			pActorNode->playAnim(m_anim);
			pActorNode->resetUpdate(true, tick);
		}
	}
}
