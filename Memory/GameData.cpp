#include "GameData.h"

#include <Windows.h>
#include <set>
#include <cstdarg>
#include <chrono>

#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
#include "../SDK/ClientInstance.h"
#include "../SDK/HIDController.h"
#include "Hooks.h"

// Singleton instance
GameData Game;

// Static members
bool GameData::keys[0x256];
SlimUtils::SlimMem* GameData::slimMem = nullptr;

// Hash for AABB relies on AABB::lower being a Vec3i-like integer position.
// Ensure AABB’s lower has x/y/z integers (or adapt hashing accordingly).
size_t AABBHasher::operator()(const AABB& i) const {
	return Utils::posToHash(i.lower);
}

// Resolve ClientInstance via signature + readPointer chain.
// Keep this in .cpp to allow forward declarations in the header.
void GameData::retrieveClientInstance() {
    static uintptr_t clientInstanceOffset = 0x0;
    if (clientInstanceOffset == 0x0) {
        clientInstanceOffset = GetOffsetFromSig(
            "48 89 0D ? ? ? ? 48 89 0D ? ? ? ? 48 85 C0 74 ? 48 8B C8 E8 ? ? ? ? 48 8B 0D ? ? ? ? 48 85 C9 74 ? 48 83 C4 28 E9 ? ? ? ? 48 83 C4 28 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC 28 48 8D 0D ? ? ? ? FF 15 ? ? ? ? 48 8B 0D ? ? ? ? 48 85 C9 74 ? 48 83 C4 28 E9 ? ? ? ? 48 83 C4 28 C3 CC CC CC CC CC 48 83 EC 28 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? 48 83 C4 28 E9 ? ? ? ? 48 8D 0D",
            3
        );
        Logger::logF("Client offset: %p", reinterpret_cast<void*>(clientInstanceOffset));
    }

    // Read pointer chain defensively
    g_Data.clientInstance = reinterpret_cast<ClientInstance*>(
        Utils::readPointer<uintptr_t*>(clientInstanceOffset, {0x0, 0x0, 0x48, 0x0})
    );

#ifdef _DEV
    if (clientInstance == nullptr)
        Logger::logF("ClientInstance is null after retrieveClientInstance");
#endif
}

bool GameData::canUseMoveKeys() {
    // Forward-safe: do not deref unless the type is complete; we're in .cpp with full definitions.
    MinecraftGame* mc = Game.clientInstance ? Game.clientInstance->minecraftGame : nullptr;
    return mc ? mc->canUseKeybinds() : false;
}

bool GameData::isKeyDown(int key) {
    return keys[(int)key];
}

bool GameData::isKeyPressed(int key) {
    if (isKeyDown(key)) {
        while (isKeyDown(key))
            Sleep(1);
        return true;
    }
    return false;
}

bool GameData::isRightClickDown() {
    return Game.hidController ? Game.hidController->rightClickDown : false;
}

bool GameData::isLeftClickDown() {
    return Game.hidController ? Game.hidController->leftClickDown : false;
}

bool GameData::isWheelDown() {
    return Game.hidController ? Game.hidController->wheelDown : false;
}

bool GameData::shouldTerminate() {
    return Game.shouldTerminateB;
}

void GameData::terminate() {
    // Guard against nulls during shutdown
    if (Game.clientInstance && Game.clientInstance->minecraft) {
        Game.clientInstance->minecraft->setTimerSpeed(20.0f);
    }
    g_Hooks.entityList.clear();
    Game.shouldTerminateB = true;
}

bool GameData::shouldHide() {
    return Game.shouldHideB;
}

void GameData::hide() {
    Game.shouldHideB = !Game.shouldHideB;
}

void GameData::updateGameData(GameMode* gm) {
    retrieveClientInstance();

    // Refresh local player via client instance
    LocalPlayer* localPlayer = Game.getLocalPlayer();

    if (localPlayer != nullptr && gm != nullptr && gm->player == (Entity*)localPlayer) {
        Game.gameMode = gm;

        LARGE_INTEGER counter{};
        QueryPerformanceCounter(&counter);
        Game.lastUpdate = static_cast<std::int64_t>(counter.QuadPart);

        if (Game.clientInstance != nullptr) {
            GuiData* guiData = Game.clientInstance->getGuiData();
            if (guiData != nullptr) {
                displayMessages(guiData);
            }
        }
    }
}

