#pragma once
#include <vector>
#include "Concepts/Comparable/Comparable.h"


template <typename T>
requires IsComparable<T>
void insertionSort(std::vector<T> &arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        for (; i > 0 && arr[i] < arr[i - 1]; --i) {
            std::swap(arr[i], arr[i - 1]);
        }
    }
}
