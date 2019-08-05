#define TYPE_POINT 0
#define TYPE_SPOT 1
#define TYPE_DIRECTIONAL 2

struct PS_INOUT
{
    float4 Pos : SV_POSITION;
    uint targetIndex : SV_RenderTargetArrayIndex;
};

cbuffer GS_CB_Light : register(b0)
{
    uint LightType;
    float3 LightOrigin;
    float4x4 ViewProjection;
    float4x4 RotatedViewProjection[5];
};

[maxvertexcount(18)]
void GS_shadow(
	triangle float4 input[3] : SV_Position,
	inout TriangleStream<PS_INOUT> OutputStream)
{
    PS_INOUT output = (PS_INOUT) 0;

    float3 cameraVector = input[0].xyz - LightOrigin.xyz/*(0, 1, -15)*/; //proof of backface culling.

    //calculate a normal based on the world position
    float3 v0 = input[1].xyz - input[0].xyz;
    float3 v1 = input[2].xyz - input[0].xyz;
    float3 norm = normalize(cross(v0, v1));
    
    if (dot(cameraVector, norm) < 0)
    {
        //output our original vertices
        for (uint j = 0; j < 3; j++)
        {
            output.Pos = mul(input[j], ViewProjection);
            output.targetIndex = 0;
		
            //add vertex to be computed in next pipeline stage
            OutputStream.Append(output);
        }

        OutputStream.RestartStrip();

        if (LightType == TYPE_POINT)
        {
            for (uint i = 0; i < 5; i++)
            {
                //output our rotated maps
                for (uint j = 0; j < 3; j++)
                {
                    output.Pos = mul(input[j], RotatedViewProjection[i]);
                    output.targetIndex = i+1;
		
                    //add vertex to be computed in next pipeline stage
                    OutputStream.Append(output);
                }

                OutputStream.RestartStrip();
            }
        }
    }
}