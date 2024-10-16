
#include "stdafx.h"
#include "EffectUnitEditBar.h"
#include "NotifyProperty.h"
#include "ObjectTypeHelper.h"
#include "EffectManager.h"
#include "MathUtility.h"
#include "AnchorSet.h"

std::map<VALUE_TYPE, CEffectPropUpdaterCreater*> CEffectPropUpdater::m_Creaters;

DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(VAL_INT,			int,		int)
DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(VAL_FLOAT,		float,		float)
DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(VAL_CSTRING,		cstring,	CString)
DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(VAL_CHAR_PTR,		charptr,	char)
DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(VAL_COLOR,		color,		Ogre::ColourValue)
DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(VAL_BLEND_MODE,	blendtype,	Ogre::UIBlendMode)
DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(VAL_BIND_BONE,	bindbone,	int)
DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(VAL_V3,			v3,			Ogre::Vector3)
DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(VAL_BOOL,			bool,		bool)
//DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(VAL_FLAG,			flag,		UINT)
DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(VAL_GEOM_V3,		geomv3,		Ogre::Vector3)
DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(VAL_ADDRESS_MODE, addressmode,int)
DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(VAL_FACEMODE,		facemode,	Ogre::FaceMode)

DECLARE_PROP_UPDATE_CLASS_CUSTOM_CALL_BEGIN(VAL_FLOAT_RATIO, float, float, CEffectEditBar)
CEffectEditBar::PropUpdate_float(ppProp,(float*) data, updatetype, m_strPropName.GetBuffer(), attrib.m_Attribs[0].fltVal , attrib.m_Attribs[1].fltVal, attrib.m_Attribs[2].fltVal);
END_CUSTOM_CALL(VAL_FLOAT_RATIO, float, float, CEffectEditBar)

DECLARE_PROP_UPDATE_CLASS_CUSTOM_CALL_BEGIN(VAL_INT_RATIO, int, int, CEffectEditBar)
CEffectEditBar::PropUpdate_int(ppProp,(int*) data, updatetype, m_strPropName.GetBuffer(), attrib.m_Attribs[0].fltVal , (int)attrib.m_Attribs[1].fltVal, (int)attrib.m_Attribs[2].fltVal);
END_CUSTOM_CALL(VAL_INT_RATIO, int, int, CEffectEditBar)

DECLARE_PROP_UPDATE_CLASS_CUSTOM_CALL_BEGIN(VAL_FLAG, flag, UINT, CEffectEditBar)
CEffectEditBar::PropUpdate_flag(ppProp,(UINT*) data, updatetype, m_strPropName.GetBuffer(), attrib.m_Attribs[0].intVal);
END_CUSTOM_CALL(VAL_FLAG, flag, UINT, CEffectEditBar)

CEffectPropUpdater* CEffectPropUpdater::CreatePropUpdater(VALUE_TYPE ValType)
{
	std::map<VALUE_TYPE, CEffectPropUpdaterCreater*>::iterator it = m_Creaters.find(ValType);
	if(it != m_Creaters.end())
	{
		CEffectPropUpdaterCreater* pCreater = it->second;
		return pCreater->CreateUpdater();
	}
	return NULL;
}


// CEffectEditBar

IMPLEMENT_DYNAMIC(CEffectEditBar, CBCGPDockingControlBar)
CEffectEditBar::CEffectEditBar()
{

}

CEffectEditBar::~CEffectEditBar()
{
}


BEGIN_MESSAGE_MAP(CEffectEditBar, CBCGPDockingControlBar)
END_MESSAGE_MAP()

void CEffectEditBar::ResetBar()
{
	//重新设置一下可以刷新面板
	SetEditTarget(GetEditTarget());
}

void CEffectEditBar::UpdateKeyFrameTree()
{
	CString strOutput;
	strOutput.Format("%s::UpdateKeyFrameTree() is not implemented\n", GetRuntimeClass()->m_lpszClassName);

	TRACE(strOutput.GetBuffer());
}

