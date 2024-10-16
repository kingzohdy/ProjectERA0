
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

struct VS_OUTPUTPS
{
	float4 pos			: POSITIONT;
	float4 color			: COLOR0;
	float2 uv0			: TEXCOORD0;
	float2 uv1			: TEXCOORD1;
	float2 uv2			: TEXCOORD2;
};

float4 mainps(VS_OUTPUTPS input):COLOR0
{
	float4 color0 = tex2D(s_DiffuseSampler,input.uv0);
	color0 *= input.color;

	if (input.uv2.r != 0.0f)
	{
		color0.g *= 0.65f;
		color0.r = color0.g;
		color0.b = color0.g;
	}

#if MASK_TEXTURE == 1
	float4 color1 = tex2D(s_MaskSampler,input.uv1);
	color0 *= color1;
#endif
	return color0;
}


technique Basic
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL;
	int lod = 0;
>
{
	pass P0
	{
		stateblock = (blend_state);

		CullMode = None;
		ZEnable = FALSE; 

		VertexShader = NULL;
 		PixelShader = compile ps_2_0 mainps();
	/*
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
*/

	}
}


