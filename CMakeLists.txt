cmake_minimum_required (VERSION 3.12)

project (asio_timer_test)

set (CMAKE_CXX_STANDARD 14)

if ((NOT DEFINED BOOST_ROOT) AND (DEFINED ENV{BOOST_ROOT}))
  set (BOOST_ROOT $ENV{BOOST_ROOT})
endif ()
file (TO_CMAKE_PATH "${BOOST_ROOT}" BOOST_ROOT)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)
if (NOT APPLE)
  set (Boost_USE_STATIC_RUNTIME ON)
endif ()
find_package (Boost 1.70 REQUIRED COMPONENTS math)

add_executable (asio_timer_test asio_timer_test.cpp)
target_link_libraries (asio_timer_test
  Boost::boost
  Boost::math)
#target_compile_definitions(asio_timer_test PRIVATE BOOST_ASIO_ENABLE_HANDLER_TRACKING)

if (NOT MSVC)
  set (THREADS_PREFER_PTHREAD_FLAG ON)
  find_package (Threads)
  target_link_libraries (asio_timer_test Threads::Threads)
endif ()



