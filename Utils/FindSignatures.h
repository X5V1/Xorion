#pragma once

#include <cstddef>

#ifdef _WIN32
#include <windows.h>
#endif

// FindSignature: scan the primary module for a byte-pattern.
// Pattern format: "48 89 5C 24 ? 57 48 83 EC ?" where '?' or "??" are wildcards.
// Returns pointer to match or nullptr.
void* FindSignature(const char* pattern);
