#pragma once
#include <stdexcept>
#include <vector>
#include <bit>

template <typename T, T(*func)(const T&, const T&)> class SparseTable {
private:
    std::vector<std::vector<T>> arr;
public:
    explicit SparseTable(const std::vector<T> &arr) noexcept
        : arr(std::bit_width(arr.size() - 1U) + 1, std::vector<T>(arr.size()))
    {
        this->arr[0] = arr;
        for (size_t level = 1; level < this->arr.size(); ++level) {
            size_t i;
            for (i = 1 << (level - 1); i < this->arr[level].size(); i += 1 << level) {
                this->arr[level][i] = this->arr[0][i];
                this->arr[level][i - 1] = this->arr[0][i - 1];
                for (size_t j = i - 1; j > i - (1 << (level - 1));) {
                    --j;
                    this->arr[level][j] = func(this->arr[0][j], this->arr[level][j + 1]);
                }
                for (size_t j = 1; i + j < this->arr[level].size() && j < (1 << (level - 1)); ++j) {
                    this->arr[level][i + j] = func(this->arr[0][i + j], this->arr[level][i + j - 1]);
                }
            }
            if (i - (1 << (level - 1)) < this->arr[level].size()) {
                i -= 1 << (level - 1);
                this->arr[level].back() = this->arr[0].back();
                for (size_t j = this->arr[0].size() - 1; j > i;) {
                    --j;
                    this->arr[level][j] = func(this->arr[0][j], this->arr[level][j + 1]);
                }
            }
        }
    }

    T get(const size_t lPtr, const size_t rPtr) {
        if (lPtr > rPtr || rPtr >= this->arr[0].size()) {
            throw std::out_of_range("SparseTable: get(size_t lPtr, size_t rPtr) - index is out of range");
        }
        const size_t level = std::bit_width(rPtr ^ lPtr);
        if (level == 0U) {
            return this->arr[0][lPtr];
        }
        return func(this->arr[level][lPtr], this->arr[level][rPtr]);
    }
};
