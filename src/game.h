#pragma once

#include <vector>

#include "alien.h"
#include "button.h"
#include "sound.h"

namespace as {
    enum class GameState { MENU, PLAYING, PAUSED, LOST };

    class Game {
    private:
        static const std::uint64_t START_SPAWN_INTERVAL = 4000;

        int scrwidth, scrheight;

        SDL_Window *win;
        SDL_Renderer *rend;
        SDL_Texture *alien_tex;
        TextManager text;
        Button play_btn, quit_btn, again_btn, menu_btn;

        SoundManager sound;
        bool muted;

        GameState state;
        bool running, clicked;
        int click_x, click_y;

        std::uint64_t spawn_timer;

        std::vector<Alien> aliens;

        unsigned int score, difficulty, passed;
        bool score_changed, diff_changed, passed_changed;

        void init_sdl();
        void handle_events();
        void spawn_aliens();
        int update_aliens(std::uint64_t dt);
        void update(std::uint64_t dt);
        void render();

        void reset();

    public:
        Game();
        ~Game() noexcept;

        void start();
    };
}  // namespace as
