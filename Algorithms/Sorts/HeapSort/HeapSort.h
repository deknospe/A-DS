#pragma once
#include <concepts>
#include <type_traits>
#include <vector>


template <typename T>
concept IsComparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};


template <typename T>
requires IsComparable<T>
void heapSort(std::vector<T> &arr) {
    size_t n = arr.size();

    auto siftDown = [&](size_t index) {
        while (2 * index + 1 < n) {
            size_t j = 2 * index + 1;
            if (j + 1 < n && arr[j + 1] > arr[j]) {
                ++j;
            }
            if (arr[index] > arr[j]) {
                break;
            }
            std::swap(arr[index], arr[j]);
            index = j;
        }
    };

    for (size_t i = n / 2 - 1; i > 0; --i) {
        siftDown(i);
    }
    if (n != 0) [[likely]] {
        siftDown(0);
    }

    while (n > 0) {
        std::swap(arr[0], arr[--n]);
        siftDown(0);
    }
}