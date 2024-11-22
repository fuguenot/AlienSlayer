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
        if (x < -SIZE / 2.f
            || x > static_cast<float>(scrwidth) / SCALE - SIZE / 2.f
            || y < -SIZE / 2.f
            || y > static_cast<float>(scrheight) / SCALE - SIZE / 2.f)
            state = AlienState::DEAD;
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
