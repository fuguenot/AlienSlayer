#include <iostream>

#include "error.h"
#include "game.h"

int main() {
    try {
        as::Game().start();
    } catch (as::Error e) {
        std::cerr << e.to_string() << std::endl;
        return 1;
    }
}
