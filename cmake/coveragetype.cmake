include ( ${CMAKE_CURRENT_LIST_DIR}/CheckAndAddFlag.cmake )

if ( NOT ( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Coverage" ))
  message ( WARNING "Code coverage results with an optimized (non-Debug) build may be misleading" )
endif ()

checkandaddflag ( -fprofile-instr-generate CLANG_COVERAGE_FLAGS )
if ( NOT CLANG_COVERAGE_FLAGS )
  checkandaddflag ( --coverage GCC_COVERAGE_FLAGS )
else ()
  checkandaddflag ( -fcoverage-mapping CLANG_COVERAGE_FLAGS )
endif ()

find_program ( GCOV_PATH gcov )
find_program ( LLVM_PROFDATA_PATH llvm-profdata )
find_program ( LLVM_SHOW_PATH llvm-cov )

if ( CLANG_COVERAGE_FLAGS AND LLVM_PROFDATA_PATH AND LLVM_SHOW_PATH )
  list ( APPEND COVERAGE_STYLES clang )
elseif ( GCC_COVERAGE_FLAGS AND GCOV_PATH )
  list ( APPEND COVERAGE_STYLES gcc )
endif ()

if ( DEFINED BUILD_COVERAGE_STYLES )
  foreach ( STYLE IN LISTS COVERAGE_STYLES )
    if ( NOT ${STYLE} IN_LIST BUILD_COVERAGE_STYLES )
      list ( REMOVE_ITEM COVERAGE_STYLES ${STYLE} )
    endif ()
  endforeach ()
endif ()

if ( COVERAGE_STYLES )
  set ( COVERAGE_SUPPORTED TRUE )
else ()
  set ( COVERAGE_SUPPORTED FALSE )
endif ()

message ( STATUS "Supported coverage styles: ${COVERAGE_STYLES}" )
