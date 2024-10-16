
#pragma once
#include "GameEngineTypes.h"
#include "OgreSingleton.h"

//tolua_begin
struct tagEntityWinPos 
{
	int m_nPosX;
	int m_nPosY;
};

enum
{
	PULL_IN,	// 拉近
	PULL_OUT,	// 拉远
	PULL_ROLLBACK,	// 非选择角色镜头
	PULL_NONE,	// 镜头不动
};

enum
{
	PULL_SPEED_MODE_IMMEDIATE,
	PULL_SPEED_MODE_SLOW,
};

enum
{
	VIEW_POINT_LOGIN, //登录界面
	VIEW_POINT_SELECT, //角色选择
	VIEW_POINT_CREATE, //角色创建
};

//tolua_end

struct tagPullScene
{
	tagPullScene()
	{
		m_fXCurScale	= 1.0F;
		m_fYCurScale	= 1.0F;
		m_fZCurScale	= 1.0F;
		m_nPullType		= PULL_NONE;
	}

	float			m_fXCurScale;
	float			m_fYCurScale;
	float			m_fZCurScale;

	float			m_fXDestScale;
	float			m_fYDestScale;
	float			m_fZDestScale;

	float			m_fXChgScale;
	float			m_fYChgScale;
	float			m_fZChgScale;

	int				m_nPullType;

	unsigned int	m_uLastPullTime;
	unsigned int	m_uMaxPassTime;
	unsigned int	m_nPullState;
};

struct tagDefaultAdjustData
{
	tagDefaultAdjustData()
	{
		m_fX = m_fY = m_fZ = 0;
	}
	
	float		m_fX;
	float		m_fY;
	float		m_fZ;
};

struct tagConfigKey
{
	int	m_nGender;
	int	m_nCareer;
};

class GameBackground//tolua_export
	: public Ogre::Singleton<GameBackground>
{//tolua_export
public:
	GameBackground(int mapid);
	~GameBackground();
	void loadPullSceneParam();
	//tolua_begin

	void setAvatarAttrib(int index, int grid, int id, int starLv = 0, bool bfirst = false);
	int getAvatarAttrib(int index,int grid);
	void setEntityAvatar(int index );
	void setPetModel( int index, int nAnimId, const char* szModelPath, const char *szAppendPath = NULL, int nResId = 0 );
	void setEntityScale( int index, float x, float y, float z );
	void setEntityPosition(int index, float x, float y, float z);
	void setEntityRotate(int index, float angle);
	void playEntityAnim(int index, int seq, const char *motion);
	void setCameraAnim(const char *path);
	void playCameraAnim(int seq, int fadetick);//fadetick表示从现在动画到将要播的动画的插值时间
	void show();
	void hide();
	void fadeEntity(int index, float target_alpha, int fadetick);//从当前alpha向目标alpha插值
	void releaseEntity( int index );
	void preLoadRes(const char *path);
	void setModelBorder(int index);
	tagEntityWinPos* getEntityWinPos( int nIndex );
	void stopEntityAnim(int nIndex);
	
	void ChangeAnimModelConfig( int nGender, int nCareer );
	bool isPullingCamera();
	void beginPull( int nPullType, int nSpeedMode = PULL_SPEED_MODE_SLOW );
	void updateAttrib( int nIndex );
	void setViewFlag( int nFlag );
	void playBGM( int nTime );
	bool isShown() { return m_bShow; }
	//tolua_end
	
	void updatePullScale();
	void update(unsigned int dtick, int sx, int sy, int width, int height);
	void roudLimit( float& fCurValue, float& fChgValue, float& fDestValue );

protected:
	int m_MapID;
	Ogre::Camera *m_pCamera;
	Ogre::GameScene *m_pScene;
	Ogre::Model *m_pCamAnimMod;
	Ogre::ShowGameMap *m_pGameMap;

	Ogre::AvatarEntity *m_pEntities[16];
	Ogre::Entity *m_pAppends[16];
	Ogre::ShowActor *m_pShowActor;
	int	m_AvatarAttrib[16][32];
	float m_EntityTargetAlpha[16];
	float m_EntityStepAlpha[16];
	char m_ModePath[32];
	bool m_bShow;

	void updateCameraAnim(unsigned int dtick);
	void loadres();
	void unloadres();
	int m_AvatarStar[16][32];

private:
	tagEntityWinPos m_winPos;
	static const float		m_fFarScale;

	unsigned int			m_uMaxPullTime;
	tagPullScene			m_pullInfo;
	tagConfigKey			m_animCameraConfigKey;
	int						m_nViewFlag;
};//tolua_export
