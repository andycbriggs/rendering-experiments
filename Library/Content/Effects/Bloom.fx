Texture2D InputTexture;
Texture2D SceneTexture;

cbuffer CBufferPerObject
{
    float BloomThreshold = 0.45f;
    float BloomIntensity = 1.5f;
};

struct VS_INPUT
{
    float4 Position : SV_Position;
    float2 TextureCoordinate : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 TextureCoordinate : TEXCOORD;
};

RasterizerState RasterState
{
    CullMode = NONE;
};

SamplerState ColorSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

VS_OUTPUT VSMain(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.Position = IN.Position;
    OUT.Position.x *= 2;
    OUT.Position.y *= 2;
    OUT.TextureCoordinate = IN.TextureCoordinate;

    return OUT;
};

const static float3 GreyScaleIntensity = { 0.299f, 0.587f, 0.114f };

float4 PSGlowExtract(VS_OUTPUT IN) : SV_Target
{
    float4 color = InputTexture.Sample(ColorSampler, IN.TextureCoordinate);

    return saturate((color - BloomThreshold) / (1.f - BloomThreshold));
}

float4 PSBloom(VS_OUTPUT IN) : SV_Target
{
    float4 sceneColor = SceneTexture.Sample(ColorSampler, IN.TextureCoordinate);
    float4 bloomColor = InputTexture.Sample(ColorSampler, IN.TextureCoordinate);

    bloomColor *= BloomIntensity;

    sceneColor *= (1 - saturate(bloomColor));

    return sceneColor + bloomColor;
}


technique11 GlowExtract
{
    pass p0
    {
        SetRasterizerState(RasterState);
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSGlowExtract()));
    }
}

technique11 Bloom
{
    pass p0
    {
        SetRasterizerState(RasterState);
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSBloom()));
    }
}
