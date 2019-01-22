struct VS_IN
{
	float3 Pos : POSITION;
	float3 Color : COLOR;
    float2 UV : UV_POS;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
    float2 UV : TEXCOORD;
};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

    //copy values from input to output
	output.Pos = float4(input.Pos, 1);
	output.Color = input.Color;
    output.UV = input.UV;
    
	return output;
}