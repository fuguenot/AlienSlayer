#pragma once

#include "spritesheet.h"

namespace as {
    enum class AlienState : char { ALIVE, PASSED, HIT, SLAIN };

    class Alien {
    private:
        // both w and h
        static const int SIZE = 32;
        static const int SCALE = 3;

        Spritesheet spritesheet;
        float x, y;
        const float vx, vy;
        AlienState state;
        unsigned int death_timer;

    public:
        Alien(SDL_Texture *tex, float x, float y, float vx, float vy);
        // v = rand[0, 5] + difficulty * 10; theta = rand[-π/2, π/2]
        static Alien spawn(SDL_Texture *tex,
                           float x,
                           float y,
                           unsigned int difficulty);

        void hit();
        void update(std::uint32_t dt);
        void render(SDL_Renderer *rend);
    };
}  // namespace as
