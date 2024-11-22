#include "spritesheet.h"

#include <SDL_image.h>

#include "error.h"

as::Spritesheet::Spritesheet(const std::string &name,
                             SDL_Texture *tex,
                             int size_w,
                             int size_h)
    : name(name),
      tex(tex),
      srcrect{0, 0, size_w, size_h} {
}

void as::Spritesheet::select(int x, int y) noexcept {
    srcrect.x = x * srcrect.w;
    srcrect.y = y * srcrect.h;
}

void as::Spritesheet::render(SDL_Renderer *rend, SDL_Rect dstrect) {
    if (SDL_RenderCopy(rend, tex, &srcrect, &dstrect) < 0)
        throw Error::sdl("rendering spritesheet texture '" + name + '\'');
}
