#pragma once
#include "../Utils/HMath.h"
#include "ComplexInventoryTransaction.h"
#include "Entity.h"
#include "TextHolder.h"
#include "ItemStack.h"
#include "Vec3.h"
#include "Vec3i.h"
#include <string>
#include <vector>
#include <cstdint>

// Base Packet type (unified as struct)
struct Packet {
    uintptr_t** vTable;  // 0x0000
private:
    char pad_0x8[0x28];
public:
    TextHolder* getName();

    template <class T>
    bool isInstanceOf() {
        T packet;
        return packet.vTable == this->vTable;
    }
};

struct LevelSoundEventPacket : public Packet {
    int sound;           // 0x28
    Vec3 pos;            // 0x2C
    int extraData = -1;  // 0x38
private:
    int unknown = 0;     // 0x3C
public:
    TextHolder entityType;               // 0x40
    bool isBabyMod = false;              // 0x60
    bool disableRelativeVolume = false;  // 0x61
    LevelSoundEventPacket();
};

struct PlayerAuthInputPacket : public Packet {
    float pitch;  // 0x28
    float yaw;
    Vec3 pos;     // 0x30
    float yawUnused;  // 0x38
    Vec3 velocity;
    float InputAD;
    float InputWS;
private:
    uint8_t padshit[12];
public:
    uint32_t inputKeys;
    char padding[0x256];

    PlayerAuthInputPacket();
    PlayerAuthInputPacket(Vec3 pos, float pitch, float yaw, float yawUnused);
};

struct PlayerActionPacket : public Packet {
    PlayerActionPacket();
    Vec3i blockPosition;    // 0x28
    int face;               // 0x34
    int action;             // 0x38
    __int64 entityRuntimeId;// 0x40
};

struct SubChunkRequestPacket : public Packet {
    SubChunkRequestPacket();
};

struct EmotePacket : public Packet {
    EmotePacket();
private:
    char padding[0x28];
public:
    int64_t entityRuntimeId;
    TextHolder emotId;
    char flags;
};

struct AnimatePacket : public Packet {
    AnimatePacket();
    AnimatePacket(int action, __int64 entityRuntimeID, float rowingTime);
    __int64 entityId;
    int action;
    float rowingTime;
};

struct NPCRequestPacket : public Packet {
    NPCRequestPacket();
    long entityRuntimeId;
    __int64 Unknown0;
    std::string Unknown1;
    __int64 Unknown2;
};

struct PlayerSkinPacket : public Packet {
    PlayerSkinPacket();
    __int64 UUID;
    __int64 skin;  // skin class placeholder
    std::string skinName;
    std::string oldSkinName;
};

struct NetworkLatencyPacket : public Packet {
    NetworkLatencyPacket();
    unsigned long timeStamp;
    bool sendBack;
    int pad[0x100];
};

struct CommandRequestPacket : public Packet {
    CommandRequestPacket();
    CommandRequestPacket(std::string cmd);
    enum class Origin {
        PLAYER = 0,
        COMMAND_BLOCK = 1,
        MINECART_COMMAND_BLOCK = 2,
        DEV_CONSOLE = 3,
        AUTOMATION_PLAYER = 4,
        CLIENT_AUTOMATION = 5,
        DEDICATED_SERVER = 6,
        ENTITY = 7,
        VIRTUAL = 8,
        GAME_ARGUMENT = 9,
        INTERNAL = 10
    };
    TextHolder command;
    Origin origin = Origin::PLAYER;
    bool isExternal = 0;
    char shit[0x256];
};

struct InteractPacket : public Packet {
    InteractPacket();
    int action;
    long target;
};

struct SetPlayerGameTypePacket : public Packet {
    SetPlayerGameTypePacket();
    int gameMode;
};

struct ActorEventPacket : public Packet {
    ActorEventPacket();
    ActorEventPacket(uint64_t entityRuntimeId, char eventId, int16_t itemId);
    uint64_t entityRuntimeId;
    char eventId;
    char pad_0x39[0x5];
    int16_t itemId;
};

