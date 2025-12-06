#include "AutoAnchor.h"
#include "../../../SDK/ClientInstance.h"
#include "../../../Utils/Utils.h"
#include "../../DrawUtils.h"
#include "../../../SDK/GameMode.h"
#include "SDK/MinecraftUIRenderContext.h"

AutoAnchor::AutoAnchor()
    : IModule(VK_NUMPAD1, Category::COMBAT, "Auto-place and blow up Respawn Anchors.") {
    registerIntSetting("Range", &range, range, 1, 10);
    registerBoolSetting("Autoplace", &autoplace, autoplace);
    registerBoolSetting("Autocharge", &autocharge, autocharge);
    delay = 0;
    prevSlot = -1;
    FinishSelect = false;
}

AutoAnchor::~AutoAnchor() {}

const char* AutoAnchor::getModuleName() {
    return "AutoAnchor";
}

static bool isAnchorBlock(int id) { return id == 758; }   // Respawn Anchor
static bool isGlowstone(int id)   { return id == 89; }    // Glowstone block

void AutoAnchor::onEnable() {
    anchorList.clear();
    delay = 0;
    prevSlot = -1;
    FinishSelect = false;
}

void AutoAnchor::onTick(GameMode* gm) {
    auto player = Game.getLocalPlayer();
    if (!player || !gm) return;
    if (!player->getRegion() || !player->level) return;

    delay++;
    anchorList.clear();

    Vec3 playerPos = player->getPos();
    vec3_ti basePos(
        (int)floor(playerPos.x),
        (int)floor(playerPos.y),
        (int)floor(playerPos.z)
    );

    for (int x = -range; x <= range; x++) {
        for (int y = -2; y <= 2; y++) {
            for (int z = -range; z <= range; z++) {
                vec3_ti pos = basePos.add(x, y, z);
                Block* block = player->getRegion()->getBlock(Vec3i(pos.x, pos.y, pos.z));
                if (!block || !block->toLegacy()) continue;

                int id = block->toLegacy()->blockId;
                if (isAnchorBlock(id)) {
                    anchorList.push_back(pos);

                    // Autocharge logic
                    int charge = 0;
                    if (block->blockLegacy) {
                        auto state = block->blockLegacy->getBlockState("respawn_anchor_charge");
                        if (state) {
                            charge = state->getValue(block->getBlock());
                        }
                    }

                    if (autocharge && charge < 4) {
                        int needed = 4 - charge;
                        auto supplies = player->getSupplies();
                        if (!supplies) continue;
                        auto inv = supplies->inventory;
                        if (!inv) continue;

                        prevSlot = supplies->selectedHotbarSlot;
                        FinishSelect = true;

                        // Scan hotbar for glowstone
                        for (int slot = 0; slot < 9; slot++) {
                            ItemStack* stack = inv->getByGlobalIndex(slot);
                            if (stack && stack->item) {
                                int itemId = stack->getItem()->itemId;
                                if (isGlowstone(itemId)) {
                                    supplies->selectedHotbarSlot = slot;
                                    for (int i = 0; i < needed; i++) {
                                        Vec3i blockPos(pos.x, pos.y, pos.z);
                                        gm->useItemOn(stack, blockPos, (uint8_t)1, vec3_t(0.5f, 0.5f, 0.5f));
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Placement logic
    if (delay == 2 && autoplace && anchorList.empty()) {
        if (player->level->hitResult.type == HitResultType::Tile) {
            Vec3i blockPos = player->level->hitResult.blockPos;
            gm->buildBlock(&blockPos, (uint8_t)player->level->hitResult.facing, true);
        }
    }

    // Restore previous slot
    if (delay == 4 && FinishSelect) {
        auto supplies = player->getSupplies();
        if (supplies && prevSlot != -1) {
            supplies->selectedHotbarSlot = prevSlot;
        }
        FinishSelect = false;
    }

    // Reset cycle
    if (delay >= 5) {
        delay = 0;
    }
}

void AutoAnchor::onPreRender(MinecraftUIRenderContext* renderCtx) {
    for (auto& pos : anchorList) {
        vec3_t start(pos.x, pos.y, pos.z);
        vec3_t end(pos.x + 1, pos.y + 1, pos.z + 1);

        DrawUtils::setColor(.25f, .75f, .9f, 0.5f);
        DrawUtils::drawBox(start, end, 0.3f);
    }
}

void AutoAnchor::onDisable() {
    delay = 0;
    anchorList.clear();
    prevSlot = -1;
    FinishSelect = false;
}
