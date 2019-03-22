Texture2D<float4> Input : register(t0);
RWTexture2D<float4> Output : register(u0);

//Gaussian Filter coefficients
static const float filter[7][7] =
{
    0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067,
    0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292,
    0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117,
    0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771,
    0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117,
    0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292,
    0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067
};

//Group size - WHAT DOES DIS DO?
#define size_x 32
#define size_y 32

//Declare one thread for each texel of the input texture.
[numthreads(size_x, size_y, 1)]

void CS_main(uint3 DTid : SV_DispatchThreadID)
{
    //Offsetting the location to the first sample location.
    int3 textureLocation = DTid - int3(3, 3, 0);

    //Colour
    float4 Colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

    //Bluring the pixel with the closest 25 texels
    for (int x = 0; x < 7; x++)
    {
        for (int y = 0; y < 7; y++)
        {
            Colour += Input.Load(textureLocation + int3(x, y, 0)) * filter[x][y];
        }
    }
    Output[DTid.xy] = Colour;
}