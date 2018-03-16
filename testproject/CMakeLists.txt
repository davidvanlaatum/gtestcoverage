list ( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )
cmake_minimum_required ( VERSION 3.6 )
project ( testcoverage VERSION 1.0.0 )
set ( CMAKE_CXX_STANDARD 11 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
if ( NOT ${CMAKE_VERSION} VERSION_LESS 3.9 )
  cmake_policy ( SET CMP0068 NEW )
endif ()

if ( NOT EXISTS ${CMAKE_SOURCE_DIR}/cmake/FindBoost.cmake )
  file ( MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
  file ( DOWNLOAD https://raw.githubusercontent.com/Kitware/CMake/release/Modules/FindBoost.cmake
         ${CMAKE_SOURCE_DIR}/cmake/FindBoost.cmake
         SHOW_PROGRESS
         STATUS find_boost_download
         TIMEOUT 10 )
endif ()

find_package ( GTest REQUIRED )
find_package ( gtestcoverage REQUIRED )

add_library ( atestlibrary SHARED atestfile.h atestfile.cpp )
setup_target_for_coverage ( atestlibrary )

if ( NOT TARGET gtestcoveragemain )
  message ( FATAL_ERROR "no gtestcoveragemain target" )
endif ()

enable_testing ()

add_executable ( atestlibrarytest unittest.cpp )
target_link_libraries ( atestlibrarytest PUBLIC atestlibrary gtestcoveragemain )
add_test ( NAME atestlibrarytest COMMAND atestlibrarytest )
setup_test_for_coverage ( atestlibrarytest atestlibrary )
set_property ( TEST atestlibrarytest PROPERTY ENVIRONMENT GTEST_OUTPUT="xml:atestlibrarytest-unit.xml" )