struct MobEquipmentPacket : public Packet {
    MobEquipmentPacket();
    MobEquipmentPacket(__int64 entityRuntimeId, ItemStack& item, int hotbarSlot, int inventorySlot);
private:
    char pad_0x8[0x28];
public:
    __int64 entityRuntimeId;  // 0x28
    ItemStack item;           // 0x30
    int inventorySlot;        // 0xB8
    int hotbarSlot;           // 0xBC
    char windowId;            // 0xC0
    char windowId1;           // 0xC1
    char inventorySlot1;      // 0xC2
    char hotbarSlot1;         // 0xC3
    char windowId2;           // 0xC4
    char unknown1;
};

struct InventoryTransactionPacket : public Packet {
    InventoryTransactionPacket();
    InventoryTransactionPacket(ComplexInventoryTransaction* transac) : InventoryTransactionPacket() {
        this->complexTransaction = transac;
    }
    int unknown = 0;                         // 0x28
    __int64* unknownStart = 0;               // 0x30
    __int64* unknownEnd = 0;                 // 0x38
    __int64 filler = 0;                      // 0x40
    ComplexInventoryTransaction* complexTransaction; // 0x48
    unsigned char numTransactions;           // 0x50
};

struct TextPacket : public Packet {
    TextPacket();
    unsigned __int8 messageType;  // 0x28
    unsigned char gap[7];
    TextHolder sourceName;        // 0x30
    TextHolder message;           // 0x50
    unsigned char field_24222[24];// 0x70
    bool translationNeeded = false;// 0x88
    unsigned char ga2p[7];        // 0x89
    TextHolder xboxUserId;
    TextHolder platformChatId;
};

struct BookEditPacket : public Packet {
    BookEditPacket();
    char action; // 0x28
private:
    char pad2[0x3];
public:
    char slot;  // 0x2C
private:
    char pad3[0x3];
public:
    char page;  // 0x30
private:
    char pad4[0x3];
public:
    char secondaryPage; // 0x34
private:
    char pad5[0x3];
public:
    TextHolder title;     // 0x38 (also Text)
    TextHolder author;    // 0x58 (also Photo name)
    TextHolder xuid;      // 0x78
};

struct DisconnectPacket : public Packet {
    DisconnectPacket();
    DisconnectPacket(TextHolder reason, bool hideDisconnectScreen);
    bool hideDisconnectScreen; // 0x28
private:
    char pad2[0xF]; // 0x29
public:
    TextHolder reason; // 0x38
};

struct ResourcePacksInfoPacket : public Packet {
    ResourcePacksInfoPacket();
    bool mustAccept;  // 0x28
    bool scripting;   // 0x29
private:
    char pad2[0x1];   // 0x2A
public:
    bool forceServerPacks; // 0x2B
};

#pragma pack(push, 8)
__declspec(align(8)) struct MovePlayerPacket : public Packet {
    MovePlayerPacket();
    MovePlayerPacket(LocalPlayer* player, Vec3 pos);
    __int64 entityRuntimeID;  // 0x28
    Vec3 Position;            // 0x30
    float pitch;              // 0x3C
    float yaw;                // 0x40
    float headYaw;            // 0x44
    uint8_t mode;             // 0x48
    bool onGround;
    __int64 ridingEid;
    int int1;
    int int2;
};
#pragma pack(pop)

struct PlayerHotbarPacket : public Packet {
    PlayerHotbarPacket();
    PlayerHotbarPacket(int selectedHotbarSlot, char containerId, bool selectHotbarSlot);
    int selectedHotbarSlot;
    char containerId;
    bool selectHotbarSlot;
};

struct MobEffectPacket : public Packet {
    MobEffectPacket();
    MobEffectPacket(__int64 entityRuntimeId, char event, int effectId, int amplifier, bool showParticles, int duration);
    __int64 eid;
    char event; // 0: None 1 : Add 2 : Modify 3 : Remove
    int effectId;
    int amplifier;
    bool showParticles;
    int duration;
};

struct RemoveActorPacket : public Packet {
    RemoveActorPacket();
};

struct PlayerArmorDamagePacket : public Packet {
    PlayerArmorDamagePacket();
};

struct SetHealthPacket : public Packet {
    SetHealthPacket();
    int health;
};
