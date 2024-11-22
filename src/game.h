#pragma once

#include <vector>

#include "alien.h"

namespace as {
    class Game {
    private:
        int scrwidth, scrheight;

        SDL_Window *win;
        SDL_Renderer *rend;
        SDL_Texture *alien_tex;

        bool running;
        // -1 if click not registered that tick
        int click_x, click_y;

        std::vector<Alien> aliens;

        unsigned int score, difficulty, passed;

        void init_sdl();
        void handle_events();
        void update(std::uint64_t dt);
        void render();

    public:
        Game();
        ~Game();

        void start();
    };
}  // namespace as
