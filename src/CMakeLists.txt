checkandaddflag ( "-undefined dynamic_lookup" DYNAMIC_LOOKUP )
add_library ( gtestcoverage SHARED GTestCoverageListener.cpp GTestCoverageListener.h
              CoverageData.cpp CoverageData.h
              FileInfo.cpp FileInfo.h
              LineInfo.cpp LineInfo.h
              TestInfo.cpp TestInfo.h
              gmockmain.cpp )
target_link_libraries ( gtestcoverage PUBLIC Boost::filesystem ${CMAKE_DL_LIBS} PRIVATE tinyxml googleinclude )
set_property ( TARGET gtestcoverage APPEND PROPERTY LINK_FLAGS "${DYNAMIC_LOOKUP_LIST}" )
target_include_directories ( gtestcoverage PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> )
generate_export_header ( gtestcoverage )
install ( TARGETS gtestcoverage EXPORT gtestcoverage
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
          PUBLIC_HEADER DESTINATION include/gtestcoverage COMPONENT devel )
target_compile_definitions ( gtestcoverage PUBLIC -DHAVE_GTEST_COVERAGE )
add_library ( coveragedumper STATIC coveragedumper.cpp )
target_compile_definitions ( coveragedumper PRIVATE -DPREFIX=main )
set_target_properties ( coveragedumper PROPERTIES POSITION_INDEPENDENT_CODE ON )
install ( TARGETS coveragedumper
          EXPORT getestcoverage
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
          PUBLIC_HEADER DESTINATION include/gtestcoverage COMPONENT devel
          )
add_library ( coveragedumperunit STATIC EXCLUDE_FROM_ALL coveragedumper.cpp )
target_compile_definitions ( coveragedumperunit PRIVATE -DPREFIX=unit )
set_target_properties ( coveragedumperunit PROPERTIES POSITION_INDEPENDENT_CODE ON )
iwyu_target ( gtestcoverage )
clang_tidy_target ( gtestcoverage )
