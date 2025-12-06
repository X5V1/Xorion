#pragma once

// Forward declarations for signature resolution
void* ResolveFirst(const char** patterns, const char* name);

// Player signature resolvers
void* ResolvePlayerGetName();
void* ResolvePlayerGetHeldItem();
void* ResolvePlayerGetPos();
void* ResolvePlayerSetPos();
void* ResolvePlayerGetHealth();
void* ResolvePlayerSetHealth();
void* ResolvePlayerSendPacket();

// LocalPlayer signature resolvers
void* ResolveLocalPlayerSwing();
void* ResolveLocalPlayerTurn();
void* ResolveLocalPlayerApplyTurn();
void* ResolveLocalPlayerGetGameMode();
void* ResolveLocalPlayerUnlock();
