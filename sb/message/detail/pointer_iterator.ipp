#pragma once

/// @TODO Add licensing, file overview, class overview, and function doc.

#include <iterator>

namespace sb::message::detail {

template <typename T>
class iterator {
  using self_type = iterator<T>;

public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

  explicit iterator(T* _p) : p(_p) {}
  iterator& operator++() {
    ++p;
    return *this;
  }
  iterator operator++(int) {
    iterator retval = *this;
    ++(*this);
    return retval;
  }
  iterator& operator--() {
    --p;
    return *this;
  }
  iterator operator--(int) {
    iterator retval = *this;
    --(*this);
    return retval;
  }
  bool operator==(const iterator& other) const { return p == other.p; }
  bool operator!=(const iterator& other) const { return !(*this == other); }
  const T& operator*() const { return *p; }
  T& operator*() { return *p; }
  self_type operator+(size_t n) {
    self_type retval = *this;
    retval.p += n;
    return retval;
  }
  self_type operator-(size_t n) {
    self_type retval = *this;
    retval.p -= n;
    return retval;
  }

private:
  T* p;
};

template <typename T>
class const_iterator {
  using self_type = const_iterator<T>;

public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

  explicit const_iterator(T* _p) : p(_p) {}
  const_iterator& operator++() {
    ++p;
    return *this;
  }
  const_iterator operator++(int) {
    const_iterator retval = *this;
    ++(*this);
    return retval;
  }
  const_iterator& operator--() {
    --p;
    return *this;
  }
  const_iterator operator--(int) {
    const_iterator retval = *this;
    --(*this);
    return retval;
  }
  bool operator==(const const_iterator& other) const { return p == other.p; }
  bool operator!=(const const_iterator& other) const { return !(*this == other); }
  const T& operator*() const { return *p; }
  T& operator*() { return *p; }
  self_type operator+(size_t n) {
    self_type retval = *this;
    retval.p += n;
    return retval;
  }
  self_type operator-(size_t n) {
    self_type retval = *this;
    retval.p -= n;
    return retval;
  }

private:
  T* p;
};

} // namespace sb::message::detail
