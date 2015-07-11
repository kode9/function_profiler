#ifndef fp_snippets_h
#define fp_snippets_h

#include <cstdint>

namespace fp {

// Avoid compiler optimization. Copy/Paste from google
// microbenchmark.
template <class T> inline void keep(T &&datum)
{
  asm volatile("" : "+r"(datum));
}

// Computes factorial
uint_least64_t factorial(uint_least64_t n);

} // namespace fp

#endif // fp_snippets_h
