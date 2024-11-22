#pragma once

#include <vector>

#include "alien.h"

namespace as {
    class Game {
    private:
        static const int SCRWIDTH = 800;
        static const int SCRHEIGHT = 600;

        SDL_Window *win;
        SDL_Renderer *rend;
        SDL_Texture *alien_tex;

        bool running;
        // -1 if click not registered that tick
        int click_x, click_y;

        std::vector<Alien> aliens;

        void init_sdl();
        void handle_events();
        void update();
        void render();

    public:
        Game();
        ~Game();

        void start();
    };
}  // namespace as
