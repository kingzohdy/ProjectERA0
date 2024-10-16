
#include "stdafx.h"
#include "ObjectTypeHelper.h"
#include "OgreLightData.h"
#include "OgreRibbonData.h"
#include "OgreBeamEmitterData.h"
#include "OgreDummyNodeData.h"
#include "OgreDecalData.h"
#include "OgreEntity.h"
#include "EffectManager.h"

CCvtHelper::CCvtHelper(void)
{
}

CCvtHelper::~CCvtHelper(void)
{
	for(int i = 0; i < (int)m_StrBufs.size(); i++)
	{
		if(m_StrBufs[i] != NULL)
			delete [] m_StrBufs[i];
	}
}


char* CCvtHelper::V3ToString(Ogre::Vector3& vec)
{
	char* buf = new char[256];
	buf[0] = 0;
	sprintf(buf, "%.3f, %.3f, %.3f", vec.x, vec.y, vec.z);
	m_StrBufs.push_back(buf);
	return buf;
}

char* CCvtHelper::ColorToString(Ogre::ColourValue& color)
{
	char* buf = new char[256];
	buf[0] = 0;
	sprintf(buf, "%.3f, %.3f, %.3f, %.3f", color.r, color.g, color.b, color.a);
	m_StrBufs.push_back(buf);
	return buf;
}

char* CCvtHelper::FloatToString(float value)
{
	char* buf = new char[256];
	buf[0] = 0;
	sprintf(buf, "%.3f", value);
	m_StrBufs.push_back(buf);
	return buf;
}
char* CCvtHelper::IntToString(int value)
{
	char* buf = new char[256];
	buf[0] = 0;
	sprintf(buf, "%d", value);
	m_StrBufs.push_back(buf);
	return buf;
}



bool CCvtHelper::ParseFloat(LPTSTR str, float* fResult)
{
	float tmp;
	if(sscanf(str, "%f", &tmp) > 0)
	{
		if(fResult != NULL)
			*fResult = tmp;
		return true;
	}
	return false;
}

bool CCvtHelper::ParseInt(LPTSTR str, int* iResult)
{
	int tmp;
	if(sscanf(str, "%d", &tmp) > 0)
	{
		if(iResult != NULL)
			*iResult = tmp;
		return true;
	}
	return false;
}

int CCvtHelper::StringToInt(LPTSTR str)
{
	int tmp;
	if(sscanf(str, "%d", &tmp) > 0)
	{
		return tmp;
	}
	return 0;
}

bool CCvtHelper::ParseVector3(LPTSTR str, Ogre::Vector3* result)
{
	float x, y, z;
	if(sscanf(str, "%f, %f, %f", &x, &y,&z) >= 3)
	{
		if(result != NULL)
		{
			result->x = x;
			result->y = y;
			result->z = z;
		}
		return true;
	}
	return false;
}

bool CCvtHelper::ParseColor(LPTSTR str, Ogre::ColourValue* result)
{
	float r, g, b, a;
	if(sscanf(str, "%f, %f, %f, %f", &r, &g,&b, &a) >= 3)
	{
		if(result != NULL)
		{
			result->r = r;
			result->g = g;
			result->b = b;
			result->a = a;
		}
		return true;
	}
	return false;
}


int CCvtHelper::EmitterTypeToValue(CString str)
{
	if (str == "平面发射")
		return (int)Ogre::EMITTER_PLANE;
	else if (str == "圆面发射")
		return (int)Ogre::EMITTER_CIRCLE;
	else if(str == "球体发射")
		return (int)Ogre::EMITTER_SPHERE;
	else if(str == "球面发射")
		return (int)Ogre::EMITTER_SPHERE_FACE;
	else if(str == "柱面发射")
		return (int)Ogre::EMITTER_COLUMN_UP;
	else if (str == "柱体发射")
		return (int)Ogre::EMITTER_COLUMN;

	return (int)Ogre::EMITTER_PLANE;;
}