void CEffectEditBar::PropUpdate_float(CBCGPProp** ppProp, float* data, DATA_UPDATA_TYPE type, CString name, float ratio, float max, float min)
{
	if(type == CREATE_PROP)
	{
		CNotifiedProp* pProp = new CNotifiedProp(name, _variant_t(float(0)));
		pProp->SetFloatDigitRatio(ratio);
		pProp->SetDigitRange((float)max, (float)min);
		(*ppProp) = pProp;
	}
	else if(type == PROP_TO_DATA)
	{
		*data = (float)((*ppProp)->GetValue());
	}
	else if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue(_variant_t((float)(*data)));
	}
	else
	{
		if(type == INC_DATA_RELATED)
		{
			if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)) && ((CNotifiedProp*)(*ppProp))->m_bRelated == true)
			{
				*data += ((CNotifiedProp*)(*ppProp))->m_fIncStep;
				(*ppProp)->SetValue(_variant_t((float)(*data)));
			}
		}
		else if(type == INC_DATA_ALL)
		{
			if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)))
			{
				*data += ((CNotifiedProp*)(*ppProp))->m_fIncStep;
				(*ppProp)->SetValue(_variant_t((float)(*data)));
			}
		}
		else if(type == DEC_DATA_RELATED)
		{
			if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)) && ((CNotifiedProp*)(*ppProp))->m_bRelated == true)
			{
				*data -= ((CNotifiedProp*)(*ppProp))->m_fIncStep;
				(*ppProp)->SetValue(_variant_t((float)(*data)));
			}
		}
		else if(type == DEC_DATA_ALL)
		{
			if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)))
			{
				*data -= ((CNotifiedProp*)(*ppProp))->m_fIncStep;
				(*ppProp)->SetValue(_variant_t((float)(*data)));
			}
		}
	}
}

void CEffectEditBar::PropUpdate_int(CBCGPProp** ppProp, int* data, DATA_UPDATA_TYPE type, CString name, float ratio, int max, int min)
{
	if(type == CREATE_PROP)
	{
		_variant_t t;
		t.intVal = 0;
		t.vt = VT_I4;

		CNotifiedProp* pProp = new CNotifiedProp(name,t);
		pProp->SetFloatDigitRatio(ratio);
		pProp->SetDigitRange((float)max, (float)min);

		(*ppProp) = pProp;
	}
	else if(type == PROP_TO_DATA)
	{
		*data = (int)((*ppProp)->GetValue());
	}
	else if(type == DATA_TO_PROP)
	{
		_variant_t t;
		t.intVal = *data;
		t.vt = VT_I4;
		(*ppProp)->SetValue(t);
	}
	else if(type == INC_DATA_RELATED)
	{
		if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)) && ((CNotifiedProp*)(*ppProp))->m_bRelated == true)
		{
			*data += (int)((CNotifiedProp*)(*ppProp))->m_fIncStep;
			(*ppProp)->SetValue(_variant_t((int)(*data)));
		}
	}
	else if(type == INC_DATA_ALL)
	{
		if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)))
		{
			*data += (int)((CNotifiedProp*)(*ppProp))->m_fIncStep;
			(*ppProp)->SetValue(_variant_t((int)(*data)));
		}
	}
	else if(type == DEC_DATA_RELATED)
	{
		if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)) && ((CNotifiedProp*)(*ppProp))->m_bRelated == true)
		{
			*data -= (int)((CNotifiedProp*)(*ppProp))->m_fIncStep;
			(*ppProp)->SetValue(_variant_t((int)(*data)));
		}
	}
	else if(type == DEC_DATA_ALL)
	{
		if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)))
		{
			*data -= (int)((CNotifiedProp*)(*ppProp))->m_fIncStep;
			(*ppProp)->SetValue(_variant_t((int)(*data)));
		}
	}
}

void CEffectEditBar::PropUpdate_flag(CBCGPProp** ppProp, unsigned int* data, DATA_UPDATA_TYPE type, CString name, int flag)
{
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
		(*ppProp)->AddOption("是");
		(*ppProp)->AddOption("否");
	}
	else if(type == PROP_TO_DATA)
	{
		CString str = OLE2T((_bstr_t)(*ppProp)->GetValue());
		if(str == "是")
			*data |= flag;
		else
			*data &= ~flag;
	}
	else if(type == DATA_TO_PROP)
	{
		CString str;
		if( (*data & flag) != 0)
			str = "是";
		else
			str = "否";
		(*ppProp)->SetValue(_variant_t(str.GetBuffer()));
	}
}

void CEffectEditBar::PropUpdate_bool(CBCGPProp** ppProp, bool* data, DATA_UPDATA_TYPE type, CString name)
{	
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
		(*ppProp)->AddOption("是");
		(*ppProp)->AddOption("否");
	}
	else if(type == PROP_TO_DATA)
	{
		CString str = OLE2T((_bstr_t)(*ppProp)->GetValue());
		if(str == "是")
			*data = true;
		else
			*data = false;
	}
	else if(type == DATA_TO_PROP)
	{
		CString str;
		if( *data == true)
			str = "是";
		else
			str = "否";
		(*ppProp)->SetValue(_variant_t(str.GetBuffer()));
	}
}


