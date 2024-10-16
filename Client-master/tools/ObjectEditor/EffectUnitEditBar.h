
#pragma once
#include "ObjectEditorTypes.h"
#include "OgrePrerequisites.h"
#include "OgreUIRenderTypes.h"
#include "OgreBillboardData.h"

#include <map>

class CBCGPProp;
class CBCGPColorProp;

class CPropUpdaterFloat
{
public:
	void	PropUpdate(CBCGPProp** ppProp, void* data, DATA_UPDATA_TYPE type);
};

class CEffectEditBar : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CEffectEditBar)

public:
	CEffectEditBar();
	virtual ~CEffectEditBar();

	virtual void SetEditTarget(CEffectUnit* pEffect) = 0;
	virtual CEffectUnit* GetEditTarget() = 0;
	virtual void UpdateList() = 0;
	virtual void ResetBar();
	virtual CKeyFrameObject* GetKeyFramObj()	{ return NULL; };
	virtual void	UpdateKeyFrameTree();
protected:
	DECLARE_MESSAGE_MAP()

public:
	static void PropUpdate_float(CBCGPProp** ppProp, float* data, DATA_UPDATA_TYPE type, CString name, float ratio = 1.0f, float max = 32767, float min = -32766);
	static void PropUpdate_int(CBCGPProp** ppProp, int* data, DATA_UPDATA_TYPE type, CString name, float ratio = 1.0f, int max = 32767, int min = -32766);
	static void PropUpdate_cstring(CBCGPProp** ppProp, CString* data, DATA_UPDATA_TYPE type, CString name);
	static void PropUpdate_charptr(CBCGPProp** ppProp, char* data, DATA_UPDATA_TYPE type, CString name);
	static void PropUpdate_color(CBCGPProp** ppProp, Ogre::ColourValue* data, DATA_UPDATA_TYPE type, CString name);
	static void PropUpdate_blendtype(CBCGPProp** ppProp, Ogre::UIBlendMode* data, DATA_UPDATA_TYPE type, CString name);
	static void PropUpdate_bindbone(CBCGPProp** ppProp, int* data, DATA_UPDATA_TYPE type, CString name);
	static void PropUpdate_v3(CBCGPProp** ppProp, Ogre::Vector3* data, DATA_UPDATA_TYPE type, CString name);
	static void PropUpdate_bool(CBCGPProp** ppProp, bool* data, DATA_UPDATA_TYPE type, CString name);
	static void PropUpdate_flag(CBCGPProp** ppProp, unsigned int* data, DATA_UPDATA_TYPE type, CString name, int flag);
	static void PropUpdate_geomv3(CBCGPProp** ppProp, Ogre::Vector3* data, DATA_UPDATA_TYPE type, CString name, float ratio = 1.0f, float max = 32767, float min = -32766);

	static void PropUpdate_eular_yaw(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name);
	static void PropUpdate_eular_pitch(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name);
	static void PropUpdate_eular_roll(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name);

	static void PropUpdate_addressmode(CBCGPProp** ppProp, int* data, DATA_UPDATA_TYPE type, CString name);

	static void PropUpdate_rot_any(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name, Ogre::Vector3 aix);

	static void PropUpdate_rot_x(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name);
	static void PropUpdate_rot_y(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name);
	static void PropUpdate_rot_z(CBCGPProp** ppProp, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name);

	static void PropUpdate_rot(CBCGPProp** ppPropX, CBCGPProp** ppPropY, CBCGPProp** ppPropZ, Ogre::Quaternion* data, DATA_UPDATA_TYPE type, CString name);
	static void PropUpdate_facemode(CBCGPProp** ppProp, Ogre::FaceMode* data, DATA_UPDATA_TYPE type, CString name);

	static void PropUpdate_color(CBCGPColorProp** ppProp, Ogre::ColourValue* data, DATA_UPDATA_TYPE type, CString name)
	{
		PropUpdate_color((CBCGPProp**)ppProp,  data,  type,  name);
	}
};


