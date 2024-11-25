#pragma once

#include <string>

#include <SDL_ttf.h>

namespace as {
    class TextTexture {
    public:
        std::string name;

        TTF_Font *font;
        SDL_Texture *tex;
        int w, h;

        TextTexture(const std::string &name) noexcept;
        void init(TTF_Font *font) noexcept;
        void init(SDL_Renderer *rend, TTF_Font *font, const std::string &text);
        ~TextTexture() noexcept;

        void render(SDL_Renderer *rend, int x, int y, bool centered_x = true);
        void update(SDL_Renderer *rend,
                    const std::string &text,
                    bool free = true);
    };

    class TextManager {
    private:
        bool initialized;

    public:
        TTF_Font *title_font;
        TTF_Font *btn_font;
        TTF_Font *medium_font;
        TTF_Font *main_font;

        TextTexture title;
        TextTexture end_stats, lost, paused;
        TextTexture score, diff, passed;

        TextManager() noexcept;

        void init(SDL_Renderer *rend,
                  const std::string &font_file,
                  unsigned int score,
                  unsigned int diff,
                  unsigned int passed);
    };
}  // namespace as
