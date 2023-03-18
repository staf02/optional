#pragma once
#include <cstddef>
#include <memory>
#include <type_traits>

struct nullopt_t {};
inline constexpr nullopt_t nullopt;

struct in_place_t {};
inline constexpr in_place_t in_place;

template <typename T, bool trivial = std::is_trivially_destructible_v<T>>
struct destructor_base {
    constexpr destructor_base() noexcept : is_present(false), dummy(0) {}

    template <typename... Args>
    explicit constexpr destructor_base(in_place_t, Args&&... args)
        : is_present(true), data(std::forward<Args>(args)...) {}

    ~destructor_base() {
        reset();
    }

    void reset() {
        if (this->is_present) {
            this->data.~T();
            this->is_present = false;
        }
    }

    bool is_present;
    union {
        T data;
        char dummy;
    };
};

template <typename T>
struct destructor_base<T, true> {
    constexpr destructor_base() noexcept : is_present(false), dummy(0) {}

    template <typename... Args>
    explicit constexpr destructor_base(in_place_t, Args&&... args)
        : is_present(true), data(std::forward<Args>(args)...) {}

    ~destructor_base() = default;

    void reset() {
        this->is_present = false;
    }

    bool is_present;
    union {
        T data;
        char dummy;
    };
};
