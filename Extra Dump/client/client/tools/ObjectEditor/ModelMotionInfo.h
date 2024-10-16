
#pragma once
#include "SRTObject.h"
#include "OgreEntityMotionData.h"

enum SAVE_POS_T
{
	SP_ON_MODEL,
	SP_ON_FILE,

	SP_ANY
};

class CTimeHolder
{
public:
	CTimeHolder(void);
	~CTimeHolder(void);


	void SetHotTime(float fHotTime);

	void SetEffectDynamic(CEffectUnit* pEffect);
	void SetEffectStatic(CEffectUnit* pEffect);

	float	m_fHotTime;

	CModelMotionInfo* m_pMotionInfo;
};

class CModelMotionInfo : public CSRTObject, public CFloatObject, public CColorObject, public CRemanetShadowObject, public COffColorObject, public CIceFrozenObject, public CFocusBlurObject
{
public:
	CModelMotionInfo(void);
	~CModelMotionInfo(void);

	//struct OBJINFO
	//{
	//	CEffectUnit* pEffect;
	//	float start;
	//	float end;
	//	bool hasStart;
	//	bool hasEnd;
	//	int anchor;
	//	Ogre::Vector3 offset;
	//	Ogre::Quaternion rotate;
	//	Ogre::MOTIONOBJTYPE_T type;
	//	int anim;
	//	int priority;
	//	Ogre::POSTEFFECT_T posteffect;
	//	Ogre::ColourValue fs_color;
	//	Ogre::KeyFrameArray<Ogre::Vector4> fs_colorkeys;
	//	Ogre::KeyFrameArray<float> transparent_keys;
	//	std::vector<Ogre::PostproPhaseType> customPE;
	//};

	struct MOTION
	{
		float				fTime;
		std::string			name;
		std::vector<CObjectLayerInfo*> objs;
		SAVE_POS_T			savePos;
		std::string			filename;
		Ogre::MOTIONLOOP_T	loop;
		bool				bIsPublic;

		int					stMotionLodLevel;

		CDocInfo			docInfo;
	};

	int m_iHoverObj;

	Ogre::Vector3 m_OldRot;
	float		 m_fMouseOffset;

	std::vector<MOTION*> m_motions;

	bool OnLButtonDown(float x, float y);
	bool OnLButtonUp(float x, float y);
	bool OnMouseMove(float x, float y);
	void DoFrame(Ogre::uint time);
	void SetSelect(int mon, int obj);
	void SetHotTime(float time);
	void SetSelectedKey(int key);
	void SetCoodPlace(CObjectLayerInfo* pLayer, float fTime);

	MOTION*	FindMotionOfObject(CObjectLayerInfo* pObj);
	int		FindMotionIndexOfObject(CObjectLayerInfo* pObj);

	int		FindObjectIndex(MOTION* pMotion, CObjectLayerInfo* pObj);
	void CreateInteract();
	void DestroyInteract();
	void Clear();
	void ClearModelPrivateMotion();

	int ChangeLayerOrder(int iMotion, bool* pSrcPlace, int num, int iInsertAfter);
	int ChangeLayerOrder(int iMotion, int iSrcPlace, int iInsertAfter);

	int SetParentRelation(int iMotion, bool* pSrcPlace, int num, int iParent);
	int SetParentRelation(CObjectLayerInfo* pSrcObj, CObjectLayerInfo* pParent);
	int SetParentRelation(int iMotion, int iSrcPlace, int iParent);
	void ArrangeOrder(MOTION* pmo);

	void LoadSingleMotion(Ogre::EntityMotionData* pmotionchunk, MOTION *pmo, const char* pFileName = NULL);
	void SaveSingleMotion(MOTION* pmo, Ogre::EntityMotionData* pMotionChunk, const char* pFileName = NULL);

	bool SaveSingleMotionToFile(MOTION* pmo, const char* pFileName, bool bAsBackup = false);
	bool LoadSingleMotionFromFile(MOTION *pmo, const char* pFileName);

	//后面两个参数为了兼容保存多个选中层，当nNUM!=0时表示保存多项，否则按照以前的保存方式
	bool SaveMotionObjectAsMotion(MOTION* pmo, int index, const char* pFileName, int nNum=0, bool* bSave=NULL);

	void LoadFromChunk(Ogre::EntityData *pentitydata);
	void CreateMotionChunk(Ogre::EntityData *pentitydata);
	void CreateSingleMotionChunk(int iMon, Ogre::EntityData *pentitydata);


	void DeleteObj(int mon, int obj);
	void DeleteMontion(int mon, bool bRemoveElement = false);
	int AddMotion(MOTION* pmo);
	int FindMotionByFileName(const char* pFileName);
	//int FindObjectIndex(CObjectLayerInfo* pObj);
	void MergeMotion(int iFrom, int iTo, bool bDeleteFrom);
	void MergeMotion(MOTION* pFrom, int iTo);


	void AddObjLayer(int mon, CObjectLayerInfo* pLayer);
	CObjectLayerInfo* GetSelectedObj();
	MOTION*	GetSelectedMotion();

	bool CanMerge(int mon, int obj);
	void MergeLayerKeyFrame(int mon, int obj);
	void OverwriteMotionFrame(int mon, int obj);
	void OverwriteEffectFrame(int mon, int obj);
	bool ApartKeyFrame(int mon, int obj);

	void RefreshEffect(CEffectUnit* pEffect);

	MOTION* GetMotionByName(CString name);

	bool IsShowPath() { return m_bShowPath; };
	void ShowPath(bool bShow) { m_bShowPath = bShow; };

	bool m_bShowPath;

	bool IsLoopPlay() { return m_bLoopPlay; };
	void LoopPlay(bool bLoop) { m_bLoopPlay = bLoop; };

	void PasteLayers();


	bool m_bLoopPlay;


	// -----------SRT Object----------- [6/17/2008]
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

	virtual bool	IsGeomScale();
	virtual CString GetObjName();

	// -----------Color and Float------------------------- [6/20/2008]
	virtual float	GetFloat();		
	virtual void	SetFloat(float val);
	virtual void	GetFloatRange(float* pmin, float* pmax, float* ratio);
	virtual CString	GetFloatName();
	virtual Ogre::ColourValue	GetColor();
	virtual void			SetColor(Ogre::ColourValue val);

	//overlay material
	virtual Ogre::BlendMode GetIceFrozenBlend();
	virtual CString GetOverlayTex();
	virtual void SetIceFrozenBlend(Ogre::BlendMode blendmode, CString &texpath, float alpha=1.0f);
	virtual float GetIceFrozenAlpha();


	CCoord* m_pCood;
	int m_iSelMontion;
	int m_iSelObj;
	float	m_fHotTime; //设置的当前时间
	int m_iSelKey;
	CTimeHolder m_timeHolder;

	COOR_ARROW m_tranArr;
};
