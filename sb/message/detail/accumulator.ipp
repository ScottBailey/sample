#ifndef sb_message_accumulator_h
#  error "sb/message/detail/accumulator.ipp must not be directly included"
#endif

#include <cassert>

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

#include <sstream>  // @todo replace stringstream with std::fmt.
#include <string.h> // memcpy

namespace sb {
namespace message {

//--- constructors/destructor ------------------------------------------------------------------------------------------

template <typename T, typename LIST>
inline accumulator<T, LIST>::accumulator() = default;

template <typename T, typename LIST>
inline accumulator<T, LIST>::~accumulator() = default;

//--- operators --------------------------------------------------------------------------------------------------------

template <typename T, typename LIST>
inline typename accumulator<T, LIST>::type accumulator<T, LIST>::operator[](size_t n) const {

  if (n >= m_size) {
    std::ostringstream ss;
    ss << "sb::message::accumulator<T,LIST>::operator[] : attempting to access element " << n << " of " << m_size << " elements.";
    throw std::out_of_range(ss.str());
  }

  for (const T& a : m_list) {
    if (n < a.size())
      return a[n];
    n -= a.size();
  }

  throw std::out_of_range("sb::message::accaccumulator<T,LIST>::operator[] : bad branch");
}

//--- alphabetic -------------------------------------------------------------------------------------------------------

template <typename T, typename LIST>
inline const T& accumulator<T, LIST>::back() const {
  if (m_list.empty())
    throw std::out_of_range("sb::message::accumulator<T,LIST> is empty, back() can not return a const ref to empty...");
  return m_list.back();
}

template <typename T, typename LIST>
inline const T& accumulator<T, LIST>::back(size_t n) {

  if (!n)
    throw std::out_of_range("back(n) where n = 0, can not return a const ref to empty...");

  if (m_size < n) {
    std::ostringstream ss;
    ss << "back() " << n << "elements when sb::message::accumulator<T,LIST> size is " << m_size;
    throw std::out_of_range(ss.str());
  }

  if (m_list.back().size() < n)
    resize_back_i(n);
  return m_list.back();
}

template <typename T, typename LIST>
inline typename accumulator<T, LIST>::iterator accumulator<T, LIST>::begin() noexcept {
  if (!m_size)
    return iterator{*this, 0, nullptr};
  return iterator{*this, 0, m_list.front().data()};
}

template <typename T, typename LIST>
inline typename accumulator<T, LIST>::const_iterator accumulator<T, LIST>::begin() const noexcept {
  return cbegin();
}

template <typename T, typename LIST>
inline typename accumulator<T, LIST>::const_iterator accumulator<T, LIST>::cbegin() const noexcept {
  if (!m_size)
    return const_iterator{*this, 0, nullptr};
  return const_iterator{*this, 0, m_list.front().data()};
}

template <typename T, typename LIST>
inline void accumulator<T, LIST>::clear() {
  m_list.clear();
  m_size = 0;
}

template <typename T, typename LIST>
inline void accumulator<T, LIST>::decrement(typename accumulator<T, LIST>::iterator& i) {

  assert(i.i < m_list.size());

  // Try to decrement within the current list item.
  {
    T& temp = m_list[i.i];
    if (i.p != temp.data()) {
      // Can decrement, so do it.
      --i.p;
      return;
    }
  }

  // Test for end.
  if (i.i == 0) {
    // end, set to nullptr.
    i.p = nullptr;
    return;
  }

  // Decrement the list index and find the updated data location.
  --i.i;
  T& temp = m_list[i.i];
  i.p = temp.data() + temp.size() - 1;
}

template <typename T, typename LIST>
inline void accumulator<T, LIST>::decrement(typename accumulator<T, LIST>::const_iterator& i) const {

  assert(i.i < m_list.size());

  // Try to decrement within the current list item.
  {
    const T& temp = m_list[i.i];
    if (i.p != temp.data()) {
      // Can decrement, so do it.
      --i.p;
      return;
    }
  }

  // Test for end.
  if (i.i == 0) {
    // end, set to nullptr.
    i.p = nullptr;
    return;
  }

  // Decrement the list index and find the updated data location.
  --i.i;
  const T& temp = m_list[i.i];
  i.p = temp.data() + temp.size() - 1;
}

template <typename T, typename LIST>
inline bool accumulator<T, LIST>::empty() const {
  return m_size == 0;
}

template <typename T, typename LIST>
inline typename accumulator<T, LIST>::iterator accumulator<T, LIST>::end() noexcept {
  return iterator{*this, m_list.size(), nullptr};
}

template <typename T, typename LIST>
inline typename accumulator<T, LIST>::const_iterator accumulator<T, LIST>::end() const noexcept {
  return const_iterator{*this, m_list.size(), nullptr};
}

template <typename T, typename LIST>
inline const T& accumulator<T, LIST>::front() const {
  if (m_list.empty())
    throw std::out_of_range("sb::message::accumulator<T,LIST> is empty, front() can not return a const ref to empty...");
  return m_list.front();
}

template <typename T, typename LIST>
inline const T& accumulator<T, LIST>::front(size_t n) {
  if (!n)
    throw std::out_of_range("front(n) where n = 0, can not return a const ref to empty...");

  if (m_size < n) {
    std::ostringstream ss;
    ss << "front() " << n << "elements when sb::message::accumulator<T,LIST> size is " << m_size;
    throw std::out_of_range(ss.str());
  }

  if (m_list.front().size() < n)
    resize_front_i(n);
  return m_list.front();
}

template <typename T, typename LIST>
inline void accumulator<T, LIST>::increment(typename accumulator<T, LIST>::iterator& i) {

  assert(i.i < m_list.size());

  T& temp = m_list[i.i];
  type* temp_end = temp.data() + temp.size();
  ++i.p;
  if (i.p < temp_end)
    return;

  ++i.i;
  if (i.i >= m_list.size())
    i.p = nullptr;
  else
    i.p = m_list[i.i].data();
}

template <typename T, typename LIST>
inline void accumulator<T, LIST>::increment(typename accumulator<T, LIST>::const_iterator& i) const {

  assert(i.i < m_list.size());

  const T& temp = m_list[i.i];
  const type* temp_end = temp.data() + temp.size();
  ++i.p;
  if (i.p < temp_end)
    return;

  ++i.i;
  if (i.i >= m_list.size())
    i.p = nullptr;
  else
    i.p = m_list[i.i].data();
}

template <typename T, typename LIST>
inline T accumulator<T, LIST>::pop_back() {

  if (m_list.empty())
    throw std::out_of_range("sb::message::accumulator<T,LIST> is empty, can not pop_back() when empty...");

  return pop_back_i();
}

template <typename T, typename LIST>
inline T accumulator<T, LIST>::pop_back(size_t n) {

  if (!n)
    return T{};

  if (m_size < n) {
    std::ostringstream ss;
    ss << "pop_back() " << n << "elements when sb::message::accumulator<T,LIST> size is " << m_size;
    throw std::out_of_range(ss.str());
  }

  resize_back_i(n);
  return pop_back_i();
}

template <typename T, typename LIST>
inline T accumulator<T, LIST>::pop_back_i() noexcept {

  assert(!m_list.empty());

  T rv = std::move(m_list.back());
  m_size -= rv.size();
  m_list.pop_back();
  return rv;
}

template <typename T, typename LIST>
inline T accumulator<T, LIST>::pop_front() {

  if (m_list.empty())
    throw std::out_of_range("sb::message::accumulator<T,LIST> is empty, can not pop_front() when empty...");

  return pop_front_i();
}

template <typename T, typename LIST>
inline T accumulator<T, LIST>::pop_front(size_t n) {

  if (!n)
    return T{};

  if (m_size < n) {
    std::ostringstream ss;
    ss << "pop_front() " << n << "elements when sb::message::accumulator<T,LIST> size is " << m_size;
    throw std::out_of_range(ss.str());
  }

  resize_front_i(n);
  return pop_front_i();
}

template <typename T, typename LIST>
inline T accumulator<T, LIST>::pop_front_i() noexcept {

  assert(!m_list.empty());

  T rv = std::move(m_list.front());
  m_size -= rv.size();
  m_list.pop_front();
  return rv;
}

template <typename T, typename LIST>
inline void accumulator<T, LIST>::maybe_resize(size_t additional_elements) {

#if defined(BOOST_CIRCULAR_BUFFER_HPP)
  if constexpr (std::is_same_v<LIST, boost::circular_buffer_space_optimized<T>>) {
    if (m_list.reserve() < additional_elements) {
      size_t needed = m_size + additional_elements;
      m_list.set_capacity(needed);
    }
  }
#endif
}

template <typename T, typename LIST>
inline void accumulator<T, LIST>::push_back(const T& t) {

  if (t.empty())
    return;
  m_size += t.size();
  maybe_resize(1);
  m_list.push_back(t);
}

template <typename T, typename LIST>
inline void accumulator<T, LIST>::push_back(T&& t) {

  if (t.empty())
    return;
  m_size += t.size();
  maybe_resize(1);
  m_list.push_back(std::forward<T, LIST>(t));
}

template <typename T, typename LIST>
inline void accumulator<T, LIST>::push_front(const T& t) {

  if (t.empty())
    return;
  m_size += t.size();
  maybe_resize(1);
  m_list.push_front(t);
}

template <typename T, typename LIST>
inline void accumulator<T, LIST>::push_front(T&& t) {

  if (t.empty())
    return;
  m_size += t.size();
  maybe_resize(1);
  m_list.push_front(std::forward<T, LIST>(t));
}

template <typename T, typename LIST>
void accumulator<T, LIST>::resize_back_i(size_t sz) noexcept {

  assert(sz <= m_size);
  assert(!m_list.empty());

  // Do nothing if back is already correctly sized.
  if (m_list.back().size() == sz)
    return;

  T t(sz); // target/replacement, will be pushed to the back after other values are popped.
  size_t remain(sz);
  while (remain) {

    size_t s2 = m_list.back().size();                           // size of the back element
    if (s2 <= remain) {                                         // If all of back fits in the target
      remain -= s2;                                             // Update remain
      memcpy(t.data() + remain, m_list.back().data(), s2);      // Copy the data.
      m_list.pop_back();                                        // Remove the element.
    } else {                                                    // We're going to need to move some, but not all.
      T extra(s2 - remain);                                     // This data is pushed back before target.
      memcpy(extra.data(), m_list.back().data(), extra.size()); // Copy from back to extra.

      memcpy(t.data(), m_list.back().data() + extra.size(), remain); // Copy the remaining bytes from back to target.
      // remain = 0;                                                  // Optimize out with a break.

      m_list.pop_back();                  // Remove back. It became extra and target.
      m_list.push_back(std::move(extra)); // Push in extra.
      break;                              // ...and break.
    }
  }

  maybe_resize(1);
  m_list.push_back(std::move(t)); // Push in target/replacement.
}

template <typename T, typename LIST>
void accumulator<T, LIST>::resize_front_i(size_t sz) noexcept {
  assert(sz <= m_size);
  assert(!m_list.empty());

  // do nothing if front() is already correctly sized
  if (m_list.front().size() == sz)
    return;

  T t(sz);       // storage for the future front
  size_t sum{0}; // running total (sum) of bytes copied to t
  while (sum < sz) {

    size_t s2 = m_list.front().size();                     // size of the current front
    size_t need = sz - sum;                                // bytes needed to complete this process
    if (need >= s2) {                                      // test: do we need at least as many bytes or more than front() has?
      memcpy(t.data() + sum, m_list.front().data(), s2);   // copy all data from front() to appropriate location in t
      m_list.pop_front();                                  // remove the now copied object
      sum += s2;                                           // increase the running total of bytes copied to t
    } else {                                               // this is the case where we need less bytes than front() has, so we will need to push the tail back in
      memcpy(t.data() + sum, m_list.front().data(), need); // copy need(ed) data from front() to appropriate location in t

      T extra(s2 - need);                                               // create a new item sized to the remainder of data in front()
      memcpy(extra.data(), m_list.front().data() + need, extra.size()); // copy remainder of data from front() to new item
      m_list.pop_front();                                               // remove the now fully copied item
      m_list.push_front(std::move(extra));                              // push the new item with the remaining data to the front
      // sum += need;                                                  // increase the running total of bytes copied to t (optimized to break)
      break; // optimize loop exit to remove add and test
    }
  }

  maybe_resize(1);
  m_list.push_front(std::move(t)); // push the 'future front item' to the front, and we are done
}

template <typename T, typename LIST>
inline size_t accumulator<T, LIST>::size() const {
  return m_size;
}

} // namespace message
} // namespace sb
