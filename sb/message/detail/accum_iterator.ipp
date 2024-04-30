#pragma once

/// @TODO Add licensing, file overview, class overview, and function doc.

#include <sb/message/detail/pointer_iterator.ipp>

// See:
//
//    http://en.cppreference.com/w/cpp/iterator/iterator
//    http://en.cppreference.com/w/cpp/concept/ForwardIterator

namespace sb::message::detail {

// There could probably be a const_iterator, a normal iterator, and a detail iterator (with only protected functions) that const
// and normal call. This might simplify this code.

template <typename T>
class accum_iterator {
  friend T;

public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = typename T::type;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

  explicit accum_iterator(T& _t, size_t _i, value_type* _p) : t{_t}, i{_i}, p{_p} {}
  accum_iterator& operator++() {
    t.increment(*this);
    return *this;
  }
  accum_iterator operator++(int) {
    accum_iterator retval = *this;
    ++(*this);
    return retval;
  }
  accum_iterator& operator--() {
    t.decrement(*this);
    return *this;
  }
  accum_iterator operator--(int) {
    accum_iterator retval = *this;
    ++(*this);
    return retval;
  }
  bool operator==(const accum_iterator& other) const { return p == other.p; }
  bool operator!=(const accum_iterator& other) const { return !(*this == other); }
  const value_type& operator*() const { return *p; }
  value_type& operator*() { return *p; }

private:
  T& t;          ///< ref to the accumulator
  size_t i;      ///< internal index for acumulator's use (it's the index into m_list)
  value_type* p; ///< pointer to the data
};

template <typename T>
class const_accum_iterator {
  friend T;

public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = typename T::type;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

  explicit const_accum_iterator(const T& _t, size_t _i, const value_type* _p) : t{_t}, i{_i}, p{_p} {}
  const_accum_iterator& operator++() {
    t.increment(*this);
    return *this;
  }
  const_accum_iterator operator++(int) {
    const_accum_iterator retval = *this;
    ++(*this);
    return retval;
  }
  const_accum_iterator& operator--() {
    t.decrement(*this);
    return *this;
  }
  const_accum_iterator operator--(int) {
    const_accum_iterator retval = *this;
    ++(*this);
    return retval;
  }
  bool operator==(const const_accum_iterator& other) const { return p == other.p; }
  bool operator!=(const const_accum_iterator& other) const { return !(*this == other); }
  const value_type& operator*() const { return *p; }

private:
  const T& t;
  size_t i;
  const value_type* p;
};

} // namespace sb::message::detail
