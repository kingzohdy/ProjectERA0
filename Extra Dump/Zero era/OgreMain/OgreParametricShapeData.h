
#ifndef __OgreParametricShapeData_H__
#define __OgreParametricShapeData_H__

#include "OgreResource.h"
#include "OgreColourValue.h"
#include "OgreKeyFrameArray.h"

namespace Ogre
{
	enum PARAMSHAPE_TYPE
	{

		GEO_SPHERE = 0,
		GEO_TORUS,
		GEO_TUBE,
		GEO_COLUMN
	};

	struct ParamShapeFrameData;
	class _OgreExport ParamShapeData : public Resource
	{
		DECLARE_RTTI(ParamShapeData)

	public:
		ParamShapeData(void);

		void prepareData(uint curtick, ParamShapeFrameData &framedata);

	protected:
		virtual ~ParamShapeData(void);
		virtual void _serialize(Archive &ar, int version);


	public:
		bool				m_bGridMode;
		PARAMSHAPE_TYPE		m_GeoType;
		int					m_iBlendMode;
		int					m_iTexCol;
		int					m_iTexRow;
		float				m_fFrameTime;
		int					m_iUVAddressMode;
		int					m_iUAddressMode;
		int					m_iVAddressMode;
		bool				m_bDistort;

		int					m_nUSeg;
		int					m_nVSeg;

		int					m_iMaskTexCol;
		int					m_iMaskTexRow;
		int					m_iMaskUVAddressMode;


		TextureData* m_pTextureRes;
		TextureData* m_pMaskTextureRes;

		KeyFrameArray<float> m_SphereR;
		KeyFrameArray<float> m_SphereAngle;

		KeyFrameArray<float> m_TorsuNR;
		KeyFrameArray<float> m_TorsuLR;

		KeyFrameArray<float> m_TubeNR;
		KeyFrameArray<float> m_TubeLR;
		KeyFrameArray<float> m_TubeH;

		KeyFrameArray<float> m_ColumnDR;
		KeyFrameArray<float> m_ColumnUR;
		KeyFrameArray<float> m_ColumnH;


		KeyFrameArray<float> m_UOff;
		KeyFrameArray<float> m_VOff;
		KeyFrameArray<float> m_Round;
		KeyFrameArray<float> m_UScl;
		KeyFrameArray<float> m_VScl;
		KeyFrameArray<float> m_UVRot;

		KeyFrameArray<ColourValue> m_Clr;

		KeyFrameArray<float> m_MaskUOff;
		KeyFrameArray<float> m_MaskVOff;
		KeyFrameArray<float> m_MaskUScl;
		KeyFrameArray<float> m_MaskVScl;
		KeyFrameArray<float> m_MaskUVRot;
	};
}

#endif