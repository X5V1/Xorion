#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <unordered_set>
#include <vector>
#include <string>
#include <tuple>
#include <memory>
#include <cstdint>
#include <cmath>

// Forward declarations to reduce include coupling.
// Include the real headers in GameData.cpp where definitions are required.
struct AABB;
struct ChestBlockActor;
struct ClientInstance;
struct GameMode;          // unified as struct
struct HIDController;
struct MoveInputHandler;
struct RakNetConnector;
struct GuiData;
struct TextHolder;
struct Entity;
struct EntityList;

// If you need methods from SlimMem/SlimModule directly, include their headers in GameData.cpp.
namespace SlimUtils {
    class SlimMem;
    class SlimModule;
}

// InfoBoxData: UI notification with a fade lifecycle.
struct InfoBoxData {
    bool isOpen = true;
    float fadeTarget = 1.0f;
    float fadeVal = 0.0f;
    float closeTimer = -1.0f;
    std::string title;
    std::string message;

    explicit InfoBoxData(std::string title, std::string message)
        : title(std::move(title)), message(std::move(message)) {}

    void fade() {
        fadeVal += (fadeTarget - fadeVal) * 0.04f;
        if (fadeTarget == 0.0f && std::abs(fadeVal) < 0.001f) {
            fadeVal = 0.0f;
            isOpen = false;
        }
    }
};

// Custom hasher for AABB use in unordered_set
struct AABBHasher {
    size_t operator()(const AABB& i) const;
};

// GameData: runtime state, access to client, players, entities, inputs, overlays.
class GameData {
private:
    ClientInstance* clientInstance = nullptr;
    EntityList*     entityList     = nullptr;
    GameMode*       gameMode       = nullptr;
    void*           hDllInst       = nullptr;

    std::unordered_set<AABB, AABBHasher> chestList;
    std::vector<std::string> textPrintList;
    std::mutex textPrintLock;
    std::mutex chestListMutex;

    int lastRequestId = 0;

    std::shared_ptr<std::string> customGeometry;
    bool customGeoActive = false;

    std::shared_ptr<std::tuple<std::shared_ptr<unsigned char[]>, size_t>> customTexture;
    bool customTextureActive = false;

    bool injectorConnectionActive = false;
    const SlimUtils::SlimModule* gameModule = nullptr;

    // Runtime flags
    bool shouldTerminateB = false;
    bool shouldHideB = false;
    bool isAllowingWIPFeatures = false;

    std::int64_t lastUpdate = 0;

    static void retrieveClientInstance();

    TextHolder* fakeName = nullptr;

public:
    // Input
    HIDController* hidController = nullptr;

    // UI
    std::vector<std::shared_ptr<InfoBoxData>> infoBoxQueue;

    // Memory helpers (shared across the project)
    static SlimUtils::SlimMem* slimMem;
    static bool keys[0x256];

    // Key state queries
    static bool canUseMoveKeys();
    static bool isKeyDown(int key);
    static bool isKeyPressed(int key);
    static bool isRightClickDown();
    static bool isLeftClickDown();
    static bool isWheelDown();

    // Lifecycle flags
    static bool shouldTerminate();
    static bool shouldHide();
    static void hide();
    static void terminate();

    // Initialization and update hooks
    static void updateGameData(GameMode* gameMode);
    static void initGameData(const SlimUtils::SlimModule* gameModule, SlimUtils::SlimMem* slimMem, void* hDllInst);
    static void addChestToList(ChestBlockActor* ChestBlock2);
    static void EntityList_tick(EntityList* list);
    static void setHIDController(HIDController* Hid);
    static void displayMessages(GuiData* guiData);
    static void log(const char* fmt, ...);

    // Metrics
    float fov = 0.0f;
    int fps = 0;
    int frameCount = 0;
    int cpsLeft = 0;
    int cpsRight = 0;
    int leftclickCount = 0;
    int rightclickCount = 0;

    // Public API

    void clearChestList() {
        std::lock_guard<std::mutex> listGuard(chestListMutex);
        chestList.clear();
    }

    inline std::shared_ptr<InfoBoxData> getFreshInfoBox() {
        while (!infoBoxQueue.empty()) {
            auto state = infoBoxQueue.front();
            if (!state->isOpen) {
                infoBoxQueue.erase(infoBoxQueue.begin());
                continue;
            }
            return state;
        }
        return {};
    }

    inline std::vector<std::shared_ptr<InfoBoxData>>& getInfoBoxList() {
        while (!infoBoxQueue.empty()) {
            auto box = infoBoxQueue.front();
            if (!box->isOpen) {
                infoBoxQueue.erase(infoBoxQueue.begin());
                continue;
            }
            break;
        }
        return infoBoxQueue;
    }

