#pragma once
#include <d3d11.h>

// Global shader handles for the unlit tint pipeline
extern ID3D11VertexShader* g_UnlitVS;
extern ID3D11PixelShader*  g_UnlitPS;
extern ID3D11Buffer*       g_TintColorCB;
