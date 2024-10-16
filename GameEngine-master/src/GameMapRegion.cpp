
#include "GameMapRegion.h"
#include "OgreResourceManager.h"
#include "OgreLog.h"
#include "OgreRect.h"
#include "OgreMovableObject.h"
#include "OgreSoundSystem.h"

namespace Ogre
{
	GameMapRegion::GameMapRegion() : m_MinPos(0,0,0), m_MaxPos(0,0,0), m_pWeatherObj(NULL), m_pSkyObj(NULL)
	{
	}

	GameMapRegion::~GameMapRegion()
	{
		OGRE_RELEASE(m_pWeatherObj);
		OGRE_RELEASE(m_pSkyObj);
	}

	bool GameMapRegion::isInRegion(WPOS_T x, WPOS_T z)
	{
		if(x>m_MinPos.x && x<m_MaxPos.x && z>m_MinPos.z && z<m_MaxPos.z)
		{
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------------
	GameMapRegionSet::GameMapRegionSet() : m_pOverlayRegion(NULL)
	{
	}

	GameMapRegionSet::~GameMapRegionSet()
	{
		DeletePointerArray(m_Regions);
	}

	struct AREA_HEAD
	{
		char magic_number[4];
		unsigned int  version;
		unsigned int  area_num;
	};

	struct AreaProcInfo_107
	{
		int link_mapid;
		int link_mapdir;
		Point2D link_mappos;
		int fail_mapdir;
		Point2D fail_mappos;
	};

	struct AreaProcInfo
	{
		int link_mapid;
		int link_mapdir;
		int link_mapcamdir;
		Ogre::Point2D link_mappos;
		int fail_mapdir;
		Ogre::Point2D fail_mappos;
		char script[256];
	};

	struct AREA_INFO
	{
		unsigned int nID;
		int type;
		char         name[32];
		char         scriptName[64];
		unsigned int nPosNum;

		Vector3 minPos,maxPos;
		AmbientInfo   ambientInfo;
		AreaProcInfo procinfo;

		Vector3 pos[128];
	};

	struct AmbientInfo_105
	{
		bool distfog_enable;
		bool heightfog_enable;
		bool specular_enable;

		ColourValue ambient_color;
		ColourValue dirlight_color;
		Vector2 dirlight_angle;  //dir.x=Cos(y)Cos(x), dir.y=-Sin(y), dir.z=Cos(y)Sin(x)
		bool  shaow;
		float shadowdensity;
		ColourValue specular_color;
		float specular_power;

		uint toplight_shadowbits;
		ColourValue toplight_color;

		ColourValue distfog_color;
		float distfog_near;
		float distfog_far;

		ColourValue heightfog_color;
		float heightfog_near;
		float heightfog_far;

		float bloom_gray;
		float bloom_high;
		float bloom_blur;

		char skyfile[128]; //天空文件
		char musicfile[128];//音乐文件
		char effectfile[128];//特效文件
	};

	struct AmbientInfo_106
	{
		bool distfog_enable;
		bool heightfog_enable;
		bool specular_enable;

		ColourValue ambient_color;
		ColourValue dirlight_color;
		Vector2 dirlight_angle;  //dir.x=Cos(y)Cos(x), dir.y=-Sin(y), dir.z=Cos(y)Sin(x)
		bool  shaow;
		float shadowdensity;
		ColourValue specular_color;
		float specular_power;

		uint toplight_shadowbits;
		ColourValue toplight_color;

		ColourValue distfog_color;
		float distfog_near;
		float distfog_far;

		ColourValue heightfog_color;
		float heightfog_near;
		float heightfog_far;

		float bloom_gray;
		float bloom_high;
		float bloom_blur;

		float musicvol; //音乐大小

		char skyfile[128]; //天空文件
		char musicfile[128];//音乐文件
		char effectfile[128];//特效文件
	};

	bool GameMapRegionSet::loadFromFile(const char *path)
	{
		DataStream *fp = ResourceManager::getSingleton().openFileStream(path, true);	
		if(fp == NULL)
		{
			LOG_SEVERE("load region file error: %s", path);
			return false;
		}

		AREA_HEAD header;
		AREA_INFO info;
		fp->read(&header, sizeof(header));

		for(unsigned int i=0; i<header.area_num; i++)
		{
			fp->read(info.nID);
			fp->read(info.type);
			fp->read(info.name);
			fp->read(info.scriptName);
			fp->read(info.nPosNum);
			fp->read(info.minPos);
			fp->read(info.maxPos);

			if(info.type == 0)
			{
				if(header.version == 105)
				{
					AmbientInfo_105 ambient;
					fp->read(ambient);

					info.ambientInfo.distfog_enable = ambient.distfog_enable;
					info.ambientInfo.heightfog_enable = ambient.heightfog_enable;
					info.ambientInfo.specular_enable = ambient.specular_enable;
					info.ambientInfo.ambient_color = ambient.ambient_color;
					info.ambientInfo.dirlight_color = ambient.dirlight_color;
					info.ambientInfo.dirlight_angle = ambient.dirlight_angle;
					info.ambientInfo.shaow = ambient.shaow;
					info.ambientInfo.shadowdensity = ambient.shadowdensity;
					info.ambientInfo.specular_color = ambient.specular_color;
					info.ambientInfo.specular_power = ambient.specular_power;

					info.ambientInfo.toplight_shadowbits = ambient.toplight_shadowbits;
					info.ambientInfo.toplight_color = ambient.toplight_color;

					info.ambientInfo.distfog_color = ambient.distfog_color;
					info.ambientInfo.distfog_near = ambient.distfog_near;
					info.ambientInfo.distfog_far = ambient.distfog_far;
					info.ambientInfo.heightfog_color = ambient.heightfog_color;
					info.ambientInfo.heightfog_near = ambient.heightfog_near;
					info.ambientInfo.heightfog_far = ambient.heightfog_far;

					info.ambientInfo.bloom_gray = ambient.bloom_gray;
					info.ambientInfo.bloom_high = ambient.bloom_high;
					info.ambientInfo.bloom_blur = ambient.bloom_blur;

					info.ambientInfo.musicvol = 1.0f;
					info.ambientInfo.bgsoundvol = 1.0f;

					strncpy(info.ambientInfo.skyfile, ambient.skyfile,128);
					strncpy(info.ambientInfo.musicfile, ambient.musicfile,128);
					strncpy(info.ambientInfo.bgsoundfile, "",128);
					strncpy(info.ambientInfo.effectfile, ambient.effectfile,128);
				}
				else if(header.version == 106)
				{
					AmbientInfo_106 ambient;
					fp->read(ambient);

					info.ambientInfo.distfog_enable = ambient.distfog_enable;
					info.ambientInfo.heightfog_enable = ambient.heightfog_enable;
					info.ambientInfo.specular_enable = ambient.specular_enable;
					info.ambientInfo.ambient_color = ambient.ambient_color;
					info.ambientInfo.dirlight_color = ambient.dirlight_color;
					info.ambientInfo.dirlight_angle = ambient.dirlight_angle;
					info.ambientInfo.shaow = ambient.shaow;
					info.ambientInfo.shadowdensity = ambient.shadowdensity;
					info.ambientInfo.specular_color = ambient.specular_color;
					info.ambientInfo.specular_power = ambient.specular_power;

					info.ambientInfo.toplight_shadowbits = ambient.toplight_shadowbits;
					info.ambientInfo.toplight_color = ambient.toplight_color;

					info.ambientInfo.distfog_color = ambient.distfog_color;
					info.ambientInfo.distfog_near = ambient.distfog_near;
					info.ambientInfo.distfog_far = ambient.distfog_far;
					info.ambientInfo.heightfog_color = ambient.heightfog_color;
					info.ambientInfo.heightfog_near = ambient.heightfog_near;
					info.ambientInfo.heightfog_far = ambient.heightfog_far;

					info.ambientInfo.bloom_gray = ambient.bloom_gray;
					info.ambientInfo.bloom_high = ambient.bloom_high;
					info.ambientInfo.bloom_blur = ambient.bloom_blur;

					info.ambientInfo.musicvol = ambient.musicvol;
					info.ambientInfo.bgsoundvol = 1.0f;

					strncpy(info.ambientInfo.skyfile, ambient.skyfile,128);
					strncpy(info.ambientInfo.musicfile, ambient.musicfile,128);
					strncpy(info.ambientInfo.bgsoundfile, "",128);
					strncpy(info.ambientInfo.effectfile, ambient.effectfile,128);
				}
				else
				{
					assert(header.version > 106);
					fp->read(info.ambientInfo);
				}
			}
			else if(info.type == 1)
			{
				if(header.version < 108)
				{
					AreaProcInfo_107 procinfo;
					fp->read(procinfo);

					info.procinfo.link_mapid = procinfo.link_mapid;
					info.procinfo.link_mapdir = procinfo.link_mapdir;
					info.procinfo.link_mapcamdir = 0;
					info.procinfo.link_mappos = procinfo.link_mappos;
					info.procinfo.fail_mapdir = procinfo.fail_mapdir;
					info.procinfo.fail_mappos = procinfo.fail_mappos;
					info.procinfo.script[0] = 0;
				}
				else fp->read(info.procinfo);
			}

			assert(info.nPosNum < 128);
			fp->read(&info.pos[0], info.nPosNum*sizeof(Vector3));

			if(info.type == 0)
			{
				GameMapRegion *pregion = new GameMapRegion;
				strncpy(pregion->m_Name, info.name,32);
				pregion->m_MinPos = info.minPos;
				pregion->m_MaxPos = info.maxPos;
				pregion->m_EnvInfo = info.ambientInfo;

				pregion->m_BoundPoints.resize(info.nPosNum);
				if(info.nPosNum > 0) memcpy(&pregion->m_BoundPoints[0], &info.pos[0], info.nPosNum*sizeof(Vector3));

				if(pregion->m_EnvInfo.effectfile[0]) pregion->m_pWeatherObj = createObjectFromResource(pregion->m_EnvInfo.effectfile);
				if(pregion->m_EnvInfo.skyfile[0]) pregion->m_pSkyObj = createObjectFromResource(pregion->m_EnvInfo.skyfile);

				m_Regions.push_back(pregion);
			}
		}

		delete fp;
		m_pOverlayRegion = NULL;
		return true;
	}

	GameMapRegion *GameMapRegionSet::findRegion(WPOS_T x, WPOS_T z)
	{
		if(m_pOverlayRegion) return m_pOverlayRegion;

		assert(m_Regions.size() > 0);
		for(size_t i=m_Regions.size(); i>1; i--)
		{
			GameMapRegion *pregion = m_Regions[i-1];
			if(pregion->isInRegion(x, z))
			{
				return pregion;
			}
		}

		return m_Regions[0];
	}

	void GameMapRegionSet::setOverlayRegion(const char *pname)
	{
		if(pname == NULL)
		{
			m_pOverlayRegion = NULL;
			return;
		}

		for(size_t i=0; i<m_Regions.size(); i++)
		{
			GameMapRegion *pregion = m_Regions[i];
			if(strcmp(pregion->m_Name, pname) == 0)
			{
				m_pOverlayRegion = pregion;
				return;
			}
		}
	}

	void GameMapRegionSet::PreloadMusic()
	{
		for (size_t i = 0, j = m_Regions.size(); i < j; ++i)
		{
			GameMapRegion *pregion = m_Regions[i];
			SoundSystem::getSingleton().LoadMusicRes(pregion->m_EnvInfo.musicfile);
			SoundSystem::getSingleton().LoadMusicRes(pregion->m_EnvInfo.bgsoundfile);
		}
	}

}