    inline std::shared_ptr<InfoBoxData> addInfoBox(std::string title, std::string message) {
        auto notification = std::make_shared<InfoBoxData>(std::move(title), std::move(message));
        infoBoxQueue.push_back(notification);
        return notification;
    }

    inline void setCustomGeometryOverride(bool setActive, std::shared_ptr<std::string> customGeoPtr) {
        customGeoActive = setActive;
        if (setActive)
            customGeometry.swap(customGeoPtr);
        else
            customGeometry.reset();
    }

    inline std::tuple<bool, std::shared_ptr<std::string>> getCustomGeoOverride() {
        return std::make_tuple(customGeoActive, customGeometry);
    }

    inline void setCustomTextureOverride(bool setActive,
        std::shared_ptr<std::tuple<std::shared_ptr<unsigned char[]>, size_t>> customTexturePtr) {
        customTextureActive = setActive;
        if (setActive)
            customTexture.swap(customTexturePtr);
        else
            customTexture.reset();
    }

    inline auto getCustomTextureOverride() {
        return std::make_tuple(customTextureActive, customTexture);
    }

    inline void* getDllModule() { return hDllInst; }
    inline ClientInstance* getClientInstance() { return clientInstance; }
    inline GuiData* getGuiData() {
        return clientInstance ? clientInstance->getGuiData() : nullptr;
    }

    inline class LocalPlayer* getLocalPlayer() {
        // Refresh local player from client each call
        if (clientInstance)
            // ClientInstance must define getCILocalPlayer(); forward declared LocalPlayer here
            return clientInstance->getCILocalPlayer();
        return nullptr;
    }

    // If callers truly need a pointer-to-pointer
    inline class LocalPlayer** getPtrLocalPlayer() {
        static class LocalPlayer* lp = getLocalPlayer();
        return &lp;
    }

    bool isInGame() { return getLocalPlayer() != nullptr; }

    const SlimUtils::SlimModule* getModule() { return gameModule; }
    const SlimUtils::SlimMem* getSlimMem() { return slimMem; }

    GameMode* getGameMode() { return gameMode; }
    EntityList* getEntityList() { return entityList; }

    HIDController** getHIDController() { return &hidController; }

    RakNetConnector* getRakNetConnector() {
        if (!clientInstance || !clientInstance->loopbackPacketSender ||
            !clientInstance->loopbackPacketSender->networkSystem ||
            !clientInstance->loopbackPacketSender->networkSystem->remoteConnectorComposite)
            return nullptr;

        return clientInstance->loopbackPacketSender->networkSystem->remoteConnectorComposite->rakNetConnector;
    }

    std::unordered_set<AABB, AABBHasher>& getChestList() { return chestList; }
    auto lockChestList() { return std::lock_guard<std::mutex>(chestListMutex); }

    void setFakeName(TextHolder* name) { fakeName = name; }
    TextHolder* getFakeName() { return fakeName; }

    // Spoofed Device ID, XUID, and IP for ban evasion
    TextHolder* fakeDeviceId = nullptr;
    TextHolder* fakeXuid = nullptr;
    TextHolder* fakeIP = nullptr;
    
    void setFakeDeviceId(TextHolder* deviceId) { fakeDeviceId = deviceId; }
    TextHolder* getFakeDeviceId() { return fakeDeviceId; }
    
    void setFakeXuid(TextHolder* xuid) { fakeXuid = xuid; }
    TextHolder* getFakeXuid() { return fakeXuid; }
    
    void setFakeIP(TextHolder* ip) { fakeIP = ip; }
    TextHolder* getFakeIP() { return fakeIP; }

    inline std::int64_t getLastUpdateTime() { return lastUpdate; }

    void forEachEntity(std::function<void(Entity*, bool)>);
    void forEachPlayer(std::function<void(Entity*, bool)>);
    void forEachMob(std::function<void(Entity*, bool)>);

    int getFPS() { return fps; }
    int getLeftCPS() { return cpsLeft; }
    int getRightCPS() { return cpsRight; }

    // Internal setters used during init/update
    inline void setClientInstance(ClientInstance* ci) { clientInstance = ci; }
    inline void setEntityList(EntityList* el) { entityList = el; }
    inline void setGameMode(GameMode* gm) { gameMode = gm; }
    inline void setDllInst(void* dll) { hDllInst = dll; }
    inline void setModule(const SlimUtils::SlimModule* mod) { gameModule = mod; }
    inline void setSlimMem(SlimUtils::SlimMem* sm) { slimMem = sm; }

    inline void setTerminate(bool v) { shouldTerminateB = v; }
    inline void setHide(bool v) { shouldHideB = v; }
    inline bool getTerminate() const { return shouldTerminateB; }
    inline bool getHide() const { return shouldHideB; }
};

// Singleton instance (classic Xorion style)
extern GameData Game;
