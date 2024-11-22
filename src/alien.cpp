#include "alien.h"

#include <cmath>

#include "util.h"

as::Alien::Alien(SDL_Texture *tex, float x, float y, float vx, float vy)
    : spritesheet("alien", tex, SIZE, SIZE),
      x(x),
      y(y),
      vx(vx),
      vy(vy),
      state(AlienState::ALIVE),
      death_timer(0) {
}

as::Alien as::Alien::spawn(SDL_Texture *tex,
                           float x,
                           float y,
                           unsigned int difficulty) {
    float theta = rand_float(-PI / 2, PI / 2);
    float v = rand_float(0, 0.5) + difficulty;
    return {tex, x, y, v * std::cosf(theta), v * std::sinf(theta)};
}

bool as::Alien::check_hit(int click_x, int click_y) const noexcept {
    return std::abs(x + SIZE / 2.f - static_cast<float>(click_x) / SCALE)
               < THRESHOLD
           && std::abs(y + SIZE / 2.f - static_cast<float>(click_y) / SCALE)
                  < THRESHOLD;
}

void as::Alien::hit() {
    state = AlienState::HIT;
    spritesheet.select(0, 1);
}

void as::Alien::update(std::uint64_t dt, int scrwidth, int scrheight) {
    switch (state) {
    case AlienState::ALIVE:
        x += vx * dt / 15;
        y += vy * dt / 15;

        // bounds check (centered on sprite)
        if (x + SIZE / 2.f < 0
            || x + SIZE / 2.f > static_cast<float>(scrwidth) / SCALE
            || y + SIZE / 2.f < 0
            || y + SIZE / 2.f > static_cast<float>(scrheight) / SCALE)
            state = AlienState::PASSED;
        break;
    case AlienState::HIT:
        if (dt * death_timer++ >= 600) state = AlienState::DEAD;
    default: break;
    }
}

void as::Alien::render(SDL_Renderer *rend) {
    spritesheet.render(rend,
                       {static_cast<int>(std::roundf(x * SCALE)),
                        static_cast<int>(std::roundf(y * SCALE)),
                        SIZE * SCALE,
                        SIZE * SCALE});
}

as::AlienState as::Alien::get_state() const noexcept {
    return state;
}
