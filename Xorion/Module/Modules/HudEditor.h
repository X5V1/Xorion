#pragma once
#include "Module.h"
#include <vector>
#include "../../../SDK/Vec2.h"
#include "../../DrawUtils.h"

struct HudElement {
    std::string name;
    vec2_t pos;
    vec2_t size;
    bool enabled;
    bool draggable;
};

class HudEditor : public IModule {
private:
    std::vector<HudElement> elements;
    bool snapToGrid = true;
    float gridSize = 8.0f;
    bool showOutlines = true;
    HudElement* dragging = nullptr;
    vec2_t dragOffset;

public:
    HudEditor();
    ~HudEditor();

    virtual const char* getModuleName() override;
    virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;

    void registerElement(const std::string& name, const vec2_t& pos, const vec2_t& size);
    const std::vector<HudElement>& getElements() const { return elements; }
};
