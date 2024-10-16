#ifndef _OTHER_PLAYER_TITLE_DRAWER_H_
#define _OTHER_PLAYER_TITLE_DRAWER_H_

#include "GameActor_TitleBaseDrawer.h"

enum
{
	OTHER_PLAYER_TITLE_NAME = 0,
	OTHER_PLAYER_TITLE_STALL_NAME = OTHER_PLAYER_TITLE_NAME,
	OTHER_PLAYER_TITLE_CLAN_NAME = 1,
	OTHER_PLAYER_TITLE_TEAM_OR_CHENG_HAO_NAME = 2,
	OTHER_PLAYER_TITLE_LOVE_SIGN = 3,
	OTHER_PLAYER_TITLE_END
};

enum
{
	OTHER_PLAYER_TITLE_ICON_HP_BACK	= 0,
	OTHER_PLAYER_TITLE_ICON_HP,
	OTHER_PLAYER_TITLE_ICON_BATTLE,
	OTHER_PLAYER_TITLE_ICON_TEAM,
	OTHER_PLAYER_TITLE_ICON_CHENG_HAO,
	OTHER_PLAYER_TITLE_ICON_CLAN_SPY,
	OTHER_PLAYER_TITLE_ICON_CLAN_GLORY,
	OTHER_PLAYER_TITLE_ICON_END
};

class OtherplayerTitleDrawer : public IActorTitleDrawer
{
public:
	OtherplayerTitleDrawer( GameActor* pOwner );
	virtual ~OtherplayerTitleDrawer();
	virtual void Update( float dtick );
	virtual void UpdateTextColor();
	virtual void UpdateTitleHp();

	virtual float GetNameDrawYOffset();

protected:
	virtual void ValidateVkMenuState();
	virtual void ClearAllTitle();
	virtual void ClearAllIconTitle();

private:
	virtual bool IsShowName();
	virtual void BuildNameTtile();
	virtual float GetNameOffsetY();
	virtual const char* GetName();
	virtual Ogre::ColorQuad GetNameColor();

	virtual bool IsShowClanName();
	virtual void BuildClanNameTtile();
	virtual float GetClanNameOffsetY();
	virtual float GetClanSPYOffsetY();
	virtual const char* GetClanName();
	virtual Ogre::ColorQuad GetClanNameColor();

	virtual bool IsShowTemaOrChengHaoName();
	virtual void BuildTemaOrChengHaoNameTtile();
	virtual float GetTemaOrChengHaoNameOffsetY();
	virtual const char* GetTemaOrChengHaoName();
	virtual Ogre::ColorQuad GetTemaOrChengHaoNameColor();

	virtual bool IsShowStallName();
	virtual void BuildStallNameTitle();
	virtual float GetStallNameOffsetY();
	virtual const char* GetStallName();
	virtual Ogre::ColorQuad GetStallNameColor();

	virtual bool IsShowLoveSign();
	virtual void BuildLoveSign();
	virtual float GetLoveSignOffsetY();
	virtual const char* GetLoveSign();
	virtual Ogre::ColorQuad GetLoveSignColor();
	
	virtual bool IsShowHp();
	virtual void BuildHp();
	virtual float GetHpOffsetX();
	virtual float GetHpOffsetY();
	virtual float GetHpWidth();
	virtual float GetHpHeight();

	virtual float GetHpBackOffsetX();
	virtual float GetHpBackOffsetY();
	virtual float GetHpBackWidth();
	virtual float GetHpBackHeight();

	virtual bool IsShowChengHaoNameIcon(); 
	virtual void BuildChengHaoNameIcon();
	virtual float GetChengHaoNameIconOffsetX();
	virtual float GetChengHaoNameIconOffsetY();
	virtual float GetChengHaoNameIconWidth();
	virtual float GetChengHaoNameIconHeight();

	virtual bool IsShowNameIcon();
	virtual void BuildNameIcon();

	virtual void DrawAllTopTextTitles( Ogre::Camera *pcamera );
	virtual void DrawAllIconTitle( Ogre::Camera *pCamera );
	
private:
	float GetBattleIconOffsetX();
	float GetBattleIconOffsetY();
	float GetBattleIconWidth();
	float GetBattleIconHeight();

	float GetTeamIconOffsetX();
	float GetTeamIconOffsetY();
	float GetTeamIconWidth();
	float GetTeamIconHeight();

	const char* GetSpecialActivityAddInfo( GameActor* pActor );

	bool IsForbidShowAllTitle();
	bool IsOwnerSelectedOrTouchByMouse();
	bool IsForceShowHPNameClanTeam();

	void AdjustInitHpHeight();
	void UpdateCrimeNameColor();
	void UpdateNameColor();
	void UpdateClanNameColor();
	Ogre::HUIRES GetTeamIcon();

	bool IsShowDaoBa();

private:
	tagDrawTextTitleInfo m_allTextTitles[OTHER_PLAYER_TITLE_END];
	tagDrawTitleIconInfo m_allDrawIcons[OTHER_PLAYER_TITLE_ICON_END];
};

#endif