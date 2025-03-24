#pragma once
#include <vector>
#include <stdexcept>


template <typename T, T(*func)(T, T), T neutralElem, T(*pushFunc)(T, T), T pushNeutralElem> class ArrayBasedMassSegmentTree {
private:
    static size_t getNearestDegreeOfTwo(size_t num) {
        if ((num & (num - 1)) == 0) {
            return num;
        }
        num |= num >> 1;
        num |= num >> 2;
        num |= num >> 4;
        num |= num >> 8;
        num |= num >> 16;
        num |= num >> 32;
        return num + 1;
    }

    struct Node {
        T value;
        T pushValue;
        size_t lProjection;
        size_t rProjection;

        explicit Node(const T &value, const T &pushValue, const size_t l, const size_t r)
            : value(value)
            , pushValue(pushValue)
            , lProjection(l)
            , rProjection(r)
        {}

        explicit Node() = default;
    };

    size_t size;
    std::vector<Node*> arr;

    void push(const size_t index) noexcept {
        this->arr[2 * index]->pushValue = pushFunc(this->arr[2 * index]->pushValue, this->arr[index]->pushValue);
        this->arr[2 * index + 1]->pushValue = pushFunc(this->arr[2 * index + 1]->pushValue, this->arr[index]->pushValue);
        this->arr[index]->pushValue = pushNeutralElem;
    }

    void set(const size_t index, const T &newValue, const size_t curIndex) noexcept {
        if (index == curIndex) {
            this->arr[index]->value = newValue;
            this->arr[index]->pushValue = pushNeutralElem;
            return;
        }
        this->push(curIndex);
        const size_t mid = (this->arr[curIndex]->lProjection + this->arr[curIndex]->rProjection) >> 1;
        if (index < mid) {
            this->set(index, newValue, 2 * curIndex);
        }
        else {
            this->set(index, newValue, 2 * curIndex + 1);
        }
        this->arr[curIndex]->value = func(pushFunc(this->arr[2 * curIndex]->value, this->arr[2 * curIndex]->pushValue),
            pushFunc(this->arr[2 * curIndex + 1]->value, this->arr[2 * curIndex + 1]->pushValue));
    }

    void massUpdate(const size_t lPtr, const size_t rPtr, const T &value, const size_t curIndex) noexcept {
        if (this->arr[curIndex]->lProjection >= rPtr || this->arr[curIndex]->rProjection <= lPtr) {
            return;
        }
        if (lPtr <= this->arr[curIndex]->lProjection && this->arr[curIndex]->rProjection <= rPtr) {
            this->arr[curIndex]->pushValue = pushFunc(this->arr[curIndex]->pushValue, value);
            return;
        }
        this->push(curIndex);
        this->massUpdate(lPtr, rPtr, value, 2 * curIndex);
        this->massUpdate(lPtr, rPtr, value, 2 * curIndex + 1);
        this->arr[curIndex]->value = func(pushFunc(this->arr[2 * curIndex]->value, this->arr[2 * curIndex]->pushValue),
            pushFunc(this->arr[2 * curIndex + 1]->value, this->arr[2 * curIndex + 1]->pushValue));
    }

    T get(const size_t lPtr, const size_t rPtr, const size_t curIndex) {
        if (this->arr[curIndex]->lProjection >= rPtr || this->arr[curIndex]->rProjection <= lPtr) {
            return neutralElem;
        }
        if (lPtr <= this->arr[curIndex]->lProjection && this->arr[curIndex]->rProjection <= rPtr) {
            return pushFunc(this->arr[curIndex]->value, this->arr[curIndex]->pushValue);
        }
        this->push(curIndex);
        const T result = func(this->get(lPtr, rPtr, 2 * curIndex), this->get(lPtr, rPtr, (2 * curIndex)  + 1));
        this->arr[curIndex]->value = func(pushFunc(this->arr[2 * curIndex]->value, this->arr[2 * curIndex]->pushValue),
            pushFunc(this->arr[2 * curIndex + 1]->value, this->arr[2 * curIndex + 1]->pushValue));
        return result;
    }
public:
    explicit ArrayBasedMassSegmentTree(const std::vector<T> &arr) noexcept
        : size(getNearestDegreeOfTwo(arr.size()))
        , arr(std::vector<Node*>(2 * this->size))
    {
        for (size_t i = 0; i < arr.size(); ++i) {
            this->arr[i + this->size] = new Node(arr[i], pushNeutralElem, i + this->size, i + 1 + this->size);
        }
        for (size_t i = arr.size(); i < this->size; ++i) {
            this->arr[i + this->size] = new Node(neutralElem, pushNeutralElem, i + this->size, i + 1 + this->size);
        }
        for (size_t i = this->size; i > 1;) {
            --i;
            this->arr[i] = new Node();
            this->arr[i]->lProjection = this->arr[i << 1]->lProjection;
            this->arr[i]->rProjection = this->arr[(i << 1) + 1]->rProjection;
            this->arr[i]->value = func(this->arr[i << 1]->value, this->arr[(i << 1) + 1]->value);
            this->arr[i]->pushValue = pushNeutralElem;
        }
    }

    explicit ArrayBasedMassSegmentTree(const size_t size) noexcept
         : size(getNearestDegreeOfTwo(size))
         , arr(std::vector<Node*>(2 * this->size))
    {
        for (size_t i = 0; i < this->size; ++i) {
            this->arr[i + this->size] = new Node(neutralElem, pushNeutralElem, i + this->size, i + 1 + this->size);
        }
        for (size_t i = this->size; i > 1;) {
            --i;
            this->arr[i] = new Node();
            this->arr[i]->lProjection = this->arr[i << 1]->lProjection;
            this->arr[i]->rProjection = this->arr[(i << 1) + 1]->rProjection;
            this->arr[i]->value = func(this->arr[i << 1]->value, this->arr[(i << 1) + 1]->value);
            this->arr[i]->pushValue = pushNeutralElem;
        }
    }

    void set(const size_t index, const T& newValue) {
        if (index >= this->size) {
            throw std::out_of_range("ArrayBasedMassSegmentTree: set(size_t index, T newValue) - index is out of range");
        }
        this->set(index + this->size, newValue, 1);
    }

    void massUpdate(const size_t lPtr, const size_t rPtr, const T& value) {
        if (lPtr >= rPtr || rPtr > this->size) {
            throw std::out_of_range("ArrayBasedMassSegmentTree: massUpdate(size_t lPtr, size_t rPtr, T value) - index is out of range");
        }
        this->massUpdate(lPtr + this->size, rPtr + this->size, value, 1);
    }

    [[nodiscard]] T get(const size_t lPtr, const size_t rPtr) {
        if (lPtr >= rPtr || rPtr > this->size) {
            throw std::out_of_range("ArrayBasedMassSegmentTree: get(size_t lPtr, size_t rPtr) - index is out of range");
        }
        return this->get(lPtr + this->size, rPtr + this->size, 1);
    }
};

