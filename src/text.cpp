#include "text.h"

#include "error.h"

as::TextTexture::TextTexture(const std::string &name) noexcept : name(name) {
}

void as::TextTexture::init(TTF_Font *font) noexcept {
    this->font = font;
}

void as::TextTexture::init(SDL_Renderer *rend,
                           TTF_Font *font,
                           const std::string &text) {
    init(font);
    update(rend, text, false);
}

as::TextTexture::~TextTexture() noexcept {
    SDL_DestroyTexture(tex);
}

void as::TextTexture::render(SDL_Renderer *rend,
                             int x,
                             int y,
                             bool centered_x) {
    SDL_Rect dstrect{centered_x ? x - w / 2 : x, y, w, h};
    if (SDL_RenderCopy(rend, tex, nullptr, &dstrect) < 0)
        throw Error::sdl("rendering text texture '" + name + '\'');
}

void as::TextTexture::update(SDL_Renderer *rend,
                             const std::string &text,
                             bool free) {
    if (free) SDL_DestroyTexture(tex);
    SDL_Surface *surf;
    if ((surf =
             TTF_RenderText_Blended(font, text.c_str(), {255, 255, 255, 255}))
        == nullptr)
        throw Error::sdl("rendering text '" + name + '\'');
    if ((tex = SDL_CreateTextureFromSurface(rend, surf)) == nullptr)
        throw Error::sdl("converting text '" + name + "' surface to texture");
    SDL_FreeSurface(surf);

    if (SDL_QueryTexture(tex, nullptr, nullptr, &w, &h) < 0)
        throw Error::sdl("querying text texture '" + name + '\'');
}

as::TextManager::TextManager() noexcept
    : initialized(false),
      title("title"),
      end_score("end score"),
      end_diff("end difficulty"),
      lost("lost"),
      paused("paused"),
      score("score"),
      diff("difficulty"),
      passed("passed") {
}

void as::TextManager::init(SDL_Renderer *rend,
                           const std::string &font_file,
                           unsigned int score,
                           unsigned int diff,
                           unsigned int passed) {
    if ((title_font = TTF_OpenFont(font_file.c_str(), 75)) == nullptr)
        throw Error::sdl("initializing title font");
    if ((btn_font = TTF_OpenFont(font_file.c_str(), 50)) == nullptr)
        throw Error::sdl("initializing button font");
    if ((medium_font = TTF_OpenFont(font_file.c_str(), 25)) == nullptr)
        throw Error::sdl("initializing medium font");
    if ((main_font = TTF_OpenFont(font_file.c_str(), 20)) == nullptr)
        throw Error::sdl("initializing main font");

    title.init(rend, title_font, "ALIEN SLAYER");

    end_score.init(main_font);
    end_diff.init(main_font);
    lost.init(rend, title_font, "you lost");
    paused.init(rend, medium_font, "paused");

    this->score.init(rend, main_font, "score: " + std::to_string(score));
    this->diff.init(rend, main_font, "difficulty: " + std::to_string(diff));
    this->passed.init(rend, main_font, "passed: " + std::to_string(passed));

    initialized = true;
}
