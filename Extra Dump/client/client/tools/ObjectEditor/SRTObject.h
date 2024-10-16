
/* ��������ΪCModelMotionInfo�ĸ��࣬�������Ҫ���ǵ���CSRTBar��Ӧ�ؼ��ϵ����ݴ��͸�CModelMotionInfo
** CSTRObject��Ӧ�Ķ����Ƚ϶�
** CFloatObject��Ӧ��ɫ��֮��Ķ����
*/
#pragma once
#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "OgreColourValue.h"

class CSRTObject
{
public:
	CSRTObject(void);
	virtual ~CSRTObject(void);

	virtual Ogre::Vector3	GetPosition();
	virtual Ogre::Quaternion	GetRotation();
	virtual Ogre::Vector3	GetScale();
	virtual float			GetTransparent();
	virtual Ogre::Vector3	GetRotateAngle();

	virtual void	SetPosition(Ogre::Vector3 pos);
	virtual void	SetRotation(Ogre::Quaternion rot);
	virtual void	SetScale(Ogre::Vector3 scale);
	virtual void	SetTransparent(float fTransparent);
	virtual void	SetRotationAngle(Ogre::Vector3 rotAngle);

	virtual void	SetOffColor(Ogre::ColourValue mOffColor);
	virtual Ogre::ColourValue GetOffColor();

	virtual void	SetFade(bool bFade);
	virtual bool	GetFade();

	virtual bool	IsGeomScale()	{ return false; };
	virtual CString GetObjName()	{ return ""; };

	static CSRTObject ms_DummyObj;

};

class CFloatObject
{
public:
	virtual float	GetFloat()				{ return 0.0f ; };
	virtual void	SetFloat(float val)		{ };
	virtual void	GetFloatRange(float* pmin, float* pmax, float* ratio) { *pmin=-100.0f; *pmax=100.0f; *ratio=1.0f; };
	virtual CString	GetFloatName()			{return "����"; };

	static CFloatObject ms_DummyObj;

};

class CColorObject
{
public:
	virtual Ogre::ColourValue	GetColor()	{ return Ogre::ColourValue(0,0,0,0); };
	virtual void			SetColor(Ogre::ColourValue val) { };

	static CColorObject ms_DummyObj;
};

class CRemanetShadowObject
{
public:
	//��Ӱ����
	virtual int GetIntervalTime(){ return 40; }
	virtual void SetIntervalTime(int nIntervalTime){}

	virtual int GetImages(){ return 8; }
	virtual void SetImages(int nImages){};

	static CRemanetShadowObject ms_DummyObj;
};

class COffColorObject
{
public:
	virtual Ogre::ColourValue	GetOffColor();
	virtual void			SetOffColor(Ogre::ColourValue mOffColor);

	static COffColorObject ms_DummyObj;
};

class CIceFrozenObject
{
public:
	virtual Ogre::BlendMode GetIceFrozenBlend();
	virtual CString GetOverlayTex();
	virtual void SetIceFrozenBlend(Ogre::BlendMode blendmode, CString &texpath,float alpha = 1.0f);
	virtual float GetIceFrozenAlpha();

	static CIceFrozenObject ms_DummyObj;
};

//����ģ����������CFloatObject��ʾ��
//��4��float����������seq��Ϊ1��2��3��4  ��Լ��������
class CFocusBlurObject
{
public:
	virtual float	GetFocusBlurParams(int nSeq) { return 0.0f; }
	virtual void	SetFocusBlurParams(int nSeq, float val){ }

	static CFocusBlurObject ms_DummyObj;
};