enum VALUE_TYPE
{
	VAL_INT,
	VAL_FLOAT,
	VAL_CSTRING,
	VAL_CHAR_PTR,
	VAL_COLOR,
	VAL_BLEND_MODE,
	VAL_BIND_BONE,
	VAL_V3,
	VAL_BOOL,
	VAL_FLAG,
	VAL_GEOM_V3,
	VAL_ADDRESS_MODE,
	VAL_FACEMODE,
	VAL_INT_RATIO,
	VAL_FLOAT_RATIO
};

class CAttribSet
{
public:
	VARIANT	m_Attribs[16];
	CAttribSet(){};
	CAttribSet(const CAttribSet& src)
	{
		memcpy((void*)m_Attribs, (void*)src.m_Attribs, sizeof(VARIANT) * 16);
	};

	CAttribSet& operator = (const CAttribSet& src)
	{
		memcpy((void*)m_Attribs, (void*)src.m_Attribs, sizeof(VARIANT) * 16);
		return *this;
	};

	void ManageRatio(float fMax, float fMin, float fRatio)
	{
		m_Attribs[0].vt = VT_R4;
		m_Attribs[0].fltVal = fRatio;
		m_Attribs[1].vt = VT_R4;
		m_Attribs[1].fltVal = fMax;
		m_Attribs[2].vt = VT_R4;
		m_Attribs[2].fltVal = fMin;
	};

	void ManageFlagPos(int iPos)
	{
		m_Attribs[0].vt = VT_I4;
		m_Attribs[0].intVal = iPos;
	};

	//static CAttribSet	ms_rotRatio;
	//static CAttribSet	ms_scaleRatio;
	//static CAttribSet	ms_offsetRatio;
	//static CAttribSet	ms_sizeRatio;
};





class CEffectPropUpdater;
class CEffectPropUpdaterCreater
{
public:
	virtual CEffectPropUpdater* CreateUpdater() = 0;
};

class CEffectPropUpdater
{
public:
	virtual void	PropUpdate(CBCGPProp** ppProp, void* data, DATA_UPDATA_TYPE updatetype, CAttribSet& attrib){};
	CString m_strPropName;
	//CAttribSet m_AttribSet;
	static CEffectPropUpdater* CreatePropUpdater(VALUE_TYPE ValType);
	static  std::map<VALUE_TYPE, CEffectPropUpdaterCreater*> m_Creaters;
};

#define DECLARE_PROP_UPDATE_CLASS(type, functype, systype, funcowner)\
class CPropUpdater_##type : public 	CEffectPropUpdater\
{\
public:\
	virtual void	PropUpdate(CBCGPProp** ppProp, void* data, DATA_UPDATA_TYPE updatetype, CAttribSet& attrib)\
{\
	funcowner::PropUpdate_##functype(ppProp, (systype*)data, updatetype, m_strPropName.GetBuffer());\
};\
struct CCreater_##type : public CEffectPropUpdaterCreater\
{\
	CCreater_##type	()\
{\
	CEffectPropUpdater::m_Creaters[type] = this;\
};\
	virtual CEffectPropUpdater* CreateUpdater()\
{\
	return new CPropUpdater_##type();\
}\
};\
};CPropUpdater_##type::CCreater_##type g_CreaterInst_##type;	

#define DECLARE_PROP_UPDATE_CLASS_DEFAULTTYPE(type, functype, systype) DECLARE_PROP_UPDATE_CLASS(type, functype, systype, CEffectEditBar)

#define DECLARE_PROP_UPDATE_CLASS_CUSTOM_CALL_BEGIN(type, functype, systype, funcowner)\
class CPropUpdater_##functype : public 	CEffectPropUpdater\
{\
public:\
	virtual void	PropUpdate(CBCGPProp** ppProp, void* data, DATA_UPDATA_TYPE updatetype, CAttribSet& attrib){



#define END_CUSTOM_CALL(type, functype, systype, funcowner)\
};\
struct CCreater_##functype : public CEffectPropUpdaterCreater\
{\
	CCreater_##functype	()\
{\
	CEffectPropUpdater::m_Creaters[type] = this;\
};\
	virtual CEffectPropUpdater* CreateUpdater()\
{\
	return new CPropUpdater_##functype();\
}\
};\
};CPropUpdater_##functype::CCreater_##functype g_CreaterInst_##functype;	