#include "ModuleManager.h"
#include "../../Utils/Logger.h"
#include "../../Utils/Json.hpp"

// Always-on cosmetic: Xorion-style nametag for local player
#include "../SDK/XorionNameTag.h"

using json = nlohmann::json;

ModuleManager::ModuleManager(GameData* gameData) {
    this->gameData = gameData;
}

ModuleManager::~ModuleManager() {
    initialized = false;
    auto lock = lockModuleListExclusive();
    moduleList.clear();
}

void ModuleManager::initModules() {
    logF("Initializing modules.");
    {
        auto lock = lockModuleListExclusive();

        // Register all modules (stock + customs)
        moduleList.emplace_back(std::make_shared<Aimbot>());
        moduleList.emplace_back(std::make_shared<AirJump>());
        moduleList.emplace_back(std::make_shared<AirSwim>());
        moduleList.emplace_back(std::make_shared<AntiBot>());
        moduleList.emplace_back(std::make_shared<AntiCheatBlocker>());  // Always-on anti-cheat protection
        moduleList.emplace_back(std::make_shared<AntiCheatDelete>());   // Aggressive anti-cheat deletion (cascades to AntiCheatBlocker)
        moduleList.emplace_back(std::make_shared<AntiCrystal>());
        moduleList.emplace_back(std::make_shared<AntiEffect>());
        moduleList.emplace_back(std::make_shared<AntiImmobile>());
        moduleList.emplace_back(std::make_shared<AntiInvis>());
        moduleList.emplace_back(std::make_shared<AntiVoid>());
        moduleList.emplace_back(std::make_shared<Arraylist>());
        moduleList.emplace_back(std::make_shared<AutoArmor>());
        moduleList.emplace_back(std::make_shared<AutoClicker>());
        moduleList.emplace_back(std::make_shared<AutoGapple>());
        moduleList.emplace_back(std::make_shared<AutoSneak>());
        moduleList.emplace_back(std::make_shared<AutoSplitter>());
        moduleList.emplace_back(std::make_shared<AutoSprint>());
        moduleList.emplace_back(std::make_shared<AutoTotem>());
        moduleList.emplace_back(std::make_shared<BedAura>());     // custom (baked into list)
        moduleList.emplace_back(std::make_shared<Blink>());
        moduleList.emplace_back(std::make_shared<BlockReach>());
        moduleList.emplace_back(std::make_shared<Bhop>());
        moduleList.emplace_back(std::make_shared<BowAimbot>());
        moduleList.emplace_back(std::make_shared<Breadcrumbs>());
        moduleList.emplace_back(std::make_shared<Breaker>());
        moduleList.emplace_back(std::make_shared<ChestAura>());
        moduleList.emplace_back(std::make_shared<ChestStealer>());
        moduleList.emplace_back(std::make_shared<ClickGuiMod>());
        moduleList.emplace_back(std::make_shared<ClientTheme>());
        moduleList.emplace_back(std::make_shared<Compass>());
        moduleList.emplace_back(std::make_shared<Crasher>());
        moduleList.emplace_back(std::make_shared<Criticals>());
        moduleList.emplace_back(std::make_shared<CrystalAura>());
        moduleList.emplace_back(std::make_shared<Derp>());
        moduleList.emplace_back(std::make_shared<Dicker>());      // custom
        moduleList.emplace_back(std::make_shared<DVDLogo>());
        moduleList.emplace_back(std::make_shared<EditionFaker>());
        moduleList.emplace_back(std::make_shared<ESP>());
        moduleList.emplace_back(std::make_shared<FeetPlace>());      // custom
        moduleList.emplace_back(std::make_shared<Fly>());
        moduleList.emplace_back(std::make_shared<FollowPathModule>());
        moduleList.emplace_back(std::make_shared<FontChanger>());
        moduleList.emplace_back(std::make_shared<Freecam>());
        moduleList.emplace_back(std::make_shared<Freelook>());
        moduleList.emplace_back(std::make_shared<FullBright>());
        moduleList.emplace_back(std::make_shared<Fucker>());
        moduleList.emplace_back(std::make_shared<Glide>());
        moduleList.emplace_back(std::make_shared<HighJump>());
        moduleList.emplace_back(std::make_shared<Hitbox>());
        moduleList.emplace_back(std::make_shared<HudEditor>());
        moduleList.emplace_back(std::make_shared<HudModule>());
        moduleList.emplace_back(std::make_shared<InfiniteTreasureAura>());
        moduleList.emplace_back(std::make_shared<InstaBreak>());
        moduleList.emplace_back(std::make_shared<InventoryCleaner>());
        moduleList.emplace_back(std::make_shared<InventoryMove>());
        moduleList.emplace_back(std::make_shared<InventoryViewer>());
        moduleList.emplace_back(std::make_shared<Impersonator>());
        moduleList.emplace_back(std::make_shared<Jesus>());
        moduleList.emplace_back(std::make_shared<Killaura>());
        moduleList.emplace_back(std::make_shared<LigmaNuts>());   // custom
        moduleList.emplace_back(std::make_shared<MaceHit>());   // custom
        moduleList.emplace_back(std::make_shared<MidClick>());
        moduleList.emplace_back(std::make_shared<NameTags>());
        moduleList.emplace_back(std::make_shared<Nbt>());
        moduleList.emplace_back(std::make_shared<NightMode>());
        moduleList.emplace_back(std::make_shared<NoClip>());
        moduleList.emplace_back(std::make_shared<NoFall>());
        moduleList.emplace_back(std::make_shared<NoFriends>());
        moduleList.emplace_back(std::make_shared<NoHurtcam>());
        moduleList.emplace_back(std::make_shared<NoPacket>());
        moduleList.emplace_back(std::make_shared<NoSlowDown>());
        moduleList.emplace_back(std::make_shared<NoSwing>());
        moduleList.emplace_back(std::make_shared<NoWeb>());
        moduleList.emplace_back(std::make_shared<Nuker>());
        moduleList.emplace_back(std::make_shared<OffhandAllow>());
        moduleList.emplace_back(std::make_shared<Phase>());
        moduleList.emplace_back(std::make_shared<Radar>());
        moduleList.emplace_back(std::make_shared<RainbowSky>());
        moduleList.emplace_back(std::make_shared<Reach>());
        moduleList.emplace_back(std::make_shared<ReachDisplay>());
        moduleList.emplace_back(std::make_shared<Reducer>());
        moduleList.emplace_back(std::make_shared<Scaffold>());
        moduleList.emplace_back(std::make_shared<SeeCrystals>());   // custom
        moduleList.emplace_back(std::make_shared<Spammer>());
        moduleList.emplace_back(std::make_shared<Speed>());
        moduleList.emplace_back(std::make_shared<Spider>());
        moduleList.emplace_back(std::make_shared<StackableItem>());
        moduleList.emplace_back(std::make_shared<Step>());
        moduleList.emplace_back(std::make_shared<StorageESP>());
        moduleList.emplace_back(std::make_shared<TargetHUD>());
        moduleList.emplace_back(std::make_shared<Teams>());
        moduleList.emplace_back(std::make_shared<Teleport>());
        moduleList.emplace_back(std::make_shared<Timer>());
        moduleList.emplace_back(std::make_shared<Tower>());
        moduleList.emplace_back(std::make_shared<Tracer>());
        moduleList.emplace_back(std::make_shared<TriggerBot>());
        moduleList.emplace_back(std::make_shared<Twerk>());
        moduleList.emplace_back(std::make_shared<Unbanner>());    // custom (baked into list)
        moduleList.emplace_back(std::make_shared<Velocity>());
        moduleList.emplace_back(std::make_shared<ViewModel>());
        moduleList.emplace_back(std::make_shared<Watermark>());
        moduleList.emplace_back(std::make_shared<Waypoints>());
        moduleList.emplace_back(std::make_shared<Xray>());
        moduleList.emplace_back(std::make_shared<Zoom>());

#ifdef _DEV
        moduleList.emplace_back(std::make_shared<PacketLogger>());
        moduleList.emplace_back(std::make_shared<VanillaPlus>());
        moduleList.emplace_back(std::make_shared<TestModule>());
#endif

        // Sort modules alphabetically by reported name
        std::sort(moduleList.begin(), moduleList.end(),
                  [](const std::shared_ptr<IModule>& lhs, const std::shared_ptr<IModule>& rhs) {
                      return std::string{ *lhs->getModuleName() } < std::string{ *rhs->getModuleName() };
                  });

        initialized = true;
    }

    // Default states after init
    if (auto cg = getModule<ClickGuiMod>()) cg->setEnabled(false);
    if (auto hm = getModule<HudModule>())  hm->setEnabled(true);
    if (auto ab = getModule<AntiBot>())    ab->setEnabled(true);
    if (auto acb = getModule<AntiCheatBlocker>()) acb->setEnabled(true);  // Always-on by default
    if (auto acd = getModule<AntiCheatDelete>()) acd->setEnabled(true);   // Always-on by default
}

