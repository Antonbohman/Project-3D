// light types
#define TYPE_POINT 0
#define TYPE_SPOT 1
#define TYPE_DIRECTIONAL 2

// render modes
#define RENDER_DEFAULT			0 //default output, everything on
#define RENDER_DIFFUSE			1 //only outputs diffuse albedo
#define RENDER_SPECULAR			2 //only outputs specular albedo
#define RENDER_NORMALS		    3 //only outputs normals
#define RENDER_POSITIONS		4 //only outputs position
#define RENDER_DEPTH			5 //only outputs depth buffer
#define RENDER_NO_SHADOWS		6 //as default but without shadows
#define RENDER_NO_SPECULAR		7 //as defaults but without specular

struct PS_IN
{
    float4 ScreenPos : SV_POSITION;
};

Texture2D NormalTexture : register(t0);
Texture2D DiffuseTexture : register(t1);
Texture2D SpecularTexture : register(t2);
Texture2D PositionTexture : register(t3);
Texture2D GlowTexture : register(t4);
Texture2D DepthTexture : register(t5);

Texture2DArray ShadowMaps : register(t8);

SamplerState Sampling : register(s0);

cbuffer PS_CB_FLAGS : register(b0)
{
    bool SplitView;
    uint RenderMode;
};

cbuffer PS_CB_CAMERA : register(b1)
{
    float4 CameraOrigin;
    float4 CameraFocus;
};

cbuffer PS_CB_LIGHT : register(b2)
{
    uint LightType;
    uint AmbientPower;
    float LightIntensity;
    float LightFocus;
    float4 LightPos;
    float4 LightDir;
    float4 LightColour;
    float4x4 LighViewProjection;
    float4x4 RotatedLightViewProjection[5];
};

bool shadow(float3 position)
{
    float4 lsp = mul(float4(position, 1.0f), LighViewProjection);

    lsp.xyz /= lsp.w;

    float2 sMapTex = float2(0.5f * lsp.x + 0.5, -0.5f * lsp.y + 0.5f);
        
    if (lsp.x < -1.0f || lsp.x > 1.0f ||
            lsp.y < -1.0f || lsp.y > 1.0f ||
            lsp.z < 0.0f || lsp.z > 1.0f)
        return true;
    
    lsp.z -= 0.0005f;

    float shadowMapDepth = ShadowMaps.Sample(Sampling, float3(sMapTex, 0)).r;

    if (shadowMapDepth < lsp.z)
        return true;

    return false;
}

bool shadow(float3 position, int index)
{
    float4 lsp = mul(float4(position, 1.0f), RotatedLightViewProjection[index]);

    lsp.xyz /= lsp.w;

    float2 sMapTex = float2(0.5f * lsp.x + 0.5, -0.5f * lsp.y + 0.5f);
        
    if (lsp.x < -1.0f || lsp.x > 1.0f ||
            lsp.y < -1.0f || lsp.y > 1.0f ||
            lsp.z < 0.0f || lsp.z > 1.0f)
        return true;
    
    lsp.z -= 0.0005f;

    float shadowMapDepth = ShadowMaps.Sample(Sampling, float3(sMapTex, index+1)).r;

    if (shadowMapDepth < lsp.z)
        return true;

    return false;
}

float4 PS_light(PS_IN input) : SV_TARGET
{
    //get current texture position
    int3 texPos = int3(input.ScreenPos.xy, 0);

    //get texture info
    float3 normal = NormalTexture.Load(texPos).xyz;
    float3 position = PositionTexture.Load(texPos).xyz;
    float3 diffuseAlbedo = DiffuseTexture.Load(texPos).xyz;
    float4 specularData = SpecularTexture.Load(texPos);
    float4 glowEffect = GlowTexture.Load(texPos);
    float depth = DepthTexture.Load(texPos).x;
    
    float3 specularAlbedo = specularData.xyz;
    float specularPower = specularData.w;

    //alternative render modes
    switch (RenderMode)
    {
        case RENDER_DIFFUSE:
            //Render diffuse albedo
            return float4(diffuseAlbedo.rgb, 1);
            break;
        case RENDER_SPECULAR:
            //Render specular albedo
            return float4(specularAlbedo.rgb, 1);
            break;
        case RENDER_NORMALS:
            //Renders normals
            return float4(normal.rgb, 1);
            break;
        case RENDER_POSITIONS:
            //Render positions
            return float4(position.rgb, 1);
            break;
        case RENDER_DEPTH:
            //Renders depth
            return float4(depth.r, depth.r, depth.r, 1);
            break;
    }
    
    //calculate angles and attenuation
    float3 lightVector = 0.0f;
    float attenuation = 1.0f;

    if (LightType.x == TYPE_POINT || LightType.x == TYPE_SPOT)
    {
        lightVector = LightPos.xyz - position.xyz;

        float ligthDist = length(lightVector);
        attenuation = clamp(1.0f - (ligthDist / LightIntensity), 0, 1.0f);
        
        if (LightType.x == TYPE_SPOT)
        {
            attenuation *= pow(max(dot(normalize(-lightVector), normalize(LightDir.xyz - LightPos.xyz)), 0.0f), LightFocus);
        }
    }
    else if (LightType.x == TYPE_DIRECTIONAL)
    {
        lightVector = (LightPos.xyz - LightDir.xyz);
    }

    //calculate angle between light source direction and normal 
    float lightFactor = clamp(dot(normalize(normal), normalize(lightVector)), 0.0f, 1.0f);

    //get ambient colour
    float4 ambientColour = float4(diffuseAlbedo * AmbientPower * 0.01f, 1.0f);

    //calculate diffuse lightning (no ligth/distance loss calculated here)
    float4 diffuseColour = float4(diffuseAlbedo * LightColour.rgb * lightFactor, 1.0f);

    //calculate vector for reflected light
    float3 lightReflectionVector = 2 * lightFactor * normalize(normal) - normalize(lightVector);
    //clamp so only positive dot product is acceptable
    float dotProduct = clamp(dot(normalize(CameraOrigin.xyz - position.xyz), normalize(lightReflectionVector)), 0.0f, 1.0f);
    //change diffuse albedo to specular albedo when added!
    float4 specularColour = float4(specularAlbedo.rgb * LightColour.rgb * pow(dotProduct, specularPower), 1);

    float4 finalColour = clamp(ambientColour + ((diffuseColour + specularColour) * attenuation), 0.0f, 1.0f);

    if (shadow(position))
    {
        finalColour = ambientColour; //return ambientColour;
    }
        

    if (LightType.x == TYPE_POINT)
    {
        /*for (int i = 0; i < 5; i++)
        {
            if (shadow(position, i))
            {
                return ambientColour;
            }
        }*/
    }
    
    //add all lightning effects for a final pixel colour and make sure it stays inside reasonable boundries
    finalColour = clamp((finalColour + glowEffect), 0.0f, 1.0f);
    return finalColour;
    //return glowEffect;
    //return clamp(ambientColour + ((diffuseColour /*+ specularColour*/) * attenuation) /*+ glowEffect*/, 0.0f, 1.0f);
}
