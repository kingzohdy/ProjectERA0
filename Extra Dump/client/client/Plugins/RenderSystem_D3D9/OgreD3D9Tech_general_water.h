
#ifndef __TECH_GENERAL_WATER_H__
#define __TECH_GENERAL_WATER_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{
	class D3DTech_general_water_lod0 : public D3DTech
	{
		enum
		{
			MTLPARAM_ALPHABLEND = 1,
		};
		uchar m_alphablend;
	public:
		D3DTech *clone()
		{
			return new D3DTech_general_water_lod0;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_alphablend = 0;
			for(int i=0; i<MaterialMacro::MAX_MACRO; i++)
			{
				int indx = macros.indices[i];
				if(indx == 0) break;

				if(indx == MTLPARAM_ALPHABLEND) m_alphablend = macros.value[i];
			}

			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].ntex = 2;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 4; //g_WaterBaseTex
			m_AllPass[0].texstage[1] = SPU_GENERAL_PARAM + 5; //g_WaterDepthTex

			initParams();
			m_UsedMacroMask = m_alphablend;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			if(m_alphablend == 0)
			{
				SetBlendState(m_pDevice, BLEND_OPAQUE);
				m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}
			else
			{
				SetBlendState(m_pDevice, BLEND_ALPHABLEND);
				m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			}
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);

			SamplerState_Wrap(m_pDevice, 0);
			SamplerState_Clamp(m_pDevice, 1);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}

	};
}

#endif
