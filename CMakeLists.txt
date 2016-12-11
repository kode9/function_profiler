cmake_minimum_required(VERSION 3.1)
project(function_profiler VERSION 0.3)

add_library(function_profiler INTERFACE)
target_compile_features(function_profiler INTERFACE cxx_thread_local cxx_constexpr cxx_auto_type)
target_compile_options(function_profiler INTERFACE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(function_profiler INTERFACE ${PROJECT_SOURCE_DIR})

find_package(Boost REQUIRED)
target_compile_definitions(function_profiler
  INTERFACE
  BOOST_CHRONO_HEADER_ONLY
  BOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
  BOOST_ERROR_CODE_HEADER_ONLY
  )
target_include_directories(function_profiler INTERFACE ${Boost_INCLUDE_DIRS})

option(FP_ENABLE "Enable function_profiler" ON)
target_compile_definitions(function_profiler INTERFACE $<$<BOOL:${FP_ENABLE}>:FP_ENABLE>)

############
# Examples #
############
find_package(Threads REQUIRED)

add_library(snippets SHARED examples/snippets.cpp)
set_target_properties(snippets PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)
target_include_directories(snippets INTERFACE examples)
target_compile_options(snippets PUBLIC -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(snippets INTERFACE function_profiler)

add_executable(basic examples/basic.cpp)
target_link_libraries(basic snippets Threads::Threads)
