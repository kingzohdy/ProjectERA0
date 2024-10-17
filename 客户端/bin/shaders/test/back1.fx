
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

stateblock blend_state = stateblock_state
{
	AlphaBlendEnable = TRUE;
	AlphaTestEnable = TRUE;
	AlphaRef = 0;
	AlphaFunc = GREATER;
	SrcBlend = SRCALPHA;
	DestBlend = ONE;
};


technique GeneralTech
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 0;
>
{
     pass P1
    <
	string vs = "Main";
	string ps = "Main";
    >
    {
	stateblock = (blend_state);
	//COLORWRITEENABLE = 0;
	ZEnable = false;
	ZWriteEnable = false;
	ZFunc = Greater;

	StencilEnable = TRUE;
	StencilFunc = NOTEQUAL;
	StencilRef = 1;
	StencilPass = Keep;
	StencilZFail = Zero;

	CullMode = CCW;
	
	Sampler[0] = (s_DiffuseSampler);
    }
}
