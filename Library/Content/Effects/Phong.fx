// I/O

Texture2D DiffuseTexture;

cbuffer CBufferPerFrame
{
    float4 AmbientLightColor = float4(0.f, 0.f, 0.f, 0.f);
    float4 DirectionalLightColor = float4(0.f, 0.f, 0.f, 0.f);
    float3 DirectionalLightDirection = float3(0.f, 0.f, 0.f);
    float4 PointLightColor = float4(0.f, 0.f, 0.f, 0.f);
    float3 PointLightPosition = float3(0.f, 0.f, 0.f);
    float PointLightRadius = 0.f;
    float3 CameraTranslation = float3(0.f, 0.f, 0.f);
};

cbuffer CBufferPerObject
{
    float4x4 WorldViewProjection;
    float4x4 World;
};

struct VS_INPUT
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TextureCoordinate : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 WorldNormal : NORMAL;
    float2 TextureCoordinate : TEXCOORD;
    float3 DirectionalLightDirection : LIGHTDIRECTION;
    float3 VertexWorldPosition : TEXCOORD1;
    float Attenuation : TEXCOORD2; 
};

// States

SamplerState ColorSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

RasterizerState RasterState
{
	CullMode = Back;
	//FillMode = Wireframe;
};

// Shaders

VS_OUTPUT VSMain(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.Position = mul(IN.Position, WorldViewProjection);
	OUT.WorldNormal = normalize(mul(IN.Normal, (float3x3)World));
    OUT.TextureCoordinate = IN.TextureCoordinate;
    OUT.DirectionalLightDirection = -DirectionalLightDirection;
    OUT.VertexWorldPosition = mul(IN.Position, World).xyz;

    float3 pointLightDistance = PointLightPosition - OUT.VertexWorldPosition;
    // Get attentiation as a percentage of the distance of the light from this vertex position
    // What does saturate do to this value? Clamp to max 1.f?
    OUT.Attenuation = saturate(1.f - (length(pointLightDistance) / PointLightRadius));

    return OUT;
}

float4 PSMain(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 worldNormal = IN.WorldNormal;
    float4 textureColor = DiffuseTexture.Sample(ColorSampler, IN.TextureCoordinate);
    float specularPower =  1.f; // Fix to 1 as we don't have specular map right now

    // directionalLight
    float normalDotLightDirection = dot(IN.DirectionalLightDirection, worldNormal);
    float3 directionalColor = (float3)0;
    if (normalDotLightDirection > 0) // dont light back faces
        directionalColor = (DirectionalLightColor.rgb * DirectionalLightColor.a) * normalDotLightDirection * textureColor.rgb;
    
    // point light
    float3 pointLightDirection = normalize(PointLightPosition - IN.VertexWorldPosition);
	float normalDotPointLightDirection = dot(worldNormal, pointLightDirection);
    float3 viewDirection = normalize(CameraTranslation - IN.VertexWorldPosition);
    float worldNormalDotLightDirection = dot(worldNormal, pointLightDirection);
    float3 halfVector = normalize(pointLightDirection + viewDirection);
    float normalDotHalfVector = dot(worldNormal, halfVector);
    float4 lightCoefficients = lit(worldNormalDotLightDirection, normalDotHalfVector, specularPower);
    float3 pointColor =  (PointLightColor.rgb * PointLightColor.a) * (lightCoefficients.y * textureColor.rgb) * IN.Attenuation;
    
    // ambient light
    float3 ambientColor = (AmbientLightColor.rgb * AmbientLightColor.a) * textureColor.rgb;

    OUT.rgb = ambientColor + directionalColor + pointColor;
	OUT.a = textureColor.a;

    return OUT;
}

technique11 main11
{
    pass p0
    {
		SetRasterizerState(RasterState);
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSMain()));
    }
}