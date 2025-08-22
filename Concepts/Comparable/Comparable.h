#pragma once
#include <type_traits>
#include <concepts>


template <typename T>
concept IsComparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};