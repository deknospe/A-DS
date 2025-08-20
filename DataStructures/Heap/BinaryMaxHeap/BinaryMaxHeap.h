#pragma once
#include <vector>
#include <stdexcept>


template <typename T>
class BinaryMaxHeap {
public:
    BinaryMaxHeap() noexcept = default;

    explicit BinaryMaxHeap(const std::vector<T> &arr) noexcept
        : arr(arr)
    {
        for (size_t i = this->arr.size() / 2; i > 0; --i) {
            this->siftDown(i);
        }
        if (!this->arr.empty()) [[likely]] {
            this->siftDown(0);
        }
    }

    T getMax() const noexcept {
        return this->arr.front();
    }

    void removeMax() {
        if (this->isEmpty()) [[unlikely]] {
            throw std::out_of_range("Heap is empty");
        }
        this->arr.front() = std::move(this->arr.back());
        this->arr.pop_back();
        this->siftDown(0);
    }

    void insert(const T &elem) {
        this->arr.push_back(elem);
        this->siftUp(this->arr.size() - 1);
    }

    T extractMax()
    {
        const T minElem = std::move(this->arr.front());
        this->arr.front() = std::move(this->arr.back());
        this->arr.pop_back();
        this->siftDown(0);
        return minElem;
    }

    bool isEmpty() const noexcept {
        return this->arr.empty();
    }

private:
    std::vector<T> arr;

    void siftUp(size_t index) noexcept {
        while (index > 0 && this->arr[index] > this->arr[(index - 1) / 2]) {
            std::swap(this->arr[index], this->arr[(index - 1) / 2]);
            index = (index - 1) / 2;
        }
    }

    void siftDown(size_t index) noexcept {
        while (2 * index + 1 < this->arr.size()) {
            size_t j = 2 * index + 1;
            if (j + 1 < this->arr.size() && this->arr[j + 1] > this->arr[j]) {
                ++j;
            }
            if (this->arr[index] > this->arr[j]) {
                break;
            }
            std::swap(this->arr[index], this->arr[j]);
            index = j;
        }
    }
};