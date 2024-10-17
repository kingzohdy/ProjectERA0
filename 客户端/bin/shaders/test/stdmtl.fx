
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
#ifdef ORIGIN_BLEND_OPAQUE
    pass P0
    <
	string vs = "Main";
	string ps = "Main";
    >
    {
        AlphaBlendEnable = FALSE;
	AlphaTestEnable = TRUE;
	AlphaRef = 0;
	AlphaFunc = GREATER;

	ZWriteEnable = TRUE;
	ZEnable = TRUE;
#if DOUBLE_SIDE==1
	CullMode = NONE;
#else
	CullMode = CCW;
#endif
	COLORWRITEENABLE = 0;

	Sampler[0] = (s_DiffuseSampler);
	Sampler[1] = (s_DepthMapSampler);
#if USE_SELFILLUM_TEX==SELFILLUM_TEX||USE_SELFILLUM_TEX==SELFILLUM_ALL
	Sampler[2] = (s_SpecSelfSampler);
#endif
    }
#endif

    pass P1
    <
	string vs = "Main";
	string ps = "Main";
    >
    {
	stateblock = (blend_state);
#ifdef ORIGIN_BLEND_OPAQUE
	COLORWRITEENABLE = 0x0000000F;
	ZFunc = LESSEQUAL;
#else
	ZFunc = LESS;
#endif

#if BLEND_MODE==0 || BLEND_MODE==1
	ZWriteEnable = TRUE;
#else
	ZWriteEnable = FALSE;
#endif

	ZEnable = TRUE;

#if DOUBLE_SIDE==1
	CullMode = NONE;
#else
	CullMode = CCW;
#endif

	Sampler[0] = (s_DiffuseSampler);
	Sampler[1] = (s_DepthMapSampler);
#if USE_SELFILLUM_TEX==SELFILLUM_TEX||USE_SELFILLUM_TEX==SELFILLUM_ALL
	Sampler[2] = (s_SpecSelfSampler);
#endif
    }
}

technique GeneralTech2
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 1;
>
{
#ifdef ORIGIN_BLEND_OPAQUE
    pass P0
    <
	string vs = "Main";
	string ps = "";
    >
    {
        AlphaBlendEnable = FALSE;
	AlphaTestEnable = TRUE;
	AlphaRef = 0;
	AlphaFunc = GREATER;

	ZWriteEnable = TRUE;
	ZEnable = TRUE;
#if DOUBLE_SIDE==1
	CullMode = NONE;
#else
	CullMode = CCW;
#endif
	COLORWRITEENABLE = 0;

	Sampler[0] = (s_DiffuseSampler);
	ColorOp[0] = MODULATE;
	AlphaOp[0] = MODULATE;
	ColorOp[1] = DISABLE;
    }
#endif

    pass P1
    <
	string vs = "Main";
	string ps = "";
    >
    {
	stateblock = (blend_state);
#ifdef ORIGIN_BLEND_OPAQUE
	COLORWRITEENABLE = 0x0000000F;
	ZFunc = LESSEQUAL;
#else
	ZFunc = LESS;
#endif

#if BLEND_MODE==0 || BLEND_MODE==1
	ZWriteEnable = TRUE;
#else
	ZWriteEnable = FALSE;
#endif

	ZEnable = TRUE;

#if DOUBLE_SIDE==1
	CullMode = NONE;
#else
	CullMode = CCW;
#endif

	Sampler[0] = (s_DiffuseSampler);
	ColorOp[0] = MODULATE;
	AlphaOp[0] = MODULATE;
	ColorOp[1] = DISABLE;
    }
}

technique ShadowmapGenTech
<
    int usage = RENDER_USAGE_SHADOWMAPGEN;
    int lod = 0;
>
{
    pass P1
    <
	string vs = "ShadowGen";
	string ps = "ShadowGen";
    >
    {
	ColorWriteEnable = 0;
	ZWriteEnable = TRUE;
	AlphaBlendEnable = FALSE;
	AlphaTestEnable = TRUE;
	ALPHAREF = 85;
	ALPHAFUNC = GREATER;
	//DepthBias = 0.0005;
	//SLOPESCALEDEPTHBIAS = 2.02;
	//DepthBias = 0.001;

#if DOUBLE_SIDE==1
	CullMode = NONE;
#else
	CullMode = CCW;
#endif
	Sampler[0] = (s_DiffuseSampler);
    }
}