void ModuleManager::disable() {
    auto lock = lockModuleList();
    for (auto& mod : moduleList) {
        if (mod->isEnabled())
            mod->setEnabled(false);
    }
}

void ModuleManager::onLoadConfig(void* confVoid) {
    auto conf = reinterpret_cast<json*>(confVoid);
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& mod : moduleList) {
        mod->onLoadConfig(conf);
    }

    // Reassert default states
    if (auto cg = getModule<ClickGuiMod>()) cg->setEnabled(false);
    if (auto hm = getModule<HudModule>())  hm->setEnabled(true);
    if (auto ab = getModule<AntiBot>())    ab->setEnabled(true);
    if (auto acb = getModule<AntiCheatBlocker>()) acb->setEnabled(true);  // Always-on by default
    if (auto acd = getModule<AntiCheatDelete>()) acd->setEnabled(true);   // Always-on by default
}

void ModuleManager::onSaveConfig(void* confVoid) {
    auto conf = reinterpret_cast<json*>(confVoid);
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& mod : moduleList) {
        mod->onSaveConfig(conf);
    }
}

void ModuleManager::onWorldTick(GameMode* gameMode) {
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& mod : moduleList) {
        if (mod->isEnabled() || mod->callWhenDisabled())
            mod->onWorldTick(gameMode);
    }
}

