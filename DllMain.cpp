#include <windows.h>
#include "InitSDK.h"
#include "Logger.h"

// Entry point for your DLL.
// PROCESS_ATTACH: start bootstrap thread to resolve signatures and init SDK.
// PROCESS_DETACH: run shutdown to clear globals and SDK pointers.

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hinstDLL);
        logF("[DllMain] PROCESS_ATTACH: starting bootstrap thread");
        StartInitThread();
        break;

    case DLL_PROCESS_DETACH:
        logF("[DllMain] PROCESS_DETACH: shutting down SDK");
        InitSDK_Shutdown();
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        // no per-thread work
        break;
    }
    return TRUE;
}
