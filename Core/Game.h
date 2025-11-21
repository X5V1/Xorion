#pragma once
#include <cstdint>

struct ModuleInfo {
    uintptr_t ptrBase{0};
};

class SlimMem {
public:
    template <typename T>
    T ReadPtr(uintptr_t base, std::initializer_list<uintptr_t> offsets) {
        uintptr_t addr = base;
        for (auto off : offsets) {
            addr = *reinterpret_cast<uintptr_t*>(addr) + off;
        }
        return *reinterpret_cast<T*>(addr);
    }
};

class GameClass {
public:
    ModuleInfo module{};
    SlimMem slim{};

    GameClass() = default;

    ModuleInfo* getModule() { return &module; }
    SlimMem* getSlimMem() { return &slim; }
};

extern GameClass Game;
