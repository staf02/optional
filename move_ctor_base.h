#pragma once
#include "move_assign_base.h"

template <typename T, bool trivial = std::is_trivially_move_constructible_v<T>>
struct trivial_move_ctor_base : trivial_move_assign_base<T> {
  using trivial_move_assign_base<T>::trivial_move_assign_base;

  trivial_move_ctor_base& operator=(trivial_move_ctor_base&& other) noexcept(
      std::is_nothrow_move_assignable_v<T>&&
          std::is_nothrow_move_constructible_v<T>) = default;

  constexpr trivial_move_ctor_base(trivial_move_ctor_base&& other) noexcept(
      std::is_nothrow_move_constructible_v<T>) {
    if (other.is_present) {
      new (&this->data) T(std::move(other.data));
      this->is_present = true;
    }
  }
};

template <typename T>
struct trivial_move_ctor_base<T, true> : trivial_move_assign_base<T> {
  using trivial_move_assign_base<T>::trivial_move_assign_base;
};

template <typename T, bool enabled_move = std::is_move_constructible_v<T>>
struct move_ctor_base {
  constexpr move_ctor_base() = default;
  constexpr move_ctor_base(const move_ctor_base&) = default;
  constexpr move_ctor_base(move_ctor_base&&) = delete;
  move_ctor_base& operator=(const move_ctor_base&) = default;
  move_ctor_base& operator=(move_ctor_base&&) = delete;
};

template <typename T>
struct move_ctor_base<T, true> {};
