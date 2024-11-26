#include "alien.h"

#include <cmath>

#include "util.h"

as::Edge as::rand_edge() noexcept {
    return static_cast<Edge>(rand_int(0, 3));
}

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
                           Edge edge,
                           int scrwidth,
                           int scrheight,
                           unsigned int difficulty) {
    float theta, x, y;
    switch (edge) {
    case Edge::LEFT:
        x = 0;
        y = rand_float(0, scrheight - SIZE * SCALE);
        theta = rand_float(y < 2.f * scrheight / 2.f ? -PI / 2 : 0,
                           y > scrheight / 3.f ? PI / 2 : 0);
        break;
    case Edge::UP:
        x = rand_float(0, scrwidth - SIZE * SCALE);
        y = 0;
        theta = rand_float(x > scrwidth / 3.f ? -PI : -PI / 2,
                           x < 2.f * scrwidth / 3.f ? 0 : -PI / 2);
        break;
    case Edge::RIGHT:
        x = scrwidth - SIZE * SCALE;
        y = rand_float(0, scrheight - SIZE * SCALE);
        theta = rand_float(y > scrheight / 3.f ? PI / 2 : PI,
                           y < 2.f * scrheight / 3.f ? 3 * PI / 2 : PI);
        break;
    case Edge::DOWN:
        x = rand_float(0, scrwidth - SIZE * SCALE);
        y = scrheight - SIZE * SCALE;
        theta = rand_float(x < 2.f * scrwidth / 3.f ? 0 : PI / 2,
                           x > scrwidth / 3.f ? PI : PI / 2);
        break;
    }
    float v = (rand_float(0, 0.5) + difficulty * 0.25) * SCALE;
    return {tex, x, y, v * std::cosf(theta), -v * std::sinf(theta)};
}

bool as::Alien::check_hit(int click_x, int click_y) const noexcept {
    return std::abs(x + SCALE * SIZE / 2.f - click_x) < THRESHOLD * SCALE
           && std::abs(y + SCALE * SIZE / 2.f - click_y) < THRESHOLD * SCALE;
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
        if (x + SCALE * SIZE / 2.f < 0 || x + SCALE * SIZE / 2.f >= scrwidth
            || y + SCALE * SIZE / 2.f < 0
            || y + SCALE * SIZE / 2.f >= scrheight)
            state = AlienState::PASSED;
        break;
    case AlienState::HIT:
        if (death_timer >= 600)
            state = AlienState::DEAD;
        else
            death_timer += dt;
    default: break;
    }
}

void as::Alien::render(SDL_Renderer *rend) {
    spritesheet.render(rend,
                       {static_cast<int>(std::roundf(x)),
                        static_cast<int>(std::roundf(y)),
                        SIZE * SCALE,
                        SIZE * SCALE});
}

as::AlienState as::Alien::get_state() const noexcept {
    return state;
}
