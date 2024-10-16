
#include "header.fx"

texture g_DiffuseTex : DIFFUSEMAP
<
	string UIName = "漫反射贴图";
	int Texcoord = 0;
	int MapChannel = 1;
>;

sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
};

texture g_SelfIllumTex
<
	string UIName = "自发光贴图";
	int Texcoord = 1;
	int MapChannel = 2;
>;

sampler s_SelfIllumSampler = sampler_state{
    Texture = <g_SelfIllumTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
};

float g_TransU
<
    string UIWidget = "g_TransU";
    float UIMin = -50;
    float UIMax = 50;
    float UIStep = 0.1;
    string UIName = "U坐标偏移";
> = 0.0f;

float g_TransV
<
    string UIWidget = "g_TransV";
    float UIMin = -50;
    float UIMax = 50;
    float UIStep = 0.1;
    string UIName = "V坐标偏移";
> = 0.0f;

float g_ScaleU
<
    string UIWidget = "g_ScaleU";
    float UIMin = -50;
    float UIMax = 50;
    float UIStep = 0.1;
    string UIName = "U坐标放缩";
> = 1.0f;

float g_ScaleV
<
    string UIWidget = "g_ScaleV";
    float UIMin = -50;
    float UIMax = 50;
    float UIStep = 0.1;
    string UIName = "V坐标放缩";
> = 1.0f;

float g_RotUV
<
    string UIWidget = "g_RotUV";
    float UIMin = -720;
    float UIMax = 720;
    float UIStep = 1;
    string UIName = "坐标旋转";
> = 0.0f;

struct VS_INPUT
{
    float4 pos        : POSITION;
    float3 normal     : NORMAL;
    float2 uv0            : TEXCOORD0;
    float2 uv1            : TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 pos            : POSITION;
    float4 color0        : COLOR0;
    float2 uv0            : TEXCOORD0;
    float2 uv1            : TEXCOORD1;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;
    
	float3 pos = mul(input.pos, g_World);
	float3 normal = normalize(mul(input.normal, (float3x3)g_World));

	output.pos = mul(input.pos, g_WorldViewProj);
	output.color0 = float4(DoDiffuseLighting(pos, normal) + float3(0.2,0.2,0.2), 1.0);

	output.uv0 = input.uv0;

	float angle = radians(g_RotUV);
	float2 uv = (input.uv1 - float2(0.5,-0.5))*float2(g_ScaleU, g_ScaleV);
	float u = uv.x*cos(angle) + uv.y*sin(angle);
	float v = -uv.x*sin(angle) + uv.y*cos(angle);

	output.uv1 = float2(u,v) + float2(0.5,-0.5) + float2(g_TransU, g_TransV);
	return output;
}

float4 PSMain(VS_OUTPUT input):COLOR0
{
	float4 color0  = tex2D(s_DiffuseSampler,input.uv0) * input.color0;
	float4 color1 = tex2D(s_SelfIllumSampler, input.uv1);

	return float4(color0.rgb+color1.rgb, color0.a);
}

technique GeneralTech
{
    pass P1
    {    
	AlphaBlendEnable = FALSE;
	AlphaTestEnable = FALSE;
	AlphaRef = 85;
	AlphaFunc = GREATEREQUAL;

	ZWriteEnable = TRUE;
	ZEnable = TRUE;

	CullMode = None;
        VertexShader =    compile vs_2_0 VSMain();
        PixelShader  =    compile ps_2_0 PSMain();   
    }
}
