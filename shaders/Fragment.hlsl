struct PS_IN
{
    float4 Pos_W : POSITION;
    float4 Pos_H : SV_POSITION;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
};

struct PS_OUT
{
    float4 Normal : SV_Target0;
    float4 Diffuse : SV_Target1;
    float4 Specular : SV_Target2;
    float4 Position : SV_Target3;
};

//Texture2D Texture : register(t4);

SamplerState Sampling : register(s0);

cbuffer PS_CB_AMBIENT_SPECULAR : register(b0)
{
    float3 SpecularAlbedo;
    float SpecularPower;
};

PS_OUT PS_main(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0;

    float4 pixelColour;

    //add normal map
    output.Normal = float4(normalize(input.Normal), 1.0f);
    
    //get texture colour for the given uv coordinate with selected sampling format
    //pixelColour = Texture.Sample(Sampling, input.UV);
    
    //alternative get a given colour
    pixelColour = float4(input.Color, 1.0f);

    //add ambient lightning
    output.Diffuse = pixelColour;
    
    //add specular map
    output.Specular = float4(SpecularAlbedo, SpecularPower);
    
    //add position map
    output.Position = input.Pos_W;

    return output;
};