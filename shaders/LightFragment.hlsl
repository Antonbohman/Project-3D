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
#define RENDER_NO_LIGHTS		8 //as default but without shadows,specular and lights

struct PS_IN
{
    float4 ScreenPos : SV_POSITION;
};

Texture2D NormalTexture : register(t0);
Texture2D DiffuseTexture : register(t1);
Texture2D SpecularTexture : register(t2);
Texture2D PositionTexture : register(t3);

//Texture2DArray ShadowMaps : register(t4);

SamplerState Sampling : register(s0);

cbuffer GS_CB_FLAGS : register(b0)
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
    int LightType;
    int AmbientPower;
    float4 LightPos;
    float4 LightDir;
    float4 LightColour;
    float LightIntensity;
    float LightFocus;
};

/*cbuffer PS_CB_WVP : register(b2)
{
    
};
*/

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
            //Renders depth (TODO)
            //return float4(depth.rgb, 1);
            break;
    }

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
    float lightFactor = clamp(dot(normal, normalize(lightVector)), 0.0f, 1.0f);

    float4 ambientColour = float4(diffuseAlbedo * AmbientPower * 0.01f, 1.0f);

    //calculate diffuse lightning (no ligth/distance loss calculated here)
    float4 diffuseColour = float4(diffuseAlbedo * LightColour.rgb * lightFactor, 1.0f);

    //calculate vector for reflected light
    float3 lightReflectionVector = 2 * lightFactor * normal - normalize(LightPos.xyz - position.xyz); //from slide
    
    //clamp so only positive dot product is acceptable
    float dotProduct = clamp(dot(normalize(CameraOrigin.xyz - position.xyz), normalize(lightReflectionVector)), 0.0f, 1.0f);
    //change diffuse albedo to specular albedo when added!
    float4 specularColour = float4(diffuseAlbedo.rgb * LightColour.rgb * pow(dotProduct, specularPower), 1);
    
    //add all lightning effects for a final pixel colour and make sure it stays inside reasonable boundries
    return clamp(ambientColour + ((diffuseColour /*+ specularColour*/) * attenuation), 0.0f, 1.0f);
}
