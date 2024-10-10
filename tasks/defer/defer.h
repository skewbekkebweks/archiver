#pragma once
#include <functional>

class Defer {
public:
    Defer(auto func) : func_(func) { // NOLINT
    }

    ~Defer() {
        func_();
    }
private:
    std::function<void()> func_;
};

#define COMBINE_HELPER(X, Y) X##Y
#define COMBINE(X, Y) COMBINE_HELPER(X, Y)
#define UNIQUE_NAME(name) COMBINE(name, __LINE__)
#define defer Defer UNIQUE_NAME(var_) = [&]() -> void

