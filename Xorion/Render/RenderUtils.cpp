#include "RenderUtils.h"
#include "../SDK/Entity.h"

// External shader handles (replace with your actual shader objects)
extern ID3D11VertexShader* g_UnlitVS;
extern ID3D11PixelShader*  g_UnlitPS;
extern ID3D11Buffer*       g_TintColorCB; // constant buffer for tint color

namespace RenderUtils {

    bool beginState(StateGuard& out, ID3D11DeviceContext* ctx) {
        if (!ctx) return false;
        ctx->OMGetDepthStencilState(&out.oldDepthState, nullptr);
        ctx->OMGetBlendState(&out.oldBlendState, nullptr, nullptr);
        ctx->RSGetState(&out.oldRasterState);
        return true;
    }

    void endState(StateGuard& guard, ID3D11DeviceContext* ctx) {
        if (!ctx) return;
        if (guard.oldDepthState) ctx->OMSetDepthStencilState(guard.oldDepthState, 0);
        if (guard.oldBlendState) ctx->OMSetBlendState(guard.oldBlendState, nullptr, 0xFFFFFFFF);
        if (guard.oldRasterState) ctx->RSSetState(guard.oldRasterState);

        if (guard.oldDepthState) guard.oldDepthState->Release();
        if (guard.oldBlendState) guard.oldBlendState->Release();
        if (guard.oldRasterState) guard.oldRasterState->Release();
    }

    void setDepthTest(bool enabled, ID3D11Device* dev, ID3D11DeviceContext* ctx) {
        D3D11_DEPTH_STENCIL_DESC desc = {};
        desc.DepthEnable = enabled;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_LESS;
        desc.StencilEnable = FALSE;

        ID3D11DepthStencilState* state = nullptr;
        dev->CreateDepthStencilState(&desc, &state);
        ctx->OMSetDepthStencilState(state, 0);
        if (state) state->Release();
    }

    void setDepthWrite(bool enabled, ID3D11Device* dev, ID3D11DeviceContext* ctx) {
        D3D11_DEPTH_STENCIL_DESC desc = {};
        desc.DepthEnable = TRUE;
        desc.DepthWriteMask = enabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
        desc.DepthFunc = D3D11_COMPARISON_LESS;
        desc.StencilEnable = FALSE;

        ID3D11DepthStencilState* state = nullptr;
        dev->CreateDepthStencilState(&desc, &state);
        ctx->OMSetDepthStencilState(state, 0);
        if (state) state->Release();
    }

    void setBlendMode(BlendMode mode, ID3D11Device* dev, ID3D11DeviceContext* ctx) {
        D3D11_BLEND_DESC desc = {};
        desc.AlphaToCoverageEnable = FALSE;
        desc.IndependentBlendEnable = FALSE;

        auto& rt = desc.RenderTarget[0];
        rt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        if (mode == BlendMode::Alpha) {
            rt.BlendEnable = TRUE;
            rt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
            rt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            rt.BlendOp = D3D11_BLEND_OP_ADD;
            rt.SrcBlendAlpha = D3D11_BLEND_ONE;
            rt.DestBlendAlpha = D3D11_BLEND_ZERO;
            rt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        } else {
            rt.BlendEnable = FALSE;
        }

        ID3D11BlendState* state = nullptr;
        dev->CreateBlendState(&desc, &state);
        ctx->OMSetBlendState(state, nullptr, 0xFFFFFFFF);
        if (state) state->Release();
    }

    void bindUnlitTintShader(ID3D11DeviceContext* ctx) {
        if (!ctx) return;
        ctx->VSSetShader(g_UnlitVS, nullptr, 0);
        ctx->PSSetShader(g_UnlitPS, nullptr, 0);
    }

    void setTintColor(ID3D11DeviceContext* ctx, const float rgba[4]) {
        if (!ctx || !g_TintColorCB) return;
        ctx->UpdateSubresource(g_TintColorCB, 0, nullptr, rgba, 0, 0);
        ctx->PSSetConstantBuffers(0, 1, &g_TintColorCB);
    }

    void drawEntityMesh(Entity* ent, ID3D11DeviceContext* ctx) {
        // TODO: Implement entity mesh rendering with new API
        // if (!ent || !ctx) return;
        // ent->renderMesh(ctx);
    }
}
