// ItemStack.cpp
#include "ItemStack.h"
#include <stdexcept>
#include "Utils/Logger.h"
#include "VersionTag.h"

int   (*ItemStack::ItemStack_getCount_fn)(void*) = nullptr;
void  (*ItemStack::ItemStack_setCount_fn)(void*, int) = nullptr;
const char* (*ItemStack::ItemStack_getName_fn)(void*) = nullptr;

ItemStack::ItemStack(void* mcItemStackPtr) : itemPtr(mcItemStackPtr) {}
ItemStack::~ItemStack() {}

void ItemStack::ensureIntegration() const {
    if (!itemPtr ||
        !ItemStack_getCount_fn ||
        !ItemStack_getName_fn) {
        logF("[ItemStack] Missing integration. Ensure InitSDK() succeeded for %s.", kMinecraftVersion.c_str());
        throw std::runtime_error("ItemStack SDK not initialized");
    }
}

int ItemStack::getCount() const {
    ensureIntegration();
    return ItemStack_getCount_fn(itemPtr);
}

void ItemStack::setCount(int newCount) {
    ensureIntegration();
    if (!ItemStack_setCount_fn) {
        logF("[ItemStack] setCount signature missing for %s", kMinecraftVersion.c_str());
        return;
    }
    ItemStack_setCount_fn(itemPtr, newCount);
}

std::string ItemStack::getItemName() const {
    ensureIntegration();
    return std::string(ItemStack_getName_fn(itemPtr));
}
