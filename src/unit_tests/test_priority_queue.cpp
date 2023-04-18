#include <catch.hpp>

#include <deque>
#include <vector>
#include <random>

#include "../priority_queue.h"

TEST_CASE("heap_sort") {
    std::mt19937 rnd(777);
    std::vector<int64_t> numbers;
    for (int64_t num = -500; num < 500; ++num) {
        numbers.push_back(num);
    }
    for (int64_t num = -300; num < 300; num += 2) {
        numbers.push_back(num);
    }
    std::vector<int64_t> sort_numbers(numbers);
    std::sort(sort_numbers.begin(), sort_numbers.end());
    std::shuffle(numbers.begin(), numbers.end(), rnd);
    {
        PriorityQueue<int64_t> heap(numbers);
        std::vector<int64_t> heap_sort_numbers;
        for (size_t i = 0; i < numbers.size(); ++i) {
            REQUIRE(!heap.Empty());
            auto val = heap.Top();
            heap_sort_numbers.push_back(heap.Pop());
            REQUIRE(heap_sort_numbers.back() == val);
        }
        REQUIRE(heap.Empty());
        REQUIRE(heap_sort_numbers == sort_numbers);
    }
    {
        PriorityQueue<std::pair<int64_t, size_t>, std::deque<std::pair<int64_t, size_t>>,
                      std::greater<std::pair<int64_t, size_t>>>
            heap;
        for (size_t i = 0; i < numbers.size(); ++i) {
            heap.Insert({numbers[i], i});
        }
        std::vector<int64_t> heap_sort_numbers;
        for (size_t i = 0; i < numbers.size(); ++i) {
            REQUIRE(!heap.Empty());
            auto val = heap.Top().first;
            heap_sort_numbers.push_back(heap.Pop().first);
            REQUIRE(heap_sort_numbers.back() == val);
        }
        REQUIRE(heap.Empty());
        std::reverse(sort_numbers.begin(), sort_numbers.end());
        REQUIRE(heap_sort_numbers == sort_numbers);
        std::reverse(sort_numbers.begin(), sort_numbers.end());
    }
}