#pragma once
#include "Algorithms/RandomUtils/RandomUtils.h"
#include "Concepts/Comparable/Comparable.h"
#include <vector>


template <typename T>
requires IsComparable<T>
void hoareQuickSort(std::vector<T> &arr, const size_t lPtr, const size_t rPtr) {
    if (rPtr - lPtr <= 1) {
        return;
    }

    const T &pivotElem = arr[getRandomNumber(lPtr, rPtr)];
    size_t i = lPtr, j = rPtr - 1;
    while (i <= j) {
        while (arr[i] < pivotElem) ++i;
        while (arr[j] > pivotElem) --j;
        if (i <= j) {
            std::swap(arr[i++], arr[j--]);
        }
    }

    hoareQuickSort(arr, lPtr, j + 1);
    hoareQuickSort(arr, i, rPtr);
}


template <typename T>
requires IsComparable<T>
void hoareQuickSort(std::vector<T> &arr) {
    hoareQuickSort(arr, 0, arr.size());
}