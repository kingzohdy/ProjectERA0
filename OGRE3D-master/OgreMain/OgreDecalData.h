
#ifndef __OgreDecalData_H__
#define __OgreDecalData_H__

#include "OgreKeyFrameArray.h"
#include "OgreResource.h"
#include "OgreColourValue.h"

namespace Ogre
{
	class DecalDesc
	{
	public:

		int m_TexRows;	//如果只有一个贴图，不需要切换，默认值是1
		int m_TexCols;	//如果只有一个贴图，不需要切换，默认值是1
		float m_TimeSlot; //如果只有一个贴图，不需要切换，默认值是0
		float m_MidPoint;  //midpoint in lifespan
		float m_SizeKeys[3];

		bool m_bDistort;
		int m_BlendMode;

		unsigned char m_reserved[192];
	};

	class _OgreExport DecalData : public Resource
	{
		DECLARE_RTTI(DecalData)

	public:
		struct ANIM_DATA
		{
			float m_fCurrSize;
			float aspect;
			ColourValue color;

			float m_UScale;
			float m_VScale;
			float m_UOff;
			float m_VOff;
			float m_UVRotate;
			int m_SeqFrame;

			float m_MaskUScale;
			float m_MaskVScale;
			float m_MaskUOff;
			float m_MaskVOff;
			float m_MaskUVRotate;
			int m_MaskSeqFrame;
		};

	public:
		DecalData();
		virtual ~DecalData();

		void prepareData(uint tick);

		DecalDesc m_Desc;

		KeyFrameArray<float> m_SizeKeyFrame;
		KeyFrameArray<float> m_AspectKeyFrame;
		KeyFrameArray<ColourValue>	m_ColorKeyFrame;

		KeyFrameArray<float> m_UScaleKeyFrame;
		KeyFrameArray<float> m_VScaleKeyFrame;
		KeyFrameArray<float> m_UOffKeyFrame;
		KeyFrameArray<float> m_VOffKeyFrame;
		KeyFrameArray<float> m_UVRotateKeyFrame;
		KeyFrameArray<float> m_SeqFrameKeyFrame;

		KeyFrameArray<float> m_MaskUScaleKeyFrame;
		KeyFrameArray<float> m_MaskVScaleKeyFrame;
		KeyFrameArray<float> m_MaskUOffKeyFrame;
		KeyFrameArray<float> m_MaskVOffKeyFrame;
		KeyFrameArray<float> m_MaskUVRotateKeyFrame;
		KeyFrameArray<float> m_MaskSeqFrameKeyFrame;

		TextureData* m_pTextureRes;
		TextureData* m_pMaskTextureRes;

		ANIM_DATA m_AnimData;

	private:
		virtual void _serialize(Archive &ar, int version);
	};

}

#endif