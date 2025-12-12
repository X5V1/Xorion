// Memory/SignatureResolver.cpp
#include "SignatureGlobals.h"
#include "../Xorion/Render/Signatures.h"
#include "SlimMem.h"   // signature scanning utility
#include "../Utils/Logger.h"    // logF or similar
#include "../Utils/Utils.h"     // for FindSignature macro

// Call once after the target module is loaded and its .text section is ready to scan
bool ResolveAllSignatures() {
    int ok = 0, fail = 0;

    auto RESOLVE = [&](const char* name, const char* pattern, void** out, std::uintptr_t fallback) {
        *out = (void*)FindSignature(pattern);
        if (*out) {
            logF("[Sig] %s resolved -> %p", name, *out);
            ++ok;
        } else if (fallback != 0) {
            *out = reinterpret_cast<void*>(fallback);
            logF("[Sig] %s FALLBACK -> %p (1.21.130)", name, *out);
            ++ok;
        } else {
            logF("[Sig] %s FAILED", name);
            ++fail;
        }
    };

    RESOLVE("WrapperEarlyCall",     Signatures::PATTERN_WrapperEarlyCall,     &g_WrapperEarlyCall,    k130_WrapperEarlyCall);
    RESOLVE("WrapperCallB",         Signatures::PATTERN_WrapperCallB,         &g_WrapperCallB,        k130_WrapperCallB);
    RESOLVE("TwoArgCallA",          Signatures::PATTERN_TwoArgCallA,          &g_TwoArgCallA,         k130_TwoArgCallA);
    RESOLVE("MethodRCX_RDX",        Signatures::PATTERN_MethodRCX_RDX,        &g_MethodRCX_RDX,       k130_MethodRCX_RDX);
    RESOLVE("FieldReadCall",        Signatures::PATTERN_FieldReadCall,        &g_FieldReadCall,       k130_FieldReadCall);

    RESOLVE("GSReadA",              Signatures::PATTERN_GSReadA,              &g_GSReadA,             k130_GSReadA);
    RESOLVE("GSReadB",              Signatures::PATTERN_GSReadB,              &g_GSReadB,             k130_GSReadB);
    RESOLVE("GSReadC",              Signatures::PATTERN_GSReadC,              &g_GSReadC,             k130_GSReadC);
    RESOLVE("GSReadD",              Signatures::PATTERN_GSReadD,              &g_GSReadD,             k130_GSReadD);

    RESOLVE("RipStaticA",           Signatures::PATTERN_RipStaticA,           &g_RipStaticA,          k130_RipStaticA);
    RESOLVE("RipStaticB",           Signatures::PATTERN_RipStaticB,           &g_RipStaticB,          k130_RipStaticB);

    RESOLVE("ConstCheckA",          Signatures::PATTERN_ConstCheckA,          &g_ConstCheckA,         k130_ConstCheckA);
    RESOLVE("ConstCheckB",          Signatures::PATTERN_ConstCheckB,          &g_ConstCheckB,         k130_ConstCheckB);

    RESOLVE("SSESetup",             Signatures::PATTERN_SSESetup,             &g_SSESetup,            k130_SSESetup);

    RESOLVE("WrapperEarlyCallAlt",  Signatures::PATTERN_WrapperEarlyCallAlt,  &g_WrapperEarlyCallAlt, k130_WrapperEarlyCallAlt);

    logF("[Sig] Resolve complete: %d OK, %d FAIL", ok, fail);
    return fail == 0;
}
