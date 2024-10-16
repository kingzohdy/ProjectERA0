
#ifndef __OgreRibbonData_H__
#define __OgreRibbonData_H__

#include "OgreResource.h"
#include "OgreColourValue.h"
#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreKeyFrameArray.h"
#include "OgreMatrix4.h"

namespace Ogre
{
	enum RIB_SEG_TYPE
	{
		RGT_REAL,
		RGT_LERP,
		RGT_EMPTY
	};

	struct RIBBON_T
	{
		Vector3 pos;
		Vector3 up;

		float above;
		float below;
		ColourValue color;
		uint emittetime;
		float life;
		Vector3 ribtemplate[40];
		RIB_SEG_TYPE type;
	};

#define RIBBON_MAGIC 8620589

	class RibbonDesc
	{
	public:
		int m_MaxSegsOrMagic; //以前是MaxSegs，但是因为版本更新，为了保持兼容，将其改为一个MagicNumber，如果该值为8620589，则表示为新版本
		float m_LifeSpan;
		float m_EmitteRate;
		int m_BlendMode;

	};
	class RibbonDescEx
	{
	public:
		int			m_MaxSegsNew;
		int			m_LerpSeg;
		bool		m_bDistort;

		int			m_iUVAddressMode;

		int			m_iMaskUVAddrMode;

		int			m_iUAddressMode;
		int			m_iVAddressMode;

	};

	class _OgreExport RibbonSectionDesc
	{
	public:
		RibbonSectionDesc(){};
		~RibbonSectionDesc(){};
		std::vector<Vector2>	m_dots;
		std::vector<float>		m_vdata;
		std::vector<int>		m_lines; //-1表示中断，例如，1,2,3,-1,5,6表示1,2,3一条线，5,6一条线

		int	GetLineSegCount();

		RibbonSectionDesc& operator = (RibbonSectionDesc& src)
		{
			m_dots = src.m_dots;
			m_vdata = src.m_vdata;
			m_lines = src.m_lines;
			return *this;
		}
		
		friend Archive &operator<<(Archive &ar, RibbonSectionDesc &desc);
	};

	struct RibbonEmitterFrameData
	{
		ColourValue color;
		float above;
		float below;

		float uvrot;
		float uscale;
		float vscale;
		float uoffset;
		float voffset;

		float maskuvrot;
		float maskuscale;
		float maskvscale;
		float maskuoffset;
		float maskvoffset;

		float elapsetime;

		Matrix4 tm;
	};

	class _OgreExport RibbonEmitterData : public Resource
	{
		DECLARE_RTTI(RibbonEmitterData)
	public:
		RibbonEmitterData();

		void PrepareGenRibbon(RibbonEmitterFrameData &framedata, int seq, uint tick);

	protected:
		~RibbonEmitterData();
		virtual void _serialize(Archive &ar, int version);

	public:
		RibbonDesc m_Desc;
		RibbonDescEx m_DescEx;
		RibbonSectionDesc m_Template;

		KeyFrameArray<ColourValue> m_Color;
		KeyFrameArray<float> m_Above;
		KeyFrameArray<float> m_Below;

		KeyFrameArray<float>	m_UVRotateKeyFrame;
		KeyFrameArray<float>	m_UScaleKeyFrame;
		KeyFrameArray<float>	m_VScaleKeyFrame;
		KeyFrameArray<float>	m_UOffsetKeyFrame;
		KeyFrameArray<float>	m_VOffsetKeyFrame;

		KeyFrameArray<float>	m_MaskUVRotateKeyFrame;
		KeyFrameArray<float>	m_MaskUScaleKeyFrame;
		KeyFrameArray<float>	m_MaskVScaleKeyFrame;
		KeyFrameArray<float>	m_MaskUOffsetKeyFrame;
		KeyFrameArray<float>	m_MaskVOffsetKeyFrame;
		KeyFrameArray<float>	m_MaskSeqFrameKeyFrame;

		TextureData* m_pTextureRes;
		TextureData* m_pMaskTextureRes;
	};
}

#endif