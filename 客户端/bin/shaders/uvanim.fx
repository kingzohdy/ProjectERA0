
#include "header.fx"

#ifdef MODEL_TRANSPARENT
#if BLEND_MODE<2
#undef BLEND_MODE
#define BLEND_MODE 2
#endif
#endif

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

texture g_DiffuseTex;
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
};

#if USE_MASK_TEX==1
texture g_MaskTex;
sampler s_MaskSampler = sampler_state{
    Texture = <g_MaskTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
};
#endif

float g_TransU = 0.0f;
float g_TransV = 0.0f;
float g_ScaleU = 1.0f;
float g_ScaleV = 1.0f;
float g_RotUV = 0.0f;

struct VS_INPUT
{
    float4 pos        : POSITION;
#if RECEIVE_LIGHTING==1
    float3 normal     : NORMAL;
#endif
    float2 uv0            : TEXCOORD0;
    float2 uv1            : TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 pos            : POSITION;
#if RECEIVE_LIGHTING==1
	float4 color0     : COLOR0;
#endif
    float2 uv0            : TEXCOORD0;
    float2 uv1            : TEXCOORD1;

#if RECEIVE_LIGHTING==1
#ifdef LIGHT_SHADOW
    float4 color1        : COLOR1; //color1.xyz=shadowed_lighting,  color1.w=specular lighting
    float4 uv2            : TEXCOORD2;
#endif
#endif

#if RECEIVE_FOG==1
#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float2 fogc	: TEXCOORD3;
#endif
#endif
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;
    
	output.pos = mul(input.pos, g_WorldViewProj);

	float angle = radians(g_RotUV);
	float2 uv = (input.uv0 - float2(0.5,0.5))*float2(g_ScaleU, g_ScaleV);
	float u = uv.x*cos(angle) + uv.y*sin(angle);
	float v = -uv.x*sin(angle) + uv.y*cos(angle);

	output.uv0 = float2(u,v) + float2(0.5,0.5) + float2(g_TransU, g_TransV);
	output.uv1 = input.uv1;

#if RECEIVE_LIGHTING==1
	output.color0 = DoLighting(input.pos, input.normal, 0);
#ifdef LIGHT_SHADOW
    output.color1 = DoLighting(input.pos, input.normal, 1);
    float4 wpos2 = mul(input.pos, g_World);
    output.uv2 = CalShadowProjUV(wpos2);
#endif
#endif

#if RECEIVE_FOG==1
#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float3 wpos = mul(input.pos, g_World);
	output.fogc = DoFog(wpos);
#endif
#endif
	return output;
}

float4 PSMain(VS_OUTPUT input):COLOR0
{
	float4 color0  = tex2D(s_DiffuseSampler,input.uv0);
#if USE_MASK_TEX==1
	float4 color1 = tex2D(s_MaskSampler, input.uv1);
#else
	float4 color1 = float4(1.0, 1.0, 1.0, 1.0);
#endif

#if RECEIVE_LIGHTING==1
     float4 lighting = input.color0;
#ifdef LIGHT_SHADOW
	float shadow = DoHardShadow(s_DepthMapSampler, input.uv2, input.uv2.z/input.uv2.w); 
	shadow = 1.0f - (1.0f - shadow)*g_shadowdensity;
	lighting += shadow*input.color1;
#endif

	float4 color = float4(lighting.rgb,1.0)*color0*color1;
#else
	float4 color = color0*color1;
#endif

#ifdef MODEL_TRANSPARENT
	color *= g_ModelTransparent;
#endif

#if RECEIVE_FOG==1

#if BLEND_MODE < 3

#if FOG_DISTANCE>0
	color.rgb = lerp(g_DistFogColor, color.rgb, input.fogc.x);
#endif
#if FOG_HEIGHT > 0
	color.rgb = lerp(g_HeightFogColor, color.rgb, input.fogc.y);
#endif

#else //blend_mode>=3

#if FOG_DISTANCE>0
	color *= input.fogc.x;
#endif
#if FOG_HEIGHT > 0
	color *= input.fogc.y;
#endif

#endif

#endif

	return color;
}

technique GeneralTech
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 0;
>
{
    pass P1
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
	ZWriteEnable = TRUE;
	ZEnable = TRUE;
#else
	ZWriteEnable = FALSE;
	ZEnable = TRUE;
#endif

	CullMode = None;
        VertexShader =    compile vs_2_0 VSMain();
        PixelShader  =    compile ps_2_0 PSMain();   
    }
}
