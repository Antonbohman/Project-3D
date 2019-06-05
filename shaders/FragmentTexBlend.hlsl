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

Texture2D Water_tex : register(t0);
Texture2D Road_tex : register(t1);
Texture2D Grass_tex : register(t2);
Texture2D Rock_tex : register(t3);

SamplerState Sampling : register(s0);

cbuffer PS_CB_AMBIENT_SPECULAR : register(b0)
{
    float3 AmbientAlbedo;
    float AmbientPower;
    float3 DiffuseAlbedo;
    float DiffusePower;
    float3 SpecularAlbedo;
    float SpecularPower;
};

PS_OUT PS_blend(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0;

    float4 pixelColour;


    //add normal map
    output.Normal = float4(normalize(input.Normal), 1.0f);
    
    //get texture colour for the given uv coordinate with selected sampling format
    float3 texture_r = input.Color.r * Road_tex.Sample(Sampling, input.UV);
    float3 texture_g = input.Color.g * Grass_tex.Sample(Sampling, input.UV);
    float3 texture_b = input.Color.b * Rock_tex.Sample(Sampling, input.UV);
    float3 texture_x = (1 - (input.Color.r + input.Color.g + input.Color.b)) * Water_tex.Sample(Sampling, input.UV);

    pixelColour = float4(texture_r + texture_g + texture_b + texture_x, 1.0f);
    
    //alternative get a given colour

    //add ambient lightning
    output.Diffuse = float4((pixelColour.r * DiffuseAlbedo.r), (pixelColour.g * DiffuseAlbedo.g), (pixelColour.b * DiffuseAlbedo.b), DiffusePower);
    
    //add specular map
    output.Specular = float4(SpecularAlbedo, SpecularPower);
    
    //add position map
    output.Position = input.Pos_W;

    return output;
};