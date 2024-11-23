#pragma once

#include "spritesheet.h"

namespace as {
    enum class AlienState : char { ALIVE, PASSED, DEAD, HIT };

    class Alien {
    private:
        // both w and h
        static const int SIZE = 32;
        static const int THRESHOLD = 15;

        Spritesheet spritesheet;
        float x, y;
        float vx, vy;
        AlienState state;
        unsigned int life_timer, death_timer;

        Alien(SDL_Texture *tex, float x, float y, float vx, float vy);

    public:
        static const int SCALE = 3;

        // v = rand[0, 0.5] + difficulty * 0.25; theta = rand[-π/2, π/2]
        static Alien spawn(SDL_Texture *tex,
                           float x,
                           float y,
                           unsigned int difficulty);

        bool check_hit(int click_x, int click_y) const noexcept;
        bool count_passed(unsigned int difficulty) const noexcept;
        void hit();
        void update(std::uint64_t dt, int scrwidth, int scrheight);
        void render(SDL_Renderer *rend);

        AlienState get_state() const noexcept;
    };
}  // namespace as
