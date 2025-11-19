#include "InitSDK.h"
#include "Memory/BootstrapHelpers.h"
#include "Memory/SignatureGlobals.h"
#include "Memory/SignatureResolver.cpp"
#include "Memory/SignatureReport.cpp"
#include "SDK/GameMode.h"
#include "Logger.h"

#include <thread>

// Initialize all SDK subsystems after signatures are resolved.
static void Init_AllSDKSubsystems() {
    Init_AllGameModeMappings();
    // Add other Init_* calls here as you expand SDK coverage
}

// Start a detached thread that runs the full bootstrap sequence.
void StartInitThread() {
    HANDLE h = CreateThread(nullptr, 0, [](LPVOID)->DWORD {
        Init_RunBootstrapBlocking();
        return 0;
    }, nullptr, 0, nullptr);

    if (h) {
        CloseHandle(h);
        logF("[InitSDK] bootstrap thread started");
    } else {
        logF("[InitSDK] CreateThread failed (%u), using std::thread fallback", GetLastError());
        std::thread t([](){ Init_RunBootstrapBlocking(); });
        t.detach();
    }
}

// Run the bootstrap sequence on the current thread (blocking).
void Init_RunBootstrapBlocking() {
    logF("[InitSDK] waiting for Minecraft.Windows.exe...");
    HMODULE target = GetModuleHandleW(L"Minecraft.Windows.exe");
    int waited = 0;
    while (!target && waited < 10000) {
        Sleep(200);
        waited += 200;
        target = GetModuleHandleW(L"Minecraft.Windows.exe");
    }
    if (!target) {
        logF("[InitSDK] ERROR: target module not found");
        return;
    }

    Sleep(200); // brief pause to avoid scanning during early init

    logF("[InitSDK] resolving signatures...");
    bool ok = ResolveAllSignatures();
    PrintSignatureReport();
    if (!ok) logF("[InitSDK] some signatures failed");

    logF("[InitSDK] initializing SDK subsystems...");
    Init_AllSDKSubsystems();
    logF("[InitSDK] bootstrap finished");
}

// Shutdown routine to clear globals and SDK pointers.
void InitSDK_Shutdown() {
    g_WrapperEarlyCall    = nullptr;
    g_WrapperCallB        = nullptr;
    g_TwoArgCallA         = nullptr;
    g_MethodRCX_RDX       = nullptr;
    g_FieldReadCall       = nullptr;
    g_GSReadA             = nullptr;
    g_GSReadB             = nullptr;
    g_GSReadC             = nullptr;
    g_RipStaticA          = nullptr;
    g_RipStaticB          = nullptr;
    g_ConstCheckA         = nullptr;
    g_ConstCheckB         = nullptr;
    g_SSESetup            = nullptr;
    g_GSReadD             = nullptr;
    g_WrapperEarlyCallAlt = nullptr;

    SDK::GameMode_attack    = nullptr;
    SDK::GameMode_someOther = nullptr;

    logF("[InitSDK] shutdown complete");
}
