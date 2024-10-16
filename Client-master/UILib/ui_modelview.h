
#pragma once
#include "ui_layoutframe.h"
#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include <map>

class Ogre::Texture;
class Ogre::TextureRenderTarget;

struct ModelViewTextureData 
{
	ModelViewTextureData();

	~ModelViewTextureData();

	Ogre::HUIRES				m_hRes;
	Ogre::Texture*				m_pModelTexture;
	Ogre::TextureRenderTarget*	m_pRenderTarget;
};

class ModelView : public LayoutFrame{//tolua_export
	DECLARE_UIOBJECT_CLONE(ModelView)

public:
	ModelView();
	virtual ~ModelView();

	//tolua_begin
	void setBackground(const char *path);
	void addBackground(const char *path);
	void setAvatarAttrib(int index, int grid, int id, int starLv = 0, bool bfirst = false );
	int getAvatarAttrib(int index,int grid);

	void setEntityAvatar(int index );
	bool isPlayingAnim( int nIndex );
	// 此为设置函数之一
	void setPetModel( int index, int nAnimId, const char* szModelPath, const char *szAppendPath = NULL );
	void setPetScale( int index, float x, float y, float z );
	void setEntityPosition(int index, float x, float y, float z);
	void setEntityRotate(int index, float angle);
	void setEntieyTextureByID( int index, int id, const char* szPath );
	void playEntityAnim(int index, int seq, const char *motion);

	bool isHaveCameraAnim( );
	void setCameraAnim(const char *path);
	void playCameraAnim(int seq, int fadetick);//fadetick表示从现在动画到将要播的动画的插值时间
	//tolua_end

	virtual void UpdateSelf(float deltatime);

	void NormalDraw();
	void DrawOutOfScreen();

	virtual void Draw();

	virtual const char *GetTypeName()
	{ 
		return "ModelView"; 
	}

protected:
	std::string					m_strTextureResPath;
	Ogre::Camera*				m_pCamera;
	Ogre::SimpleGameScene*		m_pScene;
	Ogre::Model*				m_pCamAnimMod;

	std::vector<Ogre::Entity*>		m_BGObjs;
	Ogre::AvatarEntity*				m_pEntities[16];
	Ogre::Entity *					m_pAppends[16];
	Ogre::ShowActor*				m_pShowActor;
	int								m_AvatarAttrib[16][32];	
	int								m_AvatarStar[16][32];
	char							m_ModePath[32];
	ModelViewTextureData			m_ModelTexData;

	void updateCameraAnim(unsigned int dtick);
	// 创建纹理
	void CreateModelTexture();
};//tolua_export
