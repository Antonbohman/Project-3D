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
    float4 GlowEffect : SV_Target4;
};

Texture2D Texture : register(t0);

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

PS_OUT PS_main(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0;

    float4 pixelColour;

    //add normal map
    output.Normal = float4(normalize(input.Normal), 1.0f);
    
    //get texture colour for the given uv coordinate with selected sampling format
    pixelColour = Texture.Sample(Sampling, input.UV);
    
    //alternative get a given colour
    //pixelColour = float4(input.Color, 1.0f);

    //add ambient lightning

    output.Diffuse = float4((pixelColour.r * DiffuseAlbedo.r), (pixelColour.g * DiffuseAlbedo.g), (pixelColour.b * DiffuseAlbedo.b), DiffusePower);
    
    //add specular map
    output.Specular = float4(SpecularAlbedo, SpecularPower);
    
    //add position map
    output.Position = input.Pos_W;


    //If the alpha of a pixel is above threshold send it to the computeShader
    if (length(pixelColour.xyz) > 0.95f * 1.73f)
    {
        output.GlowEffect = pixelColour;
    }
    else
    {
        output.GlowEffect = (0.0f, 0.0f, 0.0f, 0.0f);
    }

    return output;
};