#pragma once
#include <vector>
#include <type_traits>
#include <concepts>


template <typename T>
concept IsComparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};


template <typename T>
requires IsComparable<T>
void insertionSort(std::vector<T> &arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        for (; i > 0 && arr[i] < arr[i - 1]; --i) {
            std::swap(arr[i], arr[i - 1]);
        }
    }
}
