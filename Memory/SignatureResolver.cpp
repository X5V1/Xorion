// Memory/SignatureResolver.cpp
#include "SignatureGlobals.h"
#include "../Xorion/Render/Signatures.h"
#include "SlimMem.h"   // signature scanning utility
#include "../Utils/Logger.h"    // logF or similar
#include "../Utils/Utils.h"     // for FindSignature macro

// Call once after the target module is loaded and its .text section is ready to scan
bool ResolveAllSignatures() {
    int ok = 0, fail = 0;

    auto RESOLVE = [&](const char* name, const char* pattern, void** out) {
        *out = (void*)FindSignature(pattern);
        if (*out) { logF("[Sig] %s resolved -> %p", name, *out); ++ok; }
        else      { logF("[Sig] %s FAILED", name); ++fail; }
    };

    RESOLVE("WrapperEarlyCall",     Signatures::PATTERN_WrapperEarlyCall,     &g_WrapperEarlyCall);
    RESOLVE("WrapperCallB",         Signatures::PATTERN_WrapperCallB,         &g_WrapperCallB);
    RESOLVE("TwoArgCallA",          Signatures::PATTERN_TwoArgCallA,          &g_TwoArgCallA);
    RESOLVE("MethodRCX_RDX",        Signatures::PATTERN_MethodRCX_RDX,        &g_MethodRCX_RDX);
    RESOLVE("FieldReadCall",        Signatures::PATTERN_FieldReadCall,        &g_FieldReadCall);

    RESOLVE("GSReadA",              Signatures::PATTERN_GSReadA,              &g_GSReadA);
    RESOLVE("GSReadB",              Signatures::PATTERN_GSReadB,              &g_GSReadB);
    RESOLVE("GSReadC",              Signatures::PATTERN_GSReadC,              &g_GSReadC);
    RESOLVE("GSReadD",              Signatures::PATTERN_GSReadD,              &g_GSReadD);

    RESOLVE("RipStaticA",           Signatures::PATTERN_RipStaticA,           &g_RipStaticA);
    RESOLVE("RipStaticB",           Signatures::PATTERN_RipStaticB,           &g_RipStaticB);

    RESOLVE("ConstCheckA",          Signatures::PATTERN_ConstCheckA,          &g_ConstCheckA);
    RESOLVE("ConstCheckB",          Signatures::PATTERN_ConstCheckB,          &g_ConstCheckB);

    RESOLVE("SSESetup",             Signatures::PATTERN_SSESetup,             &g_SSESetup);

    RESOLVE("WrapperEarlyCallAlt",  Signatures::PATTERN_WrapperEarlyCallAlt,  &g_WrapperEarlyCallAlt);

    logF("[Sig] Resolve complete: %d OK, %d FAIL", ok, fail);
    return fail == 0;
}
