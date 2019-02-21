#define TYPE_POINT 0
#define TYPE_SPOT 1
#define TYPE_DIRECTIONAL 2

struct PS_IN
{
    float4 ScreenPos : SV_POSITION;
};

Texture2D NormalTexture : register(t0);
Texture2D DiffuseTexture : register(t1);
Texture2D SpecularTexture : register(t2);
Texture2D PositionTexture : register(t3);

SamplerState Sampling : register(s0);

cbuffer VS_CB_CAMERA : register(b0)
{
    float4 CameraOrigin;
    float4 CameraFocus;
};

cbuffer VS_CB_LIGHT : register(b1)
{
    int2 LightType;
    float4 LightPos;
    float4 LightDir;
    float4 LightColour;
    float LightIntensity;
    float2 Angles;
};

float4 PS_light(PS_IN input) : SV_TARGET
{
    //get current texture position
    int3 texPos = int3(input.ScreenPos.xy, 0);

    //get texture info
    float3 normal = NormalTexture.Load(texPos).xyz;
    float3 position = PositionTexture.Load(texPos).xyz;
    float3 diffuseAlbedo = DiffuseTexture.Load(texPos).xyz;
    float4 specularData = SpecularTexture.Load(texPos);

    float3 specularAlbedo = specularData.xyz;
    float specularPower = specularData.w;

    //calculate angles and attenuation
    float3 lightVector = 0;
    float attenuation = 1.0f;
    if (LightType.x == TYPE_POINT || LightType.x == TYPE_SPOT) 
    {
        lightVector = LightPos.xyz - position;

        float ligthDist = length(lightVector);
        attenuation = clamp(1.0f - (lightVector / LightIntensity), 0, 1.0f);
        
        if (LightType.x == TYPE_SPOT)
        {
            attenuation *= clamp((dot(-lightVector, LightDir.xyz) - Angles.y) / (Angles.x - Angles.y), 0, 1.0f);
        }
    }
    else if (LightType.x == TYPE_DIRECTIONAL)
    {
        lightVector = -LightDir.xyz;
    }
    
    //calculate angle between light source direction and normal 
    float lightFactor = clamp(dot(normal, normalize(lightVector)), 0.0f, 1.0f);

    //calculate diffuse lightning (no ligth/distance loss calculated here)
    float4 diffuseColour = float4(diffuseAlbedo * LightColour.rgb * lightFactor, 1.0f);

    //calculate vector for reflected light
    float3 lightReflectionVector = 2 * lightFactor * normal - normalize(LightPos.xyz - position.xyz); //from slide
    
    //clamp so only positive dot product is acceptable
    float dotProduct = clamp(dot(normalize(CameraOrigin.xyz - position.xyz), normalize(lightReflectionVector)), 0.0f, 1.0f);
    float4 specular = float4(diffuseAlbedo.rgb * LightColour.rgb * pow(dotProduct, specularPower), 1);
    
    //add all lightning effects for a final pixel colour and make sure it stays inside reasonable boundries
    //return clamp((diffuseColour + specular) * attenuation, 0.0f, 1.0f);
    return float4(diffuseAlbedo.rgb, 1.0f);
}