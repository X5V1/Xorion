#pragma once
#include <windows.h>

// Start a detached thread that runs the full bootstrap sequence.
// Safe to call from DllMain(PROCESS_ATTACH).
void StartInitThread();

// Run the bootstrap sequence on the current thread (blocking).
// Use this if you already know Minecraft.Windows.exe is loaded and stable.
void Init_RunBootstrapBlocking();

// Shutdown routine to clear globals and SDK pointers.
void InitSDK_Shutdown();
