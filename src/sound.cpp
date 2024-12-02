#include "sound.h"

#include "error.h"

as::SoundManager::SoundManager() noexcept {
}

void as::SoundManager::init() {
    if ((menu_ost = Mix_LoadMUS("../Resources/invaderside.ogg")) == nullptr)
        throw Error::sdl_mixer("loading menu OST");
    Mix_VolumeMusic(64);
}

as::SoundManager::~SoundManager() noexcept {
    Mix_HaltMusic();
    Mix_FreeMusic(menu_ost);
}
