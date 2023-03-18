#pragma once
#include "copy_ctor_base.h"
#include <cstddef>
#include <memory>
#include <type_traits>

template <typename T>
class optional : private move_ctor_base<T>,
    private move_assign_base<T>,
    private copy_ctor_base<T>,
    private copy_assign_base<T>,
    private trivial_copy_ctor_base<T> {
public:
    using trivial_copy_ctor_base<T>::trivial_copy_ctor_base;
    using trivial_copy_ctor_base<T>::reset;

    constexpr optional(nullopt_t) noexcept : optional() {}

    constexpr optional(T value_) : optional(in_place, std::move(value_)) {}

    optional& operator=(nullopt_t) noexcept {
        reset();
        return *this;
    }

    constexpr explicit operator bool() const noexcept {
        return this->is_present;
    }

    constexpr T& operator*() noexcept {
        return this->data;
    }

    constexpr T const& operator*() const noexcept {
        return this->data;
    }

    constexpr T* operator->() noexcept {
        return &(this->data);
    }

    constexpr T const* operator->() const noexcept {
        return &(this->data);
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        reset();
        new (&this->data) T(std::forward<Args>(args)...);
        this->is_present = true;
    }

    template <typename U>
    friend constexpr bool operator==(optional<U> const& a, optional<U> const& b);

    template <typename U>
    friend constexpr bool operator!=(optional<U> const& a, optional<U> const& b);

    template <typename U>
    friend constexpr bool operator<=(optional<U> const& a, optional<U> const& b);

    template <typename U>
    friend constexpr bool operator>=(optional<U> const& a, optional<U> const& b);

    template <typename U>
    friend constexpr bool operator<(optional<U> const& a, optional<U> const& b);

    template <typename U>
    friend constexpr bool operator>(optional<U> const& a, optional<U> const& b);
};

template <typename T>
constexpr bool operator==(optional<T> const& a, optional<T> const& b) {
    if (bool(a) != bool(b)) {
        return false;
    }
    return !bool(a) || *a == *b;
}

template <typename T>
constexpr bool operator!=(optional<T> const& a, optional<T> const& b) {
    if (bool(a) != bool(b)) {
        return true;
    }
    return bool(a) && *a != *b;
}

template <typename T>
constexpr bool operator<(optional<T> const& a, optional<T> const& b) {
    if (!bool(b))
        return false;
    if (!bool(a))
        return true;
    return *a < *b;
}

template <typename T>
constexpr bool operator<=(optional<T> const& a, optional<T> const& b) {
    if (!bool(a))
        return true;
    if (!bool(b))
        return false;
    return *a <= *b;
}

template <typename T>
constexpr bool operator>(optional<T> const& a, optional<T> const& b) {
    if (!bool(a))
        return false;
    if (!bool(b))
        return true;
    return *a > *b;
}

template <typename T>
constexpr bool operator>=(optional<T> const& a, optional<T> const& b) {
    if (!bool(b))
        return true;
    if (!bool(a))
        return false;
    return *a >= *b;
}
