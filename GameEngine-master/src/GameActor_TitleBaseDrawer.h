#ifndef _GAME_ACTOR_TITLE_BASE_DRAWER_H_
#define _GAME_ACTOR_TITLE_BASE_DRAWER_H_
#include <vector>
#include "OgreMemoryDelegation.h"
#include "GameActor.h"
#include "OgreUIRenderTypes.h"
#include "OgreCamera.h"

struct tagDrawTextTitleInfo
{
	//Ogre::HUIRES	m_hUiRes;
	std::string		m_strTitle;
	Ogre::ColorQuad	m_color;
	float			m_fOffsetX;
	float			m_fOffsetY;
};


struct tagDrawTitleIconInfo
{
	tagDrawTitleIconInfo()
	{
		m_hUiRes	= NULL;
		m_fOffsetX	= 0;
		m_fOffsetY	= 0;
		m_fWidth	= 0;
		m_fHeight	= 0;
	}

	Ogre::HUIRES	m_hUiRes;
	float			m_fOffsetX;
	float			m_fOffsetY;
	float			m_fWidth;
	float			m_fHeight;
};

// TODO:调整位置，这个要去掉，改为策划填配置文件
struct tagAdjustPosInfo
{
	int		m_nTitleYIndex;
	float	m_fHeightInHpAndFirstText;
};

enum
{
	SCALE_CHG_LIMIT_VALUE = 1,
};

class IActorTitleDrawer : public Ogre::MemoryDelegation
{
public:
	IActorTitleDrawer( GameActor* pOwner );
	virtual ~IActorTitleDrawer() = 0;
	void BuildTitle();
	void DrawTitle( Ogre::Camera *pCamera );
	void SetForceDrawFlag( bool isForceDraw );
	virtual void Update( float dtick );
	virtual void UpdateTextColor();
	virtual void UpdateTitleHp() {}

	virtual float GetNameDrawYOffset() ;
protected:
	void DrawOneTopTextTitles( Ogre::Camera *pCamera, const tagDrawTextTitleInfo& oneDrawTextInfo,
								Ogre::uint shadowColor = 0xff000000 );
	void DrawOneTopTextTitles( Ogre::Camera *pCamera, const char *pszText, const Ogre::ColorQuad& color, 
								float fOffsetX, float fOffsetY, Ogre::uint shadowColor = 0xff000000 );

	void DrawOneIconTitle( Ogre::Camera *pCamera, const tagDrawTitleIconInfo& oneDrawIconInfo );
	void DrawOneIconTitle( Ogre::Camera *pCamera, Ogre::HUIRES hUiRes, float fScreenStartX, float fScreenStartY, 
							float fWidth, float fHeight );

	const Ogre::Vector3& GetTopTitlePos();
	bool isInStall();
	virtual void ValidateVkMenuState(){}
	virtual void ClearAllTitle() {}
	virtual void ClearAllIconTitle() {}

protected:
	virtual bool IsShowName() { return true; }
	virtual void BuildNameTtile() {}
	virtual float GetNameOffsetX();
	virtual float GetNameOffsetY();
	virtual const char* GetName();
	virtual Ogre::ColorQuad GetNameColor();
	
	virtual bool IsShowClanName() { return true; }
	virtual void BuildClanNameTtile() {}
	virtual float GetClanNameOffsetX();
	virtual float GetClanNameOffsetY();
	virtual const char* GetClanName();
	virtual Ogre::ColorQuad GetClanNameColor();
	
	virtual bool IsShowTemaOrChengHaoName() { return true; }
	virtual void BuildTemaOrChengHaoNameTtile() {}
	virtual float GetTemaOrChengHaoNameOffsetX();
	virtual float GetTemaOrChengHaoNameOffsetY();
	virtual const char* GetTemaOrChengHaoName();
	virtual Ogre::ColorQuad GetTemaOrChengHaoNameColor();
	
