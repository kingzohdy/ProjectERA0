
#pragma once

class CAvatarShowInfo;
class CAnchorSet;
class CBeam;
class CBillboard;
struct BIND_INFO;
class CCameraCtrl;
class CCommandMgr;
class CCoord;
class CColorObject;
class CRemanetShadowObject;
class COffColorObject;
class CIceFrozenObject;
class CFocusBlurObject;
class CDocInfo;
class CEffectCommand;
class CEffectUnit;
class CEffectEditBar;
class CFloatObject;
class CInteract;
class CLight;
class CKeyFrameObject;
class CModelMotionInfo;
class CNotifiedProp;
class CNotifiedColorProp;
class CObjectLayerInfo;
class CParticle;
class CRefNodeMgr;
class CSceneGround;
class CSelectBox;
class CSkeleton;
class CSplitterGridView;
class CSRTObject;
class CTimeAixes;
class CTimeLayer;
class CUnknownObject;
class CUnknownObjectCommand;
class CVirtualNode;

//此类对象被对象层存储过
enum EFFECT_TYPE 
{

	EFFECT_PEMITTER,
	EFFECT_LIGHT,
	EFFECT_BILLBOARD,
	EFFECT_RIBBON,
	EFFECT_BEAM,
	EFFECT_SOUND,
	EFFECT_DECAL,
	EFFECT_UNKNOWN,
	EFFECT_GEOM,
	EFFECT_TEST,
	EFFECT_VIRTUALNODE,
	EFFECT_POSTEFFECT,
	EFFECT_ERRORTYPE
};

enum SCALE_TYPE
{
	ST_NO_SCALING,
	ST_GEOM_SCALING,
	ST_RANDOM_SCALING
};

enum COOR_ARROW
{
	COOD_X = 1,
	COOD_Y = 2,
	COOD_Z = 3,
	COOD_NONE = 4,

	COOD_RX,
	COOD_RY,
	COOD_RZ,

	COOD_SX,
	COOD_SY,
	COOD_SZ
};

enum COORD_TYPE
{
	CT_NONE,
	CT_POS,
	CT_ROTATE,
	CT_SCALE
};

enum DATA_UPDATA_TYPE
{
	DATA_TO_PROP,
	PROP_TO_DATA,
	CREATE_PROP,
	INC_DATA_RELATED,
	DEC_DATA_RELATED,
	INC_DATA_ALL,
	DEC_DATA_ALL
};

enum SKILL_DISPLAY_TYPE
{
	SDY_ANY_SKILL,
	SDY_NORMAL_ATTACK,
	SDY_SPELL_ATTACK,
	SDY_NORMAL_ATTACK_WITH_FLY,
	SDY_SPELL_ATTACK_WITH_FLY,
	SDY_NORMAL_SELF_STATE,
	SDY_SPELL_SELF_STATE
};

enum SKILL_STATE
{
	SKL_PREPARE,
	SKL_FLYING,
	SKL_HITTING,
	SKL_NOTING
};

enum SKILL_TARGET_TYPE
{
	STT_RESOURCE,
	STT_SELF
};

class CDocInfo
{
public:
	void NewDoc()		{ m_bNeedSave = true;  m_bHasName = false;};
	void Open()			{ m_bNeedSave = false; m_bHasName = true; };
	void Modify()		{ m_bNeedSave = true;};
	void Save()			{ m_bNeedSave = false; m_bHasName = true; };

	bool m_bNeedSave;
	bool m_bHasName;

	bool m_bIntegrate;
};

struct SELECT_MOTION_STRUCT
{
	CString strName;
	int layer;
	bool bIsPublic;
};

class CFrameJob
{
public:
	virtual void DoJobFrame(int dtick) = 0;
};

extern int g_nExten;
extern char *g_FileExten[];
extern char* g_strPublicMotionAddr;

extern char* g_chCharactorResPath;
extern char* g_pszModelResPath;
extern char* g_chSpecialTextureResPath;
extern char* g_chPublicEffectResPath;
extern char* g_strMaterialMotionAddr;
extern char* g_strWorkMotionAddr;

extern char* g_strShowModelEffectAddr;
extern char* g_strMaterialShowModelEffectAddr;
extern char* g_strWorkShowModelEffectAddr;

extern bool IsTextureResource(const char *path);
extern LRESULT SendMainFrameMsg(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
extern LRESULT PostMainFrameMsg(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

#define DEF_FX_TEXTURE "toolres\\fxeditor\\deftex.otex"