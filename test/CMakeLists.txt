add_executable ( coveragetest LineInfoTest.cpp FakeCoverageDumper.cpp test1.1.gcov test1.2.gcov
                 test2.1.gcov test2.2.gcov test2.3.gcov )
target_compile_definitions ( coveragetest PRIVATE -DTEST_LOCATION="${CMAKE_CURRENT_SOURCE_DIR}" )
target_link_libraries ( coveragetest PUBLIC Boost::boost gtestcoverage GTest::GTest GMock::GMock GMock::Main PRIVATE tinyxml )
