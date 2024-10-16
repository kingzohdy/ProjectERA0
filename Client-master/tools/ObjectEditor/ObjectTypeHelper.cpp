
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
	if (str == "ƽ�淢��")
		return (int)Ogre::EMITTER_PLANE;
	else if (str == "Բ�淢��")
		return (int)Ogre::EMITTER_CIRCLE;
	else if(str == "���巢��")
		return (int)Ogre::EMITTER_SPHERE;
	else if(str == "���淢��")
		return (int)Ogre::EMITTER_SPHERE_FACE;
	else if(str == "���淢��")
		return (int)Ogre::EMITTER_COLUMN_UP;
	else if (str == "���巢��")
		return (int)Ogre::EMITTER_COLUMN;

	return (int)Ogre::EMITTER_PLANE;;
}

CString CCvtHelper::ValueToEmitterType(int i)
{
	Ogre::EmitterType type = (Ogre::EmitterType)i;
	switch(type)
	{
	case Ogre::EMITTER_PLANE:
		return _T("ƽ�淢��");
	case Ogre::EMITTER_CIRCLE:
		return _T("Բ�淢��");
	case Ogre::EMITTER_SPHERE:
		return _T("���巢��");
	case Ogre::EMITTER_SPHERE_FACE:
		return _T("���淢��");
	case Ogre::EMITTER_COLUMN_UP:
		return _T("���淢��");
	case Ogre::EMITTER_COLUMN:
		return _T("���巢��");
	default:
		return _T("ƽ�淢��");
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
	bt.push_back("��͸��");
	bt.push_back("Alpha����");
	bt.push_back("Alpha���");
	bt.push_back("��ӻ��");
	bt.push_back("���");
	bt.push_back("���");
	bt.push_back("���X2");
}
int CCvtHelper::BlendTypeToValue(CString str)
{
	if(str == "��͸��")				return 0;
	else if(str == "Alpha����")		return 1;
	else if(str == "Alpha���")		return 2;
	else if(str == "��ӻ��")		return 3;
	else if(str == "���")		    return 4;
	else if(str == "���")		    return 5;
	else if(str == "���X2")		    return 6;

	return 0;
}

CString CCvtHelper::ValueToBlendType(int i)
{
	switch(i)
	{
	case 0:		return _T("��͸��");
	case 1:		return _T("Alpha����");
	case 2: return _T("Alpha���");
	case 3: return _T("��ӻ��");
	case 4: return _T("���");
	case 5: return _T("���");
	case 6: return _T("���X2");

	default:
		return _T("��͸��");
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
	if(str == _T("���β���"))
	{
		return 1;
	}
	else if(str == _T("ѭ������"))
	{
		return 0;
	}
	else if(str == _T("���β��ź�ֹͣ"))
	{
		return 2;
	}
	return 1;
}

CString CCvtHelper::LoopmodToString(int loopmod)
{
	if(loopmod == 0)
		return _T("ѭ������");
	else if(loopmod == 1)
		return _T("���β���");
	else if(loopmod == 2)
		return _T("���β��ź�ֹͣ");
	return _T("���β���");
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
		return _T("ֱ���Դ");
	case Ogre::LT_POINT:
		return _T("���Դ");
	case Ogre::LT_SPOT:
		return _T("���Դ");
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
		strcpy(tmp,  "ֱ���Դ");
	case Ogre::LT_POINT:
		strcpy(tmp, "���Դ");
	case Ogre::LT_SPOT:
		strcpy(tmp, "���Դ");
	}
	m_StrBufs.push_back(tmp);
	return tmp;
}

Ogre::LightType CCvtHelper::StringToLight(LPTSTR light)
{
	CString str = light;
	if(str == _T("ֱ���Դ"))
		return Ogre::LT_DIRECT;
	else if(str == _T("���Դ"))
		return Ogre::LT_POINT;
	else if(str == _T("���Դ"))
		return Ogre::LT_SPOT;
	return Ogre::LT_NULL;
}

CString CCvtHelper::FacemodeToString(int facemode)
{
	switch(facemode) 
	{
	case 0:
		return _T("��Թ۲��");
	case 1:
		return _T("����ֱ��ת");
	case 2:
		return _T("����Ϸ�");
	case 3:
		return _T("����Ƭ");
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
		strcpy(tmp, "��Թ۲��");
		break;
	case 1:
		strcpy(tmp, "����ֱ��ת");
		break;
	case 2:
		strcpy(tmp, "����Ϸ�");
		break;
	case 3:
		strcpy(tmp, "����Ƭ");
		break;
	}
	m_StrBufs.push_back(tmp);
	return tmp;
}

int CCvtHelper::StringToFacemode(LPTSTR facemode)
{
	CString str = facemode;
	if(str == _T("��Թ۲��"))
		return 0;
	else if(str == _T("����ֱ��ת"))
		return 1;
	else if(str == _T("����Ϸ�"))
		return 2;
	else if(str == _T("����Ƭ"))
		return 3;
	return 0;
}

CString CCvtHelper::ParticleRotToString(Ogre::ParticleDirType rot)
{
	switch(rot) 
	{
	case Ogre::PR_FACE_CAMERA:
		return _T("��Թ۲��");
	case Ogre::PR_ROT_ABOUT_UP:
		return _T("����ֱ��ת");
	case Ogre::PR_FACE_UP:
		return _T("����Ϸ�");
	case Ogre::PR_FACE_UP_NOROT:
		return _T("����Ϸ�����ת");
	case Ogre::PR_ROT_ABOUT_DIR:
		return _T("���˶�������ת");
	case Ogre::PR_FACE_DIR:
		return "�����˶�����";
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
		strcpy(tmp, "��Թ۲��");
		break;
	case Ogre::PR_ROT_ABOUT_UP:
		strcpy(tmp, "����ֱ��ת");
		break;
	case Ogre::PR_FACE_UP:
		strcpy(tmp, "����Ϸ�");
		break;
	case Ogre::PR_FACE_UP_NOROT:
		strcpy(tmp, "����Ϸ�����ת");
		break;
	case Ogre::PR_ROT_ABOUT_DIR:
		strcpy(tmp, "���˶�������ת");
		break;
	case Ogre::PR_FACE_DIR:
		strcpy(tmp, "�����˶�����");
		break;
	}
	m_StrBufs.push_back(tmp);
	return tmp;
}

Ogre::ParticleDirType CCvtHelper::StringToParticleRot(LPTSTR rot)
{
	CString str = rot;
	if(str == _T("��Թ۲��"))
		return Ogre::PR_FACE_CAMERA;
	else if(str == _T("����ֱ��ת"))
		return Ogre::PR_ROT_ABOUT_UP;
	else if(str == _T("����Ϸ�"))
		return Ogre::PR_FACE_UP;
	else if(str == _T("���˶�������ת"))
		return Ogre::PR_ROT_ABOUT_DIR;
	else if(str == "�����˶�����")
		return Ogre::PR_FACE_DIR;
	else if(str == "����Ϸ�����ת")
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
//	int   nDstLength;                 //   UNICODE���ַ���Ŀ   
//	WCHAR   wchar[128];             //   UNICODE��������   
//
//	//   �ַ���-->UNICODE��   
//	memset(wchar,0,128);//   I   added   
//	nDstLength   =   ::MultiByteToWideChar(CP_ACP,   0,   pSrc,   nSrcLength,   wchar,   128);   
//
//	//   �ߵ��ֽڶԵ������   
//	for(int   i=0;   i<nDstLength;   i++)   
//	{   
//		//   �������λ�ֽ�   
//		*pDst++   =   wchar[i]   >>   8;   
//		//   �������λ�ֽ�   
//		*pDst++   =   wchar[i]   &   0xff;   
//	}   
//
//	//   ����Ŀ����봮����   
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
	posteffects.push_back("��̬ģ��");
	posteffects.push_back("ѣ��");
	posteffects.push_back("ȫ���ữ");
	posteffects.push_back("�ڰ�");
	posteffects.push_back("��ͷ����");
	posteffects.push_back("����");
	posteffects.push_back("���淺ɫ");
	posteffects.push_back("��ɫ͸��");
	posteffects.push_back("��������");
	posteffects.push_back("�Զ���Ч��");
}

Ogre::POSTEFFECT_T CCvtHelper::StringToPostEffect(CString str)
{
	if(str == "��̬ģ��")
		return Ogre::PE_MOTION_BLUR;
	else if(str == "ѣ��")
		return Ogre::PE_GLOW;
	else if(str == "ȫ���ữ")
		return Ogre::PE_BLOOM;
	else if(str == "�ڰ�")
		return Ogre::PE_GRAY;
	else if(str == "��ͷ����")
		return Ogre::PE_SHAKE;
	else if(str == "����")
		return Ogre::PE_FROZEN;
	else if(str == "���淺ɫ")
		return Ogre::PE_FSEFFECT;
	else if(str == "��ɫ͸��")
		return Ogre::PE_TRANSPARENT;
	else if(str == "��������")
		return Ogre::PE_ZOOM;
	else if(str == "�Զ���Ч��")
		return Ogre::PE_CUSTOM;
	return Ogre::PE_MOTION_BLUR;
}

CString	CCvtHelper::PostEffectToString(Ogre::POSTEFFECT_T posteffect)
{
	switch(posteffect)
	{
	case Ogre::PE_MOTION_BLUR:
		return "��̬ģ��";
		break;
	case Ogre::PE_GLOW:
		return "ѣ��";
		break;
	case Ogre::PE_BLOOM:
		return "ȫ���ữ";
		break;
	case Ogre::PE_GRAY:
		return "�ڰ�";
		break;
	case Ogre::PE_SHAKE:
		return "��ͷ����";
		break;	
	case Ogre::PE_FROZEN:
		return "����";
		break;
	case Ogre::PE_FSEFFECT:
		return "���淺ɫ";
		break;
	case Ogre::PE_TRANSPARENT:
		return "��ɫ͸��";
	case Ogre::PE_ZOOM:
		return "��������";
		break;
	case Ogre::PE_CUSTOM:
		return "�Զ���Ч��";
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
		strcpy(tmp, "��̬ģ��");
		break;
	case Ogre::PE_GLOW:
		strcpy(tmp, "ѣ��");
		break;
	case Ogre::PE_BLOOM:
		strcpy(tmp, "ȫ���ữ");
		break;
	case Ogre::PE_GRAY:
		strcpy(tmp, "�ڰ�");
		break;
	case Ogre::PE_SHAKE:
		strcpy(tmp, "��ͷ����");
		break;
	case Ogre::PE_FROZEN:
		strcpy(tmp, "����");
		break;
	case Ogre::PE_FSEFFECT:
		strcpy(tmp, "���淺ɫ");
		break;
	case Ogre::PE_TRANSPARENT:
		strcpy(tmp, "��ɫ͸��");
		break;
	case Ogre::PE_ZOOM:
		strcpy(tmp, "��������");
		break;
	case Ogre::PE_CUSTOM:
		strcpy(tmp, "�Զ���Ч��");
		break;
	}	
	m_StrBufs.push_back(tmp);

	return tmp;
}

void CCvtHelper::EnumGeomType(std::vector<CString>& geomTypes)
{
	geomTypes.push_back("����");
	geomTypes.push_back("Բ��");
	geomTypes.push_back("ԲͰ");
	geomTypes.push_back("Բ��");
}

CString CCvtHelper::GeomTypeToString(Ogre::PARAMSHAPE_TYPE geotype)
{
	switch(geotype)
	{
	case Ogre::GEO_SPHERE:
		return "����";
		break;
	case Ogre::GEO_TORUS:
		return "Բ��";
		break;
	case Ogre::GEO_TUBE:
		return "ԲͰ";
		break;
	case Ogre::GEO_COLUMN:
		return "Բ��";
		break;
	default:
		break;
	}

	return "����";
}

char* CCvtHelper::GeomTypeToStringAsPtr(Ogre::PARAMSHAPE_TYPE geotype)
{
	char* tmp = new char[256];
	switch(geotype)
	{
	case Ogre::GEO_SPHERE:
		strcpy(tmp, "����");
		break;
	case Ogre::GEO_TORUS:
		strcpy(tmp, "Բ��");
		break;
	case Ogre::GEO_TUBE:
		strcpy(tmp, "ԲͰ");
		break;
	case Ogre::GEO_COLUMN:
		strcpy(tmp, "Բ��");
		break;
	default:
		strcpy(tmp, "����");
		break;
	}
	m_StrBufs.push_back(tmp);

	return tmp;
}

Ogre::PARAMSHAPE_TYPE CCvtHelper::StringToGeomType(LPTSTR geotype)
{
	CString str = geotype;
	if(str == "����")
		return Ogre::GEO_SPHERE;
	else if(str == "Բ��")
		return Ogre::GEO_TORUS;
	else if(str == "ԲͰ")
		return Ogre::GEO_TUBE;
	else if(str == "Բ��")
		return Ogre::GEO_COLUMN;

	return Ogre::GEO_SPHERE;
}

void CCvtHelper::EnumAnimMode(std::vector<CString>& animMode)
{
	animMode.push_back("���涯��");
	animMode.push_back("��������");
}

CString CCvtHelper::AnimModeToString(Ogre::BIND_ANIM_MODE mode)
{
	if(mode == Ogre::BAM_SELF)
		return "��������";
	if(mode == Ogre::BAM_PARENT)
		return "���涯��";

	assert(0);
	return "��������";
}

char* CCvtHelper::AnimModeToStringAsPtr(Ogre::BIND_ANIM_MODE mode)
{
	char* tmp = new char[256];
	switch(mode)
	{
	case Ogre::BAM_SELF:
		strcpy(tmp, "��������");
		break;

	case Ogre::BAM_PARENT:
		strcpy(tmp, "���涯��");
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
	if(str == "��������")
		return Ogre::BAM_SELF;
	if(str == "���涯��")
		return Ogre::BAM_PARENT;
	else
	{
		assert(0);
		return Ogre::BAM_SELF;
	}
}

void CCvtHelper::EnumAddressMode(std::vector<CString>& addressMode)
{
	addressMode.push_back("ѭ��");
	addressMode.push_back("����");
}

CString CCvtHelper::AddressModeToString(int mode)
{
	if(mode == 0)
		return "ѭ��";
	else if(mode == 1)
		return "����";

	return "����";
}

int CCvtHelper::StringToAddressMode(CString str)
{
	if(str == "ѭ��")
		return 0;
	else if (str == "����") 
		return 1;

	return 1;
}

char* CCvtHelper::AddressModeToStringAsPtr(int mode)
{
	char* tmp = new char[256];
	switch(mode)
	{
	case 0:
		strcpy(tmp, "ѭ��");
		break;

	case 1:
		strcpy(tmp, "����");
		break;
	default:
		strcpy(tmp, "����");
		break;

	}

	m_StrBufs.push_back(tmp);

	return tmp;
}

void CCvtHelper::EnumSoundPlayMode(std::vector<CString>& mode)
{
	mode.push_back("���ɲ���");
	mode.push_back("���Ʋ���");
}

Ogre::SOUND_PLAY_MODE CCvtHelper::StringToSoundPlayMode(CString str)
{
	if(str == "���ɲ���")
	{
		return Ogre::PM_FREE;
	}
	else if(str == "���Ʋ���")
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
		strcpy(tmp, "���ɲ���");
		break;

	case Ogre::PM_LIMITED:
		strcpy(tmp, "���Ʋ���");
		break;
	default:
		strcpy(tmp, "���ɲ���");
		break;

	}

	m_StrBufs.push_back(tmp);

	return tmp;
}



void CCvtHelper::EnumBillboardFrameMode(std::vector<CString>& mode)
{
	mode.push_back("�̶�֡���");
	mode.push_back("����ؼ�֡");
}

Ogre::SEQ_FRAME_TYPE CCvtHelper::StringToBillboardFrameMode(CString str)
{
	if(str == "�̶�֡���")
	{
		return Ogre::SFT_FIXED_FRAME_TIME;
	}
	else if(str == "����ؼ�֡")
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
		strcpy(tmp, "�̶�֡���");
		break;

	case Ogre::SFT_FIT_KEYFRAME:
		strcpy(tmp, "����ؼ�֡");
		break;
	default:
		strcpy(tmp, "�̶�֡���");
		break;

	}

	m_StrBufs.push_back(tmp);

	return tmp;
}

void CCvtHelper::EnumBeamWaveType(std::vector<CString>& mode)
{
	mode.push_back("�������");
	mode.push_back("������");
	mode.push_back("ֱ��");
}

Ogre::BEAM_WAVE_TYPE CCvtHelper::StringToBeamWaveType(CString str)
{
	if(str == "�������")
	{
		return Ogre::BWT_RANDOM;
	}
	else if(str == "������")
	{
		return Ogre::BWT_SIN;
	}
	else if(str == "ֱ��")
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
		strcpy(tmp, "�������");
		break;

	case Ogre::BWT_SIN:
		strcpy(tmp, "������");
		break;

	case Ogre::BWT_STRATE:
		strcpy(tmp, "ֱ��");
		break;
	default:
		strcpy(tmp, "�������");
		break;

	}

	m_StrBufs.push_back(tmp);
	return tmp;
}