struct VS_IN
{
    float3 Pos : POSITION;
    float3 Color : COLOR;
    float2 UV : UV_POS;
};

struct VS_OUT
{
    float4 Pos_W : POSITION;
    float4 Pos_H : SV_POSITION;
    float3 Color : COLOR;
    float2 UV : TEXCOORD0;
    float4 Pos_Rotated[3] : TEXCOORD1;
};

cbuffer VS_CB_SPACE : register(b0)
{
    float4x4 World;
    float4x4 ViewProjection;
    float4x4 RotatedViewProjection[3];
};

VS_OUT VS_main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    //add world position and world normal
    output.Pos_W = mul(World, float4(input.Pos, 1.0f));
    
    //copy values from input to output
    output.Pos_H = mul(output.Pos_W, ViewProjection);
    output.Pos_Rotated[0] = mul(output.Pos_W, RotatedViewProjection[0]);
    output.Pos_Rotated[1] = mul(output.Pos_W, RotatedViewProjection[1]);
    output.Pos_Rotated[2] = mul(output.Pos_W, RotatedViewProjection[2]);

    output.Color = input.Color;
    output.UV = input.UV;
    
    return output;
}