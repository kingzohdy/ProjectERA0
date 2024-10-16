
#include "header.fx"

texture g_DiffuseTex;
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

technique Basic
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL;
	int lod = 0;
>
{
	pass P0
	{
		AlphaBlendEnable = FALSE;
		CullMode = None;
		ZEnable = FALSE; 
        
		Sampler[0] = (s_DiffuseSampler);
		ColorOp[0] = MODULATE;
		AlphaOp[0] = MODULATE;
		AlphaOp[1] = MODULATE;
		ColorOp[1] = DISABLE;
	}
}
