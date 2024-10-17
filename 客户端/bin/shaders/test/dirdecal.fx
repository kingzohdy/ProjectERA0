#include "header.fx"

texture g_DiffuseTex;
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = linear;
    MinFilter = linear;
    MagFilter = linear;
	AddressU = BORDER;
    AddressV = BORDER;
    BORDERcolor	 = 0x00000000;
};

technique Basic
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
	CullMode = None;
	AlphaBlendEnable = TRUE;
#if BLEND_MODE == 0
	SrcBlend = ONE;
	DestBlend = ONE;
#elif BLEND_MODE == 1
	DestBlend = INVSRCALPHA;
	SrcBlend = SRCALPHA;
#endif
	Sampler[0] = (s_DiffuseSampler);
	}
}