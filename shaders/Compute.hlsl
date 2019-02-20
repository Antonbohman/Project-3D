struct PS_IN
{
    float4 Pos_W : POSITION;
    float4 Pos_H : SV_POSITION;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4 Ambient;
    float4 LightPos;
    float4 LightColour;
    float4 CameraView;
};

[numthreads(1, 1, 1)]
void CS_main( uint3 DTid : SV_DispatchThreadID )
{

}