if ( NOT TARGET gtestcoverage )
  include ( FindPackageHandleStandardArgs )
  if ( gtestcoverage_FIND_VERSION )
    list ( APPEND OPTS gtestcoverage_FIND_VERSION )
  endif ()
  if ( gtestcoverage_FIND_REQUIRED )
    list ( APPEND OPTS REQUIRED )
  endif ()
  if ( gtestcoverage_FIND_QUIETLY )
    list ( APPEND OPTS QUIET )
  endif ()

  find_package ( Boost 1.64 REQUIRED COMPONENTS filesystem )

  set ( gtestcoverage_DIR ${CMAKE_CURRENT_LIST_DIR}/gtestcoverage/ )
  include ( ${CMAKE_CURRENT_LIST_DIR}/gtestcoverage/gtestcoverageConfig.cmake )
  include ( ${CMAKE_CURRENT_LIST_DIR}/gtestcoverage/gtestcoverage-buildsettings.cmake )
  include ( ${CMAKE_CURRENT_LIST_DIR}/gtestcoverage/coveragetype.cmake )
  include ( ${CMAKE_CURRENT_LIST_DIR}/gtestcoverage/coveragesetup.cmake )
endif ()