void CEffectEditBar::PropUpdate_cstring(CBCGPProp** ppProp, CString* data, DATA_UPDATA_TYPE type, CString name)
{
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
	}
	else if(type == PROP_TO_DATA && *ppProp != NULL)
	{
		*data = OLE2T((_bstr_t)(*ppProp)->GetValue());
	}
	else if(type == DATA_TO_PROP && *ppProp != NULL)
	{
		(*ppProp)->SetValue(_variant_t(data->GetBuffer()));
	}
}

void CEffectEditBar::PropUpdate_charptr(CBCGPProp** ppProp, char* data, DATA_UPDATA_TYPE type, CString name)
{
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
	}
	else if(type == PROP_TO_DATA && *ppProp != NULL)
	{
		strcpy(data, OLE2T((_bstr_t)(*ppProp)->GetValue()));

	}
	else if(type == DATA_TO_PROP && *ppProp != NULL)
	{
		(*ppProp)->SetValue(_variant_t(data));
	}
}

void CEffectEditBar::PropUpdate_color(CBCGPProp** ppProp, Ogre::ColourValue* data, DATA_UPDATA_TYPE type, CString name)
{
	CBCGPColorProp* pColorProp = static_cast<CBCGPColorProp*>(*ppProp);
	CCvtHelper cvt;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedColorProp(name, RGB(0, 0, 0));
		//(*ppProp) = new CBCGPColorProp(name, RGB(0, 0, 0));
		pColorProp = static_cast<CBCGPColorProp*>(*ppProp);
		pColorProp->EnableOtherButton(_T("更多颜色"));
	}
	else if(type == PROP_TO_DATA)
	{
		COLORREF color = pColorProp->GetColor();
		(*data) = cvt.ColorIntToFloat(color);
	}
	else if(type == DATA_TO_PROP)
	{
		pColorProp->SetColor(cvt.ColorFloatToInt(*data));
	}
}

void CEffectEditBar::PropUpdate_blendtype(CBCGPProp** ppProp, Ogre::UIBlendMode* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		std::vector<CString> blendtypes;
		(*ppProp) = new CNotifiedProp(name, _T(""));
		cvt.EnumBlendType(blendtypes);
		for(int i = 0; i < (int) blendtypes.size(); i++)
		{
			(*ppProp)->AddOption(blendtypes[i].GetBuffer());
		}
	}
	else if(type == PROP_TO_DATA)
	{
		(*data) = (Ogre::UIBlendMode)cvt.BlendTypeToValue(OLE2T((_bstr_t)(*ppProp)->GetValue()));
	}
	else if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue((_bstr_t)cvt.ValueToBlendTypeAsPtr((int)(*data)));
	}
}

void CEffectEditBar::PropUpdate_addressmode(CBCGPProp** ppProp, int* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		std::vector<CString> addrmode;
		(*ppProp) = new CNotifiedProp(name, _T(""));
		cvt.EnumAddressMode(addrmode);
		for(int i = 0; i < (int) addrmode.size(); i++)
		{
			(*ppProp)->AddOption(addrmode[i].GetBuffer());
		}
	}
	else if(type == PROP_TO_DATA)
	{
		(*data) = cvt.StringToAddressMode(OLE2T((_bstr_t)(*ppProp)->GetValue()));
	}
	else if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue((_bstr_t)cvt.AddressModeToStringAsPtr(*data));
	}
}

//void CEffectEditBar::PropUpdate_bindbone(CBCGPProp** ppProp, int* data, DATA_UPDATA_TYPE type, CString name)
//{
//	CCvtHelper cvt;
//	USES_CONVERSION;
//	CSkeleton* pSkeleton = CEffectMgr::GetInstance()->GetSkeleton();
//	if(type == CREATE_PROP)
//	{
//		(*ppProp) = new CNotifiedProp(name, _T(""));
//
//		std::vector<std::string> boneNames;
//		CEffectMgr::GetInstance()->GetSkeleton()->GetAllDisplayName(boneNames);
//		for(int i = 0; i < (int)boneNames.size(); i++)
//		{
//			(*ppProp)->AddOption(boneNames[i].c_str());
//		}
//	}
//	else if(type == PROP_TO_DATA)
//	{
//		(*data) = pSkeleton->GetId(OLE2T((_bstr_t)(*ppProp)->GetValue()));
//	}
//	else
//	{
//		(*ppProp)->SetValue(pSkeleton->GetDisplayName(*data).c_str());
//	}
//}

