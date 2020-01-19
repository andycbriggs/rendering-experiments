Texture2D InputTexture;

SamplerState ColorSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
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

float4 PSGreyscale(VS_OUTPUT IN) : SV_Target
{
	static const float3x3 sepiaWeights = {
		0.299f, 0.299f, 0.299f,
		0.587f, 0.587f, 0.587f,
		0.144f, 0.144f, 0.144f
	};

	float4 color = InputTexture.Sample(ColorSampler, IN.TextureCoordinate);
	return float4(mul(color.rgb, sepiaWeights), color.a);
}

float4 PSSepia(VS_OUTPUT IN) : SV_Target
{
	static const float3x3 sepiaWeights = {
		0.393f, 0.349f, 0.272f,
		0.769f, 0.686f, 0.534f,
		0.189f, 0.168f, 0.131f
	};

	float4 color = InputTexture.Sample(ColorSampler, IN.TextureCoordinate);
	return float4(mul(color.rgb, sepiaWeights), color.a);
}

float4 PSInvert(VS_OUTPUT IN) :SV_Target
{
	float4 color = InputTexture.Sample(ColorSampler, IN.TextureCoordinate);
	return float4(1 - color.rgb, color.a);
}

technique11 NoEffectTechnique
{
    pass p0
    {
		SetRasterizerState(RasterState);
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSNoEffect()));
    }
}

technique11 GreyscaleTechnique
{
	pass p0
	{
		SetRasterizerState(RasterState);
		SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSGreyscale()));
	}
}

technique11 SepiaTechnique
{
	pass p0
	{
		SetRasterizerState(RasterState);
		SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSSepia()));
	}
}

technique11 InvertTechnique
{
	pass p0
	{
		SetRasterizerState(RasterState);
		SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSInvert()));
	}
}