#include "game.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include "error.h"
#include "util.h"

as::Game::Game() : running(false), click_x(-1), click_y(-1) {
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
                                SCRWIDTH,
                                SCRHEIGHT,
                                SDL_WINDOW_METAL))
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
}

void as::Game::handle_events() {
    SDL_Event e;
    const Uint8 *keystate = nullptr;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT: running = false; break;
        }

        keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_ESCAPE]) running = false;
    }
}

void as::Game::update(std::uint64_t dt) {
    for (Alien &alien : aliens)
        alien.update(dt);
}

void as::Game::render() {
    if (SDL_RenderClear(rend) < 0) throw Error::sdl("clearing screen");

    for (Alien &alien : aliens)
        alien.render(rend);

    SDL_RenderPresent(rend);
}

void as::Game::start() {
    running = true;
    std::uint64_t now, dt;
    std::uint64_t prev = 0;

    // debug
    aliens.push_back(Alien::spawn(alien_tex,
                                  SCRWIDTH / 2 / Alien::SCALE,
                                  SCRHEIGHT / 2 / Alien::SCALE,
                                  0));

    while (running) {
        now = SDL_GetTicks64();
        dt = now - prev;
        prev = now;
        handle_events();
        update(dt);
        render();
    }
}
