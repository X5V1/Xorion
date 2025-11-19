// Memory/SignatureReport.cpp
#include "SignatureGlobals.h"
#include "Logger.h"

void PrintSignatureReport() {
    struct Item { const char* name; void* addr; } items[] = {
        {"WrapperEarlyCall",     g_WrapperEarlyCall},
        {"WrapperCallB",         g_WrapperCallB},
        {"TwoArgCallA",          g_TwoArgCallA},
        {"MethodRCX_RDX",        g_MethodRCX_RDX},
        {"FieldReadCall",        g_FieldReadCall},

        {"GSReadA",              g_GSReadA},
        {"GSReadB",              g_GSReadB},
        {"GSReadC",              g_GSReadC},
        {"GSReadD",              g_GSReadD},

        {"RipStaticA",           g_RipStaticA},
        {"RipStaticB",           g_RipStaticB},

        {"ConstCheckA",          g_ConstCheckA},
        {"ConstCheckB",          g_ConstCheckB},

        {"SSESetup",             g_SSESetup},

        {"WrapperEarlyCallAlt",  g_WrapperEarlyCallAlt},
    };

    for (auto& it : items) {
        logF("[SigReport] %-20s -> %p", it.name, it.addr);
    }
}
