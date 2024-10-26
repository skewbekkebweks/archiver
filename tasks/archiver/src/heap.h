#include <vector>
#include "trie.h"

class HeapIsEmpty : public std::exception {
public:
    const char* what() const noexcept override {
        return "Heap is empty";
    }
};

template <typename T, typename Comparator>
class Heap {
public:
    Heap() : n_(0), heap_(std::vector<T>()) {
    }

    int Size() {
        return n_;
    }

    T Top() {
        if (n_ == 0) {
            throw HeapIsEmpty{};
        }
        return heap_[0];
    }

    void Pop() {
        if (n_ == 0) {
            throw HeapIsEmpty{};
        }
        std::swap(heap_[0], heap_[n_ - 1]);
        n_--;
        heap_.pop_back();
        ShiftDown(0);
    }

    void Push(T value) {
        n_++;
        heap_.push_back(value);

        ShiftUp(n_ - 1);
    }

private:
    int n_;
    std::vector<T> heap_;

    int GetParent(int vertex) {
        return (vertex - 1) / 2;
    }

    void ShiftUp(int vertex) {
        while (vertex > 0 && Comparator()(heap_[vertex], heap_[GetParent(vertex)])) {
            std::swap(heap_[vertex], heap_[GetParent(vertex)]);
            vertex = GetParent(vertex);
        }
    }

    void ShiftDown(int vertex) {
        while (2 * vertex + 1 < n_) {
            int left_index = 2 * vertex + 1;
            int right_index = 2 * vertex + 2;
            int min_value_index =
                (right_index < n_ && Comparator()(heap_[right_index], heap_[left_index]) ? right_index : left_index);
            if (!Comparator()(heap_[min_value_index], heap_[vertex])) {
                break;
            }
            std::swap(heap_[vertex], heap_[min_value_index]);
            vertex = min_value_index;
        }
    }
};