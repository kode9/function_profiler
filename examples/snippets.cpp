#include "snippets.h"

// Computes factorial
uint_least64_t fp::factorial(uint_least64_t n)
{
  return (n <= 1) ? 1 : (n * factorial(n - 1));
}
