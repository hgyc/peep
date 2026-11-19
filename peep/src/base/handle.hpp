#pragma once

#include <windows.h>
#include <utility>

class Handle {
public:
    Handle() noexcept = default;

    explicit Handle(HANDLE h) noexcept : h_(h) {}

    ~Handle() { reset(); }

    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;

    Handle(Handle&& other) noexcept
        : h_(std::exchange(other.h_, INVALID_HANDLE_VALUE)) {
    }

    Handle& operator=(Handle&& other) noexcept {
        if (this != &other) {
            reset();
            h_ = std::exchange(other.h_, INVALID_HANDLE_VALUE);
        }
        return *this;
    }

    void reset(HANDLE h = INVALID_HANDLE_VALUE) noexcept {
        if (valid()) {
            CloseHandle(h_);
        }
        h_ = h;
    }

    [[nodiscard]] bool valid() const noexcept {
        return h_ != nullptr && h_ != INVALID_HANDLE_VALUE;
    }

    [[nodiscard]] HANDLE get() const noexcept { return h_; }

    HANDLE release() noexcept {
        return std::exchange(h_, INVALID_HANDLE_VALUE);
    }

private:
    HANDLE h_ = INVALID_HANDLE_VALUE;
};