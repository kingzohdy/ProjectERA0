
#ifndef __GAMEMAPREGION_H__
#define __GAMEMAPREGION_H__

#include "OgreVector3.h"
#include "OgreWorldPos.h"
#include "OgreAmbientRegion.h"

namespace Ogre
{
	class GameMapRegion
	{
	public:
		GameMapRegion();
		~GameMapRegion();

		bool isInRegion(WPOS_T x, WPOS_T z);

	public:
		char m_Name[32];
		WorldPos m_MinPos;
		WorldPos m_MaxPos;
		AmbientInfo m_EnvInfo;
		MovableObject *m_pWeatherObj;
		MovableObject *m_pSkyObj;


		std::vector<Vector3>m_BoundPoints;
	};

	class GameMapRegionSet
	{
	public:
		GameMapRegionSet();
		~GameMapRegionSet();

		bool loadFromFile(const char *path);
		GameMapRegion *findRegion(WPOS_T x, WPOS_T z);
		void setOverlayRegion(const char *pname);
		void PreloadMusic();

	private:
		std::vector<GameMapRegion *>m_Regions;
		GameMapRegion *m_pOverlayRegion;
	};
}

#endif