CString CCvtHelper::ValueToEmitterType(int i)
{
	Ogre::EmitterType type = (Ogre::EmitterType)i;
	switch(type)
	{
	case Ogre::EMITTER_PLANE:
		return _T("平面发射");
	case Ogre::EMITTER_CIRCLE:
		return _T("圆面发射");
	case Ogre::EMITTER_SPHERE:
		return _T("球体发射");
	case Ogre::EMITTER_SPHERE_FACE:
		return _T("球面发射");
	case Ogre::EMITTER_COLUMN_UP:
		return _T("柱面发射");
	case Ogre::EMITTER_COLUMN:
		return _T("柱体发射");
	default:
		return _T("平面发射");
	}
}


int CCvtHelper::ParticleTypeToValue(CString str)
{
	if(str == "Normal")
		return 0;
	else if(str == "Quad")
		return 1;
	else if(str == "Unknown")
		return 2;
	else if(str == "Object")
		return 3;
	return 0;
}

CString CCvtHelper::ValueToParticleType(int i)
{
	switch(i)
	{
	case 0:
		return _T("Normal");
	case 1:
		return _T("Quad");
	case 2:
		return _T("Unknown");
	case 3:
		return _T("Object");
	default:
		return _T("Normal");
	}
}

void CCvtHelper::EnumBlendType(std::vector<CString>& bt)
{
	bt.clear();
	bt.push_back("不透明");
	bt.push_back("Alpha测试");
	bt.push_back("Alpha混合");
	bt.push_back("相加混合");
	bt.push_back("相加");
	bt.push_back("相乘");
	bt.push_back("相乘X2");
}
int CCvtHelper::BlendTypeToValue(CString str)
{
	if(str == "不透明")				return 0;
	else if(str == "Alpha测试")		return 1;
	else if(str == "Alpha混合")		return 2;
	else if(str == "相加混合")		return 3;
	else if(str == "相加")		    return 4;
	else if(str == "相乘")		    return 5;
	else if(str == "相乘X2")		    return 6;

	return 0;
}

CString CCvtHelper::ValueToBlendType(int i)
{
	switch(i)
	{
	case 0:		return _T("不透明");
	case 1:		return _T("Alpha测试");
	case 2: return _T("Alpha混合");
	case 3: return _T("相加混合");
	case 4: return _T("相加");
	case 5: return _T("相乘");
	case 6: return _T("相乘X2");

	default:
		return _T("不透明");
	}
}

char* CCvtHelper::ValueToEmitterTypeAsPtr(int i)
{
	char* buf = new char[256];
	CString ret = ValueToEmitterType(i);
	strcpy(buf, ret.GetBuffer());
	m_StrBufs.push_back(buf);
	return buf;
}


char* CCvtHelper::ValueToParticleTypeAsPtr(int i)
{
	char* buf = new char[256];
	CString ret = ValueToParticleType(i);
	strcpy(buf, ret.GetBuffer());
	m_StrBufs.push_back(buf);
	return buf;
}


char* CCvtHelper::ValueToBlendTypeAsPtr(int i)
{
	char* buf = new char[256];
	CString ret = ValueToBlendType(i);
	strcpy(buf, ret.GetBuffer());
	m_StrBufs.push_back(buf);
	return buf;
}

int CCvtHelper::StringToLoopmod(CString str)
{
	if(str == _T("单次播放"))
	{
		return 1;
	}
	else if(str == _T("循环播放"))
	{
		return 0;
	}
	else if(str == _T("单次播放后停止"))
	{
		return 2;
	}
	return 1;
}

CString CCvtHelper::LoopmodToString(int loopmod)
{
	if(loopmod == 0)
		return _T("循环播放");
	else if(loopmod == 1)
		return _T("单次播放");
	else if(loopmod == 2)
		return _T("单次播放后停止");
	return _T("单次播放");
}

CString CCvtHelper::PathToAddr(CString path)
{

	//}
	return "";
}

CString CCvtHelper::AddrToPath(CString addr)
{
	return "";
}

COLORREF CCvtHelper::ColorFloatToInt(Ogre::ColourValue clr)
{
	return RGB(clr.r * 255, clr.g * 255, clr.b * 255);
}

