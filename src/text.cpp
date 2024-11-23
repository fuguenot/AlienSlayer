#include "text.h"

#include "error.h"

as::TextManager::TextManager() noexcept : initialized(false) {
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

    SDL_Surface *surf;
    if ((surf = TTF_RenderText_Blended(title_font,
                                       "ALIEN SLAYER",
                                       {255, 255, 255, 255}))
        == nullptr)
        throw Error::sdl("rendering title text");
    if ((title_tex = SDL_CreateTextureFromSurface(rend, surf)) == nullptr)
        throw Error::sdl("converting title surface to texture");
    SDL_FreeSurface(surf);

    if ((surf = TTF_RenderText_Blended(btn_font, "play", {255, 255, 255, 255}))
        == nullptr)
        throw Error::sdl("rendering play button text");
    if ((play_btn_tex = SDL_CreateTextureFromSurface(rend, surf)) == nullptr)
        throw Error::sdl("converting play button surface to texture");
    SDL_FreeSurface(surf);

    if ((surf = TTF_RenderText_Blended(btn_font, "quit", {255, 255, 255, 255}))
        == nullptr)
        throw Error::sdl("rendering quit button text");
    if ((quit_btn_tex = SDL_CreateTextureFromSurface(rend, surf)) == nullptr)
        throw Error::sdl("converting quit button surface to texture");
    SDL_FreeSurface(surf);

    update_score(rend, score, false);
    update_diff(rend, diff, false);
    update_passed(rend, passed, false);

    initialized = true;
}

as::TextManager::~TextManager() noexcept {
    SDL_DestroyTexture(title_tex);
    SDL_DestroyTexture(play_btn_tex);
    SDL_DestroyTexture(quit_btn_tex);
    SDL_DestroyTexture(score_tex);
    SDL_DestroyTexture(diff_tex);
    SDL_DestroyTexture(passed_tex);

    TTF_CloseFont(title_font);
    TTF_CloseFont(btn_font);
    TTF_CloseFont(main_font);
}

void as::TextManager::render_title(SDL_Renderer *rend, SDL_Rect dstrect) {
    if (SDL_RenderCopy(rend, title_tex, nullptr, &dstrect) < 0)
        throw Error::sdl("rendering title texture");
}

void as::TextManager::render_play_btn(SDL_Renderer *rend, SDL_Rect dstrect) {
    if (SDL_RenderCopy(rend, play_btn_tex, nullptr, &dstrect) < 0)
        throw Error::sdl("rendering play button texture");
}

void as::TextManager::render_quit_btn(SDL_Renderer *rend, SDL_Rect dstrect) {
    if (SDL_RenderCopy(rend, quit_btn_tex, nullptr, &dstrect) < 0)
        throw Error::sdl("rendering quit button texture");
}

void as::TextManager::render_score(SDL_Renderer *rend, SDL_Rect dstrect) {
    if (SDL_RenderCopy(rend, score_tex, nullptr, &dstrect) < 0)
        throw Error::sdl("rendering score texture");
}

void as::TextManager::render_diff(SDL_Renderer *rend, SDL_Rect dstrect) {
    if (SDL_RenderCopy(rend, diff_tex, nullptr, &dstrect) < 0)
        throw Error::sdl("rendering difficulty texture");
}

void as::TextManager::render_passed(SDL_Renderer *rend, SDL_Rect dstrect) {
    if (SDL_RenderCopy(rend, passed_tex, nullptr, &dstrect) < 0)
        throw Error::sdl("rendering passed texture");
}

void as::TextManager::update_score(SDL_Renderer *rend,
                                   unsigned int score,
                                   bool free) {
    if (free) SDL_DestroyTexture(score_tex);
    SDL_Surface *surf;
    if ((surf =
             TTF_RenderText_Blended(main_font,
                                    ("Score: " + std::to_string(score)).c_str(),
                                    {255, 255, 255, 255}))
        == nullptr)
        throw Error::sdl("rendering score text");
    if ((score_tex = SDL_CreateTextureFromSurface(rend, surf)))
        SDL_FreeSurface(surf);
}