

#include "header.fx"

texture g_DiffuseTex : DIFFUSEMAP
<
	string UIName = "漫反射贴图";
	int Texcoord = 2;
	int MapChannel = 1;
>;

float g_Shininess
<
	string UIName = "高光强度";
	string UIWidget = "g_Shininess";
	float UIMin = 0;
	float UIMax = 16.0;
	float UIStep = 0.5;
> = 8.0;

sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_INPUT
{
    float4 pos        : POSITION;
    float3 normal        : NORMAL;
    float4 color        :COLOR;
    float2 uv0            : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos            : POSITION;
    float4 color0        : COLOR0;
    float4 color1	 : COLOR1;
    float2 uv0            : TEXCOORD0;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float3 pos = mul(input.pos, g_World);
    float3 normal = normalize(mul(input.normal, (float3x3)g_World));

    output.pos = mul(input.pos, g_WorldViewProj);
    output.color0 = float4(DoDiffuseLighting(pos, normal) + float3(0.2,0.2,0.2), 1.0);
    output.color1 = float4(DoSpecularLighting(pos, normal, g_Shininess), 0.0);
    output.uv0 = input.uv0;

    return output;
}

float4 PSMain(VS_OUTPUT input):COLOR0
{
	float4 color  = tex2D(s_DiffuseSampler,input.uv0);
	return input.color0*color + input.color1;
}

technique GeneralTech
{
    pass P1
    {    
#if BLEND_MODE == 0
        AlphaBlendEnable = FALSE;
#elif BLEND_MODE == 1
	AlphaBlendEnable = TRUE;
	SrcBlend = SRCALPHA;
	DestBlend = INVSRCALPHA;
	//ZWriteEnable = FALSE;
	AlphaTestEnable = TRUE;
	AlphaRef = 178;
	AlphaFunc = GREATEREQUAL;

#elif BLEND_MODE == 2
	AlphaBlendEnable = FALSE;
	AlphaTestEnable = TRUE;
	AlphaRef = 178;
	AlphaFunc = GREATEREQUAL;
#endif
	CullMode = CW;
        VertexShader =    compile vs_2_0 VSMain();
        PixelShader     =    compile ps_2_0 PSMain();   
    }
}
