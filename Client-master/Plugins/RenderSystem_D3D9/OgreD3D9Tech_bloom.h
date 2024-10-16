
#ifndef __TECH_BLOOM_H__
#define __TECH_BLOOM_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{
	class D3DTech_bloom_lod0 : public D3DTech
	{
	public:
		enum
		{
			MTLPARAM_BLOOM_PASS = 1
		};
		uchar m_BloomPass;

	public:
		D3DTech *clone()
		{
			return new D3DTech_bloom_lod0;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_BloomPass = 0;
			for(int i=0; i<MaterialMacro::MAX_MACRO; i++)
			{
				int indx = macros.indices[i];
				if(indx == 0) break;

				if(indx == MTLPARAM_BLOOM_PASS) m_BloomPass = macros.value[i];
			}

			m_nPass = 1;
			m_AllPass[0].vs = NULL;
			if(m_BloomPass == 0) //downscenepass
			{
				m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "DownScenePS", env, macros);
			}
			else if(m_BloomPass == 1) //brightpass
			{
				m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "BrightPS", env, macros);
			}
			else if(m_BloomPass == 2) //Hblurpass
			{
				m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "BloomPS1", env, macros);
			}
			else if(m_BloomPass == 3) //Vblurpass
			{
				m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "BloomPS2", env, macros);
			}
			else if(m_BloomPass == 4) //CombinePass
			{
				m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "FinalPSBloom", env, macros);
			}

			m_nPass = 1;
			m_AllPass[0].ntex = 4;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 2; //MeshTexture
			m_AllPass[0].texstage[1] = SPU_GENERAL_PARAM + 3; //BlendTexture
			m_AllPass[0].texstage[2] = SPU_GENERAL_PARAM + 4; //BloomTexture
			m_AllPass[0].texstage[3] = SPU_GENERAL_PARAM + 2; //BlurTexture

			initParams();
			m_UsedMacroMask = m_BloomPass;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			SetDefaultState(m_pDevice);

			SetBlendState(m_pDevice, BLEND_OPAQUE);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			SamplerState_Clamp(m_pDevice, 0);
			SamplerState_Clamp(m_pDevice, 1);
			SamplerState_Clamp(m_pDevice, 2);

			m_pDevice->SetSamplerState(3, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			m_pDevice->SetSamplerState(3, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
			m_pDevice->SetSamplerState(3, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			m_pDevice->SetSamplerState(3, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			m_pDevice->SetSamplerState(3, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			SetBlendState(m_pDevice, BLEND_OPAQUE);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			SetDefaultState(m_pDevice);
		}
	};
}

#endif
