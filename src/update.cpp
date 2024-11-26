#include "game.h"

void as::Game::spawn_aliens() {
    for (int i = 0; i <= difficulty; i++) {
        aliens.push_back(Alien::spawn(alien_tex,
                                      rand_edge(),
                                      scrwidth,
                                      scrheight,
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
    play_btn.update(scrwidth / 2, scrheight / 2 - 50);
    quit_btn.update(scrwidth / 2, scrheight / 2 + 50);

    if (state == GameState::PLAYING) {
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
        if (passed >= 50) {
            state = GameState::LOST;
            text_manager.end_stats.update(
                rend,
                "final score: " + std::to_string(score)
                    + " / final difficulty: " + std::to_string(difficulty));
        }
        clicked = false;

        if (score_changed)
            text_manager.score.update(rend, "score: " + std::to_string(score));
        if (diff_changed)
            text_manager.diff.update(
                rend,
                "difficulty: " + std::to_string(difficulty));
        if (passed_changed)
            text_manager.passed.update(rend,
                                       "passed: " + std::to_string(passed));
    }
}
