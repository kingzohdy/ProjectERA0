
#pragma once

#include "TerrainTile.h"

#include <vector>

namespace TE
{
	//�ƹ���Ϣ
	struct LightInfoHead
	{
		int    nVersion;
		int    nLightNum;
	};

	//�ƹ���Ϣ
	struct LightInfo_100//version 100
	{
		Ogre::Vector3  pos;
		float  fRange;
		int    nType;
		Ogre::ColourValue  color;
	};

	struct LightInfo
	{
		int nType;//0--���Դ�� 1--��ЧԴ
		Ogre::Vector3 pos;
		float  fRange;
		Ogre::ColourValue color;
		float volume;
		std::string name;
		std::string respath;

		// 102
		float time0;
		float time1;

		// 103
		float fSoundFullRange;

		// 104
		int iIsLoop;
	};

	//�ƹ��Ӧ�ڵ���Ϣ
	struct  LightNodeInfo	
	{
		Ogre::MovableObject *peffectnode;
		TerrModel*     pModel; 
	};

	class TerrainEditMgr;

	//�ƹ����
	class MapLightsManager
	{
	public:
		MapLightsManager(void);
		~MapLightsManager(void);

		void update(float dtime);

		bool   LoadLightInfo( const char* filename , TerrainEditMgr* pEditor );
		bool   SaveLightInfo( const char* filename );

		size_t getNumLight(){ return m_LightSet.size();}
		LightInfo &getLightInfo(int i);
		LightNodeInfo &getLightNodeInfo(int i);
		void updateNodeInfo(int i);
		int getIndexByModel(TerrModel *pmodel);
		void deleteLight(int i);
		void onModelMove(TerrModel *pmodel);
		TerrModel *intersectRay(Ogre::Ray &ray);
		void onCull();

		LightNodeInfo   AddLight(LightInfo  &lightInfo);

		void   AttachLightToRoom();
		void   DetachLightToRoom();
		void   Release();

		void flushAllLights(bool bake_dirlight);

		void AddLightsToBeast ();

		void   AttachLightModelToRoom();
		void   DetachLightModelToRoom();

		void setEditMode(int mode){m_CurEditMode = mode;}
		void   SetLockHeight( bool bLock ) { m_bLockHeight = bLock ;}
	private:
		void   CountChunkLight( Ogre::Vector3 pos , Ogre::ColourValue color , float fRange );

	public:
		//�ƹ�ڵ���Ϣ
		std::vector<LightNodeInfo>                 m_LightSet;
		//�ƹ���Ϣ
		std::vector<LightInfo>                     m_LightInfoSet;
		//���浱ǰ�ƹ���Ϣ
		std::vector <LightInfo>                    m_SaveLightInfo;
		//��ǰ�����ĵƹ�����
		int                                        m_nCopyLightNum;

		enum
		{
			EDIT_ADDLIGHT = 0,
			EDIT_ADDSOUND,
			EDIT_PROPERTY
		};
		int m_CurEditMode;
		//�༭ʱ�Ƿ���ס�߶�
		bool                                       m_bLockHeight ;
	};

	extern MapLightsManager   g_LightManager;
}


