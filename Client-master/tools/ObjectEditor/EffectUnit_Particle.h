
#pragma once
#include "EffectUnit.h"
#include "InteractObject.h"
#include "BindInfo.h"
#include "EffectCommand.h"

#include "OgreVector3.h"
#include "OgreParticleEmitterData.h"

enum PARTICLE_INTERACT_TARGET
{
	PIT_NODE,
	PIT_COLLISION_FACE
};

class CParticleInteract : public CInteract
{
public:
	CParticleInteract();
	virtual ~CParticleInteract();
	//virtual bool OnLBtnDown(float x, float y, bool bForceOn);
	//virtual bool OnLBtnUp(float x, float y);
	//virtual bool OnMouseMove(float x, float y);
	//virtual bool Pick(float x, float y);

	virtual void DoFrame(Ogre::uint dtick) ;

	void DrawParticleShape();

	void DrawCollisionFace();

	virtual void			OnActive();
	virtual void			OnDeactive();
	virtual Ogre::Quaternion GetOldRotation();
	virtual Ogre::Vector3	GetOldPosition();
	virtual void			SetNewRototion(Ogre::Quaternion rot);
	virtual void			SetNewPosition(Ogre::Vector3 pos);

	Ogre::SceneNode*			GetFaceParent(Ogre::PECollisionFace& face);

	void					ResetInteractObj();
	void					SetInteractObj(PARTICLE_INTERACT_TARGET type, int face = -1);

	PARTICLE_INTERACT_TARGET m_InteractObjType;
	int			m_iFaceIndex;


	//void Clear();
	//void Create(CParticle* pTarget);

	//CParticle* m_pTarget;

	//bool m_bHovering;
	//bool m_bCood;

	//COOR_ARROW m_tran;
	//CCoord* m_pCood;
};


