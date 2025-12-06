#pragma once
#include <d3d11.h>

namespace RenderUtils {

    enum class BlendMode { None, Alpha };

    struct StateGuard {
        ID3D11DepthStencilState* oldDepthState = nullptr;
        ID3D11BlendState*        oldBlendState = nullptr;
        ID3D11RasterizerState*   oldRasterState = nullptr;
    };

    bool beginState(StateGuard& out, ID3D11DeviceContext* ctx);
    void endState(StateGuard& guard, ID3D11DeviceContext* ctx);

    void setDepthTest(bool enabled, ID3D11Device* dev, ID3D11DeviceContext* ctx);
    void setDepthWrite(bool enabled, ID3D11Device* dev, ID3D11DeviceContext* ctx);
    void setBlendMode(BlendMode mode, ID3D11Device* dev, ID3D11DeviceContext* ctx);

    void bindUnlitTintShader(ID3D11DeviceContext* ctx);
    void setTintColor(ID3D11DeviceContext* ctx, const float rgba[4]);

    void drawEntityMesh(struct Entity* ent, ID3D11DeviceContext* ctx);
}
