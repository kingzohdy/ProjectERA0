
#ifndef __TECH_LINE_H__
#define __TECH_LINE_H__

namespace Ogre
{

	class D3DTech_Line_lod0 : public D3DTech
	{
	public:
		D3DTech *clone ()
		{
			return new D3DTech_Line_lod0;
		}

		virtual void init (const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = NULL;

			initParams();
		}

		virtual void beginPass (size_t pass)
		{
			m_CurPass = pass;

			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, false);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
			m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);

			applyVSPS(m_CurPass);
		}

		virtual void endPass ()
		{
			m_pDevice->SetRenderState(D3DRS_ZENABLE, true);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
		}
	};

}

#endif