//
void CEffectEditBar::PropUpdate_bindbone(CBCGPProp** ppProp, int* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	CSkeleton* pSkeleton = CEffectMgr::GetInstance()->GetSkeleton();
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));

		std::vector<std::string> boneNames;
		//CEffectMgr::GetInstance()->GetSkeleton()->GetAllDisplayName(boneNames);
		(*ppProp)->AddOption("0");
		for(int i = 0; i <(int) CEffectMgr::GetInstance()->m_anchorInfo->m_AnchorInfo.size(); i++)
		{
			int id = CEffectMgr::GetInstance()->m_anchorInfo->m_AnchorInfo[i].id;
			if(id == 0)
				continue;
			CString str;
			str.Format("%d", id);
			(*ppProp)->AddOption(str.GetBuffer());
		}

		/*for(int i = 0; i < (int)boneNames.size(); i++)
		{

		}*/
	}
	else if(type == PROP_TO_DATA)
	{
		char* pSel = OLE2T((_bstr_t)(*ppProp)->GetValue());
		sscanf(pSel, "%d", data);
		//(*data) = pSkeleton->GetId();
	}
	else if(type == DATA_TO_PROP)
	{
		CString str1;
		str1.Format("%d", *data);
		(*ppProp)->SetValue(str1.GetBuffer());
	}
}
void CEffectEditBar::PropUpdate_v3(CBCGPProp** ppProp, Ogre::Vector3* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
	}
	else if(type == PROP_TO_DATA)
	{
		cvt.ParseVector3(OLE2T((_bstr_t)(*ppProp)->GetValue()), data);
	}
	else if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue((_bstr_t)cvt.V3ToString((*data)));
	}
}

void CEffectEditBar::PropUpdate_geomv3(CBCGPProp** ppProp, Ogre::Vector3* data, DATA_UPDATA_TYPE type, CString name, float ratio, float max, float min)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		CNotifiedProp* pProp = new CNotifiedProp(name, _variant_t(float(0)));
		pProp->SetFloatDigitRatio(ratio);
		pProp->SetDigitRange((float)max, (float)min);
		(*ppProp) = pProp;
	}
	else if(type == PROP_TO_DATA)
	{
		float val = (float)(*ppProp)->GetValue();
		data->x = data->y = data->z = val;
	}
	else if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue((float)data->x);
	}
}

void CEffectEditBar::PropUpdate_eular_yaw(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name)
{
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _variant_t(float(0)));
	}
	else if(type == PROP_TO_DATA)
	{
		float y,p,r;
		C3DMath::QuaternionToEular(*data, &y, &p, &r);
		y = (float)((*ppProp)->GetValue());
		data->setEulerAngle(y, p, r);
	}
	else if(type == DATA_TO_PROP)
	{
		float y,p,r;
		C3DMath::QuaternionToEular(*data, &y, &p, &r);
		(*ppProp)->SetValue(_variant_t((float)(y)));
	}
}

void CEffectEditBar::PropUpdate_eular_pitch(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name)
{
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _variant_t(float(0)));
	}
	else if(type == PROP_TO_DATA)
	{
		float y,p,r;
		C3DMath::QuaternionToEular(*data, &y, &p, &r);
		p = (float)((*ppProp)->GetValue());
		data->setEulerAngle(y, p, r);
	}
	else if(type == DATA_TO_PROP)
	{
		float y,p,r;
		C3DMath::QuaternionToEular(*data, &y, &p, &r);
		(*ppProp)->SetValue(_variant_t((float)(p)));
	}
}

void CEffectEditBar::PropUpdate_eular_roll(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name)
{
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _variant_t(float(0)));
	}
	else if(type == PROP_TO_DATA)
	{
		float y,p,r;
		C3DMath::QuaternionToEular(*data, &y, &p, &r);
		r = (float)((*ppProp)->GetValue());
		data->setEulerAngle(y, p, r);
	}
	else if(type == DATA_TO_PROP)
	{
		float y,p,r;
		C3DMath::QuaternionToEular(*data, &y, &p, &r);
		(*ppProp)->SetValue(_variant_t((float)(r)));
	}
}


void CEffectEditBar::PropUpdate_rot_any(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name, Ogre::Vector3 aix)
{
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _variant_t(float(0)));
	}
	else if(type == PROP_TO_DATA)
	{
		float val;

		val = (float)((*ppProp)->GetValue());
		//data->setEulerAngle(y, p, r);
		data->setAxisAngle(aix, val);
	}
	else if(type == DATA_TO_PROP)
	{
		//float y,p,r;
		//C3DMath::QuaternionToEular(*data, &y, &p, &r);

		assert(0);
		/*
		D3DXQUATERNION dxq;
		dxq.w = data->w;
		dxq.x = data->x;
		dxq.y = data->y;
		dxq.z = data->z;
		FLOAT fAng = 0.0f;
		D3DXVECTOR3 dxaix;
		dxaix.x = aix.x;
		dxaix.y = aix.y;
		dxaix.z = aix.z;
		::D3DXQuaternionToAxisAngle(&dxq, &dxaix, &fAng);

		(*ppProp)->SetValue(_variant_t((float)(Ogre::Rad2Deg(fAng))));
		*/
	}
}

