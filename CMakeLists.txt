cmake_minimum_required(VERSION 3.0)

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS chrono system)

add_executable(profiler profiler.cpp foo.cpp)
set_property(TARGET profiler PROPERTY LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT} glog ${Boost_LIBRARIES})
set_property(TARGET profiler PROPERTY CXX_STANDARD 11)
