#include <windows.h>
#include "InitSDK.h"
#include "Logger.h"

// Entry point for your DLL.
// PROCESS_ATTACH: start bootstrap thread to resolve signatures and init SDK.
// PROCESS_DETACH: run shutdown to clear globals and SDK pointers.

// Patch anti-debug to allow x64dbg/debuggers to attach without crashes
void PatchAntiDebug() {
    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    if (!kernel32) return;
    
    // Patch IsDebuggerPresent to always return FALSE
    void* isDebuggerPresent = GetProcAddress(kernel32, "IsDebuggerPresent");
    if (isDebuggerPresent) {
        DWORD oldProtect;
        if (VirtualProtect(isDebuggerPresent, 3, PAGE_EXECUTE_READWRITE, &oldProtect)) {
            // xor eax, eax; ret (makes function return 0)
            *(BYTE*)isDebuggerPresent = 0x33;       // xor
            *((BYTE*)isDebuggerPresent + 1) = 0xC0; // eax, eax
            *((BYTE*)isDebuggerPresent + 2) = 0xC3; // ret
            VirtualProtect(isDebuggerPresent, 3, oldProtect, &oldProtect);
            logF("[AntiDebug] Patched IsDebuggerPresent");
        }
    }
    
    // Patch CheckRemoteDebuggerPresent to always set FALSE
    void* checkRemote = GetProcAddress(kernel32, "CheckRemoteDebuggerPresent");
    if (checkRemote) {
        DWORD oldProtect;
        if (VirtualProtect(checkRemote, 4, PAGE_EXECUTE_READWRITE, &oldProtect)) {
            // xor eax, eax; ret 8 (stdcall with 2 params)
            *(BYTE*)checkRemote = 0x33;       // xor
            *((BYTE*)checkRemote + 1) = 0xC0; // eax, eax
            *((BYTE*)checkRemote + 2) = 0xC2; // ret
            *((BYTE*)checkRemote + 3) = 0x08; // 8
            *((BYTE*)checkRemote + 4) = 0x00;
            VirtualProtect(checkRemote, 4, oldProtect, &oldProtect);
            logF("[AntiDebug] Patched CheckRemoteDebuggerPresent");
        }
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hinstDLL);
        PatchAntiDebug(); // Patch anti-debug before anything else
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
