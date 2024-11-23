#include "game.h"

#include <SDL_image.h>

#include "error.h"

as::Game::Game()
    : scrwidth(800),
      scrheight(600),
      state(GameState::PLAYING),
      running(false),
      click_x(-1),
      click_y(-1),
      spawn_timer(START_SPAWN_INTERVAL),
      score(0),
      difficulty(1),
      passed(0),
      score_changed(false),
      diff_changed(false),
      passed_changed(false) {
    init_sdl();
}

as::Game::~Game() {
    SDL_DestroyTexture(alien_tex);

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void as::Game::init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) throw Error::sdl("initializing SDL");
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        throw Error::sdl("initializing SDL_image");
    if (TTF_Init() == -1) throw Error::sdl("initializing SDL_ttf");
    if ((win = SDL_CreateWindow("AlienSlayer",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                scrwidth,
                                scrheight,
                                SDL_WINDOW_METAL | SDL_WINDOW_RESIZABLE))
        == nullptr)
        throw Error::sdl("creating SDL_Window");
    if ((rend = SDL_CreateRenderer(
             win,
             -1,
             SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
        == nullptr)
        throw Error::sdl("creating SDL_Renderer");

    if ((alien_tex = IMG_LoadTexture(rend, "../Resources/alien.png"))
        == nullptr)
        throw Error::sdl("initializing alien texture");

    text_manager.init(rend,
                      "../Resources/opensans.ttf",
                      score,
                      difficulty,
                      passed);
}

void as::Game::handle_events() {
    SDL_Event e;
    const Uint8 *keystate = nullptr;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT: running = false; break;
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT) {
                clicked = true;
                click_x = e.button.x;
                click_y = e.button.y;
            }
            break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE: running = false; break;
            case SDL_SCANCODE_F:
                SDL_SetWindowFullscreen(
                    win,
                    SDL_GetWindowFlags(win) & SDL_WINDOW_FULLSCREEN_DESKTOP
                        ? 0
                        : SDL_WINDOW_FULLSCREEN_DESKTOP);
                break;
            default: break;
            }
        }
    }
}

void as::Game::render() {
    if (SDL_RenderClear(rend) < 0) throw Error::sdl("clearing screen");

    for (Alien &alien : aliens)
        alien.render(rend);

    text_manager.score.render(rend, scrwidth / 4, 15);
    text_manager.diff.render(rend, scrwidth / 2, 15);
    text_manager.passed.render(rend, scrwidth * 3 / 4, 15);

    SDL_RenderPresent(rend);
}

void as::Game::start() {
    running = true;
    std::uint64_t now, dt;
    std::uint64_t prev = 0;

    while (running) {
        now = SDL_GetTicks64();
        dt = now - prev;
        prev = now;
        handle_events();
        update(dt);
        render();
    }
}
