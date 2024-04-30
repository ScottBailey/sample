#include <benchmark/benchmark.h>
#include <sb/message/accumulator.h>
#include <boost/container/devector.hpp>

namespace { // anonymous

void with_devector(benchmark::State& state) {

  using accumulator_type = sb::message::accumulator<std::vector<uint8_t>, boost::container::devector<std::vector<uint8_t>>>;
#include "accumulator_benchmark.ipp"
}
BENCHMARK(with_devector);

} // anonymous namespace
