#pragma once

#include <algorithm>
#include <random>
#include <vector>

template<typename T>
inline void SimpleShuffle(std::vector<T> &vec) {
    static thread_local std::mt19937 rng(std::random_device{}());
    std::shuffle(vec.begin(), vec.end(), rng);
}