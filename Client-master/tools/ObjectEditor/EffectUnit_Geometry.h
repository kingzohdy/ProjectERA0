
#pragma once
#include "EffectUnit.h"
#include "EffectCommand.h"
#include "InteractObject.h"
#include "BindInfo.h"

#include "OgreColourValue.h"
#include "OgreParametricShapeData.h"
#include "OgreParametricShape.h"

class CGeometry :
	public CEffectUnit
{
public:
	CGeometry(void);
	virtual ~CGeometry(void);

	virtual Ogre::SceneNode*		GetNode()	{ return m_pNode;};
	virtual void				ReleaseNode();
	virtual CBCGPDockingControlBar* CreateBar(CBCGPFrameWnd* pParent);
	virtual Ogre::Resource*		GetChunk();
	virtual Ogre::Resource*		GetSource() { return m_pSource;};
	virtual Ogre::SceneNode*		CreateNode(); 
	virtual void				RefreshNode();


	virtual CString				GetName();
	virtual void				SetName(CString strName) { m_strName = strName;};

	virtual bool				Create();
	virtual bool				Destroy();
	//virtual bool				IsVisible() {return m_bVisible;};
	//virtual void				SetVisible(bool visible);

	virtual bool				LoadEffect(CString path);
	virtual bool				SaveEffect(CString path);

	virtual bool				LoadFromChunk(Ogre::Resource* pChunk);

	virtual BIND_INFO*			GetBindInfo();
	virtual void				SetBindInfo(BIND_INFO* bindinfo);
	virtual EFFECT_TYPE			GetType();

	virtual void				UpdateSource();

	virtual bool				IsNew() { return false;};

	virtual CInteract*			GetInteract() { return &m_interact; };
	virtual void				DoFrame(Ogre::uint dtick) ;

	virtual float				GetRange();

	virtual void				ExecuteCommand(CEffectCommand* pCmd);
	virtual CEffectCommand*		BeginRecordCommand();
	void						EndRecordCommand(CEffectCommand* pCmd);
	virtual SCALE_TYPE			GetScaleType() { return ST_RANDOM_SCALING ; }; 	
	virtual CDocInfo*			GetDocInfo() { return &m_docInfo; };

	void SetKfToSource(Ogre::ParamShapeData* pSource);
	void SetKfFromSource(Ogre::ParamShapeData* pSource);


	struct GEO_FRAME
	{
		float fShpereR;
		float fShpereAngle;

		float fTurosNR;
		float fTurosLR;

		float fTubeH;
		float fTubeLR;
		float fTubeNR;

		float fColumnH;
		float fColumnDR;
		float fColumnUR;

		float fUOffset;
		float fVOffset;
		float fRound;

		float fUScale;
		float fVScale;
		float fUVRotate;


		float fMaskUOffset;
		float fMaskVOffset;
		float fMaskUScale;
		float fMaskVScale;
		float fMaskUVRotate;
		Ogre::ColourValue color;
		float alpha;

		long lTick;
		GEO_FRAME()
		{
			fShpereR = 40.0f;
			fShpereAngle = 180.0f;

			fTurosNR = 15.0f;
			fTurosLR = 65.0f;

			fTubeH = 40.0f;
			fTubeLR = 30.0f;
			fTubeNR = 20.0f;

			fColumnH = 40.0f;
			fColumnDR = 20.0f;;
			fColumnUR = 20.0f;;

			fUOffset = 0.0f;
			fVOffset = 0.0f;
			fRound = 360.0f;

			fUScale = 1.0f;
			fVScale = 1.0f;
			fUVRotate = 0.0f;
			color = Ogre::ColourValue(1.0f,1.0f, 1.0f, 1.0f);
			alpha = 1.0f;

			fMaskUScale = 1.0f;
			fMaskVScale = 1.0f;
			fMaskUVRotate = 0.0f;

			fMaskUOffset = 0.0f;
			fMaskVOffset = 0.0f;
		};
	};

	CDocInfo					m_docInfo;
	BIND_INFO					m_bindInfo;
	CString						m_strName;
	std::vector<GEO_FRAME>		m_KeyFrames;
	CString						m_strTexture;
	CString						m_strMaskTexture;
	CString						m_strLastMaskTexture;
	bool						m_bShowGrid;

	int							m_iUVAddressMode;
	int							m_iBlendMode;
	Ogre::PARAMSHAPE_TYPE			m_GeomType;
	int							m_iTexCol;
	int							m_iTexRow;
	float						m_fFrameTime;
	bool						m_bDistort;
	int							m_nUSeg;
	int							m_nVSeg;

	int							m_iMaskUVAddressMode;
	int							m_iMaskTexCols;
	int							m_iMaskTexRows;

	//bool						m_bVisible;

	Ogre::ParametricShape*			m_pNode;
	Ogre::ParamShapeData*		m_pSource;
	CString						m_strLastTexture;

	CInteract					m_interact;
};


class CGeometryCommand : public CEffectCommand
{
public:
	virtual CEffectUnit*	GetTarget();
	virtual bool			NeedRefresh();
	virtual CEffectCommand* GetInversCommand();
	virtual bool			IsIdentical();
	virtual bool			Merge(CEffectCommand* pCmd, bool bForceMerge);

	CGeometry* m_pTarget;

	BIND_INFO m_oldBind;
	BIND_INFO m_newBind;

	int						m_oldBlendMode;
	Ogre::PARAMSHAPE_TYPE		m_oldGeomType;
	int						m_newBlendMode;
	Ogre::PARAMSHAPE_TYPE		m_newGeomType;

	int							m_oldTexCol;
	int							m_newTexCol;
	int							m_oldTexRow;
	int							m_newTexRow;
	float						m_oldFrameTime;
	float						m_newFrameTime;

	std::vector<CGeometry::GEO_FRAME> m_oldKf;
	std::vector<CGeometry::GEO_FRAME> m_newKf;

	CString m_oldTex;
	CString m_newTex;
};
