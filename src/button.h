#pragma once

#include "text.h"

namespace as {
    class Button {
        static const int PADDING = 5;

        TextTexture tex;
        int x, y, w, h;

    public:
        Button(const std::string &name) noexcept;

        void init(SDL_Renderer *rend,
                  TTF_Font *font,
                  const std::string &text,
                  int x,
                  int y,
                  bool centered_x = true);

        void update(int x, int y, bool centered_x = true);
        bool inside(int click_x, int click_y) const noexcept;
        void render(SDL_Renderer *rend);
    };
}  // namespace as
