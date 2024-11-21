#pragma once

#include <string>

namespace as {
    class Error {
    private:
        std::string name, details, when;

        Error(const std::string &name,
              const std::string &details,
              const std::string &when) noexcept;

    public:
        static Error sdl(const std::string &when) noexcept;

        std::string to_string() const noexcept;
    };
}  // namespace as
