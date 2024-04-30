#include <benchmark/benchmark.h>
#include <sb/message/accumulator.h>

namespace { // anonymous

void with_deque(benchmark::State& state) {

  using accumulator_type = sb::message::accumulator<>;
#include "accumulator_benchmark.ipp"
}
BENCHMARK(with_deque);

} // anonymous namespace
