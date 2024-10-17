
#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

texture g_ColorMaskTex;
sampler s_ColorMaskSampler = sampler_state{
    Texture = <g_ColorMaskTex>;
    MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;
    
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_INPUT
{
	float4 pos			: POSITION;
	float2 uv0			: TEXCOORD0;
};

struct VS_OUTPUTPS
{
	float4 pos      : POSITION;
	float2 uv0      : TEXCOORD0;
};

VS_OUTPUTPS VSMainPS(VS_INPUT input)
{
	VS_OUTPUTPS output;

	output.pos  = mul(float4(input.pos.xyz,1.0), g_WorldViewProj);
	output.uv0 =  input.uv0;

   	return output;
}

float4 PSMain2_0(VS_OUTPUTPS input) : COLOR0
{
	float4 color = tex2D(s_ColorMaskSampler, input.uv0);
	return color;
}

technique USEPS2
<
	int usage = RENDER_USAGE_GENERAL;
	int lod = 0;
>
{
    pass Normal_Pass
    {
	CullMode = None;
   	AlphaBlendEnable = TRUE;
	SrcBlend = SRCALPHA;
	DestBlend = INVSRCALPHA;
	ZWriteEnable = FALSE;
	VertexShader = compile vs_2_0 VSMainPS();
	PixelShader = compile ps_2_0 PSMain2_0();
    }
}
