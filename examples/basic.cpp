#include "function_profiler.hpp"
#include "snippets.h"

#include <cstdint>
#include <future>
#include <iostream>

namespace {

// Just calls factorial
uint_least64_t suspicious_function(uint_least64_t n)
{
  PROFILE_FUNCTION();
  return fp::factorial(n);
}

// Calls suspicious_function n times and return the last result
uint_least64_t call_suspicious_function(size_t n)
{
  uint_least64_t result{0};

  for (size_t i = 0; i < n; ++i) {
    result = suspicious_function(20);
    fp::keep(result); // avoid optimization
  }

  return result;
}

} // anonymous namespace

int main(int, char **)
{
  auto handle = std::async(std::launch::async, call_suspicious_function, 10 << 20);
  auto result = handle.get();

  std::cout << "Factorial(100) => " << result << " == " << 2432902008176640000ull << " <= Should be" << std::endl;
  std::cout << "Number of calls should be #" << 10485760ul << std::endl;

  return 0;
}
