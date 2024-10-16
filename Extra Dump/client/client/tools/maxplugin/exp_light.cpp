
#include "stdafx.h"
#include "exp_light.h"

using namespace Ogre;

namespace DKEXP
{
	LightConverter::LightConverter(TimeValue animStart,TimeValue animEnd)
	{
		m_animStart = animStart;
		m_animEnd = animEnd;
	}

	LightConverter::~LightConverter()
	{
	}

	int LightConverter::Convert(Interface* pIntf, INode* pMaxNode, Object* pObj, const std::vector<INode* > &pBoneNodes)
	{
		LightState kLState;

		// first extract all the light information
		GenLight* pkGL = (GenLight *)pObj;

		pkGL->EvalLightState(m_animStart, Interval(0,0), &kLState);

		// see if this should be converted to a dynamic texture effect
		Texmap* pkLightMap = pkGL->GetProjMap();
		Texmap* pkShadowMap = pkGL->GetShadowProjMap();

		if (pkLightMap && kLState.type != OMNI_LIGHT)
		{
			// create an texture effect
		}
		// if there are both light and shadow maps on the same light,
		// just do light map
		else if (pkShadowMap)
		{
			if (pkGL->GetLightAffectsShadow())
			{
				// environment map

			}
			else if (kLState.type != OMNI_LIGHT)
			{
				// shadow map
			}
		}
		else
		{
			// build a MAY light
			switch(kLState.type) 
			{
			case OMNI_LIGHT:
				m_eType = LT_POINT;
				break;
			case FSPOT_LIGHT:
			case TSPOT_LIGHT:
				//m_eType = LIGHT_SPOT;
				//break;
			case DIR_LIGHT:
			case TDIR_LIGHT:
				m_eType = LT_DIRECT;
				break;
			default:
				m_eType = LT_NULL;
				break;
			}

			ColourValue kLightCol(kLState.color.r, kLState.color.g, kLState.color.b, 1.0f);

			m_cColor = kLightCol;

			int iDecayType = pkGL->GetDecayType();

			// attenuation
			if (iDecayType == 0)  // constant "none"
				m_fAttenuation = 1.0f;
			else if (iDecayType == 1) // linear "inverse"
				m_fAttenuation = 4.0f/kLState.attenEnd;
			else // quadratic "inverse square"
				m_fAttenuation = 16.0f/(kLState.attenEnd*kLState.attenEnd);

			m_fRange = 10000.0f;
		}

		return W3D_STAT_OK;
	}
}
