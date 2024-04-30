
{
  std::vector<std::vector<std::uint8_t>> src;
  std::vector<std::vector<std::uint8_t>> dst;

  // Load source.
  for (size_t n = 0; n < 1000; ++n) {
    for (size_t i = 1; i < 500; ++i) {
      std::vector<uint8_t> temp;
      temp.reserve(i);
      uint8_t v = std::uint8_t(i % 255);
      for (size_t j = 0; j < i; ++j) {
        ++v; // allow rollover, NOT UB for this type.
        temp.push_back(v);
      }
      src.push_back(temp);
    }
  }

  // begin the test
  for (auto _ : state) {
    constexpr size_t message_size = 80;
    accumulator_type accum;

    for (const auto& a : src) {
      // read one message from the source list and store it in the accumulator.
      accum.push_back(a);
      // If we have enough bytes to "decode", get a message out of the accumulator.
      while (accum.size() > message_size)
        dst.push_back(accum.pop_front(message_size));
    }
  }
}
