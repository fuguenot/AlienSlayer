#include "spritesheet.h"

#include <SDL_image.h>

#include "error.h"

as::Spritesheet::Spritesheet(const std::string &file,
                             SDL_Renderer *rend,
                             int size_w,
                             int size_h)
    : file(file),
      srcrect{0, 0, size_w, size_h} {
    if ((tex = IMG_LoadTexture(rend, file.c_str())) == nullptr)
        throw Error::sdl("initializing spritesheet texture '" + file + '\'');
}

as::Spritesheet::~Spritesheet() noexcept {
    SDL_DestroyTexture(tex);
}

void as::Spritesheet::select(int x, int y) noexcept {
    srcrect.x = x * srcrect.w;
    srcrect.y = y * srcrect.h;
}

void as::Spritesheet::render(SDL_Renderer *rend, SDL_Rect dstrect) {
    if (SDL_RenderCopy(rend, tex, &srcrect, &dstrect) < 0)
        throw Error::sdl("rendering spritesheet texture '" + file + '\'');
}