Ogre::ColourValue CCvtHelper::ColorIntToFloat(COLORREF clr)
{
	
	Ogre::ColourValue color = Ogre::ColourValue(GetRValue(clr) / 255.0f, GetGValue(clr) / 255.0f, GetBValue(clr) / 255.0f, 1.0f);
	return color;

}

CString CCvtHelper::LightToString(Ogre::LightType light)
{
	switch(light)
	{
	case Ogre::LT_DIRECT:
		return _T("直射光源");
	case Ogre::LT_POINT:
		return _T("点光源");
	case Ogre::LT_SPOT:
		return _T("块光源");
	}
	return _T("");
}

char* CCvtHelper::LightToStringAsPtr(Ogre::LightType light)
{
	char* tmp = new char[50];
	tmp[0] = 0;
	switch(light)
	{
	case Ogre::LT_DIRECT:
		strcpy(tmp,  "直射光源");
	case Ogre::LT_POINT:
		strcpy(tmp, "点光源");
	case Ogre::LT_SPOT:
		strcpy(tmp, "块光源");
	}
	m_StrBufs.push_back(tmp);
	return tmp;
}

Ogre::LightType CCvtHelper::StringToLight(LPTSTR light)
{
	CString str = light;
	if(str == _T("直射光源"))
		return Ogre::LT_DIRECT;
	else if(str == _T("点光源"))
		return Ogre::LT_POINT;
	else if(str == _T("块光源"))
		return Ogre::LT_SPOT;
	return Ogre::LT_NULL;
}

CString CCvtHelper::FacemodeToString(int facemode)
{
	switch(facemode) 
	{
	case 0:
		return _T("面对观察点");
	case 1:
		return _T("绕竖直旋转");
	case 2:
		return _T("面对上方");
	case 3:
		return _T("简单面片");
	}
	return _T("");
}

char* CCvtHelper::FacemodeToStringAsPtr(int facemode)
{
	char* tmp = new char[50];
	tmp[0] = 0;
	switch(facemode) 
	{
	case 0:
		strcpy(tmp, "面对观察点");
		break;
	case 1:
		strcpy(tmp, "绕竖直旋转");
		break;
	case 2:
		strcpy(tmp, "面对上方");
		break;
	case 3:
		strcpy(tmp, "简单面片");
		break;
	}
	m_StrBufs.push_back(tmp);
	return tmp;
}

int CCvtHelper::StringToFacemode(LPTSTR facemode)
{
	CString str = facemode;
	if(str == _T("面对观察点"))
		return 0;
	else if(str == _T("绕竖直旋转"))
		return 1;
	else if(str == _T("面对上方"))
		return 2;
	else if(str == _T("简单面片"))
		return 3;
	return 0;
}

CString CCvtHelper::ParticleRotToString(Ogre::ParticleDirType rot)
{
	switch(rot) 
	{
	case Ogre::PR_FACE_CAMERA:
		return _T("面对观察点");
	case Ogre::PR_ROT_ABOUT_UP:
		return _T("绕竖直旋转");
	case Ogre::PR_FACE_UP:
		return _T("面对上方");
	case Ogre::PR_FACE_UP_NOROT:
		return _T("面对上方不旋转");
	case Ogre::PR_ROT_ABOUT_DIR:
		return _T("绕运动方向旋转");
	case Ogre::PR_FACE_DIR:
		return "面向运动方向";
	}
	return _T("");
}

char* CCvtHelper::ParticleRotToStringAsPtr(Ogre::ParticleDirType rot)
{
	char* tmp = new char[50];
	tmp[0] = 0;
	switch(rot) 
	{
	case Ogre::PR_FACE_CAMERA:
		strcpy(tmp, "面对观察点");
		break;
	case Ogre::PR_ROT_ABOUT_UP:
		strcpy(tmp, "绕竖直旋转");
		break;
	case Ogre::PR_FACE_UP:
		strcpy(tmp, "面对上方");
		break;
	case Ogre::PR_FACE_UP_NOROT:
		strcpy(tmp, "面对上方不旋转");
		break;
	case Ogre::PR_ROT_ABOUT_DIR:
		strcpy(tmp, "绕运动方向旋转");
		break;
	case Ogre::PR_FACE_DIR:
		strcpy(tmp, "面向运动方向");
		break;
	}
	m_StrBufs.push_back(tmp);
	return tmp;
}

