include ( rpath )
set ( CMAKE_INSTALL_RPATH "${ORIGIN}" )
set ( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )
checkandaddflag ( "-Wl,-U,_main_coverage_dump,-U,_main_coverage_reset,-U,_unit_coverage_dump,-U,_unit_coverage_reset" DYNAMIC_LOOKUP )
add_library ( gtestcoverage SHARED GTestCoverageListener.cpp GTestCoverageListener.h
              CoverageData.cpp CoverageData.h
              FileInfo.cpp FileInfo.h
              LineInfo.cpp LineInfo.h
              TestInfo.cpp TestInfo.h
              )
target_link_libraries ( gtestcoverage PUBLIC ${CMAKE_DL_LIBS} GMock::GMock PRIVATE Boost::boost Boost::filesystem tinyxml ${DYNAMIC_LOOKUP_LIST} )
target_include_directories ( gtestcoverage PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> )
generate_export_header ( gtestcoverage )
target_compile_definitions ( gtestcoverage PUBLIC -DHAVE_GTEST_COVERAGE )
iwyu_target ( gtestcoverage )
clang_tidy_target ( gtestcoverage )

foreach ( STYLE IN LISTS COVERAGE_STYLES )
  add_library ( coveragedumper-${STYLE} STATIC coveragedumper.cpp )
  target_compile_definitions ( coveragedumper-${STYLE} PRIVATE -DPREFIX=main -DSTYLE=${STYLE} -DSTYLE_${STYLE} )
  set_target_properties ( coveragedumper-${STYLE} PROPERTIES POSITION_INDEPENDENT_CODE ON )
  install ( TARGETS coveragedumper-${STYLE}
            EXPORT gtestcoverage
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
            )
  add_library ( coveragedumperunit-${STYLE} STATIC coveragedumper.cpp )
  target_compile_definitions ( coveragedumperunit-${STYLE} PRIVATE -DPREFIX=unit -DSTYLE=${STYLE} -DSTYLE_${STYLE} )
  set_target_properties ( coveragedumperunit-${STYLE} PROPERTIES POSITION_INDEPENDENT_CODE ON )
  install ( TARGETS coveragedumperunit-${STYLE}
            EXPORT gtestcoverage
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
            )
endforeach ()

add_library ( gtestcoveragemain SHARED gmockmain.cpp )
target_link_libraries ( gtestcoveragemain PUBLIC gtestcoverage PRIVATE Boost::boost GMock::GMock ${DYNAMIC_LOOKUP_LIST} )
install ( TARGETS gtestcoverage gtestcoveragemain EXPORT gtestcoverage
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
          PUBLIC_HEADER DESTINATION include/gtestcoverage COMPONENT devel )
