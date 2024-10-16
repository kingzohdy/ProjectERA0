
#include "stdafx.h"
#include "SRTObject.h"

CSRTObject CSRTObject::ms_DummyObj;
CFloatObject CFloatObject::ms_DummyObj;
CColorObject CColorObject::ms_DummyObj;
CRemanetShadowObject CRemanetShadowObject::ms_DummyObj;
COffColorObject COffColorObject::ms_DummyObj;
CIceFrozenObject CIceFrozenObject::ms_DummyObj;
CFocusBlurObject CFocusBlurObject::ms_DummyObj;

CSRTObject::CSRTObject(void)
{
}

CSRTObject::~CSRTObject(void)
{
}


Ogre::Vector3 CSRTObject::GetPosition()
{
	return Ogre::Vector3(0.0f, 0.0f, 0.0f);
}

Ogre::Quaternion CSRTObject::GetRotation()
{
	Ogre::Quaternion rot;
	rot.identity();
	return rot;
}

Ogre::Vector3 CSRTObject::GetScale()
{
	return Ogre::Vector3(1.0f, 1.0f, 1.0f);
}

float CSRTObject::GetTransparent()
{
	return 1.0f;
}

Ogre::Vector3	CSRTObject::GetRotateAngle()
{
	return Ogre::Vector3(0.0f, 0.0f, 0.0f);
}

void	CSRTObject::SetRotationAngle(Ogre::Vector3 rotAngle)
{
}

void	CSRTObject::SetOffColor(Ogre::ColourValue mOffColor)
{
}

Ogre::ColourValue CSRTObject::GetOffColor()
{
	return Ogre::ColourValue(0.0f , 0.0f , 0.0f , 0.0f);
}

void	CSRTObject::SetFade(bool bFade)
{
}

bool CSRTObject::GetFade()
{
	return false;
}

void CSRTObject::SetTransparent(float fTransparent)
{
}

void CSRTObject::SetPosition(Ogre::Vector3 pos)
{
}

void CSRTObject::SetRotation(Ogre::Quaternion rot)
{
}

void CSRTObject::SetScale(Ogre::Vector3 scale)
{
}

Ogre::ColourValue COffColorObject::GetOffColor()
{
	return Ogre::ColourValue(0.0f , 0.0f , 0.0f , 0.0f);
}

void COffColorObject::SetOffColor(Ogre::ColourValue mOffColor)
{
}

Ogre::BlendMode CIceFrozenObject::GetIceFrozenBlend()
{
	return Ogre::BLEND_ALPHATEST;
}

CString CIceFrozenObject::GetOverlayTex()
{
	return "";
}

float CIceFrozenObject::GetIceFrozenAlpha()
{
	return 1.0f;
}

void CIceFrozenObject::SetIceFrozenBlend(Ogre::BlendMode blendmode, CString &texpath, float alpha)
{

}
