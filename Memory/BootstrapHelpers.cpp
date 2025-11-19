// Memory/BootstrapHelpers.cpp
#include "Memory/BootstrapHelpers.h"
#include "Memory/SignatureResolver.cpp"   // or include header + link .cpp normally
#include "Memory/SignatureReport.cpp"
#include "SDK/GameMode.h"
#include "Logger.h"

#include <windows.h>
#include <thread>
#include <chrono>

// Configuration: how long to wait between module checks (ms) and maximum total wait (ms)
static constexpr DWORD kModuleCheckIntervalMs = 200;   // poll interval
static constexpr DWORD kModuleMaxWaitMs         = 10000; // give up after 10s

// The module name to wait for
static constexpr const wchar_t* kTargetModuleName = L"Minecraft.Windows.exe";

// Internal helper: wait for module to be loaded into this process
static HMODULE WaitForModuleWide(const wchar_t* moduleName, DWORD intervalMs, DWORD maxWaitMs) {
    DWORD waited = 0;
    while (waited <= maxWaitMs) {
        HMODULE mod = GetModuleHandleW(moduleName);
        if (mod) return mod;
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        waited += intervalMs;
    }
    return nullptr;
}

// Main bootstrap entry (safe to call on any thread)
void Bootstrap_ResolveAndInitAll() {
    logF("[Bootstrap] bootstrap starting: waiting for %ws", kTargetModuleName);

    HMODULE target = WaitForModuleWide(kTargetModuleName, kModuleCheckIntervalMs, kModuleMaxWaitMs);
    if (!target) {
        logF("[Bootstrap] ERROR: target module %ws not found after %d ms. Aborting bootstrap.",
             kTargetModuleName, kModuleMaxWaitMs);
        // Still attempt resolution if you prefer; here we abort to avoid scanning wrong module.
        return;
    }

    logF("[Bootstrap] Found target module %p. Waiting a short moment for initialization.", target);
    // Give the module a brief moment to finish early init (reduce false negatives).
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    logF("[Bootstrap] Resolving signatures...");
    bool allResolved = ResolveAllSignatures();
    PrintSignatureReport();

    if (!allResolved) {
        logF("[Bootstrap] Warning: some signatures failed to resolve. Features that rely on missing signatures will be disabled.");
    }

    // Initialize SDK mappings (GameMode etc.)
    logF("[Bootstrap] Initializing SDK mappings...");
    Init_AllGameModeMappings();
    // Add other Init_* calls here as you add more SDK modules.

    logF("[Bootstrap] Bootstrap complete.");
}

// Thread starter safe to call from DllMain(PROCESS_ATTACH) or injector entrypoint.
// It spawns a detached thread that runs the bootstrap sequence.
static DWORD WINAPI BootstrapThreadProc(LPVOID) {
    // call the main bootstrap routine
    Bootstrap_ResolveAndInitAll();
    return 0;
}

void StartBootstrapThread() {
    // CreateThread is safe to call from DllMain to spawn a worker thread (avoid heavy work inside DllMain).
    HANDLE h = CreateThread(nullptr, 0, BootstrapThreadProc, nullptr, 0, nullptr);
    if (h) {
        CloseHandle(h); // detach thread; it will continue running
        logF("[Bootstrap] Started bootstrap thread successfully.");
    } else {
        logF("[Bootstrap] Failed to create bootstrap thread (CreateThread error %u). Trying std::thread fallback.",
             GetLastError());
        // fallback: spawn std::thread if CreateThread failed (less ideal from DllMain)
        try {
            std::thread t([](){
                Bootstrap_ResolveAndInitAll();
            });
            t.detach();
            logF("[Bootstrap] Started bootstrap std::thread fallback.");
        } catch (...) {
            logF("[Bootstrap] ERROR: failed to start fallback thread. Bootstrap not running.");
        }
    }
}