Ogre::ParticleDirType CCvtHelper::StringToParticleRot(LPTSTR rot)
{
	CString str = rot;
	if(str == _T("面对观察点"))
		return Ogre::PR_FACE_CAMERA;
	else if(str == _T("绕竖直旋转"))
		return Ogre::PR_ROT_ABOUT_UP;
	else if(str == _T("面对上方"))
		return Ogre::PR_FACE_UP;
	else if(str == _T("绕运动方向旋转"))
		return Ogre::PR_ROT_ABOUT_DIR;
	else if(str == "面向运动方向")
		return Ogre::PR_FACE_DIR;
	else if(str == "面对上方不旋转")
		return Ogre::PR_FACE_UP_NOROT;

	return Ogre::PR_FACE_CAMERA;

}

EFFECT_TYPE CCvtHelper::ChunkTypeToEffectType(Ogre::Resource *pres)
{
	EFFECT_TYPE type = EFFECT_ERRORTYPE;
	if(IS_KIND_OF(Ogre::ParticleEmitterData, pres)) type = EFFECT_PEMITTER;
	else if(IS_KIND_OF(Ogre::BillboardData, pres)) type = EFFECT_BILLBOARD;
	else if(IS_KIND_OF(Ogre::LightData, pres)) type = EFFECT_LIGHT;
	else if(IS_KIND_OF(Ogre::RibbonEmitterData, pres)) type = EFFECT_RIBBON;
	else if(IS_KIND_OF(Ogre::SoundData, pres)) type = EFFECT_SOUND;
	else if(IS_KIND_OF(Ogre::DecalData, pres)) type = EFFECT_DECAL;
	else if(IS_KIND_OF(Ogre::BeamEmitterData, pres)) type = EFFECT_BEAM;
	else if(IS_KIND_OF(Ogre::ParamShapeData, pres)) type = EFFECT_GEOM;
	else if(IS_KIND_OF(Ogre::DummyNodeData, pres)) type = EFFECT_VIRTUALNODE;
	else type = EFFECT_UNKNOWN;
	return type;
}
//int   Encode(const   char*   pSrc,   unsigned   char*   pDst,   int   nSrcLength)   
//{   
//	int   nDstLength;                 //   UNICODE宽字符数目   
//	WCHAR   wchar[128];             //   UNICODE串缓冲区   
//
//	//   字符串-->UNICODE串   
//	memset(wchar,0,128);//   I   added   
//	nDstLength   =   ::MultiByteToWideChar(CP_ACP,   0,   pSrc,   nSrcLength,   wchar,   128);   
//
//	//   高低字节对调，输出   
//	for(int   i=0;   i<nDstLength;   i++)   
//	{   
//		//   先输出高位字节   
//		*pDst++   =   wchar[i]   >>   8;   
//		//   后输出低位字节   
//		*pDst++   =   wchar[i]   &   0xff;   
//	}   
//
//	//   返回目标编码串长度   
//	return   nDstLength   *   2;   
//}


char* CCvtHelper::Utf8ToGb(const char* src)
{
	char* tmp = new char[512];
	char* ret = new char[512];

	::MultiByteToWideChar(CP_UTF8, 0, src, -1, (LPWSTR)tmp, 512);
	int end = ::WideCharToMultiByte(CP_ACP, 0, (LPWSTR)tmp, -1, ret, 512, NULL, NULL);
	delete[] tmp;
	ret[end] = 0;
	m_StrBufs.push_back(ret);
	return ret;
}

char* CCvtHelper::GbToUtf8(const char* src)
{
	char* tmp = new char[512];
	char* ret = new char[512];

	::MultiByteToWideChar(CP_ACP, 0, src, -1, (LPWSTR)tmp, 512);
	int end = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)tmp, -1, ret, 512, NULL, NULL);
	delete[] tmp;
	ret[end] = 0;
	m_StrBufs.push_back(ret);
	return ret;
}

