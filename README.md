# function_profiler: Intrusive c++11 function profiler

Non-tested c++11 adaptation of an idea by Jeff Preshing : http://preshing.com/20111203/a-c-profiling-module-for-multithreaded-apis/

## Introduction

function_profiler is a single header per-thread function profiler.

To use it simply include it and use the correct macro in each function you want to profile:

```c++
#include "function_profiler.hpp"
// ...

void hot_function()
{
  FUNCTION_PROFILE()
  // ...
}
```

The macro define a structure with c++11 `thread_local` storage specification and a scoped variable responsible to update the call statistics.

If the last call to the function was more than a second ago, it will print the call statistics: number of calls and CPU time (min, max and mean).

The timing are done using `boost::chrono` library and the statistics computed using boost::accumulators library. Although the standard now has the chrono module, boost provides an additional `thread_clock` function. The downside of `thread_chrono` is that it does not take some things into account (sleeps / waits, OpenMP, ...).

## Dependencies

- `CMake > 2.8`
- `boost::chrono`
- `boost::accumulators`
- A c++11 compiler supporting `thread_local` storage

## TODO

- Also use steady_clock measurements
- Support different logging output (stderr, stream, file, ...)
- Allow paremetrisation of logging rate (every N calls, every N seconds, manually, ...)
- Add preprocessor macro to disable it
- Change name generation to something more specific (like adding line number) too differentiate same function / different scope calls
