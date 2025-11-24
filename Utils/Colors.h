#pragma once

// Colors.h
// Common color constants for rendering and logging.
// Fixes undeclared identifiers: RED, GREEN, YELLOW (and adds a few extras).

namespace Colors {
    // RGBA arrays for DrawUtils::setColor(r,g,b,a)
    static constexpr float RED[4]    = { 0.95f, 0.30f, 0.30f, 1.0f };
    static constexpr float GREEN[4]  = { 0.30f, 0.95f, 0.45f, 1.0f };
    static constexpr float YELLOW[4] = { 0.95f, 0.90f, 0.30f, 1.0f };
    static constexpr float CYAN[4]   = { 0.30f, 0.95f, 0.95f, 1.0f };
    static constexpr float WHITE[4]  = { 1.00f, 1.00f, 1.00f, 1.0f };
    static constexpr float BLACK[4]  = { 0.00f, 0.00f, 0.00f, 1.0f };
}

// Optional ANSI escape codes for Logger text output.
#define ANSI_RED    "\x1b[31m"
#define ANSI_GREEN  "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_RESET  "\x1b[0m"
