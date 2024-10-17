

#include "header.fx"

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
       AlphaBlendEnable = FALSE;
	AlphaTestEnable = TRUE;
	AlphaRef = 0;
	AlphaFunc = GREATER;

	/* ZWriteEnable = TRUE;
	ZEnable = TRUE;*/

	CullMode = NONE;
	COLORWRITEENABLE = 0;

	StencilEnable = true;
	StencilFunc = Always;
	StencilRef = 1;
	StencilPass = Replace;
	StencilZFail = KEEP;

	Sampler[0] = (s_DiffuseSampler);
//#if USE_SELFILLUM_TEX==SELFILLUM_TEX||USE_SELFILLUM_TEX==SELFILLUM_ALL
//	Sampler[1] = (s_SpecSelfSampler);
//#endif
//	Sampler[2] = (s_DepthMapSampler);
    }
}