
#pragma once
#include "OgreRenderTypes.h"
#include "OgreParticleEmitterData.h"
#include "ObjectEditorTypes.h"
#include "OgreEntityMotionElement.h"
#include "OgreParametricShapeData.h"
#include "OgreSoundData.h"
#include "OgreBillboardData.h"
#include "OgreBeamEmitterData.h"

class CCvtHelper
{
public:
	CCvtHelper(void);
	virtual ~CCvtHelper(void);


public:
	char* V3ToString(Ogre::Vector3& vec);
	char* ColorToString(Ogre::ColourValue& color);
	char* FloatToString(float value);
	char* IntToString(int value);

	bool ParseFloat(LPTSTR str, float* fResult);
	bool ParseVector3(LPTSTR str, Ogre::Vector3* result);
	bool ParseColor(LPTSTR str, Ogre::ColourValue* result);
	bool ParseInt(LPTSTR str, int* iResult);

	int StringToInt(LPTSTR str);

	int EmitterTypeToValue(CString str);
	CString ValueToEmitterType(int i);
	char* ValueToEmitterTypeAsPtr(int i);

	int ParticleTypeToValue(CString str);
	CString ValueToParticleType(int i);
	char* ValueToParticleTypeAsPtr(int i);

	int BlendTypeToValue(CString str);
	CString ValueToBlendType(int i);
	char* ValueToBlendTypeAsPtr(int i);
	void EnumBlendType(std::vector<CString>& bt);

	CString PathToAddr(CString path);
	CString AddrToPath(CString addr);

	int StringToLoopmod(CString str);
	CString LoopmodToString(int loopmod);

	COLORREF ColorFloatToInt(Ogre::ColourValue clr);
	Ogre::ColourValue ColorIntToFloat(COLORREF clr);

	CString LightToString(Ogre::LightType light);
	char* LightToStringAsPtr(Ogre::LightType light);
	Ogre::LightType StringToLight(LPTSTR light);

	CString FacemodeToString(int facemode);
	char* FacemodeToStringAsPtr(int facemode);
	int StringToFacemode(LPTSTR facemode);

	CString ParticleRotToString(Ogre::ParticleDirType facemode);
	char* ParticleRotToStringAsPtr(Ogre::ParticleDirType facemode);
	Ogre::ParticleDirType StringToParticleRot(LPTSTR facemode);

	char*	Utf8ToGb(const char* src);
	char*	GbToUtf8(const char* src);
	WCHAR*	GbToWide(const char* src);

	EFFECT_TYPE ChunkTypeToEffectType(Ogre::Resource *pres);

	CString	AnimIDToDesc(int id);
	char*	AnimIDToDescAsPtr(int id);
	int		DescToAnimID(CString str);

	void EnumPostEffect(std::vector<CString> &posteffects);
	Ogre::POSTEFFECT_T StringToPostEffect(CString str);
	CString	PostEffectToString(Ogre::POSTEFFECT_T posteffect);
	char* PostEffectToStringAsPtr(Ogre::POSTEFFECT_T posteffect);

	void EnumGeomType(std::vector<CString>& geomTypes);
	CString GeomTypeToString(Ogre::PARAMSHAPE_TYPE geotype);
	char* GeomTypeToStringAsPtr(Ogre::PARAMSHAPE_TYPE geotype);
	Ogre::PARAMSHAPE_TYPE StringToGeomType(LPTSTR geotype);

	void EnumAnimMode(std::vector<CString>& animMode);
	CString AnimModeToString(Ogre::BIND_ANIM_MODE mode);
	Ogre::BIND_ANIM_MODE StringToAnimMode(CString str);
	char* CCvtHelper::AnimModeToStringAsPtr(Ogre::BIND_ANIM_MODE mode);

	void EnumAddressMode(std::vector<CString>& addressMode);
	CString AddressModeToString(int mode);
	int StringToAddressMode(CString str);
	char* CCvtHelper::AddressModeToStringAsPtr(int mode);

	void EnumSoundPlayMode(std::vector<CString>& addressMode);
	Ogre::SOUND_PLAY_MODE StringToSoundPlayMode(CString str);
	char* SoundPlayModeToStringAsPtr(Ogre::SOUND_PLAY_MODE mode);


	void EnumBillboardFrameMode(std::vector<CString>& mode);
	Ogre::SEQ_FRAME_TYPE StringToBillboardFrameMode(CString str);
	char* BillboardFrameModeToStringAsPtr(Ogre::SEQ_FRAME_TYPE mode);

	void EnumBeamWaveType(std::vector<CString>& mode);
	Ogre::BEAM_WAVE_TYPE StringToBeamWaveType(CString str);
	char* BeamWaveTypeToStringAsPtr(Ogre::BEAM_WAVE_TYPE mode);

protected:
	std::vector<char*> m_StrBufs;
};
