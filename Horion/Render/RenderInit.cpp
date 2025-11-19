#include "RenderGlobals.h"
#include <d3dcompiler.h>
#include <wrl/client.h> // For Microsoft::WRL::ComPtr

// Initializes the unlit tint shaders and constant buffer.
// Call this once after you have a valid ID3D11Device*.
bool RenderInit(ID3D11Device* dev) {
    if (!dev) return false;

    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;

    // Load compiled shader objects (.cso files) from your Shaders folder
    if (FAILED(D3DReadFileToBlob(L"Shaders/UnlitTintVS.cso", &vsBlob))) {
        return false;
    }
    if (FAILED(D3DReadFileToBlob(L"Shaders/UnlitTintPS.cso", &psBlob))) {
        return false;
    }

    // Create vertex shader
    if (FAILED(dev->CreateVertexShader(vsBlob->GetBufferPointer(),
                                       vsBlob->GetBufferSize(),
                                       nullptr,
                                       &g_UnlitVS))) {
        return false;
    }

    // Create pixel shader
    if (FAILED(dev->CreatePixelShader(psBlob->GetBufferPointer(),
                                      psBlob->GetBufferSize(),
                                      nullptr,
                                      &g_UnlitPS))) {
        return false;
    }

    // Create constant buffer for tint color (RGBA float4)
    D3D11_BUFFER_DESC cbd = {};
    cbd.ByteWidth = sizeof(float) * 4; // enough for a float4
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = 0;
    cbd.MiscFlags = 0;
    cbd.StructureByteStride = 0;

    if (FAILED(dev->CreateBuffer(&cbd, nullptr, &g_TintColorCB))) {
        return false;
    }

    return true;
}
