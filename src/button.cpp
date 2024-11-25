#include "button.h"

as::Button::Button(const std::string &name) noexcept : tex(name) {
}

void as::Button::init(SDL_Renderer *rend,
                      TTF_Font *font,
                      const std::string &text,
                      int x,
                      int y,
                      bool centered_x) {
    tex.init(rend, font, text);
    w = tex.w + PADDING * 2;
    h = tex.h + PADDING * 2;
    update(x, y, centered_x);
}

void as::Button::update(int x, int y, bool centered_x) {
    this->x = centered_x ? x - w / 2 : x;
    this->y = y;
}

bool as::Button::inside(int click_x, int click_y) const noexcept {
    return click_x >= x && click_x < x + w && click_y >= y && click_y < y + h;
}

void as::Button::render(SDL_Renderer *rend) {
    tex.render(rend, x + w / 2, y + PADDING);
}