	virtual bool IsShowOwnerName() { return true; }
	virtual void BuildOwnerNameTitle() {}
	virtual float GetOwnerNameOffsetX();
	virtual float GetOwnerNameOffsetY();
	virtual const char* GetOwnerName();
	virtual Ogre::ColorQuad GetOwnerNameColor();

	virtual bool IsShowStallName() { return true; }
	virtual void BuildStallNameTitle() {}
	virtual float GetStallNameOffsetX();
	virtual float GetStallNameOffsetY();
	virtual const char* GetStallName();
	virtual Ogre::ColorQuad GetStallNameColor();

	virtual bool IsShowFunctionDesc() { return true; }
	virtual void BuildFunctionDescTitle() {}
	virtual float GetFunctionDescOffsetX();
	virtual float GetFunctionDescOffsetY();
	virtual const char* GetFunctionDesc();
	virtual Ogre::ColorQuad GetFunctionDescColor();

	virtual bool IsShowLoveSign() { return true; }
	virtual void BuildLoveSign() {}
	virtual float GetLoveSignOffsetX();
	virtual float GetLoveSignOffsetY();
	virtual const char* GetLoveSign();
	virtual Ogre::ColorQuad GetLoveSignColor();

	virtual bool IsShowHp() { return true; }
	virtual void BuildHp() {}
	virtual float GetHpOffsetX();
	virtual float GetHpOffsetY();
	virtual float GetHpWidth();
	virtual float GetHpHeight();

	virtual float GetHpBackOffsetX();
	virtual float GetHpBackOffsetY();
	virtual float GetHpBackWidth();
	virtual float GetHpBackHeight();

	virtual bool IsShowChengHaoNameIcon() { return true; }
	virtual void BuildChengHaoNameIcon(){}
	virtual float GetChengHaoNameIconOffsetX();
	virtual float GetChengHaoNameIconOffsetY();
	virtual float GetChengHaoNameIconWidth();
	virtual float GetChengHaoNameIconHeight();

	virtual bool IsShowNameIcon() { return true; }
	virtual void BuildNameIcon() {}
	virtual float GetNameIconOffsetX();
	virtual float GetNameIconOffsetY();
	virtual float GetNameIconWidth();
	virtual float GetNameIconHeight();

private:
	virtual void DrawAllTopTextTitles( Ogre::Camera *pCamera ){}
	virtual void DrawAllIconTitle( Ogre::Camera *pCamera );

protected:
	
	GameActor*							m_pOwner;
	std::string							m_Name;
	bool								m_isForceDrawTitle;
	static tagAdjustPosInfo				m_posInfo;
	static Ogre::Vector3				s_curScreenPos;

	bool								m_bIsLastMenuDown;
};

inline void IActorTitleDrawer::BuildTitle()
{
	int resid = m_pOwner->getResId();
	const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo( resid );
	if ( (info.MiscType & NPCDISPLAYINFO_MISC_TYPE_NO_NAME) != 0 )
	{
		return;
	}
	
	m_posInfo.m_fHeightInHpAndFirstText	= 0.0f;
	m_posInfo.m_nTitleYIndex			= 0;
	ClearAllTitle();
	ClearAllIconTitle();
	
//	BuildStallNameTitle();
	BuildHp();
	BuildNameIcon();
	BuildNameTtile();
	BuildClanNameTtile();
	BuildChengHaoNameIcon();
	BuildTemaOrChengHaoNameTtile();
	BuildOwnerNameTitle();
	BuildFunctionDescTitle();
	BuildLoveSign();
}

inline void IActorTitleDrawer::DrawTitle( Ogre::Camera *pCamera )
{
	const Ogre::Vector3& worldPos = GetTopTitlePos();
	pCamera->pointWorldToWindow( s_curScreenPos, worldPos );
	DrawAllTopTextTitles( pCamera );
	DrawAllIconTitle( pCamera );
}

inline void IActorTitleDrawer::SetForceDrawFlag( bool isForceDraw )
{
	m_isForceDrawTitle = isForceDraw;
	BuildTitle();
}

#endif