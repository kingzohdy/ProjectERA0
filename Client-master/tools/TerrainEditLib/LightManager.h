
#pragma once

#include "marchive.h"

#include "gtitle.h"

#include <vector>

namespace TE
{

#define   LIGHT_UP_MODEL_HEIGHT  200
#define   LIGHT_MODEL_PATH       "toolres\\mapeditor\\light.omod"

//�ƹ���Ϣ
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

//�ƹ���Ϣ
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
	
//�ƹ��Ӧ�ڵ���Ϣ
struct  LightNodeInfo	
{
	Ogre::Light*  pLightNode ;	
	TerrModel*     pModel; 
};

class MTerrainEdit;

//�ƹ����
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

	//ɾ����ǰѡ��ƹ�
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
	
	//���ñ༭ʱ�Ƿ����ӵƹ�
	void   SetNoAddLight( bool sign ) { m_bNoAddLight = sign ;}
	//�����Ƿ���ס�߶��ƶ�
	void   SetLockHeight( bool bLock ) { m_bLockHeight = bLock ;}
private:
	void   CountChunkLight( MVec3 pos , MVec4 color , float fRange );

public:
	//��ǰѡ������ID
	int                                       m_nCurSelectIdx;
	//�ƹ�ڵ���Ϣ
	std::vector<LightNodeInfo>                 m_LightSet;
	//�ƹ���Ϣ
	std::vector<LightInfo>                     m_LightInfoSet;
	//���浱ǰ�ƹ���Ϣ
	std::vector <LightInfo>                    m_SaveLightInfo;
	//��ǰ�����ĵƹ�����
	int                                        m_nCopyLightNum;
	//�༭���Ƿ����ӵƹ�
	bool                                       m_bNoAddLight ;
	//�༭ʱ�Ƿ���ס�߶�
	bool                                       m_bLockHeight ;
};


};

extern TE::CLightManager   g_LightManager;
