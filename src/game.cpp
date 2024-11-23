#include "game.h"

#include <SDL_image.h>

#include "error.h"
#include "util.h"

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
            if (e.key.keysym.scancode == SDL_SCANCODE_F) {
                SDL_SetWindowFullscreen(
                    win,
                    SDL_GetWindowFlags(win) & SDL_WINDOW_FULLSCREEN_DESKTOP
                        ? 0
                        : SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
        }

        keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_ESCAPE]) running = false;
    }
}

void as::Game::spawn_aliens() {
    for (int i = 0; i <= difficulty; i++) {
        aliens.push_back(Alien::spawn(
            alien_tex,
            0,
            rand_float(0, static_cast<float>(scrheight) / Alien::SCALE),
            difficulty));
    }
}

int as::Game::update_aliens(std::uint64_t dt) {
    int hits = 0;
    for (Alien &alien : aliens) {
        if (clicked && alien.check_hit(click_x, click_y)) alien.hit();
        alien.update(dt, scrwidth, scrheight);
        if (alien.get_state() == AlienState::DEAD)
            hits++;
        else if (alien.get_state() == AlienState::PASSED) {
            passed++;
            passed_changed = true;
        }
    }
    aliens.erase(std::remove_if(aliens.begin(),
                                aliens.end(),
                                [](const Alien &a) {
                                    return a.get_state() == AlienState::DEAD
                                           || a.get_state()
                                                  == AlienState::PASSED;
                                }),
                 aliens.end());
    return hits;
}

void as::Game::update(std::uint64_t dt) {
    SDL_GetWindowSize(win, &scrwidth, &scrheight);

    score_changed = false;
    diff_changed = false;
    passed_changed = false;

    if (spawn_timer >= START_SPAWN_INTERVAL - difficulty * 100) {
        spawn_aliens();
        spawn_timer = 0;
    }
    spawn_timer += dt;

    int hits = update_aliens(dt);
    if (hits > 0) {
        score += hits * 2 - 1;
        score_changed = true;
    }
    if (score >= difficulty * 10) {
        difficulty++;
        diff_changed = true;
    }
    if (passed >= 50) state = GameState::LOST;
    clicked = false;
}

void as::Game::render() {
    if (score_changed)
        text_manager.score.update(rend, "Score: " + std::to_string(score));
    if (diff_changed)
        text_manager.diff.update(rend,
                                 "Difficulty: " + std::to_string(difficulty));
    if (passed_changed)
        text_manager.passed.update(rend, "Passed: " + std::to_string(passed));

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
