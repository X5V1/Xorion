#include "Player.h"
#include "LocalPlayer.h"
#include <iostream>

int main() {
    // instantiate wrappers with null pointer to validate compile/link.
    Player p(nullptr);
    LocalPlayer lp(nullptr);

    std::cout << "Player wrapper created. getName() -> ";
    try {
        std::string n = p.getName();
        std::cout << n << "\n";
    } catch (...) {
        std::cout << "exception when calling getName\n";
    }

    std::cout << "LocalPlayer wrapper created. getGameMode() -> ";
    try {
        auto gm = lp.getGameMode();
        std::cout << (gm ? "non-null" : "null") << "\n";
    } catch (...) {
        std::cout << "exception when calling getGameMode\n";
    }

    std::cout << "Done.\n";
    return 0;
}
