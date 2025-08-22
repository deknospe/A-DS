#pragma once
#include "Concepts/Comparable/Comparable.h"
#include <vector>


template <typename T>
requires IsComparable<T>
void lomutoQuickSort(std::vector<T> &arr, const size_t lPtr, const size_t rPtr) {
    if (rPtr - lPtr <= 1) {
        return;
    }

    const T &pivotElem = arr[rPtr - 1];
    size_t i = 0;
    for (size_t curPtr = 0; curPtr < rPtr; ++curPtr) {
        if (arr[curPtr] <= pivotElem) {
            std::swap(arr[i++], arr[curPtr]);
        }
    }

    lomutoQuickSort(arr, lPtr, i - 1);
    lomutoQuickSort(arr, i, rPtr);
}


template <typename T>
requires IsComparable<T>
void lomutoQuickSort(std::vector<T> &arr) {
    lomutoQuickSort(arr, 0, arr.size());
}