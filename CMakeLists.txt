cmake_minimum_required(VERSION 3.1)
project(function_profiler VERSION 0.3)

add_library(function_profiler INTERFACE)
target_compile_features(function_profiler INTERFACE cxx_thread_local cxx_constexpr cxx_auto_type)
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
find_package(Threads)

add_library(snippets SHARED examples/snippets.cpp)
target_include_directories(snippets INTERFACE examples)
set_property(TARGET snippets PROPERTY CXX_STANDARD 11)
set_property(TARGET snippets PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET snippets PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_options(snippets PUBLIC -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(snippets INTERFACE function_profiler)

add_executable(basic EXCLUDE_FROM_ALL examples/basic.cpp)
target_link_libraries(basic snippets)
target_link_libraries(basic Threads::Threads)
