struct GS_IN
{
    float4 Pos_W : POSITION;
    float4 Pos_H : SV_POSITION;
    float3 Color : COLOR;
    float2 UV : TEXCOORD0;
    float4 Pos_Rotated[3] : TEXCOORD1;
};

struct PS_INOUT
{
    float4 Pos_W : POSITION;
    float4 Pos_H : SV_POSITION;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
    float3 Norm : NORMAL;
    uint viewport : SV_ViewportArrayIndex;
};

cbuffer GS_CB_FLAGS : register(b0)
{
    bool SplitView;
    uint RenderMode;
};

cbuffer GS_CB_CAMERA : register(b1)
{
    float4 CameraOrigin;
    float4 CameraFocus;
};

[maxvertexcount(12)]
void GS_main(
	triangle GS_IN input[3],
	inout TriangleStream<PS_INOUT> OutputStream)
{
    PS_INOUT output = (PS_INOUT) 0;

    float3 cameraVector = input[0].Pos_W.xyz - CameraOrigin.xyz/*(0, 1, -15)*/; //proof of backface culling.

    //calculate a normal based on the world position
    float3 v0 = input[1].Pos_W.xyz - input[0].Pos_W.xyz;
    float3 v1 = input[2].Pos_W.xyz - input[0].Pos_W.xyz;
    float3 norm = normalize(cross(v0, v1));
    
    if (dot(cameraVector, norm) < 0)
    {
        for (uint j = 0; j < 3; j++)
        {
            //set world postion, homogeneous position, colour(deprecated), uv and norm for outputed vertex
            output.Pos_W = input[j].Pos_W;
            output.Pos_H = input[j].Pos_H;
            output.Color = input[j].Color;
            output.UV = input[j].UV;
            output.Norm = norm;
            output.viewport = 0;
		
                //add vertex to be computed in next pipeline stage
            OutputStream.Append(output);
        }

        if (SplitView)
        {
            for (uint i = 0; i < 3; i++)
            {
                OutputStream.RestartStrip();

                for (uint j = 0; j < 3; j++)
                {
                    //set world postion, homogeneous position, colour(deprecated), uv and norm for outputed vertex
                    output.Pos_W = input[j].Pos_W;
                    output.Pos_H = input[j].Pos_Rotated[i];
                    output.Color = input[j].Color;
                    output.UV = input[j].UV;
                    output.Norm = norm;
                    output.viewport = i + 1;
		
                    //add vertex to be computed in next pipeline stage
                    OutputStream.Append(output);
                }
            }
        }
    }    
}