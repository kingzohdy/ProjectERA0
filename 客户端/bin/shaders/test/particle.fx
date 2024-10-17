#include "header.fx"

texture g_DiffuseTex;
#if DIFFUSE_SAMPLEMODE > 0
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = linear;
    MinFilter = linear;
    MagFilter = linear;
    AddressU = CLAMP;
    AddressV = CLAMP;
};
#else
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = linear;
    MinFilter = linear;
    MagFilter = linear;
    AddressU = WRAP;
    AddressV = WRAP;
};
#endif

#if MASK_TEXTURE == 1
texture g_MaskTex;
#if MASK_SAMPLEMODE > 0
sampler s_MaskSampler = sampler_state{
    Texture = <g_MaskTex>;
    MipFilter = linear;
    MinFilter = linear;
    MagFilter = linear;
    AddressU = CLAMP;
    AddressV = CLAMP;
};
#else
sampler s_MaskSampler = sampler_state{
    Texture = <g_MaskTex>;
    MipFilter = linear;
    MinFilter = linear;
    MagFilter = linear;
    AddressU = WRAP;
    AddressV = WRAP;
};
#endif //MASK_SAMPLEMODE
#endif

technique Basic
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 0;
>
{
    pass P0
    <
	string vs = "Main";
	string ps = "";
    >
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
#elif BLEND_MODE == 5
	AlphaBlendEnable = TRUE;
	AlphaTestEnable = FALSE;
	SrcBlend = DESTCOLOR;
	DestBlend = ZERO;
#elif BLEND_MODE == 6
	AlphaBlendEnable = TRUE;
	AlphaTestEnable = FALSE;
	SrcBlend = DESTCOLOR;
	DestBlend = SRCCOLOR;
#endif

#if BLEND_MODE==0 || BLEND_MODE==1
	CullMode = None;
	ZWriteEnable = TRUE;
	ZEnable = TRUE;
#else
	CullMode = None;
	ZWriteEnable = FALSE;
	ZEnable = TRUE;
#endif

	Sampler[0] = (s_DiffuseSampler);
	ColorOp[0] = MODULATE;
	AlphaOp[0] = MODULATE;
#if MASK_TEXTURE==1
	Sampler[1] = (s_MaskSampler);
	ColorOp[1] = MODULATE;
	AlphaOp[1] = MODULATE;
	ColorOp[2] = DISABLE;
	Sampler[2] = NULL;
#else
	Sampler[1] = NULL;
	ColorOp[1] = DISABLE;
#endif
    }
}


technique Distort
<
	int usage = RENDER_USAGE_DISTORT;
	int lod = 0;
>
{
	pass P0
	<
		string vs = "Main";
		string ps = "Distort";
	>
    {
	AlphaBlendEnable = TRUE;
	SrcBlend = SRCALPHA;
	DestBlend = INVSRCALPHA;

	CullMode = None;
	Sampler[0] = (s_DiffuseSampler);
	}

}