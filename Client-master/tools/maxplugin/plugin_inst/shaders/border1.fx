

#include "header.fx"

#define SELFILLUM_NONE 0  //无自发光
#define SELFILLUM_TEX  1  //只使用自发光贴图
#define SELFILLUM_COLOR 2 //只使用自发光颜色
#define SELFILLUM_ALPHA 3 //只使用diffuse贴图alpha通道作为自发光
#define SELFILLUM_ALL 4   //使用所有

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

float3	g_SelfPower = float3(0.0f,0.0f,0.0f);
float	g_SpecPower;
float	g_SpecShiness;

// for包边
float g_strength  = 1.20f;
float3 g_color  = float3(0.16f,0.99f,0.99f);
//float3 g_color  = float3(1.0f,1.0f,1.0f);
float g_alpha = 0.23f;
float g_scale = 0.0f;

texture g_DiffuseTex;
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = POINT;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

#if USE_SELFILLUM_TEX==SELFILLUM_TEX||USE_SELFILLUM_TEX==SELFILLUM_ALL
texture g_SpecSelfTex;
sampler s_SpecSelfSampler = sampler_state{
    Texture = <g_SpecSelfTex>;
    MipFilter = linear;
    MinFilter = linear;
    MagFilter = linear;
};

float g_TransU = 0.0f;
float g_TransV = 0.0f;
float g_ScaleU = 1.0f;
float g_ScaleV = 1.0f;
float g_RotUV = 0.0f;
#endif

struct VS_INPUT
{
    float4 pos        : POSITION;
    float3 normal        : NORMAL;
    float4 color        :COLOR;
    
#if SKIN_MAXINFL > 0
    float4 blendweights : BLENDWEIGHT;
    float4 blendindices : BLENDINDICES;
#endif

    float2 uv0            : TEXCOORD0;
#if USE_SELFILLUM_TEX==SELFILLUM_ALL
	float2 uv1 : TEXCOORD1;
#endif
};

struct VS_OUTPUT
{
    float4 pos            : POSITION;
    float4 color0        : COLOR0; //color0.xyz=ambient lighting,  color0.w=specular lighting
    float2 uv0            : TEXCOORD0;

#ifdef LIGHT_SHADOW
    float4 color1        : COLOR1; //color1.xyz=shadowed_lighting,  color1.w=specular lighting
    float4 uv1            : TEXCOORD1;    
#endif

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float2 fogc	: TEXCOORD2;
#endif

#if USE_SELFILLUM_TEX==SELFILLUM_ALL
	float2 uv2 : TEXCOORD3;
#endif 
};

VS_OUTPUT VSMainBB(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float3 pos;
    float3 normal;

#if SKIN_MAXINFL > 0
	DoSkinVertex(input.blendindices, input.blendweights, input.pos, input.normal, pos, normal);
#else
    pos        = input.pos;
    normal    = input.normal;
#endif

    normal		= normalize(normal);

   output.pos = mul(float4(pos+normal*g_strength,1.0), g_WorldViewProj);

    output.color0 = DoLighting(pos, normal, 0);
#if USE_SELFILLUM_TEX==SELFILLUM_COLOR
    output.color0.rgb += g_SelfPower.rgb;
#endif

//    output.color0.rgb += input.color.rgb;
    output.color0.a *= g_SpecPower;
    output.uv0 = input.uv0;

#ifdef LIGHT_SHADOW
    output.color1 = DoLighting(pos, normal, 1);
    output.color1.a *= g_SpecPower;
   
	float4 wpos2 = mul(float4(pos,1.0), g_World);
	output.uv1 = CalShadowProjUV(wpos2);
#endif

//    float3 viewdir = normalize(g_EyePosModel - pos);
//	output.color0.a += 1.0 - dot(normal, viewdir);

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float3 wpos = mul(float4(pos,1.0), g_World);
	output.fogc = DoFog(wpos);
#endif

#if USE_SELFILLUM_TEX==SELFILLUM_ALL
	float angle = radians(g_RotUV);
	float2 uv = (input.uv1 - float2(0.5,0.5))*float2(g_ScaleU, g_ScaleV);
	float u = uv.x*cos(angle) + uv.y*sin(angle);
	float v = -uv.x*sin(angle) + uv.y*cos(angle);

	output.uv2 = float2(u,v) + float2(0.5,0.5) + float2(g_TransU, g_TransV);
#endif 
    return output;

}

float4 PSMainBB(VS_OUTPUT input):COLOR0
{
	float4 color  = tex2D(s_DiffuseSampler,input.uv0);
#if USE_SELFILLUM_TEX==SELFILLUM_TEX
	float4 selfspec = tex2D(s_SpecSelfSampler, input.uv0);
#elif USE_SELFILLUM_TEX==SELFILLUM_ALL
	float4 selfspec = tex2D(s_SpecSelfSampler, input.uv2);
#endif

     float4 lighting = input.color0;
#ifdef LIGHT_SHADOW
	float shadow = DoHardShadow(s_DepthMapSampler, input.uv1, input.uv1.z/input.uv1.w); 
	shadow = 1.0f - (1.0f - shadow)*g_shadowdensity;
	lighting += shadow*input.color1;
#endif

#if USE_SELFILLUM_TEX==SELFILLUM_ALPHA||USE_SELFILLUM_TEX==SELFILLUM_ALL
	lighting.rgb = lerp(lighting.rgb, 1.2, color.a); 
#else
	lighting = clamp(lighting, 0, 1.2);
#endif
	
#ifdef LIGHT_SPECULAR
#if USE_SELFILLUM_TEX==SELFILLUM_TEX||USE_SELFILLUM_TEX==SELFILLUM_ALL
	color.rgb = lighting.rgb*color.rgb + g_SpecularColor*lighting.a*selfspec.a + selfspec.rgb*g_SelfPower;
#else
	color.rgb = lighting.rgb*color.rgb + g_SpecularColor*lighting.a;
#endif//USE_SELFILLUM_TEX

#else
#if USE_SELFILLUM_TEX==SELFILLUM_TEX||USE_SELFILLUM_TEX==SELFILLUM_ALL
	color.rgb = lighting.rgb*color.rgb + selfspec.rgb*g_SelfPower;
#else
	color.rgb = lighting.rgb*color.rgb;
#endif//USE_SELFILLUM_TEX
#endif //LIGHT_SPECULAR

#ifdef MODEL_TRANSPARENT
	color.a *= g_ModelTransparent;
#endif
	
#if FOG_DISTANCE>0
	color.rgb = lerp(g_DistFogColor, color.rgb, input.fogc.x);
#endif
#if FOG_HEIGHT > 0
	color.rgb = lerp(g_HeightFogColor, color.rgb, input.fogc.y);
#endif
	color.rgb = g_color;

	float fScale = (sin(4.0f*g_scale) + 1.0f)/2.0f;

	color.a *= (g_alpha*fScale + 0.19f);
  
	return color;
}

technique GeneralTech
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 0;
>
{
    pass pBB
    {
	COLORWRITEENABLE = 0x0000000F;

	CullMode = CW;
	ZEnable  = TRUE;
	ZWriteEnable = TRUE;
	ZFunc = LESS;
	AlphaTestEnable = FALSE;
	ALPHAREF = 85;
	ALPHAFUNC = GREATER;

	AlphaBlendEnable = TRUE;
	SrcBlend = SRCALPHA;
	DestBlend = INVSRCALPHA;

	VertexShader = compile vs_2_0 VSMainBB();
	PixelShader = compile ps_2_0 PSMainBB();
    }
}

