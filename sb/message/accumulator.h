// sb/message/accumulator.h

#pragma once

/*
   Copyright (c) 2018-2024, Scott Bailey
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
       * Redistributions of source code must retain the above copyright
         notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above copyright
         notice, this list of conditions and the following disclaimer in the
         documentation and/or other materials provided with the distribution.
       * Neither the name of the <organization> nor the
         names of its contributors may be used to endorse or promote products
         derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL SCOTT BAILEY BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <deque>
#include <vector>
#include <cstdint>
#include <sb/message/detail/accum_iterator.ipp>

namespace sb::message {

/// The accumulator class is designed to allow intuitive, std container like access to some inbound stream of data. Containers of
/// byte sized data are pushed and popped to and from either end.

/// @TODO Add overview documentation.
/// @note T::value_type must be of size 1 and is enforced via static_assert.
/// @note T must have a data() function.
/// @note T must have a constructor with the signature T(size_t n) that will create an object of type T with n elements.
/// @tparam LIST  This is the internal storage container type and is templatated for various optimizations including speed and memory.
/// @tparam T  This is the external list type that is managed and the type that is used in push and pop calls.
template <typename T = std::vector<std::uint8_t>, typename LIST = std::deque<T>>
class accumulator final {
  using self_type = accumulator<T, LIST>;
  using list_t = LIST;

public:
  using value_type = T;                ///< Type of the container stored inside this object's list_t (m_list).
  using type = typename T::value_type; ///< Type of the object contained inside T (AKA self_type::value_type::value_type).

  using iterator = detail::accum_iterator<accumulator<T, LIST>>;             ///< mutable iterator.
  using const_iterator = detail::const_accum_iterator<accumulator<T, LIST>>; ///< const iterator.

public:
  /// Constructor
  accumulator();
  /// Destructor (non-virtual, this is a `final` class).
  ~accumulator();

  // Deleted copy functions.
  accumulator(const self_type&) = delete;
  self_type& operator=(const self_type&) = delete;

  // Deleted move functions.
  // @todo Implement move function.
  accumulator(const self_type&&) = delete;
  self_type& operator=(const self_type&&) = delete;

  /// Report the number of bytes managed by the accumulator.
  /// @return size in bytes of data stored inside this object.
  size_t size() const;
  /// @return true if the accumulator is empty.
  bool empty() const;
  /// Clear all data from the accumulator.
  void clear();

  /// Return a const reference to the first element in the accumulator's list.
  /// @throw std::out_of_range if accumulator is empty.
  const T& front() const;
  /// Return a const reference to the first element of size n or larger in the accumulator's list.
  /// @note If n is larger than the first element in the list, the first element is resized to n bytes. Data is removed from
  ///   neigboring elements until the first element is fully populated. Neighboring elements are resized or removed as necessary.
  /// @throw std::out_of_range if accumulator contains fewer than n bytes.
  /// @throw std::out_of_range if n == 0.
  /// @return const reference to a value_type of size n or greater
  const T& front(size_t n);

  /// Remove and return an element from the accumulator's list.
  /// @throw std::out_of_range if accumulator is empty.
  /// @return The value type object at the front of the list.
  T pop_front();
  /// Remove and return a number of bytes from the accumulator.
  /// @throw std::out_of_range if accumulator contains fewer than n bytes.
  /// @return A value type object containing n bytes from the front of the list.
  T pop_front(size_t n);
  /// push_front via copy.
  void push_front(const T& t);
  /// push_front via move.
  void push_front(T&& t);

  /// Return a const reference to the last element in the accumulator's list.
  /// @throw std::out_of_range if accumulator is empty.
  const T& back() const;
  /// Return a const reference to the last element of size n or larger in the accumulator's list.
  /// @note If n is larger than the last element in the list, the last element is resized to n bytes. Data is removed from
  ///   neigboring elements until the last element is fully populated. Neighboring elements are resized or removed as necessary.
  /// @throw std::out_of_range if accumulator contains fewer than n bytes.
  /// @throw std::out_of_range if n == 0.
  /// @return const reference to a value_type of size n or greater
  const T& back(size_t n);

  /// Remove and return an element from the accumulator's list.
  /// @throw std::out_of_range if accumulator is empty.
  /// @return The value type object at the back of the list.
  T pop_back();
  /// Remove and return a number of bytes from the accumulator.
  /// @throw std::out_of_range if accumulator contains fewer than n bytes.
  /// @return A value type object containing n bytes from the front of the list.
  T pop_back(size_t n);
  /// push_back via copy.
  void push_back(const T& t);
  /// push_back via move.
  void push_back(T&& t);

  /// Look at a single value
  /// @return  A single unit of data
  /// @throw std::out_of_range if accumulator contains n or fewer bytes.
  type operator[](size_t n) const;

  // iterator functionality
  // @warn pop and sized front and back calls will invalidate iterators.
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;

private:
  /// Remove and return the element at the front of the list.  Undefined behavior if the list is empty.
  /// @return The removed element
  T pop_front_i() noexcept;
  /// Remove and return the element at the back of the list.  Undefined behavior if the list is empty.
  /// @return The removed element
  T pop_back_i() noexcept;

  /// Resize the element at the front of the list to sz.  Later elements will be added, subtracted, or rezized to make room.
  /// @param sz  New size of next element.  Undefined behavior if sz >size.
  void resize_front_i(size_t sz) noexcept;
  /// Resize the element at the back of the list to sz.  Later elements will be added, subtracted, or rezized to make room.
  /// @param sz  New size of next element.  Undefined behavior if sz >size.
  void resize_back_i(size_t sz) noexcept;

  /// Ensure internal container size is adequate.
  void maybe_resize(size_t additional_elements);

  // iterator support
  friend iterator;
  friend const_iterator;

  void increment(iterator& i);
  void increment(const_iterator& i) const;
  void decrement(iterator& i);
  void decrement(const_iterator& i) const;

private:
  size_t m_size = 0;
  list_t m_list;

  // Ensure contained type is byte sized.
  static_assert(sizeof(typename T::value_type) == 1, "Accumulator cannot operate on containers with value_types of size > 1.");
};

} // namespace sb::message

#define sb_message_accumulator_h
#include <sb/message/detail/accumulator.ipp>
