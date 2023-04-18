#pragma once

#include <tuple>
#include <vector>

template <typename T, typename Container = std::vector<T>, typename Compare = std::less<typename Container::value_type>>
class PriorityQueue {
public:
    PriorityQueue() {
    }

    explicit PriorityQueue(size_t heap_size) {
        heap_.reserve(heap_size);
    }

    explicit PriorityQueue(const std::vector<T>& a) : PriorityQueue(a.size()) {
        for (const T& el : a) {
            Insert(el);
        }
    }

    void Insert(T val) {
        heap_.push_back(val);
        size_t vertex = heap_.size() - 1;
        while (Compare()(heap_[vertex], heap_[GetParent(vertex)])) {
            std::swap(heap_[GetParent(vertex)], heap_[vertex]);
            vertex = GetParent(vertex);
        }
    }

    T Pop() {
        T top_val = heap_[0];
        std::swap(heap_[0], heap_.back());
        heap_.pop_back();
        size_t vertex = 0;
        do {
            size_t left_child = vertex * 2 + 1;
            size_t right_child = vertex * 2 + 2;
            if (left_child >= heap_.size()) {
                break;
            }
            if (right_child >= heap_.size()) {
                if (!Compare()(heap_[left_child], heap_[vertex])) {
                    break;
                }
                std::swap(heap_[left_child], heap_[vertex]);
                vertex = left_child;
                continue;
            }
            if (!Compare()(heap_[left_child], heap_[vertex]) && !Compare()(heap_[right_child], heap_[vertex])) {
                break;
            }
            size_t swap_child = left_child;
            if (Compare()(heap_[right_child], heap_[left_child])) {
                swap_child = right_child;
            }
            std::swap(heap_[swap_child], heap_[vertex]);
            vertex = swap_child;
        } while (true);
        return top_val;
    }

    T Top() {
        return heap_[0];
    }

    size_t Size() {
        return heap_.size();
    }

    bool Empty() {
        return heap_.empty();
    }

private:
    static size_t GetParent(size_t vertex) {
        if (vertex == 0) {
            return 0;
        }
        return (vertex - 1) / 2;
    }

    Container heap_;
};