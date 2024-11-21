#pragma once

#include <string>

#include <SDL.h>

namespace as {
    class Spritesheet {
    private:
        std::string file;
        SDL_Texture *tex;
        SDL_Rect srcrect;

    public:
        Spritesheet(const std::string &file,
                    SDL_Renderer *rend,
                    int size_w,
                    int size_h);
        ~Spritesheet() noexcept;

        void select(int x, int y) noexcept;

        void render(SDL_Renderer *rend, SDL_Rect dstrect);
    };
}  // namespace as
