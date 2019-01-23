struct VS_OUT
{
    float4 Pos_W : POSITION;
	float4 Pos_H : SV_POSITION;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
	float3 Norm : NORMAL;
};

Texture2D tex : register(t0);
SamplerState Sampling{
    Filter = ANISTROPIC;
    MaxAnistropic = 4;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4 Ambient;
    float4 LightPos;
    float4 LightColour;
};

float4 PS_main(VS_OUT input) : SV_Target
{
    //get texture colour for the given uv coordinate with selected sampling format
    //float4 texColour = tex.Sample(Sampling, input.UV.xy);
    
    float4 texColour = float4(input.Color, 1);

    //calculate ambient lightning
    float4 ambientColour = Ambient * texColour;
    
    //calculate angle between light source direction and triangle normal 
    float factor = clamp(dot(input.Norm.xyz, normalize(LightPos.xyz)), 0, 1);

    //calculate diffuse lightning (no ligth/distance loss calculated here)
    float4 diffuseColour = float4(texColour.rgb * LightColour.rgb * factor * LightColour.a,1);
    
    //add all lightning effects for a final pixel colour and make sure it stays inside reasonable boundries
    return clamp(ambientColour + diffuseColour, 0.0f, 1.0f);
};