#include <benchmark/benchmark.h>
#include <boost/circular_buffer.hpp>
#include <sb/message/accumulator.h>

namespace { // anonymous

void with_circular_buffer_space_optimized(benchmark::State& state) {

  using accumulator_type = sb::message::accumulator<std::vector<uint8_t>, boost::circular_buffer_space_optimized<std::vector<uint8_t>>>;
#include "accumulator_benchmark.ipp"
}
BENCHMARK(with_circular_buffer_space_optimized);

} // anonymous namespace
