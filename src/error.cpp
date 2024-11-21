#include "error.h"

#include <SDL.h>

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

std::string as::Error::to_string() const noexcept {
    return name + ": " + details + " (while " + when + ")";
}