class CParticleCommand;
class CParticle :
	public CEffectUnit
{
public:

	CParticle(int iBoneID = 0, int iSrcID = 0, unsigned int flags = 0);
	virtual ~CParticle(void);

	virtual Ogre::SceneNode* GetNode();

	virtual Ogre::Resource* GetChunk();
	virtual Ogre::Resource* GetSource() { return m_pSource;};
	virtual CBCGPDockingControlBar* CreateBar(CBCGPFrameWnd* pParent) ;
	virtual CString GetName();
	virtual void SetName(CString strName){ m_strName = strName; };
	virtual void ReleaseNode();
	virtual Ogre::SceneNode* CreateNode(); 
	virtual void RefreshNode();

	virtual bool Create();
	virtual bool Destroy();
	virtual bool Refresh();
	virtual bool LoadEffect(CString name);
	virtual bool SaveEffect(CString name);

	//virtual bool IsVisible();
	//virtual void SetVisible(bool visible);

	virtual bool LoadFromChunk(Ogre::Resource* pChunk);
	bool Validate();
	virtual BIND_INFO* GetBindInfo();
	virtual void SetBindInfo(BIND_INFO* bindinfo);
	virtual EFFECT_TYPE GetType() { return EFFECT_PEMITTER; };

	virtual void UpdateSource();
	virtual void DoFrame(Ogre::uint dtick) ;
	virtual CInteract* GetInteract() { return NULL; };
	virtual SCALE_TYPE		GetScaleType() { return ST_GEOM_SCALING ; }; 	

	void SelectCollisionFace(Ogre::PECollisionFace* pFace);
	virtual CDocInfo*		GetDocInfo() { return &m_docInfo; };
	CDocInfo m_docInfo;

	Ogre::ParticleDirType m_RotStyle;

	//Interact
	//virtual bool OnLBtnDown(float x, float y);
	//virtual bool OnLBtnUp(float x, float y);
	//virtual bool OnMouseMove(float x, float y);
	//virtual void DoFrame(Ogre::uint dtick) ;
	//bool CParticle::CheckTouched(float x, float y, float range);

	virtual bool IsNew() { return m_bIsNew;};
	bool m_bIsNew;

	virtual void ExecuteCommand(CEffectCommand* pCmd);
	virtual CEffectCommand* BeginRecordCommand();
	virtual void EndRecordCommand(CEffectCommand* pCmd);


	virtual bool HasKeyFrame() { return true;};

	bool SetToEmitter(Ogre::ParticleEmitterData* pSrc);
	bool SetFromEmitter(Ogre::ParticleEmitterData* pEmitter);
	void CopyData(Ogre::ParticleEmitterData* pSrc);


	virtual float GetRange();
	void AddReplaceObject(CString strRes);
	void AddReplaceObject(CEffectUnit* pUnit);
	void RemoveReplaceObject(CEffectUnit* pUnit);
	virtual void OnBelongingRemoved(CEffectUnit* pUnit);
public:

	CParticleInteract m_interact;

	//CCoord* m_pCood;
	//bool m_bHovering;
	//bool m_bCood;
	//
	//COOR_ARROW m_tran;
	struct KeyFrameData
	{
		float speed;
		float speedvar;
		float spread;
		float spreadoffset;
		float latitude;
		float accelerate;
		Ogre::Vector3 acceleratedir;
		float lifespan;
		float rate;
		float areal;
		float areaw;
		float aread;
		float deacceleration;
		float sizevar;
		float lifevar;
		float randomspread;

		int tick;
		KeyFrameData()
		{
			areal = 10.0f;
			areaw = 10.0f;
			aread = 0.0f;
			deacceleration = 0.0f;
			accelerate = 0.0f;
			latitude = 180.0f;
			lifespan = 1.0f;
			rate = 10.0f;
			speed = 100.0f;
			acceleratedir = Ogre::Vector3(0.0f, -1.0f, 0.0f);
			speedvar = 0.0f;
			spread = 0.0f;
			spreadoffset = 0.0f;
			sizevar = 0.0f;
			lifevar = 0.0f;
			randomspread = 0.0f;
			tick = 0;
		}
	};

	struct KFItem
	{
		KeyFrameData data;
		HTREEITEM handle;
		CString name;

	};

	//bool m_bVisible;
	CString m_strName;
	CString m_strTextureName;
	CString m_strMaskTextureName;
	std::vector<KFItem> m_KeyFrames;
	Ogre::PEmitterDesc	m_Desc;
	Ogre::PEmitterDescEx	m_DescEx;
	Ogre::PEmitterDescExx m_DescExx;
	int m_PreUpdateTime;
	std::vector<Ogre::PECollisionFace> m_CollisionFaces;

	Ogre::KeyFrameArray<float> m_ParticleSizeKey;
	Ogre::KeyFrameArray<float> m_ParticleAspectKey;
	Ogre::KeyFrameArray<Ogre::ColourValue> m_ParticleColorKey;
	Ogre::KeyFrameArray<float> m_ParticleAlphaKey;

	BIND_INFO m_bindInfo;

	std::vector<std::string> m_replaceObjects; //老版本，只记录替代体的引用资源位置
	std::vector<CEffectUnit*> m_replaceEffect; //新版本，保存所有替代体
	//CParticleDockingDlg m_dockingDlg;

	//Ogre::PEmitterChunk* m_pParticleChunk;
	Ogre::ParticleEmitter* m_pEmitterNode;
	Ogre::ParticleEmitterData* m_pSource;
	CString m_strLastTexture;
	CString m_strLastObject;
	CString m_strLastMaskTexture;
	static int ms_iInstCount;
};




class CParticleCommand : public CEffectCommand
{
public:
	virtual CEffectUnit*	GetTarget();
	virtual bool			NeedRefresh();
	virtual CEffectCommand* GetInversCommand();
	virtual bool			IsIdentical();
	virtual bool			Merge(CEffectCommand* pCmd, bool bForceMerge);

	CParticle* m_pTarget;

	Ogre::PEmitterDesc m_oldDesc;
	Ogre::PEmitterDesc m_newDesc;

	std::vector<CParticle::KFItem> m_oldKf;
	std::vector<CParticle::KFItem> m_newKf;

	CString m_oldTex;
	CString m_newTex;

	BIND_INFO	m_oldBindInfo;
	BIND_INFO	m_newBindInfo;
};

