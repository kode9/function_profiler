#include "function_profiler.hpp"
#include "snippets.h"

#include <chrono>
#include <cstdint>
#include <future>
#include <iostream>
#include <thread>

namespace {

constexpr size_t N = 10 << 12;

// Just calls factorial
uint_least64_t function(uint_least64_t n)
{
  PROFILE_FUNCTION();

  // This shows the difference between the thread and the steady clocks
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  return fp::factorial(n);
}

// Calls suspicious_function n times and return the last result
uint_least64_t call_function(size_t n)
{
  uint_least64_t result{0};

  for (size_t i = 0; i < n; ++i) {
    result = function(20);
    fp::keep(result); // avoid optimization
  }

  return result;
}

} // anonymous namespace

int main(int, char **)
{
  const auto result = std::async(std::launch::async, call_function, N).get();

  std::cout << "Factorial(20) => " << result << " == " << 2432902008176640000ull << " <= Should be" << std::endl;
  std::cout << "Number of calls should be #" << N << std::endl;

  return 0;
}
