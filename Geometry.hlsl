struct GS_IN
{
    float4 Pos : SV_POSITION;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct PS_INOUT {
    float4 Pos_W : POSITION;
    float4 Pos_H : SV_POSITION;
	float3 Color : COLOR;
    float2 UV : TEXCOORD;
    float3 Norm : NORMAL;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
};

[maxvertexcount(9)]
void main(
	triangle GS_IN input[3],
	inout TriangleStream<PS_INOUT> OutputStream)
{
    PS_INOUT output;

    float4x4 ViewProjection = mul(View,Projection);
	
    //rotate each vertex position accordingly
    for (uint i = 0; i < 3; i++) {
        input[i].Pos = mul(input[i].Pos, World);
    }

    //calculate a normal based on the new rotated direction
    float3 v0 = input[1].Pos.xyz - input[0].Pos.xyz;
    float3 v1 = input[2].Pos.xyz - input[0].Pos.xyz;
    float3 norm = normalize(cross(v0, v1));
    
    //output our original vertices
	for (uint j = 0; j < 3; j++) {
        //set world postion, homogeneous position, colour(deprecated), uv and norm for outputed vertex
        output.Pos_W = input[j].Pos;
        output.Pos_H = mul(input[j].Pos, ViewProjection);
        output.Color = input[j].Color;
        output.UV = input[j].UV;
        output.Norm = norm;
		
        //add vertex to be computed in next pipeline stage
		OutputStream.Append(output);
	}

    //update our triangle to be rendered seperatly
	OutputStream.RestartStrip();

    //add the normal to our rotated vertex position to create a copy of each vertex but closer to the camera
    //can be added with negative normal to get a copy further away instead
	for (uint k = 0; k < 3; k++) {
        //set world postion, homogeneous position, colour(deprecated), uv and norm for outputed vertex
        output.Pos_W = float4(input[k].Pos.xyz + norm, 1);
        output.Pos_H = mul(float4(input[k].Pos.xyz + norm, 1), ViewProjection);
        output.Color = input[k].Color;
        output.UV = input[k].UV;
        output.Norm = norm;
		
        //add vertex to be computed in next pipeline stage
        OutputStream.Append(output);
    }
}