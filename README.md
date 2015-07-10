# function_profiler: Per-thread c++11 function profiler

## Introduction

function_profiler is a simple c++ header only library to profile
function calls.

It is an implementation of an idea of Jeff Preshing in
[his blog]("http://preshing.com/20111203/a-c-profiling-module-for-multithreaded-apis/").

It allows you to measure the number of calls and the time spent in
chosen functions, on a per-thread basis.

It is not a replacement for full-fledged profiling frameworks such as
[GNU gprof]("https://sourceware.org/binutils/docs/gprof/") or
[Linux perf]("https://perf.wiki.kernel.org/index.php/Main_Page"), but
much easier to use.

It does not collect statistical data on all functions, but rather time
specific functions. One downside is that it is intrusive. This means
you must alterate your source code to use it. Fortunately, all you
need is to include a header and add a single line in the functions you
want to profile.

## Usage

### Basic usage

To start profiling functions, drop the single header where your
compiler can find it, include it in your source and add call the
`FUNCTION_PROFILE` macro in ant function:

	#include "function_profiler.hpp"
	// ...

	void suspicious_function()
	{
	  PROFILE_FUNCTION()
	  // ...
	}

After a profiled function call it will print a report if enough time
has elapsed since the last one:

	[FP][7ff2599ff700][hot_function] #503, avg 0.27804ms, tot 139.85309ms
	[FP][7ff2599ff700][hot_function] #532, avg 0.27803ms, tot 147.91211ms

The report has the following structure:

	[FP][<thread_id>][<function_name>] #<number_of_calls>, avg <average_duration>, tot <total_duration>

### Controlling behavior

To enable profiling, add the preprocessor definition `FP_ENABLE`. If
it is not defined, the `FUNCTION_PROFILE` will not do anything.

## Implementation

The idea relies on two simple things:

- the `thread local` [storage duration specifier]("http://en.cppreference.com/w/cpp/language/storage_duration");
- the RAII ([Resource Acquisition Is Initialization]("https://en.wikipedia.org/w/index.php?title=Resource_Acquisition_Is_Initialization")) concept.


The `PROFILE_FUNCTION` macro only instantiate two objects.

First a data structure with the `thread_local` storage specifier,
accountable for collecting statistics.

The second one is a local structure whose sole purpose is to update
the statistics upon destruction.

The timing are done using
[`boost::chrono`]("http://www.boost.org/doc/libs/1_58_0/doc/html/chrono.html")
library. Although the standard now has a `std::chrono` module, boost
provides an additional `thread_clock` function.

## Dependencies

- A C++11 compiler
- `boost::chrono`
- `CMake >= 3.1` (optional)

## TODO

- Also use steady_clock measurements: thread_chrono does not provide information about sleeping time.
- Support different logging output (stderr, stream, file, ...)
- Allow paremetrisation of logging rate (every N calls, every N seconds, manually, ...)
- Add preprocessor macro to disable it
- Change name generation to something more specific (like adding line number) too differentiate same function / different scope calls

<!--  LocalWords:  Preshing preprocessor
 -->
