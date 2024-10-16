
#include "stdafx.h"
#include "exp_camera.h"

namespace DKEXP
{
	CameraConverter::CameraConverter(TimeValue animStart,TimeValue animEnd)
	{
		m_animStart = animStart;
		m_animEnd = animEnd;
	}

	//---------------------------------------------------------------------------
	CameraConverter::~CameraConverter()
	{
	}

	//---------------------------------------------------------------------------
	int CameraConverter::Convert(Interface* pIntf, INode* pMaxNode, Object* pObj, const std::vector<INode* > &pBoneNodes)
	{
		CameraObject* pCam = (CameraObject *) pObj;

		Interval itvl;
		CameraState cs;

		pCam->UpdateTargDistance(0, pMaxNode);
		pCam->EvalCameraState(m_animStart, itvl, &cs);

		float fFov = cs.fov;
		BOOL bIsOrtho = cs.isOrtho;

		bool bCameraFixed;
		if(pCam->GetManualClip())
			bCameraFixed = true;
		else
			bCameraFixed = false;

		float fTanFOV = (float) tan(fFov * 0.5f);
		float fAspectRatio = pIntf->GetRendImageAspect();

		float fTargetScalar = 1.0f;
		if (bIsOrtho)
		{
			fTargetScalar = pCam->GetTDist(0);
		}

		//设置MAY相机参数
		m_fRatio = fAspectRatio;
		m_fFov = fFov/m_fRatio;		//max 相机的fov为横向，引擎为纵向

		if (bCameraFixed)
		{
			m_fZNear = cs.hither;
			m_fZFar = cs.yon;
		}
		else
		{
			m_fZNear = 1.0f;
			m_fZFar = 5000.0f;
		}

		m_fOrthoSize = fTargetScalar;

		return W3D_STAT_OK;
	}
}
