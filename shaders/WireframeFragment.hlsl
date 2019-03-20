struct PS_INOUT
{
    float4 Pos_W : POSITION;
    float4 Pos_H : SV_POSITION;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
    float3 Norm : NORMAL;
};

float4 PS_wire(PS_INOUT input) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}