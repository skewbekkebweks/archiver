#pragma once
#include <functional>

class Defer {
public:
    Defer(auto func) : func_(func) {  // NOLINT
    }

    ~Defer() {
        func_();
    }

private:
    std::function<void()> func_;
};

#define CONCATENATE_HELPER(X, Y) X##Y
#define CONCATENATE(X, Y) CONCATENATE_HELPER(X, Y)
#define UNIQUE_NAME(name) CONCATENATE(name, __LINE__)
#define defer Defer UNIQUE_NAME(var_) = [&]() -> void
