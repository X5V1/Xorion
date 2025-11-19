// Memory/BootstrapHelpers.h
#pragma once

// Call this after Minecraft.Windows.exe is loaded and its .text is stable.
// If you call from DllMain (PROCESS_ATTACH), spawn a thread to avoid blocking in DllMain.
void Bootstrap_ResolveAndInitAll();

// Convenience helper to start the bootstrap on a background thread (safe for DllMain usage).
// Call StartBootstrapThread() from DllMain(PROCESS_ATTACH) or your injector entrypoint.
void StartBootstrapThread();
