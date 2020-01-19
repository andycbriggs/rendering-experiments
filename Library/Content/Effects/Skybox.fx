
cbuffer CBufferPerObject
{
    float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
}

TextureCube SkyboxTexture;

SamplerState TrilinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};

RasterizerState DisableCulling
{
    CullMode = None;
};

struct VS_INPUT
{
    float4 Position : SV_Position;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 TextureCoordinate : TEXCOORD;
};

VS_OUTPUT VertexShaderMain(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.Position = mul(IN.Position, WorldViewProjection);
    OUT.TextureCoordinate = IN.Position.xyz;
    
    return OUT;
}

float4 PixelShaderMain(VS_OUTPUT IN) : SV_Target
{
    return SkyboxTexture.Sample(TrilinearSampler, IN.TextureCoordinate);
}

technique11 main11
{
    pass p0
    {
        SetRasterizerState(DisableCulling);
        SetVertexShader(CompileShader(vs_5_0, VertexShaderMain()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PixelShaderMain()));
    }
}