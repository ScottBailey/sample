#include <sb/message/accumulator.h>
#include <vector>
#include <stdint.h>
#include <iostream>

#if USE_BOOST_DEVECTOR
#  include <boost/container/devector.hpp>
using accululator_type = sb::message::accumulator<std::vector<uint8_t>, boost::container::devector<std::vector<uint8_t>>>;
#else
using accululator_type = sb::message::accumulator<>;
#endif

int main(int, char**) {

  int rv = 0; // return value

  // Create and load an accumulator with data.
  accululator_type accum;
  {
    std::vector<uint8_t> v{0, 1, 2, 3};
    accum.push_back(v);
  }
  {
    std::vector<uint8_t> v{4, 5, 6, 7};
    accum.push_back(v);
  }
  {
    std::vector<uint8_t> v{8, 9, 10, 11};
    accum.push_back(v);
  }

  // Print it out. For fun.
  for (const auto& a : accum) {
    std::cout << unsigned(a) << " ";
  }
  std::cout << "\n";

  // Test data.
  for (size_t i = 0; i < accum.size(); ++i) {
    if (i != size_t(accum[i])) {
      std::cerr << "line " << __LINE__ << ": element " << i << " of accumulator has the wrong value: " << unsigned(accum[i]) << "\n";
      ++rv;
    }
  }

  // Test iteration.
  {
    auto a = accum.begin();
    if (*a != 0) {
      std::cerr << "line " << __LINE__ << ": iterator has the wrong value: " << unsigned(*a) << "\n";
      ++rv;
    }
    ++a;
    if (*a != 1) {
      std::cerr << "line " << __LINE__ << ": iterator has the wrong value: " << unsigned(*a) << "\n";
      ++rv;
    }
    std::advance(a, 3);
    if (*a != 4) {
      std::cerr << "line " << __LINE__ << ": iterator has the wrong value: " << unsigned(*a) << "\n";
      ++rv;
    }
    --a;
    if (*a != 3) {
      std::cerr << "line " << __LINE__ << ": iterator has the wrong value: " << unsigned(*a) << "\n";
      ++rv;
    }
    std::advance(a, 8);
    if (*a != 11) {
      std::cerr << "line " << __LINE__ << ": iterator has the wrong value: " << unsigned(*a) << "\n";
      ++rv;
    }
    ++a;
    if (a != accum.end()) {
      std::cerr << "line " << __LINE__ << ": iterator has the wrong value: " << unsigned(*a) << "\n";
      ++rv;
    }
  }

  // Cause the elements contained inside the accumulator to be resized/moved around.
  {
    const auto& a = accum.front(6);
    if (a.size() != 6) {
      std::cerr << "line " << __LINE__ << ": unexpected size: " << a.size() << "\n";
      ++rv;
    }
  }

  // Test data.
  for (size_t i = 0; i < accum.size(); ++i) {
    if (i != size_t(accum[i])) {
      std::cerr << "line " << __LINE__ << ": element " << i << " of accumulator has the wrong value: " << unsigned(accum[i]) << "\n";
      ++rv;
    }
  }

  // Cause the elements contained inside the accumulator to be resized/moved around.
  {
    const auto& a = accum.back(6);
    if (a.size() != 6) {
      std::cerr << "line " << __LINE__ << ": unexpected size: " << a.size() << "\n";
      ++rv;
    }
  }

  // Test data.
  for (size_t i = 0; i < accum.size(); ++i) {
    if (i != size_t(accum[i])) {
      std::cerr << "line " << __LINE__ << ": element " << i << " of accumulator has the wrong value: " << unsigned(accum[i]) << "\n";
      ++rv;
    }
  }

  {
    const auto& a = accum.back();
    if (a.size() != 6) {
      std::cerr << "line " << __LINE__ << ": unexpected size: " << a.size() << "\n";
      ++rv;
    }
  }

  // Add more tests as time allows.
  return rv;
}
