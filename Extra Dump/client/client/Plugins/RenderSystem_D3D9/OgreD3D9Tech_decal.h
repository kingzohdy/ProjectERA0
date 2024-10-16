
#ifndef __TECH_DECAL_H__
#define __TECH_DECAL_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{
	class D3DTech_decal_lod0 : public D3DTech
	{
	public:
		enum
		{
			MTLPARAM_BLEND_MODE = 1,
			MTLPARAM_MASK_TEXTURE
		};

		uchar m_BlendMode;
		uchar m_HasMaskTex;

	public:
		D3DTech *clone()
		{
			return new D3DTech_decal_lod0;
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
			}

			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = NULL;

			m_AllPass[0].ntex = 2;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 2; //diffusetex
			m_AllPass[0].texstage[1] = SPU_GENERAL_PARAM + 3; //masktex

			initParams();
			m_UsedMacroMask = m_BlendMode|(m_HasMaskTex<<8);
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			SetBlendState(m_pDevice, m_BlendMode);
			if(m_BlendMode==BLEND_OPAQUE || m_BlendMode==BLEND_ALPHATEST)
			{
				m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}
			else m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			SamplerState_Border(m_pDevice, 0);
			m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

			if(m_HasMaskTex)
			{
				SamplerState_Clamp(m_pDevice, 1);
				m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
				m_pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
				m_pDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
			}
			else m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			SetBlendState(m_pDevice, BLEND_OPAQUE);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
	};

	class D3DTech_decal_distort : public D3DTech
	{
	public:
		D3DTech *clone()
		{
			return new D3DTech_decal_distort;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Distort", env, macros);

			m_AllPass[0].ntex = 2;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 2; //diffusetex
			m_AllPass[0].texstage[1] = SPU_GENERAL_PARAM + 3; //masktex

			initParams();
			m_UsedMacroMask = 0;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			SetBlendState(m_pDevice, BLEND_OPAQUE);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			SamplerState_Border(m_pDevice, 0);
			SamplerState_Clamp(m_pDevice, 1);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
		}
	};

}

#endif
