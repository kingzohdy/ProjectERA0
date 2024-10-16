
#ifndef __TECH_UIELEMENT_H__
#define __TECH_UIELEMENT_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{
	class D3DTech_uielement_lod0 : public D3DTech
	{
		enum
		{
			MTLPARAM_BLEND_MODE = 1,
			MTLPARAM_MASK_TEXTURE,
			MTLPARAM_RGB2GRAY,
		};
		uchar m_BlendMode;
		uchar m_HasMaskTex;
		uchar m_RGB2GRAY;
	public:
		D3DTech *clone()
		{
			return new D3DTech_uielement_lod0;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_BlendMode = m_HasMaskTex = 0;

			for(int i=0; i<MaterialMacro::MAX_MACRO; i++)
			{
				int indx = macros.indices[i];
				if(indx == 0) break;

				if(indx == MTLPARAM_BLEND_MODE) m_BlendMode = macros.value[i];
				else if(indx == MTLPARAM_MASK_TEXTURE) m_HasMaskTex = macros.value[i];
				else if(indx == MTLPARAM_RGB2GRAY) m_RGB2GRAY = macros.value[i];
			}

			m_nPass = 1;
			m_AllPass[0].vs = NULL;

			if (m_RGB2GRAY)
				m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);
			else
				m_AllPass[0].ps = NULL;

			m_AllPass[0].ntex = 2;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 3; //diffusetex
			m_AllPass[0].texstage[1] = SPU_GENERAL_PARAM + 4; //masktex

			initParams();
			m_UsedMacroMask = m_BlendMode|(m_HasMaskTex<<8);
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			SetBlendState(m_pDevice, m_BlendMode);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

			if (m_RGB2GRAY)
			{
				SamplerState_Wrap(m_pDevice, 0);
				SamplerState_Clamp(m_pDevice, 1);
			}
			else
			{
				SamplerState_Wrap(m_pDevice, 0);
				m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

				if(m_HasMaskTex)
				{
					SamplerState_Clamp(m_pDevice, 1);
					m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
					m_pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
					m_pDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
				}
				else 
					m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			}

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		}

	};
}

#endif
