// UnlitTintVS.hlsl
cbuffer TransformBuffer : register(b0)
{
    float4x4 uWorldViewProj; // combined world-view-projection matrix
};

struct VSInput {
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float2 texcoord : TEXCOORD0;
};

struct VSOutput {
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = mul(float4(input.position, 1.0f), uWorldViewProj);
    output.texcoord = input.texcoord;
    return output;
}
