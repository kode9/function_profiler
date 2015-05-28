cmake_minimum_required(VERSION 3.1)
project(function_profiler VERSION 0.2)

add_library(function_profiler INTERFACE)
target_compile_features(function_profiler INTERFACE cxx_thread_local cxx_constexpr cxx_auto_type)
target_include_directories(function_profiler INTERFACE ${PROJECT_SOURCE_DIR})

find_package(Boost REQUIRED COMPONENTS chrono system)
target_include_directories(function_profiler INTERFACE ${Boost_INCLUDE_DIRS})
target_link_libraries(function_profiler INTERFACE ${Boost_LIBRARIES})

find_package(Threads)
target_link_libraries(function_profiler INTERFACE ${CMAKE_THREAD_LIBS_INIT})
