#pragma once
#include <stdexcept>
#include <vector>


template <typename T, T (*func)(T, T), T (*inverseFunc)(T, T), T neutralElem> class FenwickTree {
private:
    std::vector<T> arr;

    [[nodiscard]] T get(size_t index) noexcept {
        --index;
        T res = neutralElem;
        for (; index + 1 >= 1; index = (index & (index + 1)) - 1) {
            res = func(res, this->arr[index]);
        }
        return res;
    }

public:
    explicit FenwickTree(const size_t size) noexcept
        : arr(std::vector<T>(size, neutralElem))
    {}

    explicit FenwickTree(const std::vector<T> &arr) noexcept
        : arr(std::vector<T>(arr.size(), neutralElem))
    {
        for (size_t index = 0; index < this->arr.size(); ++index) {
            for (size_t i = index & (index + 1); i <= index; ++i) {
                this->arr[index] = func(this->arr[index], arr[i]);
            }
        }
    }

    void set(const size_t index, const T &prevValue, const T &newValue) {
        if (index >= this->arr.size()) {
            throw std::out_of_range("FenwickTree: set(size_t index, T newValue) - index is out of range");
        }
        for (size_t i = index; i < this->arr.size(); i |= i + 1) {
            this->arr[i] = inverseFunc(this->arr[i], prevValue);
            this->arr[i] = func(this->arr[i], newValue);
        }
    }

    [[nodiscard]] T get(const size_t lPtr, const size_t rPtr) {
        if (lPtr >= rPtr || rPtr > this->arr.size()) {
            throw std::out_of_range("FenwickTree: get(size_t lPtr, size_t rPtr) - index is out of range");
        }
        return inverseFunc(this->get(rPtr), this->get(lPtr));
    }
};