

#ifndef __TECH_PARTICLE_H__
#define __TECH_PARTICLE_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{

	class D3DTech_Particle_lod0 : public D3DTech
	{
	public:
		enum
		{
			MTLPARAM_BLEND_MODE = 1,
			MTLPARAM_MASK_TEXTURE,
			MTLPARAM_DIFFUSE_SAMPLEMODE,
			MTLPARAM_MASK_SAMPLEMODE
		};

		uchar m_BlendMode;
		uchar m_MaskTexture;
		uchar m_DiffuseSampleMode;
		uchar m_MaskSampleMode;

	public:
		D3DTech *clone ()
		{
			return new D3DTech_Particle_lod0;
		}

		virtual void init (const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_BlendMode = m_MaskTexture = m_DiffuseSampleMode = m_MaskSampleMode = 0;

			for(int i=0; i<MaterialMacro::MAX_MACRO; i++)
			{
				int indx = macros.indices[i];
				if(indx == 0) break;

				if(indx == MTLPARAM_BLEND_MODE) m_BlendMode = macros.value[i];
				else if(indx == MTLPARAM_MASK_TEXTURE) m_MaskTexture = macros.value[i];
				else if(indx == MTLPARAM_DIFFUSE_SAMPLEMODE) m_DiffuseSampleMode = macros.value[i];
				else if(indx == MTLPARAM_MASK_SAMPLEMODE) m_MaskSampleMode = macros.value[i];
			}

			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = NULL;

			m_AllPass[0].ntex = 1;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 4; //diffusetex

			if (m_MaskTexture)
			{
				m_AllPass[0].ntex = 2;
				m_AllPass[0].texstage[1] = SPU_GENERAL_PARAM + 5; //mask
			}

			initParams();
			m_UsedMacroMask = m_BlendMode|(m_MaskTexture<<8)|(m_DiffuseSampleMode<<16)|(m_MaskSampleMode<<24);
		}

		virtual size_t begin ()
		{
			return 1;
		}

		virtual void end ()
		{

		}

		virtual void beginPass (size_t ipass)
		{
			m_CurPass = ipass;

			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			SetBlendState(m_pDevice, m_BlendMode);

			if (m_BlendMode==0 || m_BlendMode==1)
			{
				m_pDevice->SetRenderState(D3DRS_ZENABLE, true);
				m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
			}
			else
			{
				m_pDevice->SetRenderState(D3DRS_ZENABLE, true);
				m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
			}

			if (m_DiffuseSampleMode)
				SamplerState_Clamp(m_pDevice, 0);
			else
				SamplerState_Wrap(m_pDevice, 0);

			m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

			if(m_MaskTexture)
			{
				if (m_MaskSampleMode)
					SamplerState_Clamp(m_pDevice, 1);
				else
					SamplerState_Wrap(m_pDevice, 1);

				m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
				m_pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
				m_pDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
			}
			else m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
		}
	};

	class D3DTech_particle_distort : public D3DTech
	{
	public:
		enum
		{
			MTLPARAM_BLEND_MODE = 1,
			MTLPARAM_MASK_TEXTURE,
			MTLPARAM_DIFFUSE_SAMPLEMODE,
			MTLPARAM_MASK_SAMPLEMODE
		};

		uchar m_BlendMode;
		uchar m_MaskTexture;
		uchar m_DiffuseSampleMode;
		uchar m_MaskSampleMode;

		D3DTech *clone()
		{
			return new D3DTech_particle_distort;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_BlendMode = m_MaskTexture = m_DiffuseSampleMode = m_MaskSampleMode = 0;

			for(int i=0; i<MaterialMacro::MAX_MACRO; i++)
			{
				int indx = macros.indices[i];
				if(indx == 0) break;

				if(indx == MTLPARAM_BLEND_MODE) m_BlendMode = macros.value[i];
				else if(indx == MTLPARAM_MASK_TEXTURE) m_MaskTexture = macros.value[i];
				else if(indx == MTLPARAM_DIFFUSE_SAMPLEMODE) m_DiffuseSampleMode = macros.value[i];
				else if(indx == MTLPARAM_MASK_SAMPLEMODE) m_MaskSampleMode = macros.value[i];
			}

			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Distort", env, macros);

			m_AllPass[0].ntex = 1;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 4; //diffusetex

			initParams();
			m_UsedMacroMask = 0;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			SetBlendState(m_pDevice, BLEND_OPAQUE);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			if (m_DiffuseSampleMode)
				SamplerState_Clamp(m_pDevice, 0);
			else
				SamplerState_Wrap(m_pDevice, 0);

			SamplerState_Wrap(m_pDevice, 0);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
		}
	};

}

#endif