cbuffer VS_CB_SPACE : register(b0)
{
    float4x4 WorldViewProjection;
};

float4 VS_shadow(float3 Pos : POSITION) : SVSV_Position
{
    return mul(WorldViewProjection, float4(Pos, 1.0f));;
}