void CEffectEditBar::PropUpdate_rot_x(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name)
{
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _variant_t(float(0)));
	}
	else if(type == PROP_TO_DATA)
	{
		float val = (float)((*ppProp)->GetValue());
		data->setAxisAngle(data->getAxisX(), val);
	}
	else if(type == DATA_TO_PROP)
	{
		float val =	C3DMath::QuaternionToAngleX(*data);
		(*ppProp)->SetValue(_variant_t((float)(val)));
	}
}

void CEffectEditBar::PropUpdate_rot_y(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name)
{
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _variant_t(float(0)));
	}
	else if(type == PROP_TO_DATA)
	{
		float val = (float)((*ppProp)->GetValue());
		data->setAxisAngle(data->getAxisY(), val);
	}
	else if(type == DATA_TO_PROP)
	{
		float val =	C3DMath::QuaternionToAngleY(*data);
		(*ppProp)->SetValue(_variant_t((float)(val)));
	}
}

void CEffectEditBar::PropUpdate_rot_z(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name)
{
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _variant_t(float(0)));
	}
	else if(type == PROP_TO_DATA)
	{
		float val = (float)((*ppProp)->GetValue());
		data->setAxisAngle(data->getAxisZ(), val);
	}
	else if(type == DATA_TO_PROP)
	{
		float val =	C3DMath::QuaternionToAngleZ(*data);
		(*ppProp)->SetValue(_variant_t((float)(val)));
	}
}

void CEffectEditBar::PropUpdate_facemode(CBCGPProp** ppProp, Ogre::FaceMode* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
	}
	else if(type == PROP_TO_DATA)
	{
		(*data) = (Ogre::FaceMode)cvt.StringToFacemode(OLE2T((_bstr_t)(*ppProp)->GetValue()));
	}
	else  if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue((_bstr_t)cvt.FacemodeToStringAsPtr((int)(*data)));
	}
}

void CEffectEditBar::PropUpdate_rot(CBCGPProp** ppPropX, CBCGPProp** ppPropY, CBCGPProp** ppPropZ, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name)
{
	if(type == CREATE_PROP)
	{
		(*ppPropX) = new CNotifiedProp(name + "X", _variant_t(float(0)));
		(*ppPropY) = new CNotifiedProp(name + "Y", _variant_t(float(0)));
		(*ppPropZ) = new CNotifiedProp(name + "Z", _variant_t(float(0)));
	}
	else if(type == PROP_TO_DATA)
	{
		float valX = (float)((*ppPropX)->GetValue());
		float valY = (float)((*ppPropY)->GetValue());
		float valZ = (float)((*ppPropZ)->GetValue());

		//Ogre::Quaternion qx, qy, qz;
		//qx.setAxisAngle(data->getAxisX(), valX);
		//qy.setAxisAngle(data->getAxisY(), valY);
		//qz.setAxisAngle(data->getAxisZ(), valZ);

		Ogre::Matrix4 mx, my, mz;
		mx.makeRotateX(valX);
		my.makeRotateY(valY);
		mz.makeRotateZ(valZ);

		//*data = qx * qy * qz;

		Ogre::Matrix4 mat;
		mat = mz * my * mx;
		data->setMatrix(mat);
		//data->setAxisAngle(data->getAxisX(), valX);
		//data->setAxisAngle(data->getAxisY(), valY);
		//data->setAxisAngle(data->getAxisZ(), valZ);
	}
	else if(type == DATA_TO_PROP)
	{
		//float valX =	C3DMath::QuaternionToAngleX(*data);
		//float valY =	C3DMath::QuaternionToAngleY(*data);
		//float valZ =	C3DMath::QuaternionToAngleZ(*data);
		float valX;
		float valY;
		float valZ;
		C3DMath::QuaternionToEular(*data, &valX, &valY, &valZ);


		(*ppPropX)->SetValue(_variant_t((float)(valX)));
		(*ppPropY)->SetValue(_variant_t((float)(valY)));
		(*ppPropZ)->SetValue(_variant_t((float)(valZ)));
	}
}

