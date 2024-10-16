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
#if MASK_TEXTURE==1
    float4 uv1	    : TEXCOORD1;
#endif
};

VS_OUTPUT VSMain0( float4 pos : POSITION,
                   float4 color0 : COLOR
                          )
{
	VS_OUTPUT output;
    
	output.pos = mul( float4(pos.xyz,1.0f), g_WorldViewProj);
    
	output.color0 = color0;
	output.uv0 = mul( float4(pos.xyz,1.0f), g_DecalMatrix);
#if MASK_TEXTURE==1
	output.uv1 = mul( float4(pos.xyz,1.0f), g_DecalMaskMatrix);
#endif	
	return output;
}


float4 PSMain_Distort(VS_OUTPUT	input):COLOR0
{
	float4 color = tex2Dproj(s_DiffuseSampler, input.uv0);
#if MASK_TEXTURE==1
	color *= tex2Dproj(s_MaskSampler, input.uv1);
#endif
	return (color-128.0f/255.0f)*input.color0.a + 128.0f/255.0f;
}

technique Basic
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 0;
>
{
	pass P0
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

#if BLEND_MODE==0 || BLEND_MODE==1
	CullMode = None;
	ZWriteEnable = TRUE;
	ZEnable = TRUE;
#else
	CullMode = None;
	ZWriteEnable = FALSE;
	ZEnable = TRUE;
#endif

	VertexShader = compile vs_2_0 VSMain0();
	PixelShader = Null;
        
	Sampler[0] = (s_DiffuseSampler);
	ColorOp[0] = MODULATE;
	AlphaOp[0] = MODULATE;
#if MASK_TEXTURE==1
	Sampler[1] = (s_MaskSampler);
	ColorOp[1] = MODULATE;
	AlphaOp[1] = MODULATE;
	ColorOp[2] = DISABLE;
#else
	ColorOp[1] = DISABLE;
#endif
	}
}

technique Distort
<
	int usage = RENDER_USAGE_DISTORT;
	int lod = 0;
>
{
	pass P0
    {
		CullMode = None;
		VertexShader = compile vs_2_0 VSMain0();
		PixelShader	 = compile ps_2_0 PSMain_Distort();
	}

}