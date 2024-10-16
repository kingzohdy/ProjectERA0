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

#if MASK_TEXTURE==1
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

float4x4 g_WorldViewProj : WORLDVIEWPROJ;
float4x4 g_DecalMatrix;
float4x4 g_DecalMaskMatrix;

struct VS_OUTPUT
{
    float4 pos      : POSITION;
    float4 color0   : COLOR0;
    float4 uv0      : TEXCOORD0;
};

VS_OUTPUT VSMain0( float4 pos : POSITION,
                   float4 color0 : COLOR,
			float2 uv : TEXCOORD0
                          )
{
	VS_OUTPUT output;
    
	output.pos = mul( float4(pos.xyz,1.0f), g_WorldViewProj);
    
	output.color0 = color0;
	output.uv0 = float4(uv.x,uv.y,1.0f,1.0f);

	return output;
}


float4 PSMain_Distort(VS_OUTPUT	input):COLOR0
{
	float4 color = tex2D(s_DiffuseSampler, input.uv0.xy);
	color *= input.color0;
	return color;
}

technique Basic
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 0;
>
{
	pass P0
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
	VertexShader = compile vs_2_0 VSMain0();
	PixelShader = compile ps_2_0 PSMain_Distort();
      
	}
}