#include "GameData.h"

#include <Windows.h>
#include <set>
#include <cstdarg>

#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
#include "Hooks.h"

GameData Game;
bool GameData::keys[0x256];
SlimUtils::SlimMem* GameData::slimMem = nullptr;

size_t AABBHasher::operator()(const AABB& i) const {
    return Utils::posToHash(i.lower);
}

void GameData::retrieveClientInstance() {
    static uintptr_t clientInstanceOffset = 0x0;
    if (clientInstanceOffset == 0x0) {
        clientInstanceOffset = GetOffsetFromSig(
            "48 89 0D ? ? ? ? 48 89 0D ? ? ? ? 48 85 C0 74 ? 48 8B C8 E8 ? ? ? ? 48 8B 0D ? ? ? ? 48 85 C9 74 ? 48 83 C4 28 E9 ? ? ? ? 48 83 C4 28 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC 28 48 8D 0D ? ? ? ? FF 15 ? ? ? ? 48 8B 0D ? ? ? ? 48 85 C9 74 ? 48 83 C4 28 E9 ? ? ? ? 48 83 C4 28 C3 CC CC CC CC CC 48 83 EC 28 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? 48 83 C4 28 E9 ? ? ? ? 48 8D 0D",
            3
        );
        logF("Client offset: %p", reinterpret_cast<void*>(clientInstanceOffset));
    }

    // Read pointer chain defensively
    clientInstance = reinterpret_cast<ClientInstance*>(
        Utils::readPointer<uintptr_t*>(clientInstanceOffset, {0x0, 0x0, 0x48, 0x0})
    );

#ifdef _DEV
    if (clientInstance == nullptr)
        throw std::exception("Client Instance is 0");
#endif
}

bool GameData::canUseMoveKeys() {
    MinecraftGame* mc = clientInstance ? clientInstance->minecraftGame : nullptr;
    if (mc == nullptr)
        return false;
    return mc->canUseKeybinds();
}

bool GameData::isKeyDown(int key) {
    return keys[(int)key];
}

bool GameData::isKeyPressed(int key) {
    if (isKeyDown(key)) {
        // Note: Sleep(1) blocks the calling thread; this mirrors Horion behavior
        while (isKeyDown(key))
            Sleep(1);
        return true;
    }
    return false;
}

bool GameData::isRightClickDown() {
    if (hidController == nullptr)
        return false;
    return hidController->rightClickDown;
}

bool GameData::isLeftClickDown() {
    if (hidController == nullptr)
        return false;
    return hidController->leftClickDown;
}

bool GameData::isWheelDown() {
    if (hidController == nullptr)
        return false;
    return hidController->wheelDown;
}

bool GameData::shouldTerminate() {
    return shouldTerminateB;
}

void GameData::terminate() {
    // Guard against nulls during shutdown
    if (clientInstance && clientInstance->minecraft) {
        clientInstance->minecraft->setTimerSpeed(20.0f);
    }
    g_Hooks.entityList.clear();
    shouldTerminateB = true;
}

bool GameData::shouldHide() {
    return shouldHideB;
}

void GameData::hide() {
    shouldHideB = !shouldHideB;
}

void GameData::updateGameData(GameMode* gm) {
    retrieveClientInstance();

    // Refresh local player via client instance
    localPlayer = getLocalPlayer();

    if (localPlayer != nullptr && gm != nullptr && gm->player == localPlayer) {
        gameMode = gm;

        LARGE_INTEGER counter{};
        QueryPerformanceCounter(&counter);
        lastUpdate = static_cast<std::int64_t>(counter.QuadPart);

        if (clientInstance != nullptr) {
            GuiData* guiData = clientInstance->getGuiData();
            if (guiData != nullptr) {
                displayMessages(guiData);
            }
        }
    }
}

void GameData::displayMessages(GuiData* guiData) {
    // Logger messages (DEV mode)
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

    // Local textPrintList
    {
        auto lock = std::lock_guard<std::mutex>(textPrintLock);

        auto& vec = textPrintList;
        int numPrinted = 0;
        std::vector<std::string>::iterator it;
        for (it = vec.begin(); it != vec.end(); ++it) {
            numPrinted++;
            if (numPrinted > 20)
                break;

            guiData->displayClientMessageNoSendF(it->c_str());
        }
        vec.erase(vec.begin(), it);
    }
}

void GameData::EntityList_tick(EntityList* list) {
    entityList = list;
}

void GameData::setHIDController(HIDController* Hid) {
    hidController = Hid;
}

void GameData::forEachEntity(std::function<void(Entity*, bool)> callback) {
    if (localPlayer && localPlayer->level) {
        for (const auto& ent : g_Hooks.entityList)
            if (ent.ent != nullptr && ent.ent->isPlayer())
                callback(ent.ent, false); // Only players from this list

        for (const auto& ent : getLocalPlayer()->level->getMiscEntityList())
            if (ent != nullptr && ent->getEntityTypeId() >= 1 && ent->getEntityTypeId() <= 999999999 && !ent->isPlayer())
                callback(ent, false); // Everything else
    }
}

void GameData::forEachPlayer(std::function<void(Entity*, bool)> callback) {
    if (localPlayer && localPlayer->level) {
        for (const auto& ent : g_Hooks.entityList)
            if (ent.ent != nullptr && ent.ent->isPlayer())
                callback(ent.ent, false); // All players
    }
}

void GameData::forEachMob(std::function<void(Entity*, bool)> callback) {
    if (localPlayer && localPlayer->level) {
        for (const auto& ent : getLocalPlayer()->level->getMiscEntityList())
            if (ent != nullptr && ent->getEntityTypeId() >= 1 && ent->getEntityTypeId() <= 999999999 && !ent->isPlayer())
                callback(ent, false); // All entities that are not players
    }
}

void GameData::addChestToList(ChestBlockActor* chest) {
    if (chest == nullptr || !chest->isMainSubchest())
        return;

    AABB chestAabb = chest->getFullAABB();
    std::lock_guard<std::mutex> listGuard(chestListMutex);
    if (chestList.count(chestAabb) > 0)
        return;

    chestList.insert(chestAabb);
}

void GameData::initGameData(const SlimUtils::SlimModule* mod, SlimUtils::SlimMem* sm, void* hInst) {
    gameModule = mod;
    slimMem = sm;
    hDllInst = hInst;

    retrieveClientInstance();

#ifdef _DEV
    logF("Base: %p", reinterpret_cast<void*>(gameModule ? gameModule->ptrBase : 0));
    if (clientInstance != nullptr) {
        logF("ClientInstance: %p", reinterpret_cast<void*>(clientInstance));
        logF("LocalPlayer: %p", reinterpret_cast<void*>(getLocalPlayer()));
        logF("MinecraftGame: %p", reinterpret_cast<void*>(clientInstance->minecraftGame));
        logF("LevelRenderer: %p", reinterpret_cast<void*>(clientInstance->levelRenderer));
    }
#endif
}

void GameData::log(const char* fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    char message[300];
    vsprintf_s(message, fmt, arg);
    va_end(arg);

    std::unique_lock<std::mutex> lock(textPrintLock);
    textPrintList.emplace_back(message);
}
