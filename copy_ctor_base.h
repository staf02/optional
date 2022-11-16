#pragma once
#include "copy_assign_base.h"

template <typename T, bool trivial = std::is_trivially_copy_constructible_v<T>>
struct trivial_copy_ctor_base : trivial_copy_assign_base<T> {
  using trivial_copy_assign_base<T>::trivial_copy_assign_base;

  constexpr trivial_copy_ctor_base(trivial_copy_ctor_base&& other) noexcept(
      std::is_nothrow_move_constructible_v<T>) = default;

  constexpr trivial_copy_ctor_base(
      trivial_copy_ctor_base const&
          other) noexcept(std::is_nothrow_copy_constructible_v<T>) {
    if (other.is_present) {
      new (&this->data) T(other.data);
      this->is_present = true;
    }
  }

  trivial_copy_ctor_base&
  operator=(trivial_copy_ctor_base const& other) = default;

  trivial_copy_ctor_base& operator=(trivial_copy_ctor_base&& other) noexcept(
      std::is_nothrow_move_constructible_v<T>&&
          std::is_nothrow_move_assignable_v<T>) = default;
};

template <typename T>
struct trivial_copy_ctor_base<T, true> : trivial_copy_assign_base<T> {
  using trivial_copy_assign_base<T>::trivial_copy_assign_base;
};

template <typename T, bool enabled_copy = std::is_copy_constructible_v<T>>
struct copy_ctor_base {
  constexpr copy_ctor_base() = default;

  constexpr copy_ctor_base(const copy_ctor_base&) = delete;
  constexpr copy_ctor_base(copy_ctor_base&&) = default;

  copy_ctor_base& operator=(const copy_ctor_base&) = delete;
  copy_ctor_base& operator=(copy_ctor_base&&) = default;
};

template <typename T>
struct copy_ctor_base<T, true> {};
