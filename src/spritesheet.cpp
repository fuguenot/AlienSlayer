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
    if (SDL_QueryTexture(tex, nullptr, nullptr, &w, &h) < 0)
        throw Error::sdl("querying spritesheet texture '" + name + '\'');
}

void as::Spritesheet::select(int x, int y) {
    if (x < 0 || x * srcrect.w >= w || y < 0 || y * srcrect.h >= h)
        throw Error::out_of_bounds(
            std::to_string(x) + ", " + std::to_string(y),
            "selecting from spritesheet texture '" + name + '\'');
    srcrect.x = x * srcrect.w;
    srcrect.y = y * srcrect.h;
}

void as::Spritesheet::render(SDL_Renderer *rend, SDL_Rect dstrect) {
    if (SDL_RenderCopy(rend, tex, &srcrect, &dstrect) < 0)
        throw Error::sdl("rendering spritesheet texture '" + name + '\'');
}
