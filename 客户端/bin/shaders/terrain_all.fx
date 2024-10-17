#include "header.fx"

float4 g_UVRepeat = {1.0, 1.0, 1.0, 1.0};

texture g_ProjectMap;
float4x4 g_WorldViewProj : WORLDVIEWPROJ;

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


struct VS_INPUT
{
	float4 pos			: POSITION;
	float3 normal		: NORMAL;
	float2 uv0			: TEXCOORD0;
	float4 dif			: COLOR0;
};

struct VS_OUTPUTPS
{
	float4 pos      : POSITION;
	float4 color0        : TEXCOORD3; //color0.xyz=ambient lighting,  color0.w=specular lighting
	float2 uv0      : TEXCOORD0;
#ifdef LIGHT_SHADOW
	float4 color1        : TEXCOORD4; //color1.xyz=shadowed_lighting,  color1.w=specular lighting
	float4 uv1            : TEXCOORD1;
#else	
#ifdef LIGHT_SHADOWCUBE
	float4 color1        : TEXCOORD4; //color1.xyz=shadowed_lighting,  color1.w=specular lighting
	float3 uv1           : TEXCOORD1;
#endif	
#endif

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float2 fogc	: TEXCOORD2;
#endif

};

VS_OUTPUTPS VSMainPS(VS_INPUT input)
{
	VS_OUTPUTPS output;

	output.pos  = mul(float4(input.pos.xyz,1.0), g_WorldViewProj);
	output.color0 = DoLighting(input.pos, input.normal, 0) + float4(input.dif.rgb, 0);
	output.uv0 =  input.uv0;
#ifdef LIGHT_SHADOW
	output.color1 = DoLighting(input.pos, input.normal, 1);
	output.uv1  = CalShadowProjUV(input.pos);
#endif

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	output.fogc = DoFog(input.pos);
#endif

   	return output;
}

struct VS_OUTPUTFIX
{
	float4 pos      : POSITION;
	float4 color0        : COLOR;
	float2 uv0      : TEXCOORD0;
};

VS_OUTPUTFIX VSMainFIX(VS_INPUT input, uniform int layer)
{
	VS_OUTPUTFIX output;

	output.pos  = mul(float4(input.pos.xyz,1.0), g_WorldViewProj);
	output.color0 = float4(input.dif.rgb, 0) + float4(1.0,1.0,1.0,0);

	if(layer == 0) output.uv0 = input.uv0*g_UVRepeat.x;
	else if(layer==1) output.uv0 = input.uv0*g_UVRepeat.y;
	else if(layer==2) output.uv0 = input.uv0*g_UVRepeat.z;
	else if(layer==3) output.uv0 = input.uv0*g_UVRepeat.w;

   	return output;
}


float4 PSMain2_0(VS_OUTPUTPS input) : COLOR0
{
	float4 color = tex2D(s_BlendColorSampler0, g_UVRepeat.x*input.uv0);

#if TERRAIN_LAYERS > 1	
	float4 alpha = tex2D(s_BlendAlphaSampler,input.uv0);
	color	= lerp(color,tex2D(s_BlendColorSampler1, g_UVRepeat.y*input.uv0), alpha.b);
#endif
#if TERRAIN_LAYERS > 2
	color	= lerp(color,tex2D(s_BlendColorSampler2,g_UVRepeat.z*input.uv0), alpha.g);	
#endif
#if TERRAIN_LAYERS > 3
	color	= lerp(color,tex2D(s_BlendColorSampler3,g_UVRepeat.w*input.uv0), alpha.r);	
#endif

    float4 lighting = input.color0;
#ifdef LIGHT_SHADOW
	float shadow = DoHardShadow(s_DepthMapSampler, input.uv1, input.uv1.z/input.uv1.w); 
	shadow = 1.0f - (1.0f - shadow)*g_shadowdensity;
	lighting += shadow*input.color1;
#endif

#ifdef LIGHT_SPECULAR
    color.rgb = lighting.rgb*color.rgb + color.a*lighting.a*g_SpecularColor;
#else
    color.rgb = lighting.rgb*color.rgb;
#endif

#if FOG_DISTANCE>0
	color.rgb = lerp(g_DistFogColor, color.rgb, input.fogc.x);
#endif
#if FOG_HEIGHT > 0
	color.rgb = lerp(g_HeightFogColor, color.rgb, input.fogc.y);
#endif

	return float4(color.rgb, 1.0);
}

technique USEPS2
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 0;
>
{
    pass Normal_Pass
    {
		CullMode = CCW;
		AlphaBlendEnable = FALSE;
		VertexShader = compile vs_2_0 VSMainPS();
		PixelShader = compile ps_2_0 PSMain2_0();
    }
}

technique USEFIX
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 1;
>
{
    pass Normal_Pass
    {
		CullMode = CCW;
		AlphaBlendEnable = FALSE;
		VertexShader = compile vs_2_0 VSMainFIX(0);

		Sampler[0] = (s_BlendColorSampler0);
		ColorOp[0] = MODULATE;
		AlphaOp[0] = MODULATE;
		ColorOp[1] = DISABLE;
    }
}