WCHAR*	CCvtHelper::GbToWide(const char* src)
{
	char* ret = new char[512];

	int end =::MultiByteToWideChar(CP_ACP, 0, src, -1, (LPWSTR)ret, 512);
	//ret[end] = 0;
	m_StrBufs.push_back(ret);
	return (WCHAR*)ret;
}

CString	CCvtHelper::AnimIDToDesc(int id)
{
	CString ret;
	ret.Format("%d", id);
	return ret;
}

char* CCvtHelper::AnimIDToDescAsPtr(int id)
{
	char* tmp = new char[128];
	sprintf(tmp, "%d",id);
	m_StrBufs.push_back(tmp);
	return tmp;
}

int CCvtHelper::DescToAnimID(CString str)
{
	int ret = 0;
	sscanf(str.GetBuffer(), "%d", &ret);
	return ret;
}

void CCvtHelper::EnumPostEffect(std::vector<CString> &posteffects)
{
	posteffects.clear();
	posteffects.push_back("动态模糊");
	posteffects.push_back("眩光");
	posteffects.push_back("全屏柔化");
	posteffects.push_back("黑白");
	posteffects.push_back("镜头抖动");
	posteffects.push_back("冰冻");
	posteffects.push_back("画面泛色");
	posteffects.push_back("角色透明");
	posteffects.push_back("焦距拉伸");
	posteffects.push_back("自定义效果");
}

Ogre::POSTEFFECT_T CCvtHelper::StringToPostEffect(CString str)
{
	if(str == "动态模糊")
		return Ogre::PE_MOTION_BLUR;
	else if(str == "眩光")
		return Ogre::PE_GLOW;
	else if(str == "全屏柔化")
		return Ogre::PE_BLOOM;
	else if(str == "黑白")
		return Ogre::PE_GRAY;
	else if(str == "镜头抖动")
		return Ogre::PE_SHAKE;
	else if(str == "冰冻")
		return Ogre::PE_FROZEN;
	else if(str == "画面泛色")
		return Ogre::PE_FSEFFECT;
	else if(str == "角色透明")
		return Ogre::PE_TRANSPARENT;
	else if(str == "焦距拉伸")
		return Ogre::PE_ZOOM;
	else if(str == "自定义效果")
		return Ogre::PE_CUSTOM;
	return Ogre::PE_MOTION_BLUR;
}

CString	CCvtHelper::PostEffectToString(Ogre::POSTEFFECT_T posteffect)
{
	switch(posteffect)
	{
	case Ogre::PE_MOTION_BLUR:
		return "动态模糊";
		break;
	case Ogre::PE_GLOW:
		return "眩光";
		break;
	case Ogre::PE_BLOOM:
		return "全屏柔化";
		break;
	case Ogre::PE_GRAY:
		return "黑白";
		break;
	case Ogre::PE_SHAKE:
		return "镜头抖动";
		break;	
	case Ogre::PE_FROZEN:
		return "冰冻";
		break;
	case Ogre::PE_FSEFFECT:
		return "画面泛色";
		break;
	case Ogre::PE_TRANSPARENT:
		return "角色透明";
	case Ogre::PE_ZOOM:
		return "焦距拉伸";
		break;
	case Ogre::PE_CUSTOM:
		return "自定义效果";
		break;
	}	
	return "";
}

char* CCvtHelper::PostEffectToStringAsPtr(Ogre::POSTEFFECT_T posteffect)
{
	char* tmp = new char[256];
	switch(posteffect)
	{
	case Ogre::PE_MOTION_BLUR:
		strcpy(tmp, "动态模糊");
		break;
	case Ogre::PE_GLOW:
		strcpy(tmp, "眩光");
		break;
	case Ogre::PE_BLOOM:
		strcpy(tmp, "全屏柔化");
		break;
	case Ogre::PE_GRAY:
		strcpy(tmp, "黑白");
		break;
	case Ogre::PE_SHAKE:
		strcpy(tmp, "镜头抖动");
		break;
	case Ogre::PE_FROZEN:
		strcpy(tmp, "冰冻");
		break;
	case Ogre::PE_FSEFFECT:
		strcpy(tmp, "画面泛色");
		break;
	case Ogre::PE_TRANSPARENT:
		strcpy(tmp, "角色透明");
		break;
	case Ogre::PE_ZOOM:
		strcpy(tmp, "焦距拉伸");
		break;
	case Ogre::PE_CUSTOM:
		strcpy(tmp, "自定义效果");
		break;
	}	
	m_StrBufs.push_back(tmp);

	return tmp;
}

