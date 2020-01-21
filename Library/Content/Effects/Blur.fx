Texture2D InputTexture;

const static int NumSamples = 9; // must match BlurMaterialw

cbuffer CBufferPerFrame
{
    float2 SampleOffsets[NumSamples];
    float SampleWeights[NumSamples];
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
    VS_OUTPUT OUT = (VS_OUTPUT) 0;

    OUT.Position = IN.Position;
    OUT.Position.x *= 2;
    OUT.Position.y *= 2;
    OUT.TextureCoordinate = IN.TextureCoordinate;

    return OUT;
};

float4 PSNoEffect(VS_OUTPUT IN) : SV_Target
{
    return InputTexture.Sample(ColorSampler, IN.TextureCoordinate);
}

float4 PSBlur(VS_OUTPUT IN) : SV_Target
{
    float4 color = (float4)0;

    for (int i = 0; i < NumSamples; ++i)
    {
        color += InputTexture.Sample(ColorSampler, IN.TextureCoordinate + SampleOffsets[i]) * SampleWeights[i];
    }

    return color;
}

technique11 NoBlur
{
    pass p0
    {
        SetRasterizerState(RasterState);
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSNoEffect()));
    }
}

technique11 Blur
{
    pass p0
    {
        SetRasterizerState(RasterState);
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSBlur()));
    }
}
