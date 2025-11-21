#include "BedAura.h"

#include "../../Memory/GameData.h"
#include "../../SDK/GameMode.h"
#include "../../SDK/ClientInstance.h"
#include "../../SDK/Entity.h"
#include "../../SDK/Inventory.h"
#include "../../SDK/Item.h"
#include "../../SDK/BlockLegacy.h"
#include "../../SDK/MinecraftUIRenderContext.h"
#include "../../Utils/DrawUtils.h"
#include "../../Utils/Logger.h"

static inline bool isBedBlockLegacy(BlockLegacy* legacy) {
    if (!legacy) return false;
    // Bed blocks share "bed" in their legacy name across colors
    auto name = legacy->getName().getText();
    return name.find("bed") != std::string::npos;
}

static inline bool isBedItem(Item* item) {
    if (!item) return false;
    auto name = item->name.getText();
    return name.find("bed") != std::string::npos;
}

BedAura::BedAura()
    : IModule(VK_NUMPAD2, Category::COMBAT, "Auto place and detonate beds in Nether/End.") {
    // Settings
    registerIntSetting("Range", &range, range, 2, 10);
    registerBoolSetting("Autoplace", &autoplace, autoplace);
    registerBoolSetting("AutoDetonateNearby", &autoDetonateNearby, autoDetonateNearby);
    registerBoolSetting("DimensionGuard", &dimensionGuard, dimensionGuard);
    registerIntSetting("DelayTicks", &cycleDelay, cycleDelay, 2, 20);

    // Visuals
    registerBoolSetting("RenderBeds", &renderBeds, renderBeds);
    registerFloatSetting("RenderAlpha", &renderAlpha, renderAlpha, 0.1f, 1.0f);

    // State
    delay = 0;
    prevSlot = -1;
    restoreSlotPending = false;
    slotRestoreCountdown = 0;
}

BedAura::~BedAura() {}

const char* BedAura::getModuleName() {
    return "BedAura";
}

void BedAura::onEnable() {
    delay = 0;
    prevSlot = -1;
    restoreSlotPending = false;
    slotRestoreCountdown = 0;
    bedPositions.clear();
}

void BedAura::onDisable() {
    delay = 0;
    if (restoreSlotPending) {
        auto lp = Game.getLocalPlayer();
        if (lp) {
            auto supplies = lp->getSupplies();
            if (supplies && prevSlot >= 0) {
                supplies->selectedHotbarSlot = prevSlot;
            }
        }
    }
    prevSlot = -1;
    restoreSlotPending = false;
    slotRestoreCountdown = 0;
    bedPositions.clear();
}

void BedAura::onTick(GameMode* gm) {
    auto lp = Game.getLocalPlayer();
    if (!lp || !gm) return;
    if (!lp->region || !lp->level) return;

    // Dimension guard: only act in Nether/End
    int dimId = lp->level->dimension; // confirm actual field in your SDK
    if (dimensionGuard && dimId == 0) {
        bedPositions.clear();
        return;
    }

    delay++;
    bedPositions.clear();

    const vec3_t playerPos = *lp->getPos();
    vec3_ti basePos((int)floor(playerPos.x), (int)floor(playerPos.y), (int)floor(playerPos.z));

    // 1) Scan for nearby beds and detonate them
    if (autoDetonateNearby) {
        for (int x = -range; x <= range; x++) {
            for (int y = -2; y <= 2; y++) {
                for (int z = -range; z <= range; z++) {
                    vec3_ti pos = basePos.add(x, y, z);
                    auto block = lp->region->getBlock(pos);
                    if (!block) continue;

                    auto legacy = block->toLegacy();
                    if (!legacy) continue;

                    if (isBedBlockLegacy(legacy)) {
                        bedPositions.push_back(pos);
                    }
                }
            }
        }

        for (auto& bpos : bedPositions) {
            Vec3i blockPos(bpos.x, bpos.y, bpos.z);
            uint8_t facing = 1;
            vec3_t hit(0.5f, 0.5f, 0.5f);

            auto supplies = lp->getSupplies();
            ItemStack* selected = nullptr;
            if (supplies && supplies->inventory) {
                selected = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
            }

            gm->useItemOn(selected, blockPos, facing, hit);
        }
    }

    // 2) Autoplace a bed and detonate it
    if (autoplace && delay % cycleDelay == 0) {
        auto supplies = lp->getSupplies();
        if (!supplies || !supplies->inventory) return;

        prevSlot = supplies->selectedHotbarSlot;
        restoreSlotPending = true;

        int bedSlot = -1;
        for (int slot = 0; slot < 9; slot++) {
            auto stack = supplies->inventory->getItemStack(slot);
            if (!stack || !stack->item) continue;
            if (isBedItem(stack->getItem())) {
                bedSlot = slot;
                break;
            }
        }

        if (bedSlot != -1) {
            supplies->selectedHotbarSlot = bedSlot;

            vec3_t forward = lp->viewAngles.toDirection(); // ensure you have this helper
            vec3_t placePosF = playerPos.add(forward).floor();
            Vec3i placePos((int)placePosF.x, (int)placePosF.y, (int)placePosF.z);

            uint8_t face = 1;
            if (lp->level->hitResult.type == 0) {
                face = (uint8_t)lp->level->hitResult.facing;
            }

            gm->buildBlock(&placePos, face, true);

            vec3_t hit(0.5f, 0.5f, 0.5f);
            auto stack = supplies->inventory->getItemStack(bedSlot);
            gm->useItemOn(stack, placePos, face, hit);
        }

        slotRestoreCountdown = 2;
    }

    // 3) Restore previous slot
    if (restoreSlotPending) {
        if (slotRestoreCountdown > 0) {
            slotRestoreCountdown--;
        } else {
            auto supplies = lp->getSupplies();
            if (supplies && prevSlot >= 0) {
                supplies->selectedHotbarSlot = prevSlot;
            }
            restoreSlotPending = false;
            prevSlot = -1;
        }
    }
}

void BedAura::onPreRender(MinecraftUIRenderContext* ctx) {
    if (!renderBeds || bedPositions.empty()) return;

    DrawUtils::setColor(0.9f, 0.3f, 0.3f, renderAlpha);
    for (auto& p : bedPositions) {
        vec3_t start((float)p.x, (float)p.y, (float)p.z);
        vec3_t end(start.x + 1.f, start.y + 1.f, start.z + 1.f);
        DrawUtils::drawBox(start, end, 0.4f);
    }
}
