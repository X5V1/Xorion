// Render/Signatures.h
#pragma once

// SlimMem byte pattern signatures (masked where unstable)
// Provenance: extracted from candidate dumps (addresses annotated per pattern)

namespace Signatures {

    // Wrappers / early-call dispatchers
    inline constexpr const char* PATTERN_WrapperEarlyCall =
        // 1.21.130 Dump-1.txt @ 0x7FF8074A3180
        "48 89 5C 24 08 57 48 83 EC 20 48 8B F9 E8 ? ? ? ?";

    inline constexpr const char* PATTERN_WrapperCallB =
        // 1.21.130 Dump-1.txt @ 0x7FF80752BF00
        "48 89 5C 24 08 57 48 83 EC 20 48 8B D9 33 FF E8 ? ? ? ?";

    inline constexpr const char* PATTERN_TwoArgCallA =
        // 1.21.130 Dump-1.txt @ 0x7FF80753A300
        "48 89 5C 24 08 57 48 83 EC 20 41 8B D8 8B F9 E8 ? ? ? ?";

    // Core method shapes (argument movement, no immediate call required)
    inline constexpr const char* PATTERN_MethodRCX_RDX =
        // 1.21.130 Dump-1.txt @ 0x7FF807534A70
        "48 89 5C 24 08 57 48 83 EC 20 48 8B DA 48 8B F9";

    // Member access pattern (field read then call)
    inline constexpr const char* PATTERN_FieldReadCall =
        // 1.21.130 Dump-1.txt @ 0x7FF8074B1C60
        "40 53 48 83 EC 20 48 8B D9 33 D2 8B 49 30 E8 ? ? ? ?";

    // GS-relative reads (stable anchors touching thread/PEB context)
    inline constexpr const char* PATTERN_GSReadA =
        // 1.21.130 Dump-1.txt @ 0x7FF8074AD780
        "48 89 5C 24 08 57 48 83 EC 30 65 4C 8B 0C 25 30 00 00 00";

    inline constexpr const char* PATTERN_GSReadB =
        // 1.21.130 Dump-1.txt @ 0x7FF8074B47E0
        "40 53 48 83 EC 20 65 48 8B 04 25 30 00 00 00";

    inline constexpr const char* PATTERN_GSReadC =
        // 1.21.130 Dump-1.txt @ 0x7FF8074A2088
        "40 53 48 83 EC 20 65 48 8B 04 25 60 00 00 00";

    // RIP-relative static/global loads (manager/table fetchers)
    inline constexpr const char* PATTERN_RipStaticA =
        // 1.21.130 Dump-1.txt @ 0x7FF80757F280
        "48 89 5C 24 08 57 48 83 EC 60 48 8B 05 ? ? ? ?";

    inline constexpr const char* PATTERN_RipStaticB =
        // 1.21.130 Dump-1.txt @ 0x7FF8075D8490
        "48 89 5C 24 10 57 48 83 EC 60 48 8B 05 ? ? ? ?";

    // Immediate constant checks (feature/version flags)
    inline constexpr const char* PATTERN_ConstCheckA =
        // 1.21.130 Dump-1.txt @ 0x7FF807529A00
        "40 53 48 83 EC 20 81 79 10 EE DD EE DD";

    inline constexpr const char* PATTERN_ConstCheckB =
        // 1.21.130 Dump-1.txt @ 0x7FF807532430
        "48 89 5C 24 08 57 48 83 EC 20 81 79 10 EE DD EE";

    // Distinct SSE/counter setup (rare footprint)
    inline constexpr const char* PATTERN_SSESetup =
        // 1.21.130 Dump-1.txt @ 0x7FF8075C6770
        "40 53 48 83 EC 60 48 8B D9 0F 57 C0 33 C9 4D 8B";

    // Additional clean GS/system access anchor
    inline constexpr const char* PATTERN_GSReadD =
        // 1.21.130 Dump-1.txt @ 0x7FF80752B200
        "48 89 5C 24 08 57 48 83 EC 20 65 48 8B 04 25 60 00 00 00";

    // Utility alt wrapper (prologue + setup; add a call tail when known)
    inline constexpr const char* PATTERN_WrapperEarlyCallAlt =
        // 1.21.130 Dump-1.txt @ 0x7FF80756ECC0
        "48 89 5C 24 08 57 48 83 EC 30 33 FF 48 8B DA";
}
