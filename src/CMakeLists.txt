include ( rpath )
set ( CMAKE_INSTALL_RPATH "${ORIGIN}" )
set ( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )
foreach ( I coverage_dump coverage_reset driver_name )
  list ( APPEND UNDEFINED -U )
  list ( APPEND UNDEFINED _main_${I} )
  list ( APPEND UNDEFINED -U )
  list ( APPEND UNDEFINED main_${I} )
  list ( APPEND UNDEFINED -U )
  list ( APPEND UNDEFINED _unit_${I} )
  list ( APPEND UNDEFINED -U )
  list ( APPEND UNDEFINED unit_${I} )
endforeach ()
string ( REPLACE ";" "," UNDEFINED "${UNDEFINED}" )
checkandaddflag ( "-Wl,${UNDEFINED}" DYNAMIC_LOOKUP )
add_library ( gtestcoverage SHARED GTestCoverageListener.cpp GTestCoverageListener.h
              CoverageData.cpp CoverageData.h
              FileInfo.cpp FileInfo.h
              TestInfo.cpp TestInfo.h
              ClangCoverageData.cpp ClangCoverageData.h
              ClangCoverageFunction.cpp ClangCoverageFunction.h
              ClangCoverageFile.cpp ClangCoverageFile.h
              ClangCoverageFwd.h ClangCoverageDriver.cpp
              ClangCoverageDriver.h CoverageDriver.cpp
              CoverageDriver.h
              fwd.h
              FunctionInfo.cpp FunctionInfo.h
              TestCaseInfo.cpp TestCaseInfo.h
              GCCCoverageDriver.cpp GCCCoverageDriver.h
              GTestCoverageCovers.h
              NullCoverageDriver.cpp NullCoverageDriver.h
              Block.h
              Point.h
              BlockInfo.cpp BlockInfo.h
              LineInfo.cpp LineInfo.h
              MapDiff.h
              CLI.cpp CLI.h
              CLIAction.cpp CLIAction.h BaseInputCLIAction.cpp BaseInputCLIAction.h CoberturaCLIAction.cpp CoberturaCLIAction.h clifwd.h SummaryCLIAction.cpp SummaryCLIAction.h BaseInputOutputCLIAction.cpp BaseInputOutputCLIAction.h CLIException.cpp CLIException.h TextCLIAction.cpp TextCLIAction.h )
get_target_property ( SOURCES gtestcoverage SOURCES )
list ( FILTER SOURCES INCLUDE REGEX ".*.h$" )
set_target_properties ( gtestcoverage PROPERTIES PUBLIC_HEADER "${SOURCES}" )
target_link_libraries ( gtestcoverage PUBLIC ${CMAKE_DL_LIBS} GMock::GMock Boost::boost Boost::filesystem PRIVATE json tinyxml ${DYNAMIC_LOOKUP_LIST} Boost::program_options )
target_include_directories ( gtestcoverage PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                             $<INSTALL_INTERFACE:include/gtestcoverage> )
target_compile_definitions ( gtestcoverage PRIVATE VERSION="${PROJECT_VERSION}" )
generate_export_header ( gtestcoverage )
target_compile_definitions ( gtestcoverage PUBLIC -DHAVE_GTEST_COVERAGE )
iwyu_target ( gtestcoverage )
clang_tidy_target ( gtestcoverage )

foreach ( STYLE IN LISTS COVERAGE_STYLES )
  add_library ( coveragedumper-${STYLE} STATIC coveragedumper.cpp )
  target_compile_definitions ( coveragedumper-${STYLE} PRIVATE -DPREFIX=main -DSTYLE=\"${STYLE}\" -DSTYLE_${STYLE} )
  set_target_properties ( coveragedumper-${STYLE} PROPERTIES POSITION_INDEPENDENT_CODE ON )
  install ( TARGETS coveragedumper-${STYLE}
            EXPORT gtestcoverage
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
            )
  add_library ( coveragedumperunit-${STYLE} STATIC coveragedumper.cpp )
  target_compile_definitions ( coveragedumperunit-${STYLE} PRIVATE -DPREFIX=unit -DSTYLE=\"${STYLE}\" -DSTYLE_${STYLE} )
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

add_executable ( gtestcoveragecli main.cpp )
target_link_libraries ( gtestcoveragecli PUBLIC gtestcoverage )
target_compile_definitions ( gtestcoverage PUBLIC VERSION_STR="${PROJECT_VERSION}" )
