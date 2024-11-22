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

as::Error as::Error::out_of_bounds(const std::string &details,
                                   const std::string &when) noexcept {
    return {"Out of Bounds Error", details, when};
}

std::string as::Error::to_string() const noexcept {
    return name + ": " + details + " (when " + when + ")";
}
