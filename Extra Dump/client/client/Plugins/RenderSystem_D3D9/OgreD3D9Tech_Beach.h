
#ifndef __TECH_BEACH_H__
#define __TECH_BEACH_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{
	class D3DTech_beach_lod0 : public D3DTech
	{
	public:
		enum
		{
			MTLPARAM_BLEND_MODE = 1,
		};

		uchar m_isAdd; //1:add, 0:blend

	public:
		D3DTech *clone()
		{
			return new D3DTech_beach_lod0;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_isAdd = 0;

			for(int i=0; i<MaterialMacro::MAX_MACRO; i++)
			{
				int indx = macros.indices[i];
				if(indx == 0) break;

				if(indx == MTLPARAM_BLEND_MODE)
				{
					if(macros.value[i] == BLEND_ALPHABLEND) m_isAdd = 0;
					else m_isAdd = 1;
				}
			}

			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].ntex = 2;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 1; //diffusetex
			m_AllPass[0].texstage[1] = SPU_GENERAL_PARAM + 2; //masktex

			initParams();
			m_UsedMacroMask = m_isAdd;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			if(m_isAdd) SetBlendState(m_pDevice, BLEND_ADD);
			else SetBlendState(m_pDevice, BLEND_ALPHABLEND);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			SamplerState_Wrap(m_pDevice, 0);
			SamplerState_Clamp(m_pDevice, 1);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			SetBlendState(m_pDevice, BLEND_OPAQUE);
		}
	};
}

#endif
