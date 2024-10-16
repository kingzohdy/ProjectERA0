
#ifndef __BeamEmitterData_H__
#define __BeamEmitterData_H__

#include "OgreResource.h"
#include "OgreVector3.h"
#include "OgreVector2.h"
#include "OgreColourValue.h"
#include "OgreKeyFrameArray.h"

namespace Ogre
{
	enum BEAM_ENDPOS_T
	{
		BET_LOCAL_POS,
		BET_WORLD_POS,
		BET_REF_POS 
	};

	class PBeamEmitterDesc_100
	{
	public:
		int m_Version;
		int m_BlendType;

		Vector3 m_EndPos;
		int m_MaxParticles;

		int m_HighFrequencyPoints;	//注意:高频噪波点和低频噪波点大于等于2个
		Vector2 m_HighFrequencyNoiseRange;
		int m_LowFrequencyPoints;
		Vector2 m_LowFrequencyNoiseRange;

		float m_MaxTime;
		Vector2 m_LifeTimeRange;

		float m_MidPoint;  //midpoint in lifespan
		int m_TexRows;
		int m_TexCols;

		ColourValue m_ColorKeys[3];
		float m_SizeKeys[3];
		float m_AspectRatioKeys[3];

		//m_Version >= 100
		BEAM_ENDPOS_T m_EndPosType;

		//m_Version >= 101
		float	m_fUSpeed;
		float	m_fVSpeed;
	};

	class PBeamEmitterDescEx_100
	{
	public:
		// ver >= 102
		int		m_iUVAddressMode;
		int		m_iUAddressMode;
		int		m_iVAddressMode;
	};

	enum BEAM_WAVE_TYPE
	{
		BWT_RANDOM,
		BWT_SIN,
		BWT_STRATE
	};

	class PBeamEmitterDesc
	{
	public:
		int m_BlendType;
		int m_MaxParticles;

		int m_HighFrequencyPoints;	//注意:高频噪波点和低频噪波点大于等于2个
		Vector2 m_HighFrequencyNoiseRange;
		int m_LowFrequencyPoints;
		BEAM_WAVE_TYPE m_WaveTypeY;
		float m_LFYParam1;
		float m_LFYParam2;
		float m_LFYParam3;

		BEAM_WAVE_TYPE m_WaveTypeZ;
		float m_LFZParam1;
		float m_LFZParam2;
		float m_LFZParam3;

		//m_Version >= 101


		bool	m_bEmitOnce;
		bool	m_bDistort;
		bool	m_bRandomTile;
		float	m_FrameTime;

		int		m_iUVAddressMode;
		float	m_UVRotate;
		float	m_UVScale;
		int		m_TexRows;
		int		m_TexCols;
		float	m_USpeed;
		float	m_VSpeed;
		float	m_UOgnOff;
		float	m_VOgnOff;

		int		m_iMaskUVAddressMode;
		float	m_MaskUVRotate;
		float	m_MaskUVScale;
		int		m_MaskTexRows;
		int		m_MaskTexCols;
		float	m_MaskUSpeed;
		float	m_MaskVSpeed;
		float	m_MaskUOgnOff;
		float	m_MaskVOgnOff;
	};

	struct BeamAnimData
	{
		Vector3	startrange;
		Vector3	endrange;
		Vector3	endpos;
		float	sizevar;
		float	life;
		float	lifevar;
		float	emitrate;
	};

	struct BEAM_DATA
	{
		std::vector<Vector3> pos;
		float		life;
		float		age;
		ColourValue color;
		float		width;
		float		alpha;
		float		sizevar;
		Vector3		targetpos;
		int			tile;
		int			tileoff;
		float		uspeedoff;
		float		vspeedoff;

		float		maskuspeedoff;
		float		maskvspeedoff;
	};

	class _OgreExport BeamEmitterData : public Resource
	{
		DECLARE_RTTI(BeamEmitterData)
	public:
		BeamEmitterData();

		void		EmitBeam(BEAM_DATA* pData);
		void		PrepareData(uint tick);
		void		UpdatePos (BEAM_DATA* pData); //< just for 闪电链

	protected:
		virtual ~BeamEmitterData();
		virtual void _serialize(Archive &ar, int version);

		void	GernerateLFPoints(std::vector<float>& points, int iCount, BEAM_WAVE_TYPE type, float param1, float param2, float param3);

	public:

		// begin specific for controlled FlashChain
		bool m_bControlChain;
		Vector3 m_ControledEndPos;
		// end specific for controlled FlashChain

		BeamAnimData	m_AnimData;

		PBeamEmitterDesc m_Desc;

		KeyFrameArray<float> m_WidthKey;
		KeyFrameArray<ColourValue> m_ColorKey;
		KeyFrameArray<float> m_AlphaKey;


		KeyFrameArray<Vector3> m_StartRange;
		KeyFrameArray<Vector3> m_EndRange;
		KeyFrameArray<Vector3> m_EndPos;
		KeyFrameArray<float>   m_SizeVar;
		KeyFrameArray<float>   m_Life;
		KeyFrameArray<float>   m_LifeVar;
		KeyFrameArray<float>   m_EmitRate;


		TextureData *m_pTexture;
		TextureData *m_pMaskTexture;
	};
}

#endif