void ModuleManager::onTick(GameMode* gameMode) {
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& mod : moduleList) {
        if (mod->isEnabled() || mod->callWhenDisabled())
            mod->onTick(gameMode);
    }
}

void ModuleManager::onAttack(Entity* attackEnt) {
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& mod : moduleList) {
        if (mod->isEnabled() || mod->callWhenDisabled())
            mod->onAttack(attackEnt);
    }
}

void ModuleManager::onKeyUpdate(int key, bool isDown) {
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& mod : moduleList) {
        mod->onKeyUpdate(key, isDown);
    }
}

void ModuleManager::onKey(int key, bool isDown, bool& shouldCancel) {
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& mod : moduleList) {
        if (mod->isEnabled() || mod->callWhenDisabled())
            mod->onKey(key, isDown, shouldCancel);
    }
}

void ModuleManager::onPreRender(MinecraftUIRenderContext* renderCtx) {
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& mod : moduleList) {
        if (mod->isEnabled() || mod->callWhenDisabled())
            mod->onPreRender(renderCtx);
    }
}

void ModuleManager::onPostRender(MinecraftUIRenderContext* renderCtx) {
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& mod : moduleList) {
        if (mod->isEnabled() || mod->callWhenDisabled())
            mod->onPostRender(renderCtx);
    }
}

void ModuleManager::onSendClientPacket(Packet* packet) {
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& mod : moduleList) {
        if (mod->isEnabled() || mod->callWhenDisabled())
            mod->onSendClientPacket(packet);
    }
}

void ModuleManager::onSendPacket(Packet* packet) {
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& mod : moduleList) {
        if (mod->isEnabled() || mod->callWhenDisabled())
            mod->onSendPacket(packet);
    }
}

void ModuleManager::onBaseTick(Entity* ent) {
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& mod : moduleList) {
        mod->onBaseTick(ent);
    }
}

std::vector<std::shared_ptr<IModule>>* ModuleManager::getModuleList() {
    return &moduleList;
}

int ModuleManager::getModuleCount() {
    return static_cast<int>(moduleList.size());
}

int ModuleManager::getEnabledModuleCount() {
    int i = 0;
    auto lock = lockModuleList();
    for (auto& it : moduleList) {
        if (it->isEnabled())
            i++;
    }
    return i;
}

void ModuleManager::onMove(MoveInputHandler* hand) {
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& it : moduleList) {
        if (it->isEnabled() || it->callWhenDisabled())
            it->onMove(hand);
    }
}

void ModuleManager::onPlayerTick(Player* player) {
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& it : moduleList) {
        if (it->isEnabled() || it->callWhenDisabled())
            it->onPlayerTick(player);
    }
}

void ModuleManager::onLevelRender() {
    if (!isInitialized())
        return;
    auto lock = lockModuleList();
    for (auto& it : moduleList) {
        if (it->isEnabled() || it->callWhenDisabled())
            it->onLevelRender();
    }

    // Always render Xorion-style nametag for the local player (cosmetic, client-side)
    XorionNameTag::renderLocalPlayerName();
}

// Global instance
ModuleManager* moduleMgr = new ModuleManager(&Game);
