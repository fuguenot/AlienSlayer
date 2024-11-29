#include "error.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

as::Error::Error(const std::string &name,
                 const std::string &details,
                 const std::string &when) noexcept
    : name(name),
      details(details),
      when(when) {
}

as::Error as::Error::sdl(const std::string &when) noexcept {
    return {"SDL Error", SDL_GetError(), when};
}

as::Error as::Error::sdl_image(const std::string &when) noexcept {
    return {"SDL_image Error", IMG_GetError(), when};
}

as::Error as::Error::sdl_ttf(const std::string &when) noexcept {
    return {"SDL_ttf Error", TTF_GetError(), when};
}

as::Error as::Error::sdl_mixer(const std::string &when) noexcept {
    return {"SDL_mixer Error", Mix_GetError(), when};
}

as::Error as::Error::out_of_bounds(const std::string &details,
                                   const std::string &when) noexcept {
    return {"Out of Bounds Error", details, when};
}

std::string as::Error::to_string() const noexcept {
    return name + ": " + details + " (when " + when + ")";
}
