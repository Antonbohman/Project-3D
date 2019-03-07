struct GS_IN
{
    float4 Pos_W : POSITION;
    float4 Pos_H : SV_POSITION;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct PS_INOUT
{
    float4 Pos_W : POSITION;
    float4 Pos_H : SV_POSITION;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
    float3 Norm : NORMAL;
};

cbuffer VS_CB_CAMERA : register(b0)
{
    float4 CameraOrigin;
    float4 CameraFocus;
};

[maxvertexcount(9)]
void GS_main(
	triangle GS_IN input[3],
	inout TriangleStream<PS_INOUT> OutputStream)
{
    PS_INOUT output = (PS_INOUT) 0;

    float3 cameraVector = input[0].Pos_W.xyz - CameraOrigin.xyz/*(0, 1, -15)*/;//proof of backface culling.

    //calculate a normal based on the world position
    float3 v0 = input[1].Pos_W.xyz - input[0].Pos_W.xyz;
    float3 v1 = input[2].Pos_W.xyz - input[0].Pos_W.xyz;
    float3 norm = normalize(cross(v0, v1));
    
    if (dot(cameraVector, norm) < 0) 
    {
        //output our original vertices
        for (uint j = 0; j < 3; j++)
        {
            //set world postion, homogeneous position, colour(deprecated), uv and norm for outputed vertex
            output.Pos_W = input[j].Pos_W;
            output.Pos_H = input[j].Pos_H;
            output.Color = input[j].Color;
            output.UV = input[j].UV;
            output.Norm = norm;
		
            //add vertex to be computed in next pipeline stage
            OutputStream.Append(output);
        }
    }    
}