void CCvtHelper::EnumGeomType(std::vector<CString>& geomTypes)
{
	geomTypes.push_back("球体");
	geomTypes.push_back("圆环");
	geomTypes.push_back("圆桶");
	geomTypes.push_back("圆柱");
}

CString CCvtHelper::GeomTypeToString(Ogre::PARAMSHAPE_TYPE geotype)
{
	switch(geotype)
	{
	case Ogre::GEO_SPHERE:
		return "球体";
		break;
	case Ogre::GEO_TORUS:
		return "圆环";
		break;
	case Ogre::GEO_TUBE:
		return "圆桶";
		break;
	case Ogre::GEO_COLUMN:
		return "圆柱";
		break;
	default:
		break;
	}

	return "球体";
}

char* CCvtHelper::GeomTypeToStringAsPtr(Ogre::PARAMSHAPE_TYPE geotype)
{
	char* tmp = new char[256];
	switch(geotype)
	{
	case Ogre::GEO_SPHERE:
		strcpy(tmp, "球体");
		break;
	case Ogre::GEO_TORUS:
		strcpy(tmp, "圆环");
		break;
	case Ogre::GEO_TUBE:
		strcpy(tmp, "圆桶");
		break;
	case Ogre::GEO_COLUMN:
		strcpy(tmp, "圆柱");
		break;
	default:
		strcpy(tmp, "球体");
		break;
	}
	m_StrBufs.push_back(tmp);

	return tmp;
}

Ogre::PARAMSHAPE_TYPE CCvtHelper::StringToGeomType(LPTSTR geotype)
{
	CString str = geotype;
	if(str == "球体")
		return Ogre::GEO_SPHERE;
	else if(str == "圆环")
		return Ogre::GEO_TORUS;
	else if(str == "圆桶")
		return Ogre::GEO_TUBE;
	else if(str == "圆柱")
		return Ogre::GEO_COLUMN;

	return Ogre::GEO_SPHERE;
}

void CCvtHelper::EnumAnimMode(std::vector<CString>& animMode)
{
	animMode.push_back("跟随动画");
	animMode.push_back("独立动画");
}

CString CCvtHelper::AnimModeToString(Ogre::BIND_ANIM_MODE mode)
{
	if(mode == Ogre::BAM_SELF)
		return "独立动画";
	if(mode == Ogre::BAM_PARENT)
		return "跟随动画";

	assert(0);
	return "独立动画";
}

char* CCvtHelper::AnimModeToStringAsPtr(Ogre::BIND_ANIM_MODE mode)
{
	char* tmp = new char[256];
	switch(mode)
	{
	case Ogre::BAM_SELF:
		strcpy(tmp, "独立动画");
		break;

	case Ogre::BAM_PARENT:
		strcpy(tmp, "跟随动画");
		break;

	default:
		assert(0);
		break;
	}

	m_StrBufs.push_back(tmp);
	return tmp;
}


Ogre::BIND_ANIM_MODE CCvtHelper::StringToAnimMode(CString str)
{
	if(str == "独立动画")
		return Ogre::BAM_SELF;
	if(str == "跟随动画")
		return Ogre::BAM_PARENT;
	else
	{
		assert(0);
		return Ogre::BAM_SELF;
	}
}

void CCvtHelper::EnumAddressMode(std::vector<CString>& addressMode)
{
	addressMode.push_back("循环");
	addressMode.push_back("单次");
}

CString CCvtHelper::AddressModeToString(int mode)
{
	if(mode == 0)
		return "循环";
	else if(mode == 1)
		return "单次";

	return "单次";
}

