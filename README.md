# Sample Code

## Author

Scott Bailey <br>


## Why this code?

This bit of code I have hanging around that has some real world
utility. I've used something like it to accumulate inbound data from
various interfaces many times over the years.

In usage, our application might consume serial, TCP, or other streamed
data. This `accumulator` allows us to easily peek at the data using
iterators or the array operator as though it were a contiguous memory
block. We might do this to search for unique header indicators or test
checksums. We can then pop the bytes out of the accumulator and use or
discard them as determined by prior tests.


## CMake

CMake is useful. It's a solid choice for ensuring source can build
just about anywhere. And [CPM](https://github.com/cpm-cmake/CPM.cmake)
makes dependency managmenent simple and repeatable.


## Benchmark

You can't tell if something is efficient without
benchmarking. Google's
[benchmark](https://github.com/google/benchmark) seems pretty good for
this purpose.


## Boost

Boost is a great place to find libraries you might want to use but
don't yet exist in the standard library. In this example, we look at
the performance of `std::deque`, `boost::container::devector`, and
`boost::circular_buffer_space_optimized` as internal containers for
`accumulator`.

Spoiler: on my system, `deque` executes in about 6% more time than
`devector` _for this toy benchmark_. And
`circular_buffer_space_optimized` yielded 20% more time cost than
`deque`! An implementation that also supported std::vector as the
internal container might provide an intersting comparison as well.


## Build and Test

This is one option. It works in Linux.

```bash
mkdir Build \
 && cd Build \
 && cmake .. -DCMAKE_BUILD_TYPE=Release \
 && cmake --build . \
 && ctest
```

For Windows, you might do the following in a gitbash window:
```
mkdir Build
cd Build
cmake ..
cmake --build .
ctest -C Debug
```
