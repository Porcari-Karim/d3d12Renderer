
struct VertexInput
{
    float3 inPos : POSITION;
    float3 inColor : COLOR;
};

struct VertexOutput
{
    float3 color : COLOR;
    float4 position : SV_POSITION;
};


VertexOutput main(VertexInput vertexInput)
{
    VertexOutput output;
    output.position = float4(vertexInput.inPos, 1.0);
    output.color = vertexInput.inColor;
    return output;
}