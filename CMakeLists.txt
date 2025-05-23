cmake_minimum_required(VERSION 3.28)
project(Timer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(timer_test tests/test.cpp timer.h)
target_link_libraries(timer_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(timer_test)
