
#ifndef __OgreD3D9Tech_Uvanim2Layer_h__
#define __OgreD3D9Tech_Uvanim2Layer_h__

#include "OgreD3D9Technique.h"

namespace Ogre
{

	class D3DTech_Uvanim2Layer_lod0 : public D3DTech
	{
	public:
		D3DTech *clone ()
		{
			return new D3DTech_Uvanim2Layer_lod0;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].ntex = 2;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 0; //diffusetex
			m_AllPass[0].texstage[1] = SPU_GENERAL_PARAM + 1;

			initParams();
			m_UsedMacroMask = 0;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, true);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

			SetBlendState(m_pDevice, BLEND_ALPHATEST, 85);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
		}


	};

}

#endif