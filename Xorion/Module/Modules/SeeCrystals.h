#pragma once
#include "Module.h"

class SeeCrystals : public Module {
public:
    SeeCrystals();

    void onEnable() override;
    void onDisable() override;
    void onLevelRender() override; // called each frame during world rendering

private:
    // RGBA for #7418D6
    float m_color[4] = { 0x74 / 255.0f, 0x18 / 255.0f, 0xD6 / 255.0f, 1.0f };
    bool  m_throughWalls = true;

    void renderCrystalNormal(class Entity* ent, ID3D11Device* dev, ID3D11DeviceContext* ctx);
    void renderCrystalOverlay(class Entity* ent, ID3D11Device* dev, ID3D11DeviceContext* ctx);

    void rebuildCrystalCache();
    std::vector<class Entity*> m_crystals;
};
