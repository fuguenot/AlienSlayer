#include "game.h"

#include <SDL_image.h>

#include "error.h"

as::Game::Game()
    : scrwidth(800),
      scrheight(600),
      play_btn("play button"),
      quit_btn("quit button"),
      again_btn("play again button"),
      menu_btn("menu button"),
      state(GameState::MENU) {
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

    play_btn.init(rend,
                  text_manager.btn_font,
                  "play",
                  scrwidth / 2,
                  scrheight / 2 - 50);
    quit_btn.init(rend,
                  text_manager.btn_font,
                  "quit",
                  scrwidth / 2,
                  scrheight / 2 + 50);
    again_btn.init(rend,
                   text_manager.btn_font,
                   "play again",
                   scrwidth / 2,
                   scrheight / 2 + 20);
    menu_btn.init(rend,
                  text_manager.btn_font,
                  "menu",
                  scrwidth / 2,
                  scrheight / 2 + 120);
}

void as::Game::handle_events() {
    SDL_Event e;
    const Uint8 *keystate = nullptr;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT: running = false; break;
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT) {
                switch (state) {
                case GameState::PLAYING:
                    clicked = true;
                    click_x = e.button.x;
                    click_y = e.button.y;
                    break;
                case GameState::MENU:
                    if (play_btn.inside(e.button.x, e.button.y)) {
                        reset();
                        state = GameState::PLAYING;
                    } else if (quit_btn.inside(e.button.x, e.button.y))
                        running = false;
                    break;
                case GameState::LOST:
                    if (again_btn.inside(e.button.x, e.button.y)) {
                        reset();
                        state = GameState::PLAYING;
                    } else if (menu_btn.inside(e.button.x, e.button.y))
                        state = GameState::MENU;
                    break;
                default: break;
                }
                break;
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
            case SDL_SCANCODE_P:
                if (state == GameState::PLAYING)
                    state = GameState::PAUSED;
                else if (state == GameState::PAUSED)
                    state = GameState::PLAYING;
                break;
            default: break;
            }
        }
    }
}

void as::Game::render() {
    if (SDL_RenderClear(rend) < 0) throw Error::sdl("clearing screen");

    if (state == GameState::MENU) {
        text_manager.title.render(rend, scrwidth / 2, 15);

        play_btn.render(rend);
        quit_btn.render(rend);
    } else {
        for (Alien &alien : aliens)
            alien.render(rend);

        text_manager.score.render(rend, scrwidth / 4, 15);
        text_manager.diff.render(rend, scrwidth / 2, 15);
        text_manager.passed.render(rend, scrwidth * 3 / 4, 15);

        if (state == GameState::PAUSED)
            text_manager.paused.render(rend, scrwidth / 2, scrheight / 2);
        else if (state == GameState::LOST) {
            text_manager.lost.render(rend, scrwidth / 2, scrheight / 2 - 150);
            text_manager.end_stats.render(rend,
                                          scrwidth / 2,
                                          scrheight / 2 - 25);
            again_btn.render(rend);
            menu_btn.render(rend);
        }
    }

    SDL_RenderPresent(rend);
}

void as::Game::reset() {
    text_manager.score.update(rend, "score: 0");
    text_manager.diff.update(rend, "difficulty: 1");
    text_manager.passed.update(rend, "passed: 0");

    clicked = false;

    spawn_timer = START_SPAWN_INTERVAL;
    aliens.clear();

    score = 0;
    difficulty = 1;
    passed = 0;
    score_changed = false;
    diff_changed = false;
    passed_changed = false;
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
