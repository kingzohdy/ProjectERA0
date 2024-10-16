

texture g_Texture0;


sampler2D g_samSrcColor =
sampler_state
{
    Texture = <g_Texture0>;
    AddressU = Clamp;
    AddressV = Clamp;
    MinFilter = Point;
    MagFilter = Linear;
    MipFilter = Linear;
};



//-----------------------------------------------------------------------------
// Pixel Shader: PostProcessPS
// Desc: Performs post-processing effect that detects and highlights edges.
//-----------------------------------------------------------------------------


float4 PostProcessPS( float2 Tex : TEXCOORD0 ) : COLOR0
{

    return tex2D( g_samSrcColor, Tex );
}




//-----------------------------------------------------------------------------
// Technique: PostProcess
// Desc: Performs post-processing effect that detects and highlights edges.
//-----------------------------------------------------------------------------
technique PostProcess
{
    pass p0
    {
        VertexShader		= null;
        PixelShader			= compile ps_2_0 PostProcessPS();
        ZEnable				= false;
        alphablendenable	=	true;
        srcblend			=	one;
        destblend			=	one;
    }
}
