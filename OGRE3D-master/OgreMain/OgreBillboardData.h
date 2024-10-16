
#ifndef __BillboardData_H__
#define __BillboardData_H__

#include "OgreResource.h"
#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreColourValue.h"
#include "OgreKeyFrameArray.h"

namespace Ogre
{
	// mode selection
	enum FaceMode
	{
		ALWAYS_FACE_CAMERA = 0,
		ROTATE_ABOUT_UP,
		FACE_UP,
		SIMPLE_PANE
	};

	class BillboardDesc
	{
	public:
		FaceMode m_iType;
		Vector3 m_Center;
		//Vector2 m_Extent;
		Vector3 m_Pos;
		int m_TexRows;	//如果只有一个贴图，不需要切换，默认值是1
		int m_TexCols;	//如果只有一个贴图，不需要切换，默认值是1

		float m_TimeSlot; //如果只有一个贴图，不需要切换，默认值是0

		int m_BlendType;
		float m_MidPoint;  //midpoint in lifespan

		ColourValue m_ColorKeys[3];
		float m_SizeKeys[3];
		float m_AspectRatioKeys[3];
	};

	enum SEQ_FRAME_TYPE
	{
		SFT_FIXED_FRAME_TIME,
		SFT_FIT_KEYFRAME
	};

	class BillboardDescEx
	{
	public:
		float				 m_ColorMid;
		float				 m_SizeMid;
		float				 m_AspectMid;
		float				 m_UVRotate;
		float				 m_UVScale;
		SEQ_FRAME_TYPE		 m_FrameType;
		bool				 m_bDistort;
		int					 m_iUVAddressMode;

		int					 m_iUAddressMode;
		int					 m_iVAddressMode;

		int					m_iMaskUVAddrMode;
		int					m_MaskTexRows;
		int					m_MaskTexCols;

		bool m_CenterAbsOffset;//绝对还是相对方式
		Vector2 m_CenterOffset;//中心点的偏移， 如果是相对方式， 以粒子当前大小为单位
	};

	class BillboardDesc103 // 在104版本已经作废，参与数据保存，角色编辑器已经去掉该功能
	{
	public:
		BillboardDesc103 ()
		{
			m_SizeIsHeng = true;
		}
		~BillboardDesc103 ()
		{

		}

		bool				 m_SizeIsHeng;
	};

	class BillboardDesc104
	{
	public:
		BillboardDesc104 ()
		{
			m_bSizeXYDepend = false;
		}

		~BillboardDesc104 ()
		{

		}

		bool m_bSizeXYDepend;
	};

	class _OgreExport BillboardData : public Resource
	{
		DECLARE_RTTI(BillboardData)
	public:
		BillboardData();

	protected:
		~BillboardData();
		virtual void _serialize(Archive &ar, int version);

	public:
		BillboardDesc m_Desc;
		BillboardDescEx m_DescEx;
		BillboardDesc103 m_Desc103;
		BillboardDesc104 m_Desc104;

		void PrepareData(uint tick);

		KeyFrameArray<ColourValue>	m_ColorKeyFrame;
		KeyFrameArray<float>	m_SizeKeyFrame;
		KeyFrameArray<float>	m_AspectKeyFrame;

		// version 103
		KeyFrameArray<float>	m_UVRotateKeyFrame;
		KeyFrameArray<float>	m_UScaleKeyFrame;
		KeyFrameArray<float>	m_VScaleKeyFrame;
		KeyFrameArray<float>	m_UOffsetKeyFrame;
		KeyFrameArray<float>	m_VOffsetKeyFrame;
		KeyFrameArray<float>	m_SeqFrameKeyFrame;

		KeyFrameArray<float>	m_MaskUVRotateKeyFrame;
		KeyFrameArray<float>	m_MaskUScaleKeyFrame;
		KeyFrameArray<float>	m_MaskVScaleKeyFrame;
		KeyFrameArray<float>	m_MaskUOffsetKeyFrame;
		KeyFrameArray<float>	m_MaskVOffsetKeyFrame;
		KeyFrameArray<float>	m_MaskSeqFrameKeyFrame;

		// version 104
		KeyFrameArray<float>	m_SizeXKeyFrame;
		KeyFrameArray<float>	m_SizeYKeyFrame;

		ColourValue m_Color;
		float	m_Size;
		float	m_Aspect;
		float   m_SizeX; // 独立的xy大小
		float	m_SizeY;

		float	m_UVRotate;
		float	m_UScale;
		float	m_VScale;
		float	m_UOffset;
		float	m_VOffset;
		int		m_SeqFrame;

		float	m_MaskUVRotate;
		float	m_MaskUScale;
		float	m_MaskVScale;
		float	m_MaskUOffset;
		float	m_MaskVOffset;
		int		m_MaskSeqFrame;

		TextureData* m_pTexture;
		TextureData* m_pMaskTexture;
	};
}

#endif