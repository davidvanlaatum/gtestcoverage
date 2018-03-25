list ( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )
cmake_minimum_required ( VERSION 3.6 )
project ( gtestcoverage VERSION 1.0.0 )

if ( CMAKE_CXX_COMPILER_ID STREQUAL "clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.9.0 )
  message ( FATAL_ERROR "Unsupported clang version" )
endif ()

if ( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9.0 )
  message ( FATAL_ERROR "Unsupported gcc version" )
endif ()

set ( CMAKE_CXX_STANDARD 11 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
if ( NOT ${CMAKE_VERSION} VERSION_LESS 3.9 )
  cmake_policy ( SET CMP0068 NEW )
endif ()
cmake_policy ( SET CMP0042 NEW )
include ( CheckAndAddFlag )
include ( GenerateExportHeader )
include ( GNUInstallDirs )
add_custom_target ( checks )

option ( BUILD_TESTING "Testing" ON )
if ( BUILD_TESTING )
  enable_testing ()
endif ()

find_package ( GTest REQUIRED )
if ( NOT EXISTS ${CMAKE_SOURCE_DIR}/cmake/FindBoost.cmake )
  file ( DOWNLOAD https://raw.githubusercontent.com/Kitware/CMake/release/Modules/FindBoost.cmake
         ${CMAKE_SOURCE_DIR}/cmake/FindBoost.cmake
         SHOW_PROGRESS
         STATUS find_boost_download
         TIMEOUT 10 )
endif ()
find_package ( Boost 1.65 REQUIRED COMPONENTS filesystem program_options )
find_package ( tinyxml REQUIRED )
find_package ( iwyu )
find_package ( clang-tidy )
find_package ( valgrind )
include ( coveragetype )
if ( NOT COVERAGE_SUPPORTED )
  message ( FATAL_ERROR "No supported coverage types" )
endif ()
set ( CMAKE_EXPORT_COMPILE_COMMANDS ON )
checkandaddflag ( -Weverything CMAKE_CXX_FLAGS )
if ( NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
  checkandaddflag ( -Wall CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wextra CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wunused CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wuninitialized CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wmaybe-uninitialized CMAKE_CXX_FLAGS )
  checkandaddflag ( -Winit-self CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wfloat-equal CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wshadow CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wshadow-all CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wsign-conversion CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wconversion CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wpedantic CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wdocumentation CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wsuggest-override CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wduplicated-branches CMAKE_CXX_FLAGS )
  checkandaddflag ( -Wduplicated-cond CMAKE_CXX_FLAGS )
endif ()
checkandaddflag ( -fcolor-diagnostics CMAKE_CXX_FLAGS )
checkandaddflag ( -fcomment-block-commands=requirement,internal,endinternal,nosubgrouping CMAKE_CXX_FLAGS )
checkandaddflag ( -fdebug-macro CMAKE_CXX_FLAGS )
checkandaddflag ( -fdiagnostics-color CMAKE_CXX_FLAGS )
checkandaddflag ( -fdiagnostics-show-template-tree CMAKE_CXX_FLAGS )
checkandaddflag ( -fmerge-constants CMAKE_CXX_FLAGS )
checkandaddflag ( -fmessage-length=160 CMAKE_CXX_FLAGS )
checkandaddflag ( -fspell-checking CMAKE_CXX_FLAGS )
checkandaddflag ( -fstrict-aliasing CMAKE_CXX_FLAGS )
checkandaddflag ( -Qunused-arguments CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-c++98-compat CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-c++98-compat-bind-to-temporary-copy CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-c++98-compat-pedantic CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-c99-extensions CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-covered-switch-default CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-documentation-unknown-command CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-error=unknown-pragmas CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-exit-time-destructors CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-global-constructors CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-long-long CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-missing-prototypes CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-packed CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-padded CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-reserved-id-macro CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-undef CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-unknown-pragmas CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-unused-command-line-argument CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-used-but-marked-unused CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-variadic-macros CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-weak-vtables CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-zero-as-null-pointer-constant CMAKE_CXX_FLAGS )
checkandaddflag ( -Wno-unknown-warning-option CMAKE_CXX_FLAGS ) # keep last

option ( WERROR "Turn warnings into errors" OFF )
if ( WERROR )
  checkandaddflag ( -Werror CMAKE_CXX_FLAGS )
endif ()

add_subdirectory ( json )
add_subdirectory ( src )
if ( BUILD_TESTING )
  include ( coveragesetup )
  add_subdirectory ( test )
  add_subdirectory ( testproject )
endif ()

include ( CMakePackageConfigHelpers )
write_basic_package_version_file ( "${CMAKE_CURRENT_BINARY_DIR}/gtestcoverageVersion.cmake"
                                   VERSION ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion )
export ( EXPORT gtestcoverage FILE "${CMAKE_CURRENT_BINARY_DIR}/gtestcoverageConfig.cmake" )
install ( EXPORT gtestcoverage
          FILE gtestcoverageConfig.cmake
          COMPONENT devel
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/gtestcoverage
          )
install ( FILES "${CMAKE_CURRENT_BINARY_DIR}/gtestcoverageVersion.cmake"
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/gtestcoverage
          COMPONENT devel
          )
configure_file ( gtestcoverage-buildsettings.cmake.in gtestcoverage-buildsettings.cmake )
install ( FILES cmake/coveragetype.cmake cmake/CheckAndAddFlag.cmake cmake/coveragesetup.cmake
          ${CMAKE_BINARY_DIR}/gtestcoverage-buildsettings.cmake cmake/DownloadProject.cmake cmake/DownloadProject.CMakeLists.cmake.in
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/gtestcoverage/ COMPONENT devel )
install ( FILES cmake/Findgtestcoverage.cmake cmake/FindGTest.cmake
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ COMPONENT devel )

if ( NOT DEFINED SUBBUILD )
  set ( CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} )
  set ( CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR} )
  set ( CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH} )
  set ( CPACK_PACKAGE_VENDOR "David van Laatum" )
  set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "gtest coverage collector" )
  set ( CPACK_PROJECT_CONFIG_FILE ${CMAKE_SOURCE_DIR}/CPack.cmake )
  include ( CPack )
  include ( CPackComponent )

  foreach ( C ${CPACK_COMPONENTS_ALL} )
    list ( APPEND COMMANDS COMMAND ${CMAKE_COMMAND} -E env DESTDIR=installtest ${CMAKE_COMMAND} -DCOMPONENT=${C} -P ${CMAKE_BINARY_DIR}/cmake_install.cmake )
  endforeach ()
  if ( NOT COMMANDS )
    list ( APPEND COMMANDS COMMAND ${CMAKE_COMMAND} -E env DESTDIR=installtest ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_install.cmake )
  endif ()
  list ( APPEND COMMANDS COMMAND find installtest -type f )
  list ( APPEND COMMANDS COMMAND ${CMAKE_COMMAND} -E make_directory installtest/testproject )
  list ( APPEND COMMANDS COMMAND ${CMAKE_COMMAND} -E chdir installtest/testproject ${CMAKE_COMMAND}
         -DCMAKE_MODULE_PATH=${CMAKE_BINARY_DIR}/installtest/${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake/
         -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
         -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
         -G "${CMAKE_GENERATOR}"
         -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
         -DCMAKE_VERBOSE_MAKEFILE=ON
         ${CMAKE_SOURCE_DIR}/testproject/ )
  list ( APPEND COMMANDS COMMAND ${CMAKE_COMMAND} --build installtest/testproject )
  list ( APPEND COMMANDS COMMAND ${CMAKE_COMMAND} -E chdir installtest/testproject ${CMAKE_CTEST_COMMAND} -V --output-on-failure )
  if ( Unspecified IN_LIST CPACK_COMPONENTS_ALL )
    message ( WARNING "Files that have not been assigned to a category exist" )
  endif ()
  if ( BUILD_TESTING )
    add_custom_target ( installtest
                        COMMAND ${CMAKE_COMMAND} -E remove_directory installtest
                        COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/install_manifest*
                        ${COMMANDS} USES_TERMINAL
                        )
    add_test ( NAME installtest COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target installtest )
  endif ()
endif ()