void GameData::displayMessages(GuiData* guiData) {
    // Logger messages (DEV mode)
    {
        auto vecLock = Logger::GetTextToPrintLock();
        auto* stringPrintVector = Logger::GetTextToPrint();
#ifdef _DEV
        int numPrinted = 0;
        std::vector<TextForPrint>::iterator it;
        for (it = stringPrintVector->begin(); it != stringPrintVector->end(); ++it) {
            numPrinted++;
            if (numPrinted > 20)
                break;

            guiData->displayClientMessageNoSendF("%s%s%s%s", GOLD, it->time, RESET, it->text);
        }
        stringPrintVector->erase(stringPrintVector->begin(), it);
#else
        stringPrintVector->clear();
#endif
    }

    // Local textPrintList
    {
        auto lock = std::lock_guard<std::mutex>(Game.textPrintLock);
        int numPrinted = 0;
        auto it = Game.textPrintList.begin();
        for (; it != Game.textPrintList.end(); ++it) {
            numPrinted++;
            if (numPrinted > 20)
                break;

            guiData->displayClientMessageNoSendF(it->c_str());
        }
        Game.textPrintList.erase(Game.textPrintList.begin(), it);
    }
}

void GameData::EntityList_tick(EntityList* list) {
    Game.entityList = list;
}

void GameData::forEachEntity(std::function<void(Entity*, bool)> callback) {
    LocalPlayer* lp = Game.getLocalPlayer();
    if (lp && lp->level) {
        // Players from hook-managed list
        for (const auto& ent : g_Hooks.entityList)
            if (ent.ent != nullptr && ent.ent->isPlayer())
                callback(ent.ent, false);

        // Everything else from level's misc list
        for (const auto& ent : lp->level->getMiscEntityList())
            if (ent != nullptr && ent->getEntityTypeId() >= 1 && ent->getEntityTypeId() <= 999999999 && !ent->isPlayer())
                callback(ent, false);
    }
}

void GameData::forEachPlayer(std::function<void(Entity*, bool)> callback) {
    LocalPlayer* lp = Game.getLocalPlayer();
    if (lp && lp->level) {
        for (const auto& ent : g_Hooks.entityList)
            if (ent.ent != nullptr && ent.ent->isPlayer())
                callback(ent.ent, false);
    }
}

void GameData::forEachMob(std::function<void(Entity*, bool)> callback) {
    LocalPlayer* lp = Game.getLocalPlayer();
    if (lp && lp->level) {
        for (const auto& ent : lp->level->getMiscEntityList())
            if (ent != nullptr && ent->getEntityTypeId() >= 1 && ent->getEntityTypeId() <= 999999999 && !ent->isPlayer())
                callback(ent, false);
    }
}

void GameData::addChestToList(ChestBlockActor* chest) {
    if (chest == nullptr || !chest->isMainSubchest())
        return;

    AABB chestAabb = chest->getFullAABB();
    std::lock_guard<std::mutex> listGuard(Game.chestListMutex);
    if (Game.chestList.count(chestAabb) > 0)
        return;

    Game.chestList.insert(chestAabb);
}

void GameData::initGameData(const SlimUtils::SlimModule* mod, SlimUtils::SlimMem* sm, void* hInst) {
    Game.gameModule = mod;
    slimMem = sm;
    Game.hDllInst = hInst;

    retrieveClientInstance();

#ifdef _DEV
    Logger::logF("Base: %p", reinterpret_cast<void*>(Game.gameModule ? Game.gameModule->ptrBase : 0));
    if (Game.clientInstance != nullptr) {
        Logger::logF("ClientInstance: %p", reinterpret_cast<void*>(Game.clientInstance));
        Logger::logF("LocalPlayer: %p", reinterpret_cast<void*>(Game.getLocalPlayer()));
        Logger::logF("MinecraftGame: %p", reinterpret_cast<void*>(Game.clientInstance->minecraftGame));
        Logger::logF("LevelRenderer: %p", reinterpret_cast<void*>(Game.clientInstance->levelRenderer));
    }
#endif
}

void GameData::log(const char* fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    char message[300];
    vsprintf_s(message, fmt, arg);
    va_end(arg);

    std::unique_lock<std::mutex> lock(Game.textPrintLock);
    Game.textPrintList.emplace_back(message);
}
