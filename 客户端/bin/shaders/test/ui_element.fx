
#include "header.fx"

texture g_DiffuseTex;
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = POINT;
    MinFilter = linear;
    MagFilter = linear;
};

#if MASK_TEXTURE == 1
texture g_MaskTex;
sampler s_MaskSampler = sampler_state{
    Texture = <g_MaskTex>;
    MipFilter = linear;
    MinFilter = linear;
    MagFilter = linear;
    AddressU = CLAMP;
    AddressV = CLAMP;
};
#endif

stateblock blend_state = stateblock_state
{
#if BLEND_MODE == 0
        AlphaBlendEnable = FALSE;
	AlphaTestEnable = FALSE;
#elif BLEND_MODE == 1
	AlphaBlendEnable = FALSE;
	AlphaTestEnable = TRUE;
	AlphaRef = 85;
	AlphaFunc = GREATEREQUAL;
#elif BLEND_MODE == 2
	AlphaBlendEnable = TRUE;
	AlphaTestEnable = TRUE;
	AlphaRef = 0;
	AlphaFunc = GREATER;
	SrcBlend = SRCALPHA;
	DestBlend = INVSRCALPHA;
#elif BLEND_MODE == 3
	AlphaBlendEnable = TRUE;
	AlphaTestEnable = FALSE;
	SrcBlend = ONE;
	DestBlend = INVSRCALPHA;
#elif BLEND_MODE == 4
	AlphaBlendEnable = TRUE;
	AlphaTestEnable = FALSE;
	SrcBlend = ONE;
	DestBlend = ONE;
#endif
};

technique Basic
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL;
	int lod = 0;
>
{
	pass P0
	<
		string vs = "";
		string ps = "Main";
    	>
	{
		stateblock = (blend_state);

		CullMode = None;
		ZEnable = FALSE; 

		VertexShader = NULL;

#if RGB2GRAY == 1
		Sampler[0] = (s_DiffuseSampler);
	#if MASK_TEXTURE == 1
		Sampler[1] = (s_MaskSampler);
	#endif
#else
		Sampler[0] = (s_DiffuseSampler);
		ColorOp[0] = MODULATE;
		AlphaOp[0] = MODULATE;

	#if MASK_TEXTURE == 1
		Sampler[1] = (s_MaskSampler);
		ColorOp[1] = MODULATE;
		AlphaOp[1] = MODULATE;
		ColorOp[2] = DISABLE;
	#else
		ColorOp[1] = DISABLE;
	#endif

#endif

	}
}


