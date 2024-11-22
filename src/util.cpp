#include "util.h"

#include <random>

const std::random_device dev;
const std::mt19937 rng(dev);

float as::rand_float(float low, float high) noexcept {
    return std::uniform_real_distribution<float>(low, high)(rng);
}

int as::rand_int(int low, int high) noexcept {
    return std::uniform_int_distribution<>(low, high)(rng);
}
