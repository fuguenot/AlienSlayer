#pragma once

#include <string>

#include <SDL_ttf.h>

namespace as {
    class TextManager {
    private:
        bool initialized;

        TTF_Font *title_font;
        TTF_Font *btn_font;
        TTF_Font *main_font;

        SDL_Texture *title_tex;
        SDL_Texture *play_btn_tex;
        SDL_Texture *quit_btn_tex;
        SDL_Texture *score_tex;
        SDL_Texture *diff_tex;
        SDL_Texture *passed_tex;

    public:
        TextManager() noexcept;

        void init(SDL_Renderer *rend,
                  const std::string &font_file,
                  unsigned int score,
                  unsigned int diff,
                  unsigned int passed);

        ~TextManager() noexcept;

        void render_title(SDL_Renderer *rend, SDL_Rect dstrect);
        void render_play_btn(SDL_Renderer *rend, SDL_Rect dstrect);
        void render_quit_btn(SDL_Renderer *rend, SDL_Rect dstrect);
        void render_score(SDL_Renderer *rend, SDL_Rect dstrect);
        void render_diff(SDL_Renderer *rend, SDL_Rect dstrect);
        void render_passed(SDL_Renderer *rend, SDL_Rect dstrect);

        void update_score(SDL_Renderer *rend,
                          unsigned int score,
                          bool free = true);
        void update_diff(SDL_Renderer *rend,
                         unsigned int diff,
                         bool free = true);
        void update_passed(SDL_Renderer *rend,
                           unsigned int passed,
                           bool free = true);
    };
}  // namespace as
