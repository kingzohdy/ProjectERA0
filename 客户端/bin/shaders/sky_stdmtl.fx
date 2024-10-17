

#include "header.fx"

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
};

struct VS_OUTPUT
{
    float4 pos            : POSITION;
    float4 color0        : COLOR0; //color0.xyz=ambient lighting,  color0.w=specular lighting
    float2 uv0            : TEXCOORD0;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float3 pos;
    float3 normal;
    
    pos        = input.pos;
    normal    = input.normal;

    output.pos = mul(float4(pos,1.0), g_WorldViewProj);
    output.color0 = DoLighting(pos, normal, 0);
    output.uv0 = input.uv0;


    return output;
}

float4 PSMain(VS_OUTPUT input):COLOR0
{
	float4 color  = tex2D(s_DiffuseSampler,input.uv0);   
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
#elif BLEND_MODE == 1
	AlphaBlendEnable = TRUE;
	SrcBlend = SRCALPHA;
	DestBlend = INVSRCALPHA;
#elif BLEND_MODE == 2
	AlphaBlendEnable = FALSE;
	AlphaTestEnable = TRUE;
	AlphaRef = 178;
	AlphaFunc = GREATEREQUAL;
#endif
		cullmode	=	none;
        VertexShader =    compile vs_2_0 VSMain();
        PixelShader     =    compile ps_2_0 PSMain();   
    }
}

technique ShadowmapGenTech
<
    int usage = RENDER_USAGE_SHADOWMAPGEN;
    int lod = 0;
>
{
    pass P1
    {
		colorwriteenable = 0;
		zwriteenable	 = false;    
        AlphaBlendEnable = FALSE;
        cullmode		=	none;
        VertexShader =    compile vs_2_0 VSMain();  
        PixelShader     =    compile ps_2_0 PSMain();   
        
    }
}
