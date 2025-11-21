// File: C:\Xorion\Utils\Utils.cpp

#include "Utils.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <wtsapi32.h>
#include <playsoundapi.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")

#include "Logger.h"
#include "HMath.h"
#include "../Memory/Hooks.h"

static uintptr_t cachedBase = 0;

// UTF‑8 <-> UTF‑16 helpers
static std::wstring utf8_to_wide(const std::string& txt) {
    if (txt.empty()) return L"";
    int wlen = MultiByteToWideChar(CP_UTF8, 0, txt.c_str(), (int)txt.size(), nullptr, 0);
    std::wstring wstr(wlen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, txt.c_str(), (int)txt.size(), &wstr[0], wlen);
    return wstr;
}

static std::string wide_to_utf8(const std::wstring& w) {
    if (w.empty()) return "";
    int len = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::string out(len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), &out[0], len, nullptr, nullptr);
    return out;
}

// --------------------------------------------------------------------------------------
// Time
// --------------------------------------------------------------------------------------
void Utils::ApplySystemTime(std::stringstream* ss) {
    using namespace std::chrono;
#ifdef _DEV
    const system_clock::time_point systemNow = system_clock::now();
    auto ms = duration_cast<milliseconds>(systemNow.time_since_epoch()) % 10000;
    time_t now_c = system_clock::to_time_t(systemNow);
    tm timeInfo{};
    localtime_s(&timeInfo, &now_c);
    *ss << "[" << std::put_time(&timeInfo, "%T") << "." << std::setfill('0') << std::setw(4) << ms.count() << "] ";
#endif
}

void Utils::GetCurrentSystemTime(tm& timeInfo) {
    const auto systemNow = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(systemNow);
    localtime_s(&timeInfo, &now_c);
}

// --------------------------------------------------------------------------------------
// Base module address
// --------------------------------------------------------------------------------------
uintptr_t Utils::getBase() {
    if (cachedBase == 0) cachedBase = (uintptr_t)GetModuleHandleA("Minecraft.Windows.exe");
    return cachedBase;
}

// --------------------------------------------------------------------------------------
// String utilities
// --------------------------------------------------------------------------------------
static bool invalidChar(char c) {
    return !(c >= 0 && *reinterpret_cast<unsigned char*>(&c) < 128);
}

std::string Utils::sanitize(std::string text) {
    std::string out;
    bool wasValid = true;
    for (char c : text) {
        bool isValid = !invalidChar(c);
        if (wasValid) {
            if (!isValid) {
                wasValid = false;
            } else {
                out += c;
            }
        } else {
            wasValid = isValid;
        }
    }
    return out;
}

std::wstring Utils::stringToWstring(std::string txt) {
    return utf8_to_wide(txt);
}

// --------------------------------------------------------------------------------------
// RTTI (MSVC layout parsing)
// --------------------------------------------------------------------------------------
std::string Utils::getRttiBaseClassName(void* ptr) {
#define retIfBad(m, c) \
    if (IsBadReadPtr(reinterpret_cast<void*>(m), c)) DebugBreak();

    retIfBad(ptr, 8);

    __int64 vtable = *reinterpret_cast<__int64*>(ptr);
    retIfBad(vtable - sizeof(uintptr_t), 8);

    uintptr_t moduleBase;
    if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            reinterpret_cast<char*>(vtable), reinterpret_cast<HMODULE*>(&moduleBase)))
        return std::string("invalid handle");

    auto objLocator = *reinterpret_cast<__int64*>(vtable - sizeof(uintptr_t));
    retIfBad(objLocator + 0x10, 8);
    auto classHierachyDescriptorOffset = *reinterpret_cast<int*>(objLocator + 0x10);
    auto classHierachy = moduleBase + classHierachyDescriptorOffset;
    retIfBad(classHierachy + 0x8, sizeof(unsigned int));
    int numBaseClasses = *reinterpret_cast<int*>(classHierachy + 0x8);
    if (numBaseClasses < 0 || numBaseClasses > 25)
        numBaseClasses = 0;
    retIfBad(classHierachy + 0xC, sizeof(uintptr_t));
    auto baseClassArrayOffset = *reinterpret_cast<int*>(classHierachy + 0xC);
    auto baseClassArray = moduleBase + baseClassArrayOffset;
    retIfBad(baseClassArray, sizeof(unsigned int));

    auto classDescriptorOffset = *reinterpret_cast<unsigned int*>(baseClassArray);
    auto classDescriptor = moduleBase + classDescriptorOffset;
    retIfBad(classDescriptor, sizeof(int));
    auto typeDescripterOffset = *reinterpret_cast<int*>(classDescriptor);
    auto typeDescriptor = moduleBase + typeDescripterOffset;
    retIfBad(typeDescriptor, 0x10 + 512);

    size_t rttiLength = strnlen_s(reinterpret_cast<char*>(typeDescriptor + 0x11), 512);
    if (rttiLength > 5 && rttiLength < 512) {
        auto name = std::string(reinterpret_cast<char*>(typeDescriptor + 0x10 + 1));
        if (name.substr(0, 3) == "?AV") name = name.substr(3);
        if (name.size() >= 2 && name.substr(name.size() - 2, 2) == "@@") name = name.substr(0, name.size() - 2);
        return name;
    }

    return std::string("invalid");
}

