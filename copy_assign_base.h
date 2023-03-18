#pragma once
#include "move_ctor_base.h"

template <typename T, bool trivial = std::is_trivially_copy_assignable_v<T>&&
    std::is_trivially_copy_constructible_v<T>>
    struct trivial_copy_assign_base : trivial_move_ctor_base<T> {

    using trivial_move_ctor_base<T>::trivial_move_ctor_base;

    constexpr trivial_copy_assign_base(trivial_copy_assign_base&& other) noexcept(
        std::is_nothrow_move_constructible_v<T>) = default;

    trivial_copy_assign_base&
        operator=(trivial_copy_assign_base&& other) noexcept(
            std::is_nothrow_move_constructible_v<T>&&
            std::is_nothrow_move_assignable_v<T>) = default;

    trivial_copy_assign_base& operator=(trivial_copy_assign_base const& other) {
        if (this == &other) {
            return *this;
        }

        if (other.is_present) {
            if (this->is_present) {
                this->data = other.data;
            }
            else {
                new (&this->data) T(other.data);
                this->is_present = true;
            }
        }
        else {
            this->reset();
        }

        return *this;
    }
};

template <typename T>
struct trivial_copy_assign_base<T, true> : trivial_move_ctor_base<T> {
    using trivial_move_ctor_base<T>::trivial_move_ctor_base;
};

template <typename T, bool enabled_copy = std::is_copy_constructible_v<T>&&
    std::is_copy_assignable_v<T>>
    struct copy_assign_base {
    constexpr copy_assign_base() = default;
    constexpr copy_assign_base(copy_assign_base const&) = default;
    constexpr copy_assign_base(copy_assign_base&&) = default;
    copy_assign_base& operator=(copy_assign_base const&) = delete;
    copy_assign_base& operator=(copy_assign_base&&) = default;
};

template <typename T>
struct copy_assign_base<T, true> {};
