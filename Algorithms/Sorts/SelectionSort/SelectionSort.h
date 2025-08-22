#pragma once
#include "Concepts/Comparable/Comparable.h"
#include <vector>


template <typename T>
requires IsComparable<T>
void selectionSort(std::vector<T> &arr) {
    for (size_t lPtr = 0; lPtr < arr.size() - 1; ++lPtr) {
        size_t minIndex = lPtr;
        for (size_t i = lPtr + 1; i < arr.size(); ++i) {
            if (arr[i] < arr[minIndex]) {
                minIndex = i;
            }
        }

        if (lPtr != minIndex) {
            std::swap(arr[lPtr], arr[minIndex]);
        }
    }
}