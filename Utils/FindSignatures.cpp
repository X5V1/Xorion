#include "FindSignatures.h"
#include <cctype>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#endif

static bool hexCharToByte(char c, uint8_t &out) {
    if (c >= '0' && c <= '9') { out = c - '0'; return true; }
    if (c >= 'a' && c <= 'f') { out = 10 + (c - 'a'); return true; }
    if (c >= 'A' && c <= 'F') { out = 10 + (c - 'A'); return true; }
    return false;
}

// Parse "48 89 5C 24 ? 57" -> bytes = {0x48,0x89,0x5C,0x24,0x00,0x57}, mask = {1,1,1,1,0,1}
static void parsePattern(const char* pattern, std::vector<uint8_t>& bytes, std::vector<uint8_t>& mask) {
    bytes.clear();
    mask.clear();
    const char* p = pattern;
    while (*p) {
        while (*p && isspace((unsigned char)*p)) ++p;
        if (!*p) break;
        if (*p == '?') {
            // wildcard byte
            ++p;
            // skip optional second '?'
            if (*p == '?') ++p;
            bytes.push_back(0);
            mask.push_back(0);
        } else {
            // expect two hex chars (or one then space)
            char c1 = *p++;
            if (!*p) break;
            char c2 = *p++;
            uint8_t hi, lo;
            if (!hexCharToByte(c1, hi) || !hexCharToByte(c2, lo)) {
                // attempt to skip malformed input
                continue;
            }
            bytes.push_back(static_cast<uint8_t>((hi << 4) | lo));
            mask.push_back(1);
        }
        // skip whitespace
        while (*p && isspace((unsigned char)*p)) ++p;
    }
}

#ifdef _WIN32

// get base and size of main module
static bool getModuleRange(uint8_t*& start, size_t& size) {
    HMODULE hModule = GetModuleHandleA(NULL);
    if (!hModule) return false;
    MODULEINFO modInfo;
    if (!GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(modInfo))) return false;
    start = reinterpret_cast<uint8_t*>(modInfo.lpBaseOfDll);
    size = static_cast<size_t>(modInfo.SizeOfImage);
    return true;
}

void* FindSignature(const char* pattern) {
    if (!pattern) return nullptr;
    std::vector<uint8_t> patBytes;
    std::vector<uint8_t> patMask;
    parsePattern(pattern, patBytes, patMask);
    if (patBytes.empty() || patBytes.size() != patMask.size()) return nullptr;

    uint8_t* base = nullptr;
    size_t size = 0;
    if (!getModuleRange(base, size)) return nullptr;

    uint8_t* data = base;
    const size_t dataSize = size;
    const size_t patLen = patBytes.size();

    // fast fail
    if (patLen > dataSize) return nullptr;

    // naive scan
    for (size_t i = 0; i <= dataSize - patLen; ++i) {
        bool ok = true;
        for (size_t j = 0; j < patLen; ++j) {
            if (patMask[j] && data[i + j] != patBytes[j]) { ok = false; break; }
        }
        if (ok) return reinterpret_cast<void*>(data + i);
    }
    return nullptr;
}

#else

// POSIX stub — implement as needed for non-Windows targets using dlopen / dladdr / parsing /proc/self/maps
void* FindSignature(const char* /*pattern*/) {
    // Not implemented here — add a POSIX pattern scanner for Linux/macOS
    return nullptr;
}

#endif
