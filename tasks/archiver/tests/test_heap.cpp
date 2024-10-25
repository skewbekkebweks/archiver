#include "../src/heap.h"

#include <exception>
#include <iostream>

#include <functional>
#include <queue>

const int VALUES_COUNT = 10;

int main() {
    const std::string red = "\033[31m";
    const std::string green = "\033[32m";
    const std::string reset = "\033[0m";
    const std::string passed = "PASSED";
    const std::string failed = "FAILED";

    std::cout << "Heap Test" << std::endl;

    Heap<int, std::less<int>> custom_heap;
    std::priority_queue<int, std::vector<int>, std::greater<int>> std_queue;

    for (int i = VALUES_COUNT; i > 0; --i) {
        custom_heap.Push(i);
        std_queue.push(i);
    }

    std::cout << "Test 1: Compare top elements." << std::endl;
    if (custom_heap.Top() == std_queue.top()) {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << " Heap.Top() == " << custom_heap.Top() << " priority_queue.top() == " << std_queue.top() << reset << std::endl;
    }

    std::cout << "Test 2: Remove elements." << std::endl;
    while (custom_heap.Size() > 0) {
        if (custom_heap.Top() == std_queue.top()) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " Heap.Top() == " << custom_heap.Top() << " priority_queue.top() == " << std_queue.top() << reset << std::endl;
        }
        custom_heap.Pop();
        std_queue.pop();
    }

    std::cout << "Test 3: Check sizes." << std::endl;
    if (custom_heap.Size() == std_queue.size()) {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << "Heap.Size() == " << custom_heap.Size() << " priority_queue.size() == " << std_queue.size() << reset << std::endl;
    }

    std::cout << "Test 4: Try to pop from empty heap." << std::endl;
    try {
        custom_heap.Pop();
        std::cout << red << failed << " No exception when pop from empty heap" << reset << std::endl;
    } catch (const std::exception& e) {
        std::cout << green << passed << reset << std::endl;
    }

    std::cout << "Test 5: Check Top on empty heap." << std::endl;
    try {
        custom_heap.Top();
        std::cout << red << failed << " No exception when get top on empty heap" << reset << std::endl;
    } catch (const std::exception& e) {
        std::cout << green << passed << reset << std::endl;
    }
}