#pragma once

#include <string>

#include <SDL.h>

namespace as {
    class Spritesheet {
    private:
        std::string name;
        SDL_Rect srcrect;
        SDL_Texture *tex;

    public:
        Spritesheet(const std::string &name,
                    SDL_Texture *tex,
                    int size_w,
                    int size_h);

        void select(int x, int y) noexcept;

        void render(SDL_Renderer *rend, SDL_Rect dstrect);
    };
}  // namespace as
