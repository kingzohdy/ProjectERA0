
#pragma once

#include "marchive.h"

#include "gtitle.h"

#include <vector>

namespace TE
{

#define   LIGHT_UP_MODEL_HEIGHT  200
#define   LIGHT_MODEL_PATH       "toolres\\mapeditor\\light.omod"

//灯光信息
struct LightInfoHead
{
	int    nVersion;
	int    nLightNum;

	friend Ogre::MArchive& operator<<( Ogre::MArchive& Ar, LightInfoHead& info )
	{
		Ar << info.nVersion << info.nLightNum ;
		return Ar;
	}
};

//灯光信息
struct LightInfo
{
	MVec3  pos;
	float  fRange;
	int    nType;
	MVec4  color;

	friend Ogre::MArchive& operator<<( Ogre::MArchive& Ar, LightInfo& info )
	{
		Ar.Serialize( &info ,sizeof( LightInfo ) );
		return Ar;
	}
};
	
//灯光对应节点信息
struct  LightNodeInfo	
{
	Ogre::Light*  pLightNode ;	
	TerrModel*     pModel; 
};

class MTerrainEdit;

//灯光管理
class CLightManager
{
public:
	CLightManager(void);
	~CLightManager(void);

	void update(float dtime);

	bool   LoadLightInfo( const char* filename , MTerrainEdit* pEditor );
    bool   SaveLightInfo( const char* filename );

	enum ADD_LIGHT_MODE
	{
		ADD_LIGHT = 0 ,
		FILE_ADD_LIGHT , 
	};

	LightNodeInfo   AddLight( LightInfo  &lightInfo , ADD_LIGHT_MODE nMode = ADD_LIGHT );

	void   AttachLightToRoom();
	void   DetachLightToRoom();

	void   Serialize( Ogre::MArchive& Ar );

	void   Release();

	void   UpdateLight( int index );

	void flushAllLights(bool bake_dirlight);

	//删除当前选择灯光
	void   DelCurSelectLight( MTerrainEdit *pEditor );

	bool   GetCurSelectLightInfo( LightInfo&  lightInfo );

	bool   SetEnableShadow( int bEnable);

	bool   SetCurSelectLightPos( MVec3 pos );
	bool   SetCurSelectLightRange( float fRange);
	bool   SetCurSelectLightColor( MVec4 color );

	void   AttachLightModelToRoom();
	void   DetachLightModelToRoom();

	void   CopyCurLight();
    void   CopySelectLightToTile();
	
	//设置编辑时是否增加灯光
	void   SetNoAddLight( bool sign ) { m_bNoAddLight = sign ;}
	//设置是否锁住高度移动
	void   SetLockHeight( bool bLock ) { m_bLockHeight = bLock ;}
private:
	void   CountChunkLight( MVec3 pos , MVec4 color , float fRange );

public:
	//当前选择索引ID
	int                                       m_nCurSelectIdx;
	//灯光节点信息
	std::vector<LightNodeInfo>                 m_LightSet;
	//灯光信息
	std::vector<LightInfo>                     m_LightInfoSet;
	//保存当前灯光信息
	std::vector <LightInfo>                    m_SaveLightInfo;
	//当前拷贝的灯光数量
	int                                        m_nCopyLightNum;
	//编辑是是否增加灯光
	bool                                       m_bNoAddLight ;
	//编辑时是否锁住高度
	bool                                       m_bLockHeight ;
};


};

extern TE::CLightManager   g_LightManager;
