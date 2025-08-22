#pragma once
#include "Concepts/Comparable/Comparable.h"
#include <vector>


template <typename T>
requires IsComparable<T>
void bubbleSort(std::vector<T> &arr) {
    bool isSorted;
    for (size_t rPtr = arr.size(); rPtr > 0; --rPtr) {
        isSorted = true;
        for (size_t lPtr = 1; lPtr < rPtr; ++lPtr) {
            if (arr[lPtr - 1] > arr[lPtr]) {
                std::swap(arr[lPtr - 1], arr[lPtr]);
                isSorted = false;
            }
        }

        if (isSorted) {
            return;
        }
    }
}