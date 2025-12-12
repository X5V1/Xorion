// Memory/SignatureGlobals.h
#pragma once

#include <cstdint>

// Resolved addresses for signatures (opaque pointers)
extern void* g_WrapperEarlyCall;
extern void* g_WrapperCallB;
extern void* g_TwoArgCallA;
extern void* g_MethodRCX_RDX;
extern void* g_FieldReadCall;
extern void* g_GSReadA;
extern void* g_GSReadB;
extern void* g_GSReadC;
extern void* g_RipStaticA;
extern void* g_RipStaticB;
extern void* g_ConstCheckA;
extern void* g_ConstCheckB;
extern void* g_SSESetup;
extern void* g_GSReadD;
extern void* g_WrapperEarlyCallAlt;

// Known-good 1.21.130 resolved addresses (from Dump-1.txt)
constexpr std::uintptr_t k130_WrapperEarlyCall    = 0x7FF8074A3180;
constexpr std::uintptr_t k130_WrapperCallB        = 0x7FF80752BF00;
constexpr std::uintptr_t k130_TwoArgCallA         = 0x7FF80753A300;
constexpr std::uintptr_t k130_MethodRCX_RDX       = 0x7FF807534A70;
constexpr std::uintptr_t k130_FieldReadCall       = 0x7FF8074B1C60;
constexpr std::uintptr_t k130_GSReadA             = 0x7FF8074AD780;
constexpr std::uintptr_t k130_GSReadB             = 0x7FF8074B47E0;
constexpr std::uintptr_t k130_GSReadC             = 0x7FF8074A2088;
constexpr std::uintptr_t k130_GSReadD             = 0x7FF80752B200;
constexpr std::uintptr_t k130_RipStaticA          = 0x7FF80757F280;
constexpr std::uintptr_t k130_RipStaticB          = 0x7FF8075D8490;
constexpr std::uintptr_t k130_ConstCheckA         = 0x7FF807529A00;
constexpr std::uintptr_t k130_ConstCheckB         = 0x7FF807532430;
constexpr std::uintptr_t k130_SSESetup            = 0x7FF8075C6770;
constexpr std::uintptr_t k130_WrapperEarlyCallAlt = 0x7FF80756ECC0;
