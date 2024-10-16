
#ifndef __OgreAmbientRegion_H__
#define __OgreAmbientRegion_H__

#include "OgreColourValue.h"
#include "OgreWorldPos.h"
#include "OgreVector2.h"
#include "OgreLoadWrap.h"

namespace Ogre
{
	struct AmbientInfo
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
		float bgsoundvol; //背景音效大小

		char skyfile[128]; //天空文件
		char musicfile[128];//音乐文件
		char bgsoundfile[128]; //背景音效文件（风声）
		char effectfile[128];//特效文件
	};

	class _OgreExport AmbientManager : public LoadWrap
	{
	public:
		AmbientManager(GameScene *pscene);
		~AmbientManager();

		void setCurAmbient(const AmbientInfo &info, bool immediate, bool hasOverlay = false);
		void update(uint dtick);
		void updatecamera(Camera* pCamerra, const WorldPos &center);
		void enableTopLight(bool b){m_TopLightEnable = b;}
		void setAlphaAddSpeed( float fSpeed ) { m_AlphaAddSpeed = fSpeed; }
	public:
		Vector3 m_vCamerapos;
	private:
		struct AmbientData
		{
			AmbientInfo info;
			MovableObject *pskymodel;
			MovableObject *peffect;
		};
		AmbientData m_AmbientSet[2];
		ResourceHandle m_skymodelRes;
		ResourceHandle m_effectRes;
		int m_NumSetUsed;

		GameScene *m_pScene;
		float m_BlendAlpha;
		float m_AlphaAddSpeed;

		Light *m_pLight;
		FogEffect *m_pDistFog;
		FogEffect *m_pHeightFog;
		Light *m_pTopLight;
		bool m_TopLightEnable;

		char m_CurMusic[2][128];

	private:
		void clearAmbientData(int i);
		void switchToMusic(const char *path, float volume, int channel);
		virtual void ResourceLoaded(Resource *rec, ResourceHandle h);
	};
}

#endif