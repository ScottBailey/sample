// sb/devector.h
// Set the type for double ended queue.
//
// You'll want to experiment with std::deque and boost::container::devector to decide which provides the best performance for your
// application.

#pragma once

#if USE_BOOST_DEVECTOR
// Older versions of devector may be more performant; however, they grow unbonded in this implementation and will cause a crash.
#  include <boost/version.hpp>
#  if BOOST_VERSION >= 108100
#    include <boost/container/devector.hpp>
#  else
#    error "Boost version MUST be semi-recent! (Alternately, just use std::deque.)"
#  endif
#else
#  include <deque>
#endif

namespace sb {

#if USE_BOOST_DEVECTOR
template <typename T>
using devector = boost::container::devector<T>;
#else
template <typename T>
using devector = std::deque<T>;
#endif

} // namespace sb
