Texture2D<float4> Input : register(t0);
RWTexture2D<float4> Output : register(u0);

//Gaussian Filter coefficients
static const float filter[5][5] =
{
    0.0030, 0.0133, 0.0219, 0.0133, 0.0030,
    0.0133, 0.0596, 0.0983, 0.0596, 0.0133,
    0.0219, 0.0983, 0.1621, 0.0983, 0.0219,
    0.0133, 0.0596, 0.0983, 0.0596, 0.0133,
    0.0030, 0.0133, 0.0219, 0.0133, 0.0030
};

//Group size - WHAT DOES DIS DO?
#define size_x 32
#define size_y 32

//Declare one thread for each texel of the input texture.
[numthreads(size_x, size_y, 1)]

void CS_main(uint3 DTid : SV_DispatchThreadID)
{
    //Offsetting the location to the first sample location.
    int3 textureLocation = DTid - int3(2, 2, 0);

    //Colour
    float4 Colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

    //Bluring the pixel with the closest 25 texels
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            Colour += Input.Load(textureLocation + int3(x, y, 0)) * filter[x][y];
        }
    }
    Output[DTid.xy] = Colour;
}