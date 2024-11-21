#pragma once

#include <SDL.h>

namespace as {
    class Game {
    private:
        static const int SCRWIDTH = 800;
        static const int SCRHEIGHT = 600;

        SDL_Window *win;
        SDL_Renderer *rend;

        bool running;
        int click_x, click_y;

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
