#pragma once
#include "destructor_base.h"

template <typename T, bool trivial = std::is_trivially_move_constructible_v<T>&&
                          std::is_trivially_move_assignable_v<T>>
struct trivial_move_assign_base : destructor_base<T> {

  using destructor_base<T>::destructor_base;

  trivial_move_assign_base&
  operator=(trivial_move_assign_base&& other) noexcept(
      std::is_nothrow_move_assignable_v<T>&&
          std::is_nothrow_move_constructible_v<T>) {
    if (this == &other) {
      return *this;
    }

    if (other.is_present) {
      if (this->is_present) {
        this->data = std::move(other.data);
      } else {
        new (&this->data) T(std::move(other.data));
        this->is_present = true;
      }
    } else {
      this->reset();
    }

    return *this;
  }
};

template <typename T>
struct trivial_move_assign_base<T, true> : destructor_base<T> {
  using destructor_base<T>::destructor_base;
};

template <typename T, bool enabled_move = std::is_move_constructible_v<T>&&
                          std::is_move_assignable_v<T>>
struct move_assign_base {
  constexpr move_assign_base() = default;
  constexpr move_assign_base(const move_assign_base&) = default;
  constexpr move_assign_base(move_assign_base&&) = default;
  move_assign_base& operator=(const move_assign_base&) = default;
  move_assign_base& operator=(move_assign_base&&) = delete;
};

template <typename T>
struct move_assign_base<T, true> {};
