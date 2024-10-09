#include "queue.h"

Node::Node(int32_t value, Node* next) : value(value), next(next) {}

Queue::Queue() : front_(nullptr), tail_(nullptr), size_(0) {}

Queue::~Queue() {
    Clear();
}

void Queue::Push(int32_t value) {
    Node* n = new Node{value, nullptr};

    if (Empty()) {
        front_ = n;
    } else {
        tail_->next = n;
    }
    tail_ = n;

    size_++;
}

void Queue::Pop() {
    if (Empty()) {
        return;
    }

    Node* to_delete = front_;
    front_ = front_->next;

    delete to_delete;
    size_--;

    if (Empty()) {
        tail_ = nullptr;
    }
}

int32_t Queue::Front() const {
    if (Empty()) {
        return 0;
    }

    return front_->value;
}

int32_t Queue::Size() const {
    return size_;
}

void Queue::Clear() {
    while (!Empty()) {
        Pop();
    }
    size_ = 0;
}

bool Queue::Empty() const {
    return size_ == 0;
}