
#ifndef __EXP_CAMERA_H__
#define __EXP_CAMERA_H__

#include "exp_types.h"

namespace DKEXP
{
	class CameraConverter
	{
	public:
		int Convert(Interface* pIntf, INode* pMaxNode, Object* pObj, const std::vector<INode* > &pBoneNodes);

		CameraConverter(TimeValue animStart,TimeValue animEnd);
		~CameraConverter();

	public:
		float m_fFov; // the fov in the direction of height
		float m_fOrthoSize; //if fov == 0, OrthoSize is the view width
		float m_fRatio; //tg(fovx)/tg(fovy)
		float m_fZNear;
		float m_fZFar;
		int m_iBoneID;

	private:
		TimeValue m_animStart;
		TimeValue m_animEnd;
	};
}

#endif