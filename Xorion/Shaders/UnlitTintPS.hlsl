// UnlitTintPS.hlsl
cbuffer TintColorBuffer : register(b1)
{
    float4 uTintColor; // RGBA tint color
};

struct PSInput {
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
    // Pure flat tint, ignore lighting/textures
    return uTintColor;
}
