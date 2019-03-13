cbuffer VS_CB_WORLD : register(b0)
{
    float4x4 World;
};

float4 VS_shadow(float3 Pos : POSITION) : SV_Position
{
    return mul(World, float4(Pos, 1.0f));
}