int CCvtHelper::StringToAddressMode(CString str)
{
	if(str == "循环")
		return 0;
	else if (str == "单次") 
		return 1;

	return 1;
}

char* CCvtHelper::AddressModeToStringAsPtr(int mode)
{
	char* tmp = new char[256];
	switch(mode)
	{
	case 0:
		strcpy(tmp, "循环");
		break;

	case 1:
		strcpy(tmp, "单次");
		break;
	default:
		strcpy(tmp, "单次");
		break;

	}

	m_StrBufs.push_back(tmp);

	return tmp;
}

void CCvtHelper::EnumSoundPlayMode(std::vector<CString>& mode)
{
	mode.push_back("自由播放");
	mode.push_back("限制播放");
}

Ogre::SOUND_PLAY_MODE CCvtHelper::StringToSoundPlayMode(CString str)
{
	if(str == "自由播放")
	{
		return Ogre::PM_FREE;
	}
	else if(str == "限制播放")
	{
		return Ogre::PM_LIMITED;
	}
	return Ogre::PM_FREE;
}

char* CCvtHelper::SoundPlayModeToStringAsPtr(Ogre::SOUND_PLAY_MODE mode)
{
	char* tmp = new char[256];
	switch(mode)
	{
	case Ogre::PM_FREE:
		strcpy(tmp, "自由播放");
		break;

	case Ogre::PM_LIMITED:
		strcpy(tmp, "限制播放");
		break;
	default:
		strcpy(tmp, "自由播放");
		break;

	}

	m_StrBufs.push_back(tmp);

	return tmp;
}



void CCvtHelper::EnumBillboardFrameMode(std::vector<CString>& mode)
{
	mode.push_back("固定帧间隔");
	mode.push_back("跟随关键帧");
}

Ogre::SEQ_FRAME_TYPE CCvtHelper::StringToBillboardFrameMode(CString str)
{
	if(str == "固定帧间隔")
	{
		return Ogre::SFT_FIXED_FRAME_TIME;
	}
	else if(str == "跟随关键帧")
	{
		return Ogre::SFT_FIT_KEYFRAME;
	}
	return Ogre::SFT_FIXED_FRAME_TIME;
}

char* CCvtHelper::BillboardFrameModeToStringAsPtr(Ogre::SEQ_FRAME_TYPE mode)
{
	char* tmp = new char[256];
	switch(mode)
	{
	case Ogre::SFT_FIXED_FRAME_TIME:
		strcpy(tmp, "固定帧间隔");
		break;

	case Ogre::SFT_FIT_KEYFRAME:
		strcpy(tmp, "跟随关键帧");
		break;
	default:
		strcpy(tmp, "固定帧间隔");
		break;

	}

	m_StrBufs.push_back(tmp);

	return tmp;
}

void CCvtHelper::EnumBeamWaveType(std::vector<CString>& mode)
{
	mode.push_back("随机折线");
	mode.push_back("正弦线");
	mode.push_back("直线");
}

Ogre::BEAM_WAVE_TYPE CCvtHelper::StringToBeamWaveType(CString str)
{
	if(str == "随机折线")
	{
		return Ogre::BWT_RANDOM;
	}
	else if(str == "正弦线")
	{
		return Ogre::BWT_SIN;
	}
	else if(str == "直线")
	{
		return Ogre::BWT_STRATE;
	}
	return Ogre::BWT_RANDOM;
}

char* CCvtHelper::BeamWaveTypeToStringAsPtr(Ogre::BEAM_WAVE_TYPE mode)
{
	char* tmp = new char[256];
	switch(mode)
	{
	case Ogre::BWT_RANDOM:
		strcpy(tmp, "随机折线");
		break;

	case Ogre::BWT_SIN:
		strcpy(tmp, "正弦线");
		break;

	case Ogre::BWT_STRATE:
		strcpy(tmp, "直线");
		break;
	default:
		strcpy(tmp, "随机折线");
		break;

	}

	m_StrBufs.push_back(tmp);
	return tmp;
}