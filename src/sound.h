#pragma once

#include <SDL_mixer.h>

namespace as {
    class SoundManager {
    public:
        Mix_Music *menu_ost;

        SoundManager() noexcept;
        void init();
        ~SoundManager() noexcept;

        // TODO: other sounds!!
    };
}  // namespace as
