#pragma once
#include <random>
#include <concepts>
#include <type_traits>


template <typename T>
requires std::is_integral_v<T>
T getRandomNumber(const T l, const T r) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<T> distrib(l, r - 1);
    return distrib(gen);
}