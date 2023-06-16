
struct PixelInput
{
    float3 inColor : COLOR;
};

struct PixelOutput
{
    float4 attachement0 : SV_Target0;
};

PixelOutput main(PixelInput pixelInput)
{
    PixelOutput output;
    output.attachement0 = float4(pixelInput.inColor, 1.0);
    return output;
}

