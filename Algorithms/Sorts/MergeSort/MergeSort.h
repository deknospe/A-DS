#pragma once
#include <type_traits>
#include <concepts>
#include <vector>
#include <algorithm>
#include <span>


template <typename T>
concept HasLessOperator = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
};


template <typename T>
requires HasLessOperator<T>
static void merge(std::vector<T> &arr, const std::span<T> &leftPart, const std::span<T> &rightPart) {
    size_t i = 0, j = 0, k = 0;
    while (i < leftPart.size() && j < rightPart.size()) {
        if (leftPart[i] < rightPart[j]) {
            arr[k++] = leftPart[i++];
        }
        else {
            arr[k++] = rightPart[j++];
        }
    }

    while (i < leftPart.size()) {
        arr[k++] = leftPart[i++];
    }

    while (j < rightPart.size()) {
        arr[k++] = rightPart[j++];
    }
}


template <typename T>
static void mergeSort(std::vector<T> &arr, std::vector<T> &buffer, const size_t lPtr, const size_t rPtr) {
    const size_t size = rPtr - lPtr;
    if (size <= 1) {
        return;
    }

    const size_t mid = lPtr + size / 2;

    mergeSort(arr, buffer, lPtr, mid);
    mergeSort(arr, buffer, mid, rPtr);

    const std::span<T> leftPart(arr.begin() + lPtr, mid - lPtr);
    const std::span<T> rightPart(arr.begin() + mid, rPtr - mid);

    merge(buffer, leftPart, rightPart);
    std::copy(buffer.begin(), buffer.begin() + rPtr - lPtr, arr.begin() + lPtr);
}


template <typename T>
void mergeSort(std::vector<T> &arr) {
    std::vector<T> buffer(arr.size());
    mergeSort(arr, buffer, 0, arr.size());
}
