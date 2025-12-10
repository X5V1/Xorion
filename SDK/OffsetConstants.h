#pragma once

#include <cstddef>

// Shared raw offsets for MCBE 1.21.130 structures (extracted from current layout).
constexpr ptrdiff_t OFF_SUPPLIES_PTR_DEFAULT = 0x360; // try 0x360, 0x370, 0x378, 0x3D0 if needed
constexpr ptrdiff_t OFF_DEVICE_ID_DEFAULT   = 0x848;
constexpr ptrdiff_t ITEM_RAW_OFF_PTR        = 0x10;
constexpr ptrdiff_t ITEM_RAW_OFF_COUNT      = 0x14;
constexpr ptrdiff_t ITEM_RAW_OFF_AUX        = 0x18;
constexpr ptrdiff_t ITEM_RAW_OFF_NAME       = 0x20;
constexpr ptrdiff_t ITEM_RAW_OFF_TAG        = 0x40;
constexpr ptrdiff_t LOCALPLAYER_GAMEMODE_OFF= 0xEF8;
