
#include "header.fx"

#define MAX_SAMPLES 16

texture g_BaseTexture;              // Color texture for mesh
texture g_BloomTexture;

float g_avSampleOffsets[MAX_SAMPLES];
float g_avSampleWeights[MAX_SAMPLES];
float g_HighScene = 1.0f; //(0.1-1.0)
float g_GrayValue = 0.9f; //(0.5-1.5)
float g_lum		=	0.3f;	 

// ¾¶ÏòÄ£ºý
const float g_fSampleDist = 0.8f;
const float g_fSampleStrength = 1.5f;

// äöÎÐ
float textureX = 1024.0f;
float textureY = 768.0f;
float radiusRate = 0.4f;
float degree = 0.0f;

// Gauss
float gaussRate = 0.0f;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_BaseTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    addressu	=	clamp;
    addressv	=	clamp;
};

texture g_BlendTexture;

sampler BlendTextureSampler = 
sampler_state
{
    Texture = <g_BlendTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    addressu	=	clamp;
    addressv	=	clamp;
};
//#endif

sampler BloomSampler = 
sampler_state
{
    Texture = <g_BloomTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    addressu	=	clamp;
    addressv	=	clamp;
};

sampler BlurSampler = 
sampler_state
{
    Texture = <g_BaseTexture>;
    MipFilter = LINEAR;
    MinFilter = POINT;
    MagFilter = POINT;
    
    addressu	= clamp;
    addressv	= clamp;

};

struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float3 TextureUV  : TEXCOORD0;  // vertex texture coords 
};

struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};

//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
// color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT DownScenePS( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    
    Output.RGBColor = tex2D(MeshTextureSampler, In.TextureUV.xy);

#if BLUR_TYPE == 1
    float4 blendSample = tex2D(BlendTextureSampler, In.TextureUV.xy);
    Output.RGBColor = Output.RGBColor*0.45f + blendSample*0.55f;
#endif

    return Output;
}

PS_OUTPUT BrightPS( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output;
    
    float4 vSample = tex2D( MeshTextureSampler, In.TextureUV.xy );

#if BLUR_TYPE != 2
        	
    // Subtract out dark pixels
    vSample.rgb -= g_GrayValue*normalize(vSample.rgb)*g_lum;
    // Clamp to 0
    vSample = max(vSample, 0.0f);
    
    vSample.rgb = vSample.rgb * vSample.rgb;

#endif
    
    Output.RGBColor = vSample;

    return Output;
}

PS_OUTPUT BloomPS1(VS_OUTPUT In)
{
    PS_OUTPUT Output;
    float4 vSample = 0.0f;
    float4 vColor = 0.0f;
        
    float2 vSamplePosition;    
    // Perform a one-directional gaussian blur
    for(int iSample = 0; iSample < 15; iSample++)
    {
        vSamplePosition =  In.TextureUV.xy + float2(g_avSampleOffsets[iSample], 0);
        vColor = tex2D(BlurSampler, vSamplePosition);
        vSample += g_avSampleWeights[iSample]*vColor;
    }
    Output.RGBColor = vSample;

    return Output;
}

PS_OUTPUT BloomPS2(VS_OUTPUT In)
{
    PS_OUTPUT Output;
    float4 vSample = 0.0f;
    float4 vColor = 0.0f;
        
    float2 vSamplePosition;    
    // Perform a one-directional gaussian blur
    for(int iSample = 0; iSample < 15; iSample++)
    {
        vSamplePosition =  In.TextureUV.xy + float2(0, g_avSampleOffsets[iSample]);
        vColor = tex2D(BlurSampler, vSamplePosition);
        vSample += g_avSampleWeights[iSample]*vColor;
    }
    Output.RGBColor = vSample;

    return Output;
}

float samples[10] =   
{  
   -0.05,  
   -0.04,  
   -0.03,  
   -0.02,  
   -0.01,  
   0.01,  
   0.02,  
   0.03,  
   0.04,  
   0.05  
}; 

PS_OUTPUT FinalPSBloom(VS_OUTPUT In)
{
	PS_OUTPUT Output;

	float2 texCoord = In.TextureUV.xy;

#if BLUR_TYPE == 4
	float2 texSize = float2(textureX, textureY);
	float2 center = texSize * 0.5f;
	float2 tc = texCoord * texSize;
	tc -= center;
	float distance = length(tc);

	float radius = radiusRate * textureY;

	if (distance < radius)
	{
		float percent = (radius - distance)/radius;
		float theta = percent*percent*degree*8;
		float s = sin(theta);
		float c = cos(theta);
		tc = float2(dot(tc, float2(c, -s)), dot(tc, float2(s, c)));
	}
	tc += center;
	texCoord = tc/texSize;
#endif

#if BLUR_TYPE == 2
	float4 vSample = tex2D(MeshTextureSampler, texCoord);
	float4 vBloom	= tex2D(BloomSampler, texCoord);
	float boomRate = 0.75f + gaussRate * 0.25f;
	float sampRate = 0.25f - gaussRate * 0.25f;
	Output.RGBColor = vSample*sampRate + vBloom*boomRate;
#else
	float4 vSample = tex2D(MeshTextureSampler, texCoord);
    	float4 vBloom	= tex2D(BloomSampler, texCoord);	
    	vSample += g_HighScene * vBloom;
	Output.RGBColor = vSample;
#endif
	
#if BLUR_TYPE == 3
	float2 dir = float2(0.5, 0.5) - texCoord;
	float dist = length(dir);
	dir /= dist;

	float4 sum = Output.RGBColor;
   	for (int i = 0; i < 10; ++i)  
   	{  
      		sum += tex2D(MeshTextureSampler, texCoord + dir * samples[i] * g_fSampleDist);  
   	}

 	sum /= 11.0;

	float t = saturate(dist * g_fSampleStrength);  
     
   	Output.RGBColor = lerp(Output.RGBColor, sum, t);  
#endif	

    	return Output;   
}


technique Bloom
<
	int usage = RENDER_USAGE_GENERAL;
	int lod = 0;
>
{
#if BLOOM_PASS == 0
    pass downscenepass
    {          
        VertexShader 	 = NULL;
        PixelShader  	 = compile ps_2_0 DownScenePS();
        cullmode		 = none;
        zenable			 = false; 
    }
#elif BLOOM_PASS == 1
    pass brightpass
    {          
        VertexShader 	 = NULL;
        PixelShader  	 = compile ps_2_0 BrightPS();
        cullmode		 = none;
        zenable			 = false; 
    }
#elif BLOOM_PASS == 2
    pass Hblurpass
    {          
        VertexShader 	 = NULL;
        PixelShader  	 = compile ps_2_0 BloomPS1();
        cullmode	 = none;
        zenable		 = false; 
    }
#elif BLOOM_PASS == 3
    pass Vblurpass
    {          
        VertexShader 	 = NULL;
        PixelShader  	 = compile ps_2_0 BloomPS2();
        cullmode	 = none;
        zenable		 = false; 
    }
#elif BLOOM_PASS == 4
    pass CombinePass
    {          
        VertexShader 	 = NULL;
        PixelShader  	 = compile ps_2_0 FinalPSBloom();
        cullmode		 = none;
        zenable		 = false; 
    }
#endif
}