// --------------------------------------------------------------------------------------
// Clipboard (UTF‑8)
// --------------------------------------------------------------------------------------
std::string Utils::getClipboardText() {
    if (!OpenClipboard(nullptr)) return "";
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (!hData) {
        CloseClipboard();
        return "";
    }
    wchar_t* pData = static_cast<wchar_t*>(GlobalLock(hData));
    std::string result;
    if (pData) {
        result = wide_to_utf8(std::wstring(pData));
        GlobalUnlock(hData);
    }
    CloseClipboard();
    return result;
}

void Utils::setClipboardText(std::string& text) {
    if (!OpenClipboard(nullptr)) return;
    EmptyClipboard();

    std::wstring wtext = utf8_to_wide(text);
    size_t bytes = (wtext.size() + 1) * sizeof(wchar_t);
    HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, bytes);
    if (!hGlob) {
        CloseClipboard();
        return;
    }
    void* ptr = GlobalLock(hGlob);
    if (ptr) {
        memcpy(ptr, wtext.c_str(), bytes);
        GlobalUnlock(hGlob);
        SetClipboardData(CF_UNICODETEXT, hGlob);
    } else {
        GlobalFree(hGlob);
    }
    CloseClipboard();
}

// --------------------------------------------------------------------------------------
// Signature scanning
// --------------------------------------------------------------------------------------
// Uses GET_BYTE/GET_BITS macros defined in Utils.h for hex parsing with wildcards
uintptr_t Utils::FindSignatureModule(const char* szModule, const char* szSignature) {
    const char* pattern = szSignature;
    uintptr_t firstMatch = 0;
    static const auto rangeStart = (uintptr_t)GetModuleHandleA(szModule);
    static MODULEINFO miModInfo;
    static bool init = false;
    if (!init) {
        init = true;
        GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
    }
    static const uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;

    BYTE patByte = GET_BYTE(pattern);
    const char* oldPat = pattern;

    for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++) {
        if (!*pattern)
            return firstMatch;

        while (*(PBYTE)pattern == ' ')
            pattern++;

        if (!*pattern)
            return firstMatch;

        if (oldPat != pattern) {
            oldPat = pattern;
            if (*(PBYTE)pattern != '\?')
                patByte = GET_BYTE(pattern);
        }

        if (*(PBYTE)pattern == '\?' || *(BYTE*)pCur == patByte) {
            if (!firstMatch)
                firstMatch = pCur;

            if (!pattern[2] || !pattern[1])
                return firstMatch;

            pattern += 2;
        } else {
            pattern = szSignature;
            firstMatch = 0;
        }
    }
#ifdef _DEV
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
    const char* sig = szSignature;
    logF("Signature dead: %s", szSignature);
    if (false) {
        const char* msgToTheOverwhelmedDebugger = "SIGNATURE NOT FOUND";
        __debugbreak();
    }
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif
    return 0u;
}

uintptr_t Utils::getOffsetFromSignature(const char* szSignature, int offset) {
    static uintptr_t signatureOffset = 0x0;
    if (signatureOffset == 0x0) {
        uintptr_t sigOffset = FindSignature(szSignature);
        if (sigOffset != 0x0) {
            int finalOffset = *reinterpret_cast<int*>((sigOffset + offset)); // rel32
            signatureOffset = sigOffset - Game.getModule()->ptrBase + finalOffset + 7; // 7 = instruction length in your pattern
            return signatureOffset;
        }
    }
    return 0u;
}

uintptr_t** Utils::getVtableFromSignature(const char* szSignature, int offset) {
    static uintptr_t** signatureOffset = 0x0;
    if (signatureOffset == 0x0) {
        uintptr_t sigOffset = FindSignature(szSignature);
        if (sigOffset != 0x0) {
            int finalOffset = *reinterpret_cast<int*>((sigOffset + offset)); // rel32
            signatureOffset = reinterpret_cast<uintptr_t**>(sigOffset + finalOffset + 7);
            return signatureOffset;
        }
    }
    return 0u;
}

// --------------------------------------------------------------------------------------
// Hashing (Vec3i)
// --------------------------------------------------------------------------------------
struct Vec3i { int x, y, z; };

size_t Utils::posToHash(const Vec3i& pos) {
    return rotBy(pos.x, 0) | rotBy(pos.z, 24) | (static_cast<unsigned __int64>(pos.y) << 48u);
}

// --------------------------------------------------------------------------------------
// Memory patching (protected regions + cache flush)
// --------------------------------------------------------------------------------------
static bool protect_region(void* addr, size_t size, DWORD newProt, DWORD& oldProt) {
    return VirtualProtect(addr, size, newProt, &oldProt) != 0;
}

void Utils::nopBytes(void* dst, unsigned int size) {
    if (!dst || size == 0) return;
    DWORD oldProt{};
    if (protect_region(dst, size, PAGE_EXECUTE_READWRITE, oldProt)) {
        std::memset(dst, 0x90, size);
        DWORD dummy{};
        VirtualProtect(dst, size, oldProt, &dummy);
        FlushInstructionCache(GetCurrentProcess(), dst, size);
    }
}

void Utils::copyBytes(void* src, void* dst, unsigned int size) {
    if (!src || !dst || size == 0) return;
    DWORD oldProt{};
    if (protect_region(dst, size, PAGE_EXECUTE_READWRITE, oldProt)) {
        std::memcpy(dst, src, size);
        DWORD dummy{};
        VirtualProtect(dst, size, oldProt, &dummy);
        FlushInstructionCache(GetCurrentProcess(), dst, size);
    }
}

void Utils::patchBytes(void* dst, void* src, unsigned int size) {
    copyBytes(src, dst, size);
}
