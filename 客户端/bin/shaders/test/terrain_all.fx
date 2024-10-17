
#include "header_macro.fx"

#define LOD_BIAS  -0.7f

texture g_BlendColorTex0;
sampler s_BlendColorSampler0 = sampler_state{
    Texture = <g_BlendColorTex0>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
    MIPMAPLODBIAS = LOD_BIAS;
};

texture g_BlendColorTex1;
sampler s_BlendColorSampler1 = sampler_state{
    Texture = <g_BlendColorTex1>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
    MIPMAPLODBIAS = LOD_BIAS;
};

texture g_BlendColorTex2;
sampler s_BlendColorSampler2 = sampler_state{
    Texture = <g_BlendColorTex2>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
    MIPMAPLODBIAS = LOD_BIAS;
};

texture g_BlendColorTex3;
sampler s_BlendColorSampler3 = sampler_state{
    Texture = <g_BlendColorTex3>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
    MIPMAPLODBIAS = LOD_BIAS;
};


texture g_BlendAlphaTex;
sampler s_BlendAlphaSampler = sampler_state{
    Texture = <g_BlendAlphaTex>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Clamp;
    AddressV = Clamp;
};

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

technique USEPS2
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 0;
>
{
    pass Normal_Pass
    <
	string vs = "Main";
	string ps = "Main";
    >
    {
		CullMode = CCW;
		AlphaBlendEnable = FALSE;

		Sampler[0] = (s_BlendColorSampler0);
		Sampler[1] = (s_BlendAlphaSampler);
		Sampler[2] = (s_BlendColorSampler1);
		Sampler[3] = (s_BlendColorSampler2);
		Sampler[4] = (s_BlendColorSampler3);
		Sampler[5] = (s_DepthMapSampler);
    }
}

technique USEFIX
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 1;
>
{
	pass Normal_Pass
	<
		string vs = "Fix";
		string ps = "";
	>
    {
		CullMode = CCW;
		AlphaBlendEnable = FALSE;

		Sampler[0] = (s_BlendColorSampler0);
		ColorOp[0] = MODULATE;
		AlphaOp[0] = MODULATE;
		ColorOp[1] = DISABLE;
    }
}