#include "text.h"

#include "error.h"

as::TextTexture::TextTexture(const std::string &name) noexcept : name(name) {
}

void as::TextTexture::init(SDL_Renderer *rend,
                           TTF_Font *font,
                           const std::string &text) {
    this->font = font;
    update(rend, text, false);
}

as::TextTexture::~TextTexture() noexcept {
    SDL_DestroyTexture(tex);
}

void as::TextTexture::render(SDL_Renderer *rend, int x, int y) {
    SDL_Rect dstrect{x, y, w, h};
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
      play_btn("play button"),
      quit_btn("quit button"),
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
    if ((title_font = TTF_OpenFont(font_file.c_str(), 50)) == nullptr)
        throw Error::sdl("initializing title font");
    if ((btn_font = TTF_OpenFont(font_file.c_str(), 25)) == nullptr)
        throw Error::sdl("initializing button font");
    if ((main_font = TTF_OpenFont(font_file.c_str(), 20)) == nullptr)
        throw Error::sdl("initializing main font");

    title.init(rend, title_font, "ALIEN SLAYER");
    play_btn.init(rend, btn_font, "play");
    quit_btn.init(rend, btn_font, "quit");
    paused.init(rend, btn_font, "paused");
    this->score.init(rend, main_font, "Score: " + std::to_string(score));
    this->diff.init(rend, main_font, "Difficulty: " + std::to_string(diff));
    this->passed.init(rend, main_font, "Passed: " + std::to_string(passed));

    initialized = true;
}
