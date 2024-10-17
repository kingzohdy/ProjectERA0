#include "header.fx"

texture g_DiffuseTex;
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = linear;
    MinFilter = linear;
    MagFilter = linear;
	AddressU = CLAMP;
    AddressV = CLAMP;
};

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

struct VS_OUTPUT
{
    float4 pos      : POSITION;
    float4 color0   : COLOR0;
    float2 uv0      : TEXCOORD0;
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
		AlphaBlendEnable = TRUE;
		SrcBlend = DESTCOLOR;
		DestBlend = ZERO;

		CullMode = None;
		ZWriteEnable = FALSE;
		ZEnable = TRUE;
		Sampler[0] = (s_DiffuseSampler);
	}
}
/*
technique Basic1
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 1;
>
{
	pass P0
	{
		AlphaBlendEnable = TRUE;
		SrcBlend = DESTCOLOR;
		DestBlend = ZERO;

		CullMode = None;
		ZWriteEnable = FALSE;
		ZEnable = TRUE;

		VertexShader = compile vs_2_0 VSMain0();
		Sampler[0] = (s_DiffuseSampler);
		ColorOp[0] = MODULATE;
		AlphaOp[0] = MODULATE;
		ColorOp[1] = DISABLE;
	}
}
*/