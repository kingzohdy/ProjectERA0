
#include "header_macro.fx"

texture g_DiffuseTex;
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = POINT;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

#if USE_SELFILLUM_TEX==SELFILLUM_TEX||USE_SELFILLUM_TEX==SELFILLUM_ALL
texture g_SpecSelfTex;
sampler s_SpecSelfSampler = sampler_state{
    Texture = <g_SpecSelfTex>;
    MipFilter = linear;
    MinFilter = linear;
    MagFilter = linear;
};
#endif

texture   g_depthtexture	   : DEPTH_MAP;
sampler  s_DepthMapSampler = sampler_state{
    Texture = <g_depthtexture>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = BORDER ;
    AddressV = BORDER ;
    BORDERCOLOR		= 0x00ffffff;
};

#ifdef MODEL_TRANSPARENT
#if BLEND_MODE <= 1
#undef BLEND_MODE
#define BLEND_MODE 2
#define ORIGIN_BLEND_OPAQUE
#endif
#endif

/*
stateblock blend_state = stateblock_state
{
	AlphaBlendEnable = TRUE;
	AlphaTestEnable = TRUE;
	AlphaRef = 0;
	AlphaFunc = GREATER;
	SrcBlend = SRCALPHA;
	DestBlend = ONE;
};
*/


stateblock blend_state = stateblock_state
{
#if BLEND_MODE == 0
        AlphaBlendEnable = FALSE;
	AlphaTestEnable = FALSE;
#elif BLEND_MODE == 1
	AlphaBlendEnable = FALSE;
	AlphaTestEnable = TRUE;
	#if USE_SELFILLUM_TEX==SELFILLUM_ALPHA||USE_SELFILLUM_TEX==SELFILLUM_ALL
		AlphaRef = 0;
		AlphaFunc = GREATER;
	#else
		AlphaRef = 85;
		AlphaFunc = GREATEREQUAL;
	#endif
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

technique GeneralTech
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 0;
>
{
    pass P0
    <
	string vs = "Main";
	string ps = "Main";
    >
    {
	stateblock = (blend_state);
	COLORWRITEENABLE = 0;

	ZEnable = true;
	ZWriteEnable = false;	
	ZFunc = EQUAL;
	
	StencilEnable = true;
	StencilFunc = Always;
	StencilRef = 1;
	StencilPass = Replace;
	StencilZFail = KEEP;

	CullMode = NONE;
	
	Sampler[0] = (s_DiffuseSampler);
    }
}
