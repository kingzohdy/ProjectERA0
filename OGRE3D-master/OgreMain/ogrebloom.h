
#ifndef __BLOOM_H__
#define __BLOOM_H__

#include "OgreSceneRenderer.h"
#include "OgreVertexFormat.h"
#include "OgreSingleton.h"

namespace Ogre
{
	class RT_TEXTURE;

	class _OgreExport BloomEffect : public SceneRenderer, public Singleton<BloomEffect>
	{
	public:
		RT_TEXTURE				*m_pSceneTexture;
		TextureRenderTarget		*m_pSceneRT;

		RT_TEXTURE				*m_pSceneTextureBlend;
		TextureRenderTarget		*m_pSceneRTBlend;
		RT_TEXTURE				*m_pSceneTextureSavedArray[2];
		TextureRenderTarget		*m_pSceneRTSavedArray[2];
		int m_iCurIndex;
		RenderTarget			*m_pOutputRenderTarget;

		RT_TEXTURE				*m_pBloomTexture[2];
		TextureRenderTarget		*m_pBloomRT[2];

		float				m_fHighScene;
		float				m_fGrayValue;
		float				m_fBlur;

		VertexFormat m_VertFmt;
		VertexDeclHandle m_VertDecl;
		Material *m_pMtl;

	public:
		BloomEffect();
		~BloomEffect();

		bool isValid()
		{
			return m_pBloomTexture[0]!=NULL && m_pBloomRT[0]!=NULL;
		}

		void enableRadialBloom (bool enable)
		{
			m_bRadialBloom = enable;
		}

		bool isRadialBloomEnable ()
		{
			return m_bRadialBloom;
		}

		void update (double tick);

		// BLUR_TYPE 
		// 0 正常
		// 1 运动模糊
		// 2 高斯模糊
		// 3 径向模糊
		// 4 漩涡

		// 高斯模糊
		void setGaussBlur (bool blur);
		
		// 切换地图时播放一次径向模糊(毫秒)
		void playRadialBloom (double time, bool forwrd);
		void stopRadial ();

		void SetSampleDist (float sampleDist) { m_fSampleDist = sampleDist; }
		float GetSampleDist () { return m_fSampleDist; }
		void SetSampleStrength (float sampleStrength) { m_fSampleStrength = sampleStrength; }
		float GetSampleStrength () { return m_fSampleStrength; }

		// 播放运动模糊
		void playBlend ();
		void playBlend (double time);
		void stopBlend ();

		// 漩涡
		void setVortexMaxDegree (float vortexMaxDegree=1.0f);
		void setRadiusRateFromTo (float from=0.2f, float to=1.0f);
		void playVortex (double time, bool forward = true);

		virtual void onLostDevice();
		virtual void onRestoreDevice();
		virtual void doRender();

		void SetBlur(float fBlur);
		void SetGrayValue(float fGrayValue);
		void SetHighScene(float fHighScene);

	private:
		void getSampleOffsetWeight(uint texsize, float *sampleoffset, float *sampleweight, float fDeviation, float fMultiplier);
		float gaussianDistribution( float x, float y, float rho );
		void doRenderQue(RenderTarget *ptarget, Material *pmtl);
		bool ValidateCreateResult();

	protected:
		bool m_bGaussBlur;
		float m_fGaussRate;
		double m_dGaussPlayTime;
		double m_dCurrentGaussPlayTime;
		bool m_bGaussForward;

		bool m_bRadialBloom;

		bool m_bForword;
		double m_dRadialPlayTime;
		double m_dCurrentRadialPlayTime;
		
		float m_fMaxSampleDist;
		float m_fSampleDist;

		float m_fMaxSampleStrength;
		float m_fSampleStrength;

		bool m_bBlend;
		bool m_bBlendOnce;
		double m_dBlendPlayTime;
		double m_dBlendTimeUpdate;
		double m_SaveBlendTime;

		// Vortex
		bool m_bUseVortex;
		bool m_bVortexForward;
		double m_dVortexMaxDegree;
		float m_fVortexFrom;
		float m_fVortexTo;
		double m_dVortexPlayTime;
		double m_dCurrentVortexPlayTime;
		float m_textureX;
		float m_textureY;
		float m_timeRate;
		float m_radiusRate;
		float m_degree;
	};
}

#endif