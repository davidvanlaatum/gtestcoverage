add_executable ( coveragetest LineInfoTest.cpp FakeCoverageDumper.cpp test1.1.gcov test1.2.gcov
                 test2.1.gcov test2.2.gcov test2.3.gcov )
target_compile_definitions ( coveragetest PRIVATE -DTEST_LOCATION="${CMAKE_CURRENT_SOURCE_DIR}" )
target_link_libraries ( coveragetest PUBLIC Boost::boost Boost::filesystem gtestcoveragemain GMock::GMock json PRIVATE tinyxml )

add_test ( NAME coveragetest COMMAND ${CMAKE_SOURCE_DIR}/scripts/testdriver.sh $<TARGET_FILE:coveragetest> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
set_property ( TEST coveragetest PROPERTY ENVIRONMENT "NAME=coveragetest" )
set_property ( TEST coveragetest PROPERTY SKIP_RETURN_CODE 10 )
set_property ( TEST coveragetest PROPERTY TIMEOUT 600 )

if ( VALGRIND_PATH )
  add_test ( NAME coveragetest_valgrind COMMAND ${CMAKE_SOURCE_DIR}/scripts/testdriver.sh $<TARGET_FILE:coveragetest> )

  set_property ( TEST coveragetest_valgrind PROPERTY SKIP_RETURN_CODE 10 )
  set_property ( TEST coveragetest_valgrind PROPERTY TIMEOUT 600 )
  if ( VALGRIND_XML )
    set ( VALGRIND_XML_OPTS "--xml=yes --xml-file=coveragetest-valgrind.xml" )
  endif ()
  set_property ( TEST coveragetest_valgrind PROPERTY ENVIRONMENT "NAME=coveragetest"
                 "VALGRIND_OPTS=${VALGRIND_ARGS} ${VALGRIND_XML_OPTS}"
                 "VALGRIND=${VALGRIND_PATH}"
                 )
endif ()
if ( NOT SUBBUILD )
  iwyu_target ( coveragetest )
endif ()
