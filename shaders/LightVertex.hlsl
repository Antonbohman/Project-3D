struct VS_IN
{
    float2 Pos : POSITION;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
};

/*cbuffer VS_CB_SPACE : register(b0)
{
    float4x4 WorldViewProjection;
};*/

VS_OUT VS_light(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
    //copy values from input to output
    output.Pos = float4(input.Pos, 0.0f, 1.0f);
    